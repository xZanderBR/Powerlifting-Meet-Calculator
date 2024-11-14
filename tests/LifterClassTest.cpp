#include <gtest/gtest.h>
#include "../Lifter.h"

TEST(LifterTest, DefaultConstructor) {
    Lifter lifter;
    EXPECT_EQ(lifter.getName(), "");
    EXPECT_EQ(lifter.getAge(), 0);
    EXPECT_EQ(lifter.getBodyweight(), 0.0);
    EXPECT_EQ(lifter.getTotal(), 0.0);
}

TEST(LifterTest, GettersAndSetters) {
    Lifter lifter;
    lifter.setName("Kristy Hawkins");
    lifter.setGender(Gender::FEMALE);
    lifter.setAge(43);
    lifter.setBodyweight(74.0);
    lifter.setTotal(725.0);

    EXPECT_EQ(lifter.getName(), "Kristy Hawkins");
    EXPECT_EQ(lifter.getGender(), Gender::FEMALE);
    EXPECT_EQ(lifter.getAge(), 43);
    EXPECT_DOUBLE_EQ(lifter.getBodyweight(), 74.0);
    EXPECT_DOUBLE_EQ(lifter.getTotal(), 725.0);
}

TEST(LifterTest, UpdateTotal) {
    Lifter lifter;
    lifter.setSquat(100.0);
    lifter.setBench(80.0);
    lifter.setDeadlift(120.0);

    EXPECT_DOUBLE_EQ(lifter.getTotal(), 300.0);
}
