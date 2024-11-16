#pragma once

#include "Meet.h"
#include "Lifter.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Serialize Gender enum
NLOHMANN_JSON_SERIALIZE_ENUM(Gender, {
    {Gender::MALE, "Male"},
    {Gender::FEMALE, "Female"},
})

// Serialize Unit enum
NLOHMANN_JSON_SERIALIZE_ENUM(Unit, {
    {Unit::KG, "KG"},
    {Unit::LBS, "LBS"}
})

// Serialize Federation enum
NLOHMANN_JSON_SERIALIZE_ENUM(Federation, {
    {Federation::IPF, "IPF"},
    {Federation::USPA, "USPA"},
    {Federation::USAPL, "USAPL"},
    {Federation::WRPF, "WRPF"},
    {Federation::OTHER, "OTHER"}
})

// Serialize EquipmentClass enum
NLOHMANN_JSON_SERIALIZE_ENUM(EquipmentClass, {
    {EquipmentClass::RAW, "RAW"},
    {EquipmentClass::EQUIPPED, "EQUIPPED"}
})

// Lifter Serialization
void to_json(json &j, const Lifter &lifter);
void from_json(const json &j, Lifter &lifter);

// Meet Serialization
void to_json(json &j, const Meet &meet);
void from_json(const json &j, Meet &meet);