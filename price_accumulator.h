//
// Created by evgen on 12.09.2026.
//

#ifndef MAP_REDUCE_PRICE_ACCUMULATOR_H
#define MAP_REDUCE_PRICE_ACCUMULATOR_H
#include <cstddef>

namespace mapreduce {

    /**
     * @brief Однопроходный накопитель статистик цены для reducer'а.
     *
     * Копит sum, sum of squares и count; из них считает среднее и популяционную
     * дисперсию по формуле E[X^2] - E[X]^2 за один проход (без второго прогона
     * пайплайна). Аккумуляторы целочисленные: для целых цен Airbnb накопление
     * точное, плавающая ошибка появляется лишь в финальных делениях.
     */
    class PriceAccumulator {
    public:
        /**
         * @brief Добавляет одно значение цены в накопитель.
         * @param price Неотрицательная целая цена (валидация — выше, в PriceField).
         */
        void Add(long long price);

        /**
         * @brief Среднее по добавленным значениям.
         * @pre Count() > 0 (reducer эмитит группу, только увидев хотя бы одно значение).
         * @return sum / count.
         */
        double Mean() const;

        /**
         * @brief Популяционная дисперсия (деление на N).
         * @pre Count() > 0.
         * @return sum_sq / count - Mean()^2; клампится снизу нулём против округления.
         */
        double Variance() const;

        /// Число добавленных значений.
        std::size_t Count() const;

    private:
        long long sum_ = 0;      ///< Сумма цен.
        long long sum_sq_ = 0;   ///< Сумма квадратов цен.
        std::size_t count_ = 0;  ///< Количество значений.
    };

} // namespace mapreduce

#endif //MAP_REDUCE_PRICE_ACCUMULATOR_H