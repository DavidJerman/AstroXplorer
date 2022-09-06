#ifndef ENUMS_H
#define ENUMS_H

#include <string>
#include <QString>

enum ORIGIN {
    APOD_JSON,
    APOD_IMAGE,
    OPPORTUNITY,
    CURIOSITY,
    SPIRIT,
    FHAZ,
    RHAZ,
    MAST,
    CHEMCAM,
    MAHLI,
    MARDI,
    NAVCAM,
    PANCAM,
    MINITES,
    C_FHAZ,
    C_RHAZ,
    C_MAST,
    C_CHEMCAM,
    C_MAHLI,
    C_MARDI,
    C_NAVCAM,
    C_FHAZ_P,
    C_RHAZ_P,
    C_MAST_P,
    C_CHEMCAM_P,
    C_MAHLI_P,
    C_MARDI_P,
    C_NAVCAM_P,
    O_FHAZ,
    O_RHAZ,
    O_NAVCAM,
    O_PANCAM,
    O_MINITES,
    O_FHAZ_P,
    O_RHAZ_P,
    O_NAVCAM_P,
    O_PANCAM_P,
    O_MINITES_P,
    S_FHAZ,
    S_RHAZ,
    S_NAVCAM,
    S_PANCAM,
    S_MINITES,
    S_FHAZ_P,
    S_RHAZ_P,
    S_NAVCAM_P,
    S_PANCAM_P,
    S_MINITES_P,
    EPIC_JSON,
    EPIC_IMAGE,
    EPIC_NATURAL,
    EPIC_ENCHANCED
};

constexpr unsigned int SIZE = 52;

class E {
    static std::string VALUES[SIZE];
public:
    static std::string eToS(ORIGIN value) {
        return VALUES[value];
    }

    static QString eToQs(ORIGIN value) {
        return QString::fromStdString(VALUES[value]);
    }
};

#endif // ENUMS_H
