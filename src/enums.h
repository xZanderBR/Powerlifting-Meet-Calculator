#ifndef ENUMS_H
#define ENUMS_H

// Enum class for Units (Kilograms or Pounds)
enum class Unit {
    KG,
    LBS
};

// Enum class for Gender
enum class Gender {
    MALE,
    FEMALE
};

// Enum class for Equipment Class (Raw, Wraps, or Equipped)
enum class EquipmentClass {
    RAW,
    EQUIPPED
};

// Enum class for Federation (IPF, USPA, etc.)
enum class Federation {
    IPF,
    USPA,
    USAPL,
    WRPF,
    OTHER
};

#endif //ENUMS_H
