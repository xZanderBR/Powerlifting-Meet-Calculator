#include "Lifter.h"
#include <cmath>

struct Coefficients {
    double a = 0;
    double b = 0;
    double c = 0;
    double d = 0;
    double e = 0;
    double f = 0;
};

void Lifter::calculateIPFScore(const double bw, const double total, const bool equipped,
                               const bool benchOnly = false) {
    // https://www.powerlifting.sport/fileadmin/ipf/data/ipf-formula/Models_Evaluation-I-2020.pdf

    constexpr Coefficients maleCoeffsClassic = {310.6700, 857.7850, 53.2160, 147.0835};
    constexpr Coefficients maleCoeffsBenchClassic = {86.4745, 259.1550, 17.5785, 53.1220};
    constexpr Coefficients maleCoeffsEquipped = {387.2650, 1121.2800, 80.6324, 222.4896};
    constexpr Coefficients maleCoeffsBenchEquipped = {133.9400, 441.4650, 35.3938, 113.0057};

    constexpr Coefficients femaleCoeffsClassic = {125.1435, 228.0300, 34.5246, 86.8301};
    constexpr Coefficients femaleCoeffsBenchClassic = {25.0485, 43.8480, 6.7172, 13.9520};
    constexpr Coefficients femaleCoeffsEquipped = {176.5800, 373.3150, 48.4534, 110.0103};
    constexpr Coefficients femaleCoeffsBenchEquipped = {49.1060, 124.2090, 23.1990, 67.4926};

    Coefficients coeff;

    if (gender == Gender::MALE && !equipped && !benchOnly) {
        coeff = maleCoeffsClassic;
    } else if (gender == Gender::MALE && equipped && !benchOnly) {
        coeff = maleCoeffsEquipped;
    } else if (gender == Gender::MALE && !equipped) {
        coeff = maleCoeffsBenchClassic;
    } else if (gender == Gender::MALE) {
        coeff = maleCoeffsBenchEquipped;
    } else if (gender == Gender::FEMALE && !equipped && !benchOnly) {
        coeff = femaleCoeffsClassic;
    } else if (gender == Gender::FEMALE && equipped && !benchOnly) {
        coeff = femaleCoeffsEquipped;
    } else if (gender == Gender::FEMALE && !equipped) {
        coeff = femaleCoeffsBenchClassic;
    } else {
        coeff = femaleCoeffsBenchEquipped;
    }

    const double ipf_num = total - (coeff.a * log(bw) - coeff.b);
    const double ipf_denom = coeff.c * log(bw) - coeff.d;
    ipf_score = std::max(500 + 100 * (ipf_num / ipf_denom), 0.0);
}

void Lifter::calculateIPFGLScore(const double bw, const double total, const bool equipped,
                                 const bool benchOnly = false) {
    // https://www.powerlifting.sport/fileadmin/ipf/data/ipf-formula/IPF_GL_Coefficients-2020.pdf

    if (bodyweight < 35) { return; }
    constexpr Coefficients maleCoeffsClassic = {1199.72839, 1025.18162, 0.00921};
    constexpr Coefficients maleCoeffsBenchClassic = {320.98041, 281.40258, 0.01008};
    constexpr Coefficients maleCoeffsEquipped = {1236.25115, 1449.21864, 0.01644};
    constexpr Coefficients maleCoeffsBenchEquipped = {381.22073, 733.79378, 0.02398};

    constexpr Coefficients femaleCoeffsClassic = {610.32796, 1045.59282, 0.03048};
    constexpr Coefficients femaleCoeffsBenchClassic = {142.40398, 442.52671, 0.04724};
    constexpr Coefficients femaleCoeffsEquipped = {758.63878, 949.31382, 0.02435};
    constexpr Coefficients femaleCoeffsBenchEquipped = {221.82209, 357.00377, 0.02937};

    Coefficients coeff;

    if (gender == Gender::MALE && !equipped && !benchOnly) {
        coeff = maleCoeffsClassic;
    } else if (gender == Gender::MALE && equipped && !benchOnly) {
        coeff = maleCoeffsEquipped;
    } else if (gender == Gender::MALE && !equipped) {
        coeff = maleCoeffsBenchClassic;
    } else if (gender == Gender::MALE) {
        coeff = maleCoeffsBenchEquipped;
    } else if (gender == Gender::FEMALE && !equipped && !benchOnly) {
        coeff = femaleCoeffsClassic;
    } else if (gender == Gender::FEMALE && equipped && !benchOnly) {
        coeff = femaleCoeffsEquipped;
    } else if (gender == Gender::FEMALE && !equipped) {
        coeff = femaleCoeffsBenchClassic;
    } else {
        coeff = femaleCoeffsBenchEquipped;
    }

    const double gl_denom = coeff.a - coeff.b * std::exp(-coeff.c * bw);
    ipf_gl_score = total * 100 / gl_denom;
}

void Lifter::calculateDOTSScore(const double bw, const double total) {
    // https://drive.google.com/file/d/1GG22jYL3JaalTUydcigkZNCjddtH-0rv/view

    constexpr Coefficients maleCoeffs = {-0.000001093, 0.0007391293, -0.1918759221, 24.0900756, -307.75076};
    constexpr Coefficients femaleCoeffs = {-0.0000010706, 0.0005158568, -0.1126655495, 13.6175032, -57.96288};

    // Ternary operator to choose the right coefficients based on gender
    const Coefficients coeff = gender == Gender::MALE ? maleCoeffs : femaleCoeffs;

    const double dots_denom = coeff.a * std::pow(bw, 4) + coeff.b * std::pow(bw, 3) + coeff.c * std::pow(bw, 2) +
                              coeff.d * bw + coeff.e;
    dots_score = std::max(total * 500 / dots_denom, 0.0);
}

void Lifter::calculateWILKSOldScore(const double bw, const double total) {
    // https://www.omnicalculator.com/sports/wilks

    const Coefficients maleCoeffs = {
        -216.0475144,
        16.2606339,
        -0.002388645,
        -0.00113732,
        7.01863 * std::pow(10, -6),
        -1.291 * std::pow(10, -8)
    };
    const Coefficients femaleCoeffs = {
        594.31747775582,
        -27.23842536447,
        0.82112226871,
        -0.00930733913,
        4.731582 * std::pow(10, -5),
        -9.054 * std::pow(10, -8)
    };

    // Ternary operator to choose the right coefficients based on gender
    const Coefficients coeff = gender == Gender::MALE ? maleCoeffs : femaleCoeffs;

    const double wilks_denom = coeff.a + coeff.b * bw + coeff.c * pow(bw, 2) + coeff.d * pow(bw, 3) + coeff.e *
                               pow(bw, 4) + coeff.f * pow(bw, 5);
    wilks_old_score = std::max(total * 500 / wilks_denom, 0.0);
}

void Lifter::calculateWILKSScore(const double bw, const double total) {
    // https://worldpowerlifting.com/wilks-formula/

    const Coefficients maleCoeffs = {
        47.46178854,
        8.472061379,
        0.07369410346,
        -0.001395833811,
        7.07665973070743 * std::pow(10, -6),
        -1.20804336482315 * std::pow(10, -8)
    };
    const Coefficients femaleCoeffs = {
        -125.4255398,
        13.71219419,
        -0.03307250631,
        -0.001050400051,
        9.38773881462799 * std::pow(10, -6),
        -2.3334613884954 * std::pow(10, -8)
    };

    // Ternary operator to choose the right coefficients based on gender
    const Coefficients coeff = gender == Gender::MALE ? maleCoeffs : femaleCoeffs;

    const double wilks_denom = coeff.a + coeff.b * bw + coeff.c * pow(bw, 2) + coeff.d * pow(bw, 3) + coeff.e *
                               pow(bw, 4) + coeff.f * pow(bw, 5);
    wilks_score = total * 600 / wilks_denom;
}

double Lifter::convertToKG(const double value) const {
    return units == Unit::KG ? value : value / KG_TO_LBS;
}

double Lifter::convertToLBS(const double value) const {
    return units == Unit::LBS ? value : value * KG_TO_LBS;
}

void Lifter::calculateScores(const bool equipped) {
    constexpr double MIN_BW = 40;
    constexpr double MAX_MALE_BW = 210;
    constexpr double MAX_FEMALE_BW = 150;

    const double lifted = convertToKG(total);
    const double bw = convertToKG(bodyweight);

    if (bw < MIN_BW || bw > MAX_MALE_BW && gender == Gender::MALE || bw > MAX_FEMALE_BW && gender == Gender::FEMALE) {
        return;
    }

    calculateDOTSScore(bw, lifted);
    calculateIPFScore(bw, lifted, equipped);
    calculateIPFGLScore(bw, lifted, equipped);
    calculateWILKSScore(bw, lifted);
    calculateWILKSOldScore(bw, lifted);
}

void Lifter::convertUnits() {
    if (units == Unit::KG) {
        bodyweight = convertToLBS(bodyweight);
        squat = convertToLBS(squat);
        bench = convertToLBS(bench);
        deadlift = convertToLBS(deadlift);
        total = convertToLBS(total);
        units = Unit::LBS;
    } else {
        bodyweight = convertToKG(bodyweight);
        squat = convertToKG(squat);
        bench = convertToKG(bench);
        deadlift = convertToKG(deadlift);
        total = convertToKG(total);
        units = Unit::KG;
    }
}

void Lifter::updateTotal() { total = squat + bench + deadlift; }

// Getters
std::string Lifter::getName() const { return name; }
int Lifter::getAge() const { return age; }
Gender Lifter::getGender() const { return gender; }
double Lifter::getBodyweight() const { return bodyweight; }
double Lifter::getSquat() const { return squat; }
double Lifter::getBench() const { return bench; }
double Lifter::getDeadlift() const { return deadlift; }
double Lifter::getTotal() const { return total; }
double Lifter::getIPFScore() const { return ipf_score; }
double Lifter::getIPFGLScore() const { return ipf_gl_score; }
double Lifter::getDOTSScore() const { return dots_score; }
double Lifter::getWILKSScore() const { return wilks_score; }
double Lifter::getWILKSOldScore() const { return wilks_old_score; }
bool Lifter::getSteroidUse() const { return steroids; }
Unit Lifter::getUnits() const { return units; }

// Setters
void Lifter::setName(const std::string &name) { this->name = name; }
void Lifter::setGender(const Gender gender) { this->gender = gender; }
void Lifter::setAge(const int age) { this->age = age; }
void Lifter::setBodyweight(const double bodyweight) { this->bodyweight = bodyweight; }
void Lifter::setTotal(const double total) { this->total = total; }
void Lifter::setIPFScore(const double ipf_score) { this->ipf_score = ipf_score; }
void Lifter::setIPFGLScore(const double ipf_gl_score) { this->ipf_gl_score = ipf_gl_score; }
void Lifter::setDOTSScore(const double dots_score) { this->dots_score = dots_score; }
void Lifter::setWILKSScore(const double wilks_score) { this->wilks_score = wilks_score; }
void Lifter::setWILKSOldScore(const double wilks_old_score) { this->wilks_old_score = wilks_old_score; }
void Lifter::setUnits(const Unit units) { this->units = units; }

void Lifter::setSquat(const double squat) {
    this->squat = squat;
    updateTotal();
}

void Lifter::setBench(const double bench) {
    this->bench = bench;
    updateTotal();
}

void Lifter::setDeadlift(const double deadlift) {
    this->deadlift = deadlift;
    updateTotal();
}
