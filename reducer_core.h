//
// Created by evgen on 12.09.2026.
//

#ifndef MAP_REDUCE_REDUCER_CORE_H
#define MAP_REDUCE_REDUCER_CORE_H

#include "price_accumulator.h"

#include <functional>
#include <iosfwd>
#include <optional>
#include <string_view>
#include <utility>

namespace mapreduce {

    /**
     * @brief Разбирает строку reducer'а формата "key\tvalue" в пару (ключ, цена).
     * @param line Строка без завершающего перевода строки.
     * @return Пара {ключ, цена} либо nullopt, если нет таба или value не число.
     */
    std::optional<std::pair<std::string_view, long long>> ParseKeyValue(std::string_view line);

    /// Чем считать итог завершённой группы: точка различия mean- и var-редьюсеров.
    using StatFn = std::function<double(const PriceAccumulator&)>;

    /**
     * @brief Потоковый reducer: читает отсортированный по ключу поток "key\tvalue",
     *        агрегирует по группам одного ключа и на закрытии группы пишет
     *        "key\t<stat>" в out.
     *
     * Копим, пока ключ тот же; на смене ключа эмитим предыдущую группу и сбрасываем
     * накопитель; на EOF — финальный флаш последней группы. Битые строки пропускаем.
     *
     * @param in   Входной поток (stdin в проде, istringstream в тестах).
     * @param out  Выходной поток (stdout в проде, ostringstream в тестах).
     * @param stat Как получить итог группы (Mean или Variance).
     */
    void RunReducer(std::istream& in, std::ostream& out, const StatFn& stat);

} // namespace mapreduce



#endif //MAP_REDUCE_REDUCER_CORE_H
