#pragma once

#include <vector>
#include "Lifter.h"

class Meet {
private:
    std::string name;
    std::string date;
    std::string location;
    std::vector<Lifter> lifters;
    Federation federation;
    EquipmentClass equipClass;
    bool benchOnly;
    bool drugTested;
    Unit units;

public:
    Meet() : federation(Federation::IPF), equipClass(EquipmentClass::RAW), benchOnly(false), drugTested(true),
             units(Unit::KG) {
    }

    Meet(std::string name, std::string date, std::string location,
         const Federation federation, const EquipmentClass equipClass, const bool benchOnly, const bool drugTested,
         const Unit units)
        : name(std::move(name)), date(std::move(date)), location(std::move(location)), federation(federation),
          equipClass(equipClass), benchOnly(benchOnly), drugTested(drugTested), units(units) {
    }

    // Getters
    [[nodiscard]] std::string getName() const;
    [[nodiscard]] std::string getDate() const;
    [[nodiscard]] std::string getLocation() const;
    std::vector<Lifter>& getLifters();
    [[nodiscard]] const std::vector<Lifter>& getLifters() const;
    [[nodiscard]] Federation getFederation() const;
    [[nodiscard]] EquipmentClass getEquipClass() const;
    [[nodiscard]] bool getBenchOnly() const;
    [[nodiscard]] bool getDrugTested() const;
    [[nodiscard]] Unit getUnits() const;

    // Setters
    void setName(const std::string &name);
    void setDate(const std::string &date);
    void setLocation(const std::string &location);
    void setFederation(Federation federation);
    void setEquipClass(EquipmentClass equipClass);
    void setBenchOnly(bool benchOnly);
    void setDrugTested(bool drugTested);
    void setUnits(Unit units);

    // Lifter Management
    void addLifter(const Lifter &lifter);
    bool removeLifter(const Lifter &lifter);
    bool sortLifters(const std::string &attribute, size_t num_threads = 0, size_t threshold = 2000);

    void clearLifters();
    void drugTest();
    void convertAllUnits();
    void getResults();
};
