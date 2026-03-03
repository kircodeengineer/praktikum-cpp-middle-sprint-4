#include <unistd.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <print>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

#include "file.hpp"
#include "function.hpp"
#include "metric.hpp"
#include "metric_accumulator.hpp"

namespace analyser {

namespace rv = std::ranges::views;
namespace rs = std::ranges;
/**
 * @brief Анализирует список Python-файлов и извлекает метрики для всех функций и методов.
 *
 * Эта функция — центральный "конвейер" обработки:
 * 1. Принимает имена файлов.
 * 2. Для каждого файла создаёт объект `File`, который автоматически парсит его через tree-sitter
 *    и строит AST.
 * 3. Извлекает из AST все функции и методы с помощью `FunctionExtractor`.
 * 4. Объединяет все функции из всех файлов в один плоский список (`join`).
 * 5. Для каждой функции вычисляет набор метрик через переданный `metric_extractor`.
 * 6. Возвращает вектор пар: (функция, результаты её метрик).
 */
auto AnalyseFunctions(const std::vector<std::string> &files,
                      const analyser::metric::MetricExtractor &metric_extractor) {
    auto transformed{files | std::views::transform([&](const auto &file) {
                         analyser::function::FunctionExtractor func_extr;
                         return func_extr.Get(file) | std::views::transform([&](const auto &func) {
                                    return std::pair{func, metric_extractor.Get(func)};
                                });
                     })};

    return transformed | std::views::join | std::ranges::to<std::vector>();
}

/**
 *
 * @brief Группирует результаты анализа по классам.
 *
 * Эта функция:
 * 1. Отфильтровывает только те функции, которые являются **методами классов**
 *    (у них `class_name.has_value()` == true).
 * 2. Группирует последовательные элементы с одинаковым именем класса с помощью `chunk_by`.
 *
 * Важно:
 * - `chunk_by` работает только с **последовательными** одинаковыми элементами!
 *   Поэтому предполагается, что входной диапазон уже упорядочен по классам
 *   (например, порядок методов в AST сохраняется как в исходном файле).
 * - Если порядок нарушен, один и тот же класс может быть разбит на несколько групп.
 *
 *  Чтобы убедиться, что фильтрация работает, проверьте, что свободные функции (без class_name)
 * действительно исчезают из результата.
 */
auto SplitByClasses(const auto &analysis) {
    return analysis | std::views::filter([](const auto &pair) {
               const auto &func{std::get<0>(pair)};
               return func.class_name.has_value();
           }) |
           std::ranges::to<std::vector>() | std::views::chunk_by([](const auto &left, const auto &right) {
               const auto &left_func{std::get<0>(left)};
               const auto &right_func{std::get<0>(right)};
               return left_func.class_name == right_func.class_name;
           }) |
           std::views::transform([](const auto &chunk) {
               return std::vector<std::pair<analyser::function::Function, analyser::metric::MetricResults>>(
                   chunk.begin(), chunk.end());
           });
}

/**
 * @brief Группирует результаты анализа по исходным файлам.
 *
 * Эта функция:
 * - Разбивает весь список функций на группы, где каждая группа содержит
 *   только функции из одного и того же файла (`filename`).
 * - Использует `chunk_by`, поэтому **порядок функций в `analysis` должен быть по файлам**.
 */
auto SplitByFiles(const auto &analysis) {
    return analysis | std::views::chunk_by([](const auto &left, const auto &right) {  // нет фильтрации
               const auto &left_func{std::get<0>(left)};
               const auto &right_func{std::get<0>(right)};
               return left_func.filename == right_func.filename;  // прямое сравнение строк
           }) |
           std::views::transform([](const auto &chunk) {
               return std::vector<std::pair<analyser::function::Function, analyser::metric::MetricResults>>(
                   chunk.begin(), chunk.end());
           });
}

/**
 * @brief Агрегирует метрики всех функций с помощью аккумулятора.
 *
 * Эта функция:
 * - Проходит по каждому элементу результата `AnalyseFunctions`
 *   (то есть по каждой функции и её метрикам).
 * - Передаёт результаты метрик (`elem.second`) в аккумулятор через `AccumulateNextFunctionResults`.
 */
void AccumulateFunctionAnalysis(const auto &analysis,
                                const analyser::metric_accumulator::MetricsAccumulator &accumulator) {
    std::ranges::for_each(analysis,
                          [&accumulator](const auto &elem) { accumulator.AccumulateNextFunctionResults(elem.second); });
}

}  // namespace analyser
