#include "price_accumulator.h"
#include "reducer_core.h"

#include <iostream>

int main() {
    mapreduce::RunReducer(std::cin, std::cout,
        [](const mapreduce::PriceAccumulator& acc) { return acc.Mean(); });
    return 0;
}