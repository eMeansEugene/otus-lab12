//
// Created by evgen on 12.09.2026.
//

#include "price_accumulator.h"

void mapreduce::PriceAccumulator::Add(const long long price) {
    sum_ += price;
    sum_sq_ += price * price;
    ++count_;
}

double mapreduce::PriceAccumulator::Mean() const {
    return static_cast<double>(sum_) / static_cast<double>(count_);
}

double mapreduce::PriceAccumulator::Variance() const {
    const double mean = Mean();
    const double variance =
          static_cast<double>(sum_sq_) / static_cast<double>(count_) - mean * mean;
    return variance < 0.0 ? 0.0 : variance;
}

std::size_t mapreduce::PriceAccumulator::Count() const {
    return count_;
}
