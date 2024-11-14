#include <gtest/gtest.h>
#include "../Meet.h"
#include "../Serialization.h"

TEST(MeetSerialization, ExportAndImport) {
    Meet originalMeet;
    originalMeet.setName("Test Meet");
    originalMeet.setDate("11-10-2024");
    originalMeet.setLocation("Test Location");
    originalMeet.setFederation(Federation::IPF);
    originalMeet.setDrugTested(true);
    originalMeet.setBenchOnly(false);
    originalMeet.setEquipClass(EquipmentClass::RAW);
    originalMeet.setUnits(Unit::KG);

    Lifter lifter1("John Doe", Gender::MALE, 30, 100.0f, 200.0f, 150.0f, 250.0f, Unit::KG);
    Lifter lifter2("Jane Smith", Gender::FEMALE, 25, 80.0f, 150.0f, 100.0f, 200.0f, Unit::KG);
    originalMeet.addLifter(lifter1);
    originalMeet.addLifter(lifter2);

    // Export to JSON string instead of file for testing
    nlohmann::json j = originalMeet;
    Meet importedMeet = j.get<Meet>();

    EXPECT_EQ(originalMeet.getName(), importedMeet.getName());
    EXPECT_EQ(originalMeet.getDate(), importedMeet.getDate());
    EXPECT_EQ(originalMeet.getLocation(), importedMeet.getLocation());
    EXPECT_EQ(originalMeet.getFederation(), importedMeet.getFederation());
    EXPECT_EQ(originalMeet.getDrugTested(), importedMeet.getDrugTested());
    EXPECT_EQ(originalMeet.getBenchOnly(), importedMeet.getBenchOnly());
    EXPECT_EQ(originalMeet.getEquipClass(), importedMeet.getEquipClass());
    EXPECT_EQ(originalMeet.getUnits(), importedMeet.getUnits());

    const auto& originalLifters = originalMeet.getLifters();
    const auto& importedLifters = importedMeet.getLifters();
    ASSERT_EQ(originalLifters.size(), importedLifters.size());

    for (size_t i = 0; i < originalLifters.size(); ++i) {
        EXPECT_EQ(originalLifters[i].getName(), importedLifters[i].getName());
        EXPECT_EQ(originalLifters[i].getGender(), importedLifters[i].getGender());
        EXPECT_EQ(originalLifters[i].getAge(), importedLifters[i].getAge());
        EXPECT_FLOAT_EQ(originalLifters[i].getBodyweight(), importedLifters[i].getBodyweight());
        EXPECT_FLOAT_EQ(originalLifters[i].getSquat(), importedLifters[i].getSquat());
        EXPECT_FLOAT_EQ(originalLifters[i].getBench(), importedLifters[i].getBench());
        EXPECT_FLOAT_EQ(originalLifters[i].getDeadlift(), importedLifters[i].getDeadlift());
        EXPECT_EQ(originalLifters[i].getUnits(), importedLifters[i].getUnits());
    }
}
