#include "Serialization.h"

void to_json(json &j, const Lifter &lifter) {
    j = json{
        {"name", lifter.getName()},
        {"gender", lifter.getGender()},
        {"age", lifter.getAge()},
        {"bodyweight", lifter.getBodyweight()},
        {"squat", lifter.getSquat()},
        {"bench", lifter.getBench()},
        {"deadlift", lifter.getDeadlift()},
        {"total", lifter.getTotal()},
        {"unit", lifter.getUnits()},
        {"scores", {
            {"DOTS", lifter.getDOTSScore()},
            {"GL Points", lifter.getIPFGLScore()},
            {"Wilks", lifter.getWILKSScore()}
        }}
    };
}

void from_json(const json &j, Lifter &lifter) {
    lifter.setName(j.at("name").get<std::string>());
    lifter.setGender(j.at("gender").get<Gender>());
    lifter.setAge(j.at("age").get<int>());
    lifter.setBodyweight(j.at("bodyweight").get<float>());
    lifter.setSquat(j.at("squat").get<float>());
    lifter.setBench(j.at("bench").get<float>());
    lifter.setDeadlift(j.at("deadlift").get<float>());
    lifter.setUnits(j.at("unit").get<Unit>());

    if (j.contains("total")) {
        lifter.setTotal(j.at("total").get<float>());
    } else {
        lifter.updateTotal();
    }

    if (j.contains("scores")) {
        const auto &scores = j.at("scores");
        if (scores.contains("DOTS")) {
            lifter.setDOTSScore(scores.at("DOTS").get<float>());
        }

        if (scores.contains("GL Points")) {
            lifter.setIPFGLScore(scores.at("GL Points").get<float>());
        }

        if (scores.contains("Wilks")) {
            lifter.setWILKSScore(scores.at("Wilks").get<float>());
        }
    }
}

void to_json(json &j, const Meet &meet) {
    j = json{
        {"name", meet.getName()},
        {"date", meet.getDate()},
        {"location", meet.getLocation()},
        {"federation", meet.getFederation()},
        {"drugTested", meet.getDrugTested()},
        {"benchOnly", meet.getBenchOnly()},
        {"equipmentClass", meet.getEquipClass()},
        {"units", meet.getUnits()},
        {"lifters", meet.getLifters()}
    };
}

void from_json(const json &j, Meet &meet) {
    if (!j.contains("name") || !j.contains("date") || !j.contains("location") ||
        !j.contains("federation") || !j.contains("drugTested") ||
        !j.contains("benchOnly") || !j.contains("equipmentClass") ||
        !j.contains("units") || !j.contains("lifters")) {
        throw std::invalid_argument("Missing required fields in JSON.");
    }

    meet.setName(j.at("name").get<std::string>());
    meet.setDate(j.at("date").get<std::string>());
    meet.setLocation(j.at("location").get<std::string>());
    meet.setFederation(j.at("federation").get<Federation>());
    meet.setDrugTested(j.at("drugTested").get<bool>());
    meet.setBenchOnly(j.at("benchOnly").get<bool>());
    meet.setEquipClass(j.at("equipmentClass").get<EquipmentClass>());
    meet.setUnits(j.at("units").get<Unit>());

    meet.clearLifters();
    for (const auto &jsonLifter: j.at("lifters")) {
        meet.addLifter(jsonLifter.get<Lifter>());
    }
}
