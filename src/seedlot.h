#ifndef SEEDLOT_H
#define SEEDLOT_H

#include <string>

class SeedLot {
private:
    // Поля, що описують партію насіння
    std::string varietyName_ = "";
    std::string crop_ = "";
    int harvestYear_ = 2026;
    double totalMassKg_ = 0.0;
    double releasedMassKg_ = 0.0;
    std::string batchCode_ = "";

    // Перевіряє, чи відповідає об'єкт інваріанту
    bool checkInvariant() const;

public:
    // Отримання значень полів
    const std::string& getVarietyName() const;
    const std::string& getCrop() const;
    int getHarvestYear() const;
    double getTotalMassKg() const;
    double getReleasedMassKg() const;
    const std::string& getBatchCode() const;

    // Обчислення похідних величин
    double getRemainingMassKg() const;
    bool hasAvailableSeed() const;

    // Зміна даних із перевіркою допустимих значень
    bool setVarietyName(const std::string& name);
    bool setCrop(const std::string& crop);
    bool setHarvestYear(int year);
    bool setBatchCode(const std::string& code);

    // Операції з масою насіння
    bool releaseForSowing(double massKg);
    bool returnToStock(double massKg);
    bool addStock(double massKg);

    // Статична перевірка формату коду партії
    static bool isValidBatchCode(const std::string& code);
};

#endif