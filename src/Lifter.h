#pragma once

#include <string>
#include "enums.h"

class Lifter {
private:
    std::string name;
    Gender gender = Gender::MALE;
    int age = 0;
    double bodyweight = 0;
    double squat = 0;
    double bench = 0;
    double deadlift = 0;
    double total = 0;
    double ipf_score = 0;
    double ipf_gl_score = 0;
    double dots_score = 0;
    double wilks_score = 0;
    double wilks_old_score = 0;
    bool steroids = false;
    Unit units = Unit::KG;

    constexpr static double KG_TO_LBS = 2.2046226218;

public:
    Lifter() = default;

    Lifter(std::string name, const Gender gender, const int age, const double bodyweight, const double squat,
           const double bench, const double deadlift, const Unit units)
        : name(std::move(name)), gender(gender), age(age), bodyweight(bodyweight), squat(squat), bench(bench),
          deadlift(deadlift), total(squat + bench + deadlift), units(units) {
    }

    Lifter &operator=(const Lifter &other) {
        if (this != &other) {
            name = other.name;
            gender = other.gender;
            age = other.age;
            bodyweight = other.bodyweight;
            squat = other.squat;
            bench = other.bench;
            deadlift = other.deadlift;
            total = other.total;
            ipf_score = other.ipf_score;
            ipf_gl_score = other.ipf_gl_score;
            dots_score = other.dots_score;
            wilks_score = other.wilks_score;
            wilks_old_score = other.wilks_old_score;
            steroids = other.steroids;
            units = other.units;
        }

        return *this;
    }

    // Getters
    [[nodiscard]] std::string getName() const;
    [[nodiscard]] Gender getGender() const;
    [[nodiscard]] int getAge() const;
    [[nodiscard]] double getBodyweight() const;
    [[nodiscard]] double getSquat() const;
    [[nodiscard]] double getBench() const;
    [[nodiscard]] double getDeadlift() const;
    [[nodiscard]] double getTotal() const;
    [[nodiscard]] double getIPFScore() const;
    [[nodiscard]] double getIPFGLScore() const;
    [[nodiscard]] double getDOTSScore() const;
    [[nodiscard]] double getWILKSScore() const;
    [[nodiscard]] double getWILKSOldScore() const;
    [[nodiscard]] bool getSteroidUse() const;
    [[nodiscard]] Unit getUnits() const;
    [[nodiscard]] double convertToKG(double value) const;
    [[nodiscard]] double convertToLBS(double value) const;

    // Setters
    void setName(const std::string &name);
    void setGender(Gender gender);
    void setAge(int age);
    void setBodyweight(double bodyweight);
    void setSquat(double squat);
    void setBench(double bench);
    void setDeadlift(double deadlift);
    void setTotal(double total);
    void setIPFScore(double ipf_score);
    void setIPFGLScore(double ipf_gl_score);
    void setDOTSScore(double dots_score);
    void setWILKSScore(double wilks_score);
    void setWILKSOldScore(double wilks_old_score);
    void setUnits(Unit units);
    void updateTotal();

    // Score Calculations
    void calculateIPFScore(double bw, double total, bool equipped, bool benchOnly);
    void calculateIPFGLScore(double bw, double total, bool equipped, bool benchOnly);
    void calculateDOTSScore(double bw, double total);
    void calculateWILKSScore(double bw, double total);
    void calculateWILKSOldScore(double bw, double total);
    void calculateScores(bool equipped);

    void convertUnits();

    // Equality operator overload
    friend bool operator==(const Lifter &lhs, const Lifter &rhs) {
        return lhs.name == rhs.name && lhs.gender == rhs.gender && lhs.age == rhs.age &&
               lhs.bodyweight == rhs.bodyweight && lhs.total == rhs.total;
    }
};
