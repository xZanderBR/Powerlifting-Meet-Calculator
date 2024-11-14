#include <gtest/gtest.h>
#include "../Meet.h"
#include "../Lifter.h"

TEST(MeetSortingTest, SortByTotal) {
    Meet meet;
    Lifter lifter1("Lifter A", Gender::MALE, 30, 100.0, 200.0, 150.0, 250.0, Unit::KG);
    Lifter lifter2("Lifter B", Gender::FEMALE, 25, 80.0, 150.0, 100.0, 200.0, Unit::KG);
    lifter1.updateTotal();
    lifter2.updateTotal();
    meet.addLifter(lifter1);
    meet.addLifter(lifter2);

    meet.sortLifters("Total");

    EXPECT_EQ(meet.getLifters()[0].getName(), "Lifter A");
    EXPECT_EQ(meet.getLifters()[1].getName(), "Lifter B");
}

TEST(MeetSortingTest, SortStability) {
    Meet meet;
    Lifter lifter1("Lifter A", Gender::MALE, 30, 100.0, 200.0, 150.0, 250.0, Unit::KG);
    Lifter lifter2("Lifter B", Gender::MALE, 25, 100.0, 200.0, 150.0, 250.0, Unit::KG);

    meet.addLifter(lifter1);
    meet.addLifter(lifter2);

    meet.sortLifters("Total");

    EXPECT_EQ(meet.getLifters()[0].getName(), "Lifter A");
    EXPECT_EQ(meet.getLifters()[1].getName(), "Lifter B");
}
