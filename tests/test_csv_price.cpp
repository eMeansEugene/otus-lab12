//
// Created by evgen on 12.09.2026.
//
#include "csv_price.h"

#include <gtest/gtest.h>

namespace {

using mapreduce::PriceField;

// --- Позитивные кейсы: цена извлекается корректно, включая битые строки ---

TEST(PriceField, PlainRow) {
    auto p = PriceField(
        "2595,Skylit Midtown Castle,2845,Jennifer,Manhattan,Midtown,"
        "40.75362,-73.98377,Entire home/apt,225,1,45,2019-05-21,0.38,2,355");
    ASSERT_TRUE(p.has_value());
    EXPECT_EQ(*p, "225");
}

TEST(PriceField, QuotedCommaInName) {
    // Запятая внутри name сдвигает поля слева, но price = 7-е с конца.
    auto p = PriceField(
        "8045552,\"A Gem Studio Apartment Astoria,NYC\",42488595,Tony,Queens,"
        "Astoria,40.76762,-73.90727,Entire home/apt,87,3,151,2019-06-17,3.32,1,210");
    ASSERT_TRUE(p.has_value());
    EXPECT_EQ(*p, "87");
}

TEST(PriceField, SemicolonMixedRow) {
    // Смешанный разделитель ';' + экранированные кавычки; хвост всё равно чист.
    auto p = PriceField(
        "729306,\"Clean & Quiet BR in Sunset Park; BK\";3787686;"
        "\"Porfirio \"\"Firo\"\" & Maria\",Brooklyn,Borough Park,40.64431,-74.00016,"
        "Private room,70,1,139,2019-06-04,1.70,1,364");
    ASSERT_TRUE(p.has_value());
    EXPECT_EQ(*p, "70");
}

TEST(PriceField, MultipleCommasInName) {
    auto p = PriceField(
        "9935095,\"Entire pvt. house for rent ,2 floors 3 bdrms., 2.5 baths. "
        "Newly renovated,avail. From Nov thru April / month\",51024536,Nadia,"
        "Brooklyn,Sheepshead Bay,40.59195,-73.94639,Private room,500,30,0,,,2,173");
    ASSERT_TRUE(p.has_value());
    EXPECT_EQ(*p, "500");
}

TEST(PriceField, ZeroIsKept) {
    // Ноль — валидное число; политика "скипать нули" живёт выше, не в парсере.
    auto p = PriceField("1,name,2,host,g,ne,la,lo,rt,0,1,9,d,r,6,365");
    ASSERT_TRUE(p.has_value());
    EXPECT_EQ(*p, "0");
}

// --- Негативные кейсы: строка должна быть пропущена (nullopt) ---

TEST(PriceField, EmptyPriceRejected) {
    EXPECT_FALSE(PriceField("1,name,2,host,g,ne,la,lo,rt,,1,9,d,r,6,365").has_value());
}

TEST(PriceField, NonNumericRejected) {
    EXPECT_FALSE(PriceField("1,name,2,host,g,ne,la,lo,rt,abc,1,9,d,r,6,365").has_value());
}

TEST(PriceField, TrailingGarbageRejected) {
    EXPECT_FALSE(PriceField("1,name,2,host,g,ne,la,lo,rt,70abc,1,9,d,r,6,365").has_value());
}

TEST(PriceField, TooFewFieldsRejected) {
    EXPECT_FALSE(PriceField("too,few,commas,here,only").has_value());
}

// Отрицательная цена невалидна -> минус-гард в PriceField её режет.
TEST(PriceField, NegativeRejected) {
    EXPECT_FALSE(PriceField("1,name,2,host,g,ne,la,lo,rt,-50,1,9,d,r,6,365").has_value());
}

} // namespace