#include <gtest/gtest.h>
#include "../Lifter.h"

TEST(LifterFormulaCalculationTest, IPFScoreClassicMale) {
    constexpr double bw = 87.2;
    Lifter lifter("John Haack", Gender::MALE, 31, bw, 365.5, 262.5, 385.0, Unit::KG);
    lifter.calculateIPFScore(bw, lifter.getTotal(), false, false);

    EXPECT_NEAR(lifter.getIPFScore(), 1032.16, 0.1);
}

TEST(LifterFormulaCalculationTest, IPFScoreClassicFemale) {
    constexpr double bw = 74.0;
    Lifter lifter("Kristy Hawkins", Gender::FEMALE, 43, bw, 310.0, 152.5, 262.5, Unit::KG);
    lifter.calculateIPFScore(bw, lifter.getTotal(), false, false);

    EXPECT_NEAR(lifter.getIPFScore(), 1170.93, 0.1);
}

TEST(LifterFormulaCalculationTest, GLScoreMaleClassic) {
    constexpr double bw = 87.2;
    Lifter lifter("John Haack", Gender::MALE, 31, bw, 365.5, 262.5, 385.0, Unit::KG);
    lifter.calculateIPFGLScore(bw, lifter.getTotal(), false, false);

    EXPECT_NEAR(lifter.getIPFGLScore(), 136.8, 0.1);
}

TEST(LifterFormulaCalculationTest, GLScoreMaleEquipped) {
    constexpr double bw = 90;
    Lifter lifter("Kalle Rasanen", Gender::MALE, 29, bw, 420, 337.5, 373.5, Unit::KG);
    lifter.calculateIPFGLScore(bw, lifter.getTotal(), true, false);

    EXPECT_NEAR(lifter.getIPFGLScore(), 124.8, 0.1);
}

TEST(LifterFormulaCalculationTest, GLScoreMaleBenchOnlyClassic) {
    constexpr double bw = 89;
    Lifter lifter("Andrey Sapozhonkov", Gender::MALE, 34, bw, 0.0, 280.0, 0.0, Unit::KG);
    lifter.calculateIPFGLScore(bw, lifter.getTotal(), false, true);

    EXPECT_NEAR(lifter.getIPFGLScore(), 135.76, 0.1);
}

TEST(LifterFormulaCalculationTest, GLScoreMaleBenchOnlyEquipped) {
    constexpr double bw = 134.9;
    Lifter lifter("Jimmy Kolb", Gender::MALE, 31, bw, 0.0, 508.0, 0.0, Unit::KG);
    lifter.calculateIPFGLScore(bw, lifter.getTotal(), true, true);

    EXPECT_NEAR(lifter.getIPFGLScore(), 144.16, 0.1);
}

TEST(LifterFormulaCalculationTest, GLScoreFemaleClassic) {
    constexpr double bw = 74.0;
    Lifter lifter("Kristy Hawkins", Gender::FEMALE, 43, bw, 310.0, 152.5, 262.5, Unit::KG);
    lifter.calculateIPFGLScore(bw, lifter.getTotal(), false, false);

    EXPECT_NEAR(lifter.getIPFGLScore(), 144.79, 0.1);
}

TEST(LifterFormulaCalculationTest, GLScoreFemaleEquipped) {
    constexpr double bw = 76.0;
    Lifter lifter("Agata Sitko", Gender::FEMALE, 19, bw, 270.0, 195.0, 261.0, Unit::KG);
    lifter.calculateIPFGLScore(bw, lifter.getTotal(), true, false);

    EXPECT_NEAR(lifter.getIPFGLScore(), 119.12, 0.1);
}

TEST(LifterFormulaCalculationTest, GLScoreFemaleBenchOnlyClassic) {
    constexpr double bw = 120.2;
    Lifter lifter("April Mathis", Gender::FEMALE, 28, bw, 0.0, 205.0, 0.0, Unit::KG);
    lifter.calculateIPFGLScore(bw, lifter.getTotal(), false, true);

    EXPECT_NEAR(lifter.getIPFGLScore(), 145.5, 0.1);
}

TEST(LifterFormulaCalculationTest, GLScoreFemaleBenchOnlyEquipped) {
    constexpr double bw = 126.5;
    Lifter lifter("Rochelle Dubbs", Gender::FEMALE, 47, bw, 0.0, 259.0, 0.0, Unit::KG);
    lifter.calculateIPFGLScore(bw, lifter.getTotal(), true, true);

    EXPECT_NEAR(lifter.getIPFGLScore(), 121.52, 0.1);
}

TEST(LifterFormulaCalculationTest, DOTSScoreMale) {
    constexpr double bw = 87.2;
    Lifter lifter("John Haack", Gender::MALE, 31, bw, 365.5, 262.5, 385.0, Unit::KG);
    lifter.calculateDOTSScore(bw, lifter.getTotal());

    EXPECT_NEAR(lifter.getDOTSScore(), 665.75, 0.1);
}

TEST(LifterFormulaCalculationTest, DOTSScoreFemale) {
    constexpr double bw = 74.0;
    Lifter lifter("Kristy Hawkins", Gender::FEMALE, 43, bw, 310.0, 152.5, 262.5, Unit::KG);
    lifter.calculateDOTSScore(bw, lifter.getTotal());

    EXPECT_NEAR(lifter.getDOTSScore(), 711.19, 0.1);
}

TEST(LifterFormulaCalculationTest, WilksOldScoreMale) {
    constexpr double bw = 87.2;
    Lifter lifter("John Haack", Gender::MALE, 31, bw, 365.5, 262.5, 385.0, Unit::KG);
    lifter.calculateWILKSOldScore(bw, lifter.getTotal());

    EXPECT_NEAR(lifter.getWILKSOldScore(), 657.51, 0.1);
}

TEST(LifterFormulaCalculationTest, WilksOldScoreFemale) {
    constexpr double bw = 74.0;
    Lifter lifter("Kristy Hawkins", Gender::FEMALE, 43, bw, 310.0, 152.5, 262.5, Unit::KG);
    lifter.calculateWILKSOldScore(bw, lifter.getTotal());

    EXPECT_NEAR(lifter.getWILKSOldScore(), 695.09, 0.1);
}

TEST(LifterFormulaCalculationTest, WilksScoreMale) {
    constexpr double bw = 87.2;
    Lifter lifter("John Haack", Gender::MALE, 31, bw, 365.5, 262.5, 385.0, Unit::KG);
    lifter.calculateWILKSScore(bw, lifter.getTotal());

    EXPECT_NEAR(lifter.getWILKSScore(), 790.04, 0.1);
}

TEST(LifterFormulaCalculationTest, WilksScoreFemale) {
    constexpr double bw = 74.0;
    Lifter lifter("Kristy Hawkins", Gender::FEMALE, 43, bw, 310.0, 152.5, 262.5, Unit::KG);
    lifter.calculateWILKSScore(bw, lifter.getTotal());

    EXPECT_NEAR(lifter.getWILKSScore(), 849.19, 0.1);
}
