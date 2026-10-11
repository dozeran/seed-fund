#include <gtest/gtest.h>
#include <limits>

#include "seedlot.h"

// Тест 1. Перевірка початкових значень
TEST(SeedLotTest, HasCorrectInitialValues) {
    SeedLot lot;

    EXPECT_EQ(lot.getVarietyName(), "");
    EXPECT_EQ(lot.getCrop(), "");
    EXPECT_EQ(lot.getHarvestYear(), 2026);
    EXPECT_DOUBLE_EQ(lot.getTotalMassKg(), 0.0);
    EXPECT_DOUBLE_EQ(lot.getReleasedMassKg(), 0.0);
    EXPECT_DOUBLE_EQ(lot.getRemainingMassKg(), 0.0);
    EXPECT_FALSE(lot.hasAvailableSeed());
}

// Тест 2. Встановлення назви сорту
TEST(SeedLotTest, SetsVarietyName) {
    SeedLot lot;

    EXPECT_TRUE(lot.setVarietyName("Богдана"));
    EXPECT_EQ(lot.getVarietyName(), "Богдана");
}

// Тест 3. Відхилення порожньої назви сорту
TEST(SeedLotTest, RejectsEmptyVarietyName) {
    SeedLot lot;

    EXPECT_FALSE(lot.setVarietyName(""));
    EXPECT_EQ(lot.getVarietyName(), "");
}

// Тест 4. Встановлення назви культури
TEST(SeedLotTest, SetsCrop) {
    SeedLot lot;

    EXPECT_TRUE(lot.setCrop("Пшениця"));
    EXPECT_EQ(lot.getCrop(), "Пшениця");
}

// Тест 5. Встановлення року врожаю
TEST(SeedLotTest, SetsHarvestYear) {
    SeedLot lot;

    EXPECT_TRUE(lot.setHarvestYear(2025));
    EXPECT_EQ(lot.getHarvestYear(), 2025);
}

// Тест 6. Відхилення недопустимого року.
TEST(SeedLotTest, RejectsInvalidHarvestYear) {
    SeedLot lot;

    EXPECT_FALSE(lot.setHarvestYear(1800));
    EXPECT_EQ(lot.getHarvestYear(), 2026);
}

// Тест 7. Перевірка правильного коду партії
TEST(SeedLotTest, AcceptsValidBatchCode) {
    EXPECT_TRUE(SeedLot::isValidBatchCode("SF-2026-001"));
}

// Тест 8. Перевірка неправильного коду партії.
TEST(SeedLotTest, RejectsInvalidBatchCode) {
    EXPECT_FALSE(SeedLot::isValidBatchCode("SF-26-001"));
    EXPECT_FALSE(SeedLot::isValidBatchCode("AB-2026-001"));
    EXPECT_FALSE(SeedLot::isValidBatchCode("SF-2026-01A"));
}

// Тест 9. Додавання насіння до запасу
TEST(SeedLotTest, AddsStock) {
    SeedLot lot;

    EXPECT_TRUE(lot.addStock(100.0));
    EXPECT_DOUBLE_EQ(lot.getTotalMassKg(), 100.0);
    EXPECT_DOUBLE_EQ(lot.getRemainingMassKg(), 100.0);
}

// Тест 10. Відпуск насіння на посів
TEST(SeedLotTest, ReleasesSeedForSowing) {
    SeedLot lot;

    lot.addStock(100.0);

    EXPECT_TRUE(lot.releaseForSowing(30.0));
    EXPECT_DOUBLE_EQ(lot.getReleasedMassKg(), 30.0);
    EXPECT_DOUBLE_EQ(lot.getRemainingMassKg(), 70.0);
}

// Тест 11. Відмова у відпуску надмірної маси
TEST(SeedLotTest, RejectsReleaseAboveAvailableStock) {
    SeedLot lot;

    lot.addStock(100.0);

    EXPECT_FALSE(lot.releaseForSowing(120.0));
    EXPECT_DOUBLE_EQ(lot.getReleasedMassKg(), 0.0);
    EXPECT_DOUBLE_EQ(lot.getRemainingMassKg(), 100.0);
}

// Тест 12. Повернення насіння до запасу
TEST(SeedLotTest, ReturnsSeedToStock) {
    SeedLot lot;

    lot.addStock(100.0);
    lot.releaseForSowing(30.0);

    EXPECT_TRUE(lot.returnToStock(10.0));
    EXPECT_DOUBLE_EQ(lot.getReleasedMassKg(), 20.0);
    EXPECT_DOUBLE_EQ(lot.getRemainingMassKg(), 80.0);
}

// Тест 13. Відмова у поверненні надмірної маси
TEST(SeedLotTest, RejectsReturningTooMuchSeed) {
    SeedLot lot;

    lot.addStock(100.0);
    lot.releaseForSowing(20.0);

    EXPECT_FALSE(lot.returnToStock(30.0));
    EXPECT_DOUBLE_EQ(lot.getReleasedMassKg(), 20.0);
}

// Тест 14. Перевірка відсутності насіння
TEST(SeedLotTest, ReportsNoAvailableSeed) {
    SeedLot lot;

    lot.addStock(50.0);

    EXPECT_TRUE(lot.releaseForSowing(50.0));
    EXPECT_DOUBLE_EQ(lot.getRemainingMassKg(), 0.0);
    EXPECT_FALSE(lot.hasAvailableSeed());
}

// Тест 15. Відхилення нульової та від'ємної маси
TEST(SeedLotTest, RejectsNonPositiveMass) {
    SeedLot lot;

    EXPECT_FALSE(lot.addStock(0.0));
    EXPECT_FALSE(lot.addStock(-10.0));

    EXPECT_DOUBLE_EQ(lot.getTotalMassKg(), 0.0);
}

// Тест 16. Відхилення нескінченної маси
TEST(SeedLotTest, RejectsInfiniteMass) {
    SeedLot lot;

    EXPECT_FALSE(lot.addStock(
        std::numeric_limits<double>::infinity()
    ));

    EXPECT_DOUBLE_EQ(lot.getTotalMassKg(), 0.0);
}

// Тест 17. Невдалий відпуск не змінює стан партії
TEST(SeedLotTest, FailedReleaseDoesNotChangeState) {
    SeedLot lot;

    ASSERT_TRUE(lot.addStock(100.0));
    ASSERT_TRUE(lot.releaseForSowing(30.0));

    EXPECT_FALSE(lot.releaseForSowing(80.0));

    EXPECT_DOUBLE_EQ(lot.getTotalMassKg(), 100.0);
    EXPECT_DOUBLE_EQ(lot.getReleasedMassKg(), 30.0);
    EXPECT_DOUBLE_EQ(lot.getRemainingMassKg(), 70.0);
}

// Тест 18. Невдале повернення не змінює стан партії
TEST(SeedLotTest, FailedReturnDoesNotChangeState) {
    SeedLot lot;

    ASSERT_TRUE(lot.addStock(100.0));
    ASSERT_TRUE(lot.releaseForSowing(20.0));

    EXPECT_FALSE(lot.returnToStock(30.0));

    EXPECT_DOUBLE_EQ(lot.getTotalMassKg(), 100.0);
    EXPECT_DOUBLE_EQ(lot.getReleasedMassKg(), 20.0);
    EXPECT_DOUBLE_EQ(lot.getRemainingMassKg(), 80.0);
}