//
// Created by evgen on 12.09.2026.
//

#ifndef MAP_REDUCE_CSV_PRICE_H
#define MAP_REDUCE_CSV_PRICE_H


#include <optional>
#include <string_view>

namespace mapreduce {

    /**
     * @brief Извлекает поле price из одной строки CSV датасета Airbnb NYC.
     *
     * Берёт 7-е поле с конца (fields[N-7]): шесть хвостовых колонок гарантированно
     * без запятых, поэтому запятые внутри name/host_name не сдвигают позицию цены.
     * Возвращает подстроку price дословно (без round-trip через double), провалидировав,
     * что это число. Пустая/некорректная цена или строка без нужного числа полей -> nullopt.
     *
     * @param line Сырая строка CSV без завершающего перевода строки.
     * @return Подстрока с ценой, либо nullopt, если строку нужно пропустить.
     */
    std::optional<std::string_view> PriceField(std::string_view line);

} // namespace mapreduce

#endif //MAP_REDUCE_CSV_PRICE_H