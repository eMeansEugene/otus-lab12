#include "csv_price.h"

#include <iostream>
#include <string>

namespace {

    // Константный ключ: у всех строк один ключ -> после sort все пары идут подряд
    // и попадают в один reducer одной группой (single reducer, -numReduceTasks 1).
    constexpr char KEY[] = "price";

} // namespace

int main() {
    std::string line;
    while (std::getline(std::cin, line)) {
        if (const auto price = mapreduce::PriceField(line)) {
            std::cout << KEY << '\t' << *price << '\n';
        }
    }
    return 0;
}