//
// Created by evgen on 12.09.2026.
//

// csv_price.cpp
#include "csv_price.h"

#include <charconv>

namespace mapreduce {

std::optional<std::string_view> PriceField(std::string_view line) {
    // price = 7-е поле с конца. Его границы — это 6-я и 7-я запятые, считая СПРАВА:
    //   ... , <price> , t10 , t11 , t12 , t13 , t14 , t15
    //        ^7-я        ^6-я    (нумерация запятых справа налево)
    // 6-я справа = запятая сразу ПОСЛЕ price -> правая граница (конец, exclusive).
    // 7-я справа = запятая сразу ДО price   -> левая граница  (start = позиция + 1).

    std::size_t commas = 0;
    std::size_t end = std::string_view::npos;  // правая граница price (6-я запятая справа)
    std::size_t start = 0;                     // левая граница; по умолчанию — начало строки

    for (std::size_t i = line.size(); i-- > 0;) {
        if (line[i] != ',') {
            continue;
        }
        ++commas;
        if (commas == 6) {
            end = i;
        } else if (commas == 7) {
            start = i + 1;
            break;
        }
    }

    if (end == std::string_view::npos) {
        return std::nullopt;
    }

    const std::string_view field = line.substr(start, end - start);

    // Цена не может быть отрицательной. from_chars<long long> принял бы знак '-',
    // поэтому отсекаем его явно (осознанное ужесточение, см. развилку по знаку).
    if (!field.empty() && field.front() == '-') {
        return std::nullopt;
    }

    long long value = 0;
    const char* first = field.data();
    const char* last = field.data() + field.size();
    const auto [ptr, ec] = std::from_chars(first, last, value);
    if (ec != std::errc{} || ptr != last) {
        return std::nullopt;    // пусто, не число, или хвост-мусор
    }


    return field;
}

} // namespace mapreduce
