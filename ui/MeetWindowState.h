#pragma once

#include <string>

struct MeetWindowState {
    // Meet options
    std::string meetName;
    std::string date;
    std::string location;
    int fedIndex = 0;
    bool benchOnly = false;
    bool equipped = false;
    bool drugTested = true;

    // Lifter inputs
    std::string lifterName;
    int age = 0;
    double bodyweight = 0.0;
    double squat = 0.0;
    double bench = 0.0;
    double deadlift = 0.0;
    int genderIndex = 0;  // 0: Male, 1: Female
    int unitIndex = 0;    // 0: KGS, 1: LBS

    // Update Inputs
    std::string updateName;
    int updateAge = 0;
    double updateBodyweight = 0.0;
    double updateSquat = 0.0;
    double updateBench = 0.0;
    double updateDeadlift = 0.0;
    int updateGenderIndex = 0;  // 0: Male, 1: Female
    int updateUnitIndex = 0;    // 0: KGS, 1: LBS
    bool isDataLoaded = false;

    // Sorting and Units
    int sortingIndex = 4; // 0: Squat, 1: Bench, 2: Deadlift, 3: Total, 4: IPF, 5: IPF_GL, 6: DOTS, 7: Wilks, 8: Wilks2
    int previousSortingIndex = -1;
    int meetUnitIndex = 0;  // 0: KGS, 1: LBS
    int previousMeetUnitIndex = -1;

    // Options arrays
    const char* genders[2] = { "Male", "Female" };
    const char* units[2] = { "KG", "LBS" };
    const char* fedOptions[5] = { "IPF", "USPA", "USAPL", "WRPF", "OTHER" };
    const char* sortOptions[7] = { "Squat", "Bench", "Deadlift", "Total", "DOTS", "GL Points", "Wilks"};
};