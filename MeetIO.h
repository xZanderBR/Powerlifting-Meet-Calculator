#pragma once

#include "Meet.h"
#include <string>
#include <future>

class MeetIO {
public:
    static bool importMeetFromJSON(Meet& meet, const std::string& filename);
    static bool exportMeetToJSON(const Meet& meet, const std::string& filename);

    static std::future<bool> asyncImportMeetFromJSON(Meet& meet, const std::string &filename);
    static std::future<bool> asyncExportMeetToJSON(const Meet& meet, const std::string &filename);

private:
    static void logError(const std::string &message);
};