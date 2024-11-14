#include <gtest/gtest.h>
#include "../Meet.h"
#include "../Lifter.h"

TEST(MeetTest, DefaultConstructor) {
    Meet meet;
    EXPECT_EQ(meet.getName(), "");
    EXPECT_EQ(meet.getDate(), "");
    EXPECT_EQ(meet.getLocation(), "");
    EXPECT_TRUE(meet.getLifters().empty());
}

TEST(MeetTest, SettersAndGetters) {
    Meet meet;
    meet.setName("Power Meet");
    meet.setDate("2024-01-01");
    meet.setLocation("City Arena");
    meet.setFederation(Federation::IPF);
    meet.setEquipClass(EquipmentClass::EQUIPPED);
    meet.setBenchOnly(true);
    meet.setDrugTested(true);
    meet.setUnits(Unit::KG);

    EXPECT_EQ(meet.getName(), "Power Meet");
    EXPECT_EQ(meet.getDate(), "2024-01-01");
    EXPECT_EQ(meet.getLocation(), "City Arena");
    EXPECT_EQ(meet.getFederation(), Federation::IPF);
    EXPECT_EQ(meet.getEquipClass(), EquipmentClass::EQUIPPED);
    EXPECT_TRUE(meet.getBenchOnly());
    EXPECT_TRUE(meet.getDrugTested());
    EXPECT_EQ(meet.getUnits(), Unit::KG);
}

TEST(MeetTest, AddAndRemoveLifters) {
    Meet meet;
    Lifter lifter1("John Doe", Gender::MALE, 30, 100.0, 200.0, 150.0, 250.0, Unit::KG);
    Lifter lifter2("Jane Smith", Gender::FEMALE, 25, 80.0, 150.0, 100.0, 200.0, Unit::KG);

    meet.addLifter(lifter1);
    meet.addLifter(lifter2);

    EXPECT_EQ(meet.getLifters().size(), 2);
    meet.removeLifter(lifter1);
    EXPECT_EQ(meet.getLifters().size(), 1);
    meet.clearLifters();
    EXPECT_TRUE(meet.getLifters().empty());
}
