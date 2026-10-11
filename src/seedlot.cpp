#include "seedlot.h"

#include <cmath>
#include <cctype>

// Отримання значень полів
const std::string& SeedLot::getVarietyName() const {
    return varietyName_;
}

const std::string& SeedLot::getCrop() const {
    return crop_;
}

int SeedLot::getHarvestYear() const {
    return harvestYear_;
}

double SeedLot::getTotalMassKg() const {
    return totalMassKg_;
}

double SeedLot::getReleasedMassKg() const {
    return releasedMassKg_;
}

const std::string& SeedLot::getBatchCode() const {
    return batchCode_;
}

// Обчислення похідних величин
double SeedLot::getRemainingMassKg() const {
    return totalMassKg_ - releasedMassKg_;
}

bool SeedLot::hasAvailableSeed() const {
    return getRemainingMassKg() > 0.0;
}

// Зміна назви сорту насіння
bool SeedLot::setVarietyName(const std::string& name) {
    if (name.find_first_not_of(" \t\n\r") == std::string::npos) {
        return false;
    }

    const std::string oldName = varietyName_;
    varietyName_ = name;

    if (!checkInvariant()) {
        varietyName_ = oldName;
        return false;
    }

    return true;
}

// Зміна назви сільськогосподарської культури
bool SeedLot::setCrop(const std::string& crop) {
    if (crop.find_first_not_of(" \t\n\r") == std::string::npos) {
        return false;
    }

    const std::string oldCrop = crop_;
    crop_ = crop;

    if (!checkInvariant()) {
        crop_ = oldCrop;
        return false;
    }

    return true;
}

// Зміна року врожаю
bool SeedLot::setHarvestYear(int year) {
    if (year < 1900 || year > 2026) {
        return false;
    }

    const int oldYear = harvestYear_;
    harvestYear_ = year;

    if (!checkInvariant()) {
        harvestYear_ = oldYear;
        return false;
    }

    return true;
}

// Зміна коду партії
bool SeedLot::setBatchCode(const std::string& code) {
    if (!isValidBatchCode(code)) {
        return false;
    }

    const std::string oldCode = batchCode_;
    batchCode_ = code;

    if (!checkInvariant()) {
        batchCode_ = oldCode;
        return false;
    }

    return true;
}

// Відпуск насіння на посів
bool SeedLot::releaseForSowing(double massKg) {
    if (!std::isfinite(massKg) || massKg <= 0.0) {
        return false;
    }

    if (massKg > getRemainingMassKg()) {
        return false;
    }

    const double oldReleasedMass = releasedMassKg_;
    releasedMassKg_ += massKg;

    if (!checkInvariant()) {
        releasedMassKg_ = oldReleasedMass;
        return false;
    }

    return true;
}

// Повернення раніше відпущеного насіння
bool SeedLot::returnToStock(double massKg) {
    if (!std::isfinite(massKg) || massKg <= 0.0) {
        return false;
    }

    if (massKg > releasedMassKg_) {
        return false;
    }

    const double oldReleasedMass = releasedMassKg_;
    releasedMassKg_ -= massKg;

    if (!checkInvariant()) {
        releasedMassKg_ = oldReleasedMass;
        return false;
    }

    return true;
}

// Додавання насіння до загального запасу
bool SeedLot::addStock(double massKg) {
    if (!std::isfinite(massKg) || massKg <= 0.0) {
        return false;
    }

    const double newTotalMass = totalMassKg_ + massKg;

    if (!std::isfinite(newTotalMass)) {
        return false;
    }

    const double oldTotalMass = totalMassKg_;
    totalMassKg_ = newTotalMass;

    if (!checkInvariant()) {
        totalMassKg_ = oldTotalMass;
        return false;
    }

    return true;
}

// Перевірка інваріанта класу
bool SeedLot::checkInvariant() const {
    // Перевіряємо, що назви порожні або містять не лише пробіли.
    const auto containsNonSpace = [](const std::string& value) {
        for (unsigned char ch : value) {
            if (!std::isspace(ch)) {
                return true;
            }
        }
        return false;
    };

    const bool validVarietyName =
        varietyName_.empty() || containsNonSpace(varietyName_);

    const bool validCrop =
        crop_.empty() || containsNonSpace(crop_);

    const bool validYear =
        harvestYear_ >= 1900 && harvestYear_ <= 2026;

    const bool validMasses =
        std::isfinite(totalMassKg_) &&
        std::isfinite(releasedMassKg_) &&
        totalMassKg_ >= 0.0 &&
        releasedMassKg_ >= 0.0 &&
        releasedMassKg_ <= totalMassKg_;

    const bool validCode =
        batchCode_.empty() || isValidBatchCode(batchCode_);

    return validVarietyName &&
           validCrop &&
           validYear &&
           validMasses &&
           validCode;
}

// Перевірка формату коду партії SF-YYYY-NNN
bool SeedLot::isValidBatchCode(const std::string& code) {
    if (code.size() != 11) {
        return false;
    }

    if (code[0] != 'S' || code[1] != 'F' ||
        code[2] != '-' || code[7] != '-') {
        return false;
    }

    for (std::size_t i = 3; i <= 6; ++i) {
        if (!std::isdigit(static_cast<unsigned char>(code[i]))) {
            return false;
        }
    }

    for (std::size_t i = 8; i <= 10; ++i) {
        if (!std::isdigit(static_cast<unsigned char>(code[i]))) {
            return false;
        }
    }

    return true;
}