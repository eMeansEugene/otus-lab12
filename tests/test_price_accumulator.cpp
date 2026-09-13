//
// Created by evgen on 12.09.2026.
//
#include "price_accumulator.h"

#include <gtest/gtest.h>

namespace {

    using mapreduce::PriceAccumulator;

    TEST(PriceAccumulator, InitiallyEmpty) {
        PriceAccumulator acc;
        EXPECT_EQ(acc.Count(), 0u);
    }

    TEST(PriceAccumulator, TextbookMeanAndVariance) {
        // Набор {2,4,4,4,5,5,7,9}: mean = 5, популяционная дисперсия = 4, std = 2.
        PriceAccumulator acc;
        for (long long x : {2, 4, 4, 4, 5, 5, 7, 9}) {
            acc.Add(x);
        }
        EXPECT_EQ(acc.Count(), 8u);
        EXPECT_DOUBLE_EQ(acc.Mean(), 5.0);      // double -> EXPECT_DOUBLE_EQ, не EXPECT_EQ
        EXPECT_DOUBLE_EQ(acc.Variance(), 4.0);
    }

    TEST(PriceAccumulator, SingleValueHasZeroVariance) {
        PriceAccumulator acc;
        acc.Add(10);
        EXPECT_DOUBLE_EQ(acc.Mean(), 10.0);
        EXPECT_DOUBLE_EQ(acc.Variance(), 0.0);
    }

    TEST(PriceAccumulator, ZeroPricesAreValid) {
        PriceAccumulator acc;
        acc.Add(0);
        acc.Add(0);
        acc.Add(0);
        EXPECT_EQ(acc.Count(), 3u);
        EXPECT_DOUBLE_EQ(acc.Mean(), 0.0);
        EXPECT_DOUBLE_EQ(acc.Variance(), 0.0);
    }

} // namespace