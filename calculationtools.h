#ifndef CALCULATIONTOOLS_H
#define CALCULATIONTOOLS_H

#include <QString>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QSet>
#include <QChar>
#include <QDebug>
#include "error.h"

class ExpressionTree;

/*!
 * \brief Структура для токена
 */
struct Token {
    QString value;  //!< Значение токена
    int start;      //!< Его стартовая позиция в строке
    int end;        //!< Его конечная позиция в строке

    Token() {
        value = "";
        start = -1;
        end = -1;
    }

    Token(QString value) {
        this->value = value;
        this->start = -1;
        this->end = -1;
    }

    Token(QString value, int start, int end) {
        this->value = value;
        this->start = start;
        this->end = end;
    }
};

/*!
 * \brief Вспомогательные функции для функции tokenize
 */
namespace TokenizerUtils {
    /*!
     * \brief Определяет, является ли символ оператором или скобкой
     * \param [in] symbol  - символ
     * \return true - символ является оператором, false - не является оператором
     */
    bool isMathOperator(const QChar& symbol);

    /*!
     * \brief Определяет, является ли символ невалидным для математического выражения
     * \param [in] symbol - символ
     * \return true - символ является невалидным, false - является валидным
     */
    bool isInvalidMathSymbol(const QChar& symbol);

    /*!
     * \brief Определяет, является ли число в формате строки валидным числом
     * \param [in] number - число в формате строки
     * \return true - является валидным, false - не является валидным
     */
    bool isWellFormedNumber(const QString& number);

    /*!
     * \brief Собирает последовательность пробелов в единый токен
     * \param [in] expression - строка математического выражения
     * \param [in, out] index - индекс текущего символа в строке
     * \return токен последовательности пробелов
     */
    Token extractSpaces(const QString& expression, int& index);

    /*!
     * \brief Собирает последовательность цифр и точек в единый токен числа
     * \param [in] expression - строка математического выражения
     * \param [in, out] index - индекс текущего символа в строке
     * \return токен последовательности цифр и точек
     */
    Token extractNumberLiteral(const QString& expression, int& index);

    /*!
     * \brief Определяет, является ли минус унарным
     * \param [in] expression - строка математического выражения
     * \param [in, out] index - индекс текущего символа в строке
     * \return true - является унарным, false - не является унарным
     */
    bool isUnaryMinus(const QString& expression, int index);

    /*!
     * \brief Собирает последовательность неизвестных символов в единый токен
     * \param [in] expression - строка математического выражения
     * \param [in, out] index - индекс текущего символа в строке
     * \return токен последовтельности неизвестных символов
     */
    Token extractInvalidSymbols(const QString& expression, int& index);

    /*!
     * \brief Полноценно собирает и формирует невалидный токен, который может включать в себя и невалидное число,
     * и неизвестную последовательность символов
     * \param [in] expression - строка математического выражения
     * \param [in, out] index - индекс текущего символа в строке
     * \param [in] initialValue - начальная последовательность символов, которая уже признана невалидной
     * \param [in] initialStart - начальная позиция в строке для формируемого токена
     * \return невалидный токен
     */
    Token collectInvalidToken(const QString& expression, int& index, const QString& initialValue, int initialStart);
}

/*!
 * \brief Считывает строку из файла по указанному пути
 * \param [in] filePath - путь к файлу
 * \param [in, out] expression - полученная строка из файла
 * \param [in, out] errors - список ошибок
 * \return true - прочитать файл удалось, false - прочитать файл не удалось
 */
bool readInputFile(const QString& filePath, QString& expression, QSet<Error>& errors);

/*!
 * \brief Разбивает строку математического выражения на валидные токены
 * \param [in] expression - строка математического выражения
 * \param [in, out] errors - список ошибок
 * \return список валидных токенов
 */
QList<Token> tokenize(const QString& expression, QSet<Error>& errors);

/*!
 * \brief Записывает данные в файл по указанному пути
 * \param [in] filePath - путь к файлу
 * \param [in, out] content - данные для записи
 * \param [in, out] errors - список ошибок
 * \return true - запись в файл удалась, false - запись в файл не удалась
 */
bool writeOutputFile(const QString& filePath, QString& content, QSet<Error>& errors);

#endif // CALCULATIONTOOLS_H
