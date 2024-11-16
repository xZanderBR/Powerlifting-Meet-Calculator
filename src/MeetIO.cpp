#include "MeetIO.h"

#include "Serialization.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

bool MeetIO::importMeetFromJSON(Meet &meet, const std::string &filename) {
    std::ifstream inFile(filename);

    if (!inFile.is_open()) {
        logError("Failed to open" + filename + " for reading");
        return false;
    }

    try {
        json j;
        inFile >> j;
        meet = j.get<Meet>();
    } catch (const std::exception &e) {
        logError("Error parsing JSON: " + std::string(e.what()));
        return false;
    }

    inFile.close();
    return true;
}

bool MeetIO::exportMeetToJSON(const Meet &meet, const std::string &filename) {
    const json j = meet;
    std::ofstream outFile(filename);

    if (!outFile.is_open()) {
        logError("Failed to open" + filename + " for writing");
        return false;
    }

    outFile << j.dump(4);
    outFile.close();

    return true;
}

std::future<bool> MeetIO::asyncExportMeetToJSON(const Meet &meet, const std::string &filename) {
    return std::async(std::launch::async, exportMeetToJSON, std::cref(meet), filename);
}

std::future<bool> MeetIO::asyncImportMeetFromJSON(Meet &meet, const std::string &filename) {
    return std::async(std::launch::async, importMeetFromJSON, std::ref(meet), filename);
}

void MeetIO::logError(const std::string &message) {
    std::cerr << message << std::endl;
}
