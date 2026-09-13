#include "reducer_core.h"

#include "price_accumulator.h"

#include <charconv>
#include <iomanip>
#include <istream>
#include <ostream>
#include <string>

namespace mapreduce {

std::optional<std::pair<std::string_view, long long>> ParseKeyValue(std::string_view line) {
    const std::size_t tab = line.find('\t');
    if (tab == std::string_view::npos) {
        return std::nullopt;
    }
    const std::string_view key = line.substr(0, tab);
    const std::string_view value = line.substr(tab + 1);

    long long price = 0;
    const char* first = value.data();
    const char* last = value.data() + value.size();
    const auto [ptr, ec] = std::from_chars(first, last, price);
    if (ec != std::errc{} || ptr != last) {
        return std::nullopt;
    }
    return std::make_pair(key, price);
}

void RunReducer(std::istream& in, std::ostream& out, const StatFn& stat) {
    out << std::setprecision(10);

    std::string line;
    std::string current_key;
    PriceAccumulator acc;
    bool has_group = false;

    while (std::getline(in, line)) {
        const auto kv = ParseKeyValue(line);
        if (!kv) {
            continue;
        }
        const auto [key, price] = *kv;

        if (has_group && key != current_key) {
            out << current_key << '\t' << stat(acc) << '\n';
            acc = PriceAccumulator{};
            has_group = false;
        }
        if (!has_group) {
            current_key.assign(key);
            has_group = true;
        }
        acc.Add(price);
    }

    if (has_group) {
        out << current_key << '\t' << stat(acc) << '\n';
    }
}

} // namespace mapreduce