#include "Meet.h"

#include <algorithm>
#include <future>
#include <thread>
#include <map>
#include <functional>
#include <iostream>

void Meet::addLifter(const Lifter &lifter) {
    lifters.push_back(lifter);
}

bool Meet::removeLifter(const Lifter &lifter) {
    if (lifters.empty()) {
        return false;
    }

    for (int i = 0; i < lifters.size(); i++) {
        if (lifters[i] == lifter) {
            lifters.erase(lifters.begin() + i);
            return true;
        }
    }

    return false;
}

void Meet::clearLifters() {
    lifters.clear();
}

bool Meet::sortLifters(const std::string &attribute, size_t num_threads, const size_t threshold) {
    const size_t num_lifters = lifters.size();
    if (num_lifters <= 1) {
        return true;
    }

    static const std::map<std::string, std::function<bool(const Lifter &, const Lifter &)> > sortAttribute = {
        {"Squat", [](const Lifter &a, const Lifter &b) { return a.getSquat() > b.getSquat(); }},
        {"Bench", [](const Lifter &a, const Lifter &b) { return a.getBench() > b.getBench(); }},
        {"Deadlift", [](const Lifter &a, const Lifter &b) { return a.getDeadlift() > b.getDeadlift(); }},
        {"Total", [](const Lifter &a, const Lifter &b) { return a.getTotal() > b.getTotal(); }},
        {"IPF", [](const Lifter &a, const Lifter &b) { return a.getIPFScore() > b.getIPFScore(); }},
        {"GL Points", [](const Lifter &a, const Lifter &b) { return a.getIPFGLScore() > b.getIPFGLScore(); }},
        {"DOTS", [](const Lifter &a, const Lifter &b) { return a.getDOTSScore() > b.getDOTSScore(); }},
        {"Wilks", [](const Lifter &a, const Lifter &b) { return a.getWILKSScore() > b.getWILKSScore(); }},
        {"WilksOld", [](const Lifter &a, const Lifter &b) { return a.getWILKSOldScore() > b.getWILKSOldScore(); }}
    };

    const auto comparator_it = sortAttribute.find(attribute);
    if (comparator_it == sortAttribute.end()) {
        std::cerr << "Invalid criteria for sorting!" << std::endl;
        return false;
    }
    auto &comparator = comparator_it->second;

    if (num_lifters < threshold) {
        std::ranges::sort(lifters, comparator);
        return true;
    }

    if (num_threads == 0) {
        num_threads = std::min(num_lifters / 500, static_cast<size_t>(std::thread::hardware_concurrency()));
        if (num_threads == 0) num_threads = 2;
    }

    using difference_type = std::vector<Lifter>::iterator::difference_type;
    const auto chunkSize = static_cast<difference_type>((num_lifters + num_threads - 1) / num_threads);

    std::vector<std::future<void> > futures;
    for (difference_type i = 0; i < num_threads; i++) {
        auto start = lifters.begin() + i * chunkSize;
        auto end = (i == num_threads - 1) ? lifters.end() : start + chunkSize;

        futures.push_back(std::async(std::launch::async, [start, end, &comparator]() {
            std::sort(start, end, comparator);
        }));
    }

    for (auto &future: futures) {
        future.get();
    }

    for (difference_type i = 1; i < num_threads; ++i) {
        const auto start = lifters.begin();
        auto middle = lifters.begin() + i * chunkSize;
        const auto end = (i == num_threads - 1) ? lifters.end() : middle + chunkSize;

        std::inplace_merge(start, middle, end, comparator);
    }

    return true;
}

void Meet::convertAllUnits() {
    for (Lifter &lifter: lifters) {
        if (lifter.getUnits() != units) {
            lifter.convertUnits();
        }
    }
}

void Meet::drugTest() {
    for (Lifter &lifter: lifters) {
        if (lifter.getSteroidUse()) {
            removeLifter(lifter);
        }
    }
}

void Meet::getResults() {
    if (lifters.empty()) {
        return;
    }

    const bool isEquipped = (equipClass == EquipmentClass::EQUIPPED);
    const bool isBenchOnly = benchOnly;

    for (auto &lifter: lifters) {
        const double bw = lifter.getBodyweight();
        const double total = lifter.getTotal();

        if (isBenchOnly) {
            lifter.calculateIPFScore(bw, total, isEquipped, true);
            lifter.calculateIPFGLScore(bw, total, isEquipped, true);
        } else {
            lifter.calculateScores(isEquipped);
        }
    }
}

// Getters
std::string Meet::getName() const { return name; }
std::string Meet::getDate() const { return date; }
std::string Meet::getLocation() const { return location; }
std::vector<Lifter> &Meet::getLifters() { return lifters; }
const std::vector<Lifter> &Meet::getLifters() const { return lifters; }
Federation Meet::getFederation() const { return federation; }
EquipmentClass Meet::getEquipClass() const { return equipClass; }
bool Meet::getBenchOnly() const { return benchOnly; }
bool Meet::getDrugTested() const { return drugTested; }
Unit Meet::getUnits() const { return units; }

// Setters
void Meet::setName(const std::string &name) { this->name = name; }
void Meet::setDate(const std::string &date) { this->date = date; }
void Meet::setLocation(const std::string &location) { this->location = location; }
void Meet::setFederation(const Federation federation) { this->federation = federation; }
void Meet::setEquipClass(const EquipmentClass equipClass) { this->equipClass = equipClass; }
void Meet::setBenchOnly(const bool benchOnly) { this->benchOnly = benchOnly; }
void Meet::setDrugTested(const bool drugTested) { this->drugTested = drugTested; }
void Meet::setUnits(const Unit units) { this->units = units; }
