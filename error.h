#ifndef ERROR_H
#define ERROR_H

#include <QString>
#include <QHash>

/*!
 * \brief Класс ошибок
 */
class Error
{
public:
    /*!
     * \brief Перечисление типов ошибки
     */
    enum Type {
        inputFileWay,               //!< Указанный входной файл не существует, нет доступа к указанному файлу
        outputFileWay,              //!< Невозможно создать указанный выходной файл
        countStrings,               //!< Во входном файле больше одной строки
        unknownSymbolsSequence,     //!< Неизвестная последовательность символов
        divisionByZero,             //!< Деление на ноль в выражении
        noLeftOperand,              //!< Отсутствие левого операнда в определённой операции вычисления
        noRightOperand,             //!< Отсутствие правого операнда в определённой операции вычисления
        noBothOperands,             //!< Отсутствие обоих операндов у определённой операции вычисления
        noClosingParenthesis,       //!< Отсутствует закрывающая скобка
        noOpeningParenthesis,       //!< Отсутствует открывающая скобка
        noCalcOperation,            //!< Отсутствует оператор вчисления
        incorrectRoot,              //!< Извлечение корня, являющегося правильной дробью, из отрицательного числа
        operandOutOfRange           //!< Один из операндов выходит за пределы обрабатываемых чисел [-1.7E308..1.7E308]
    };

    /*!
     * \brief Устанавливает тип ошибки
     * \param [in] type - тип ошибки
     */
    void setType(Type type);

    /*!
     * \brief Устанавливает последовательность неизвестных символов
     * \param [in] symbolsSequence - последовательность неизвестных символов
     */
    void setSymbolsSequence(const QString& symbolsSequence);

    /*!
     * \brief Устанавливает выражение
     * \param [in] expression - выражение
     */
    void setExpression(const QString& expression);

    /*!
     * \brief Устанавливает левый операнд
     * \param [in] leftOperand - левый операнд
     */
    void setLeftOperand(const QString& leftOperand);

    /*!
     * \brief Устанавливает правый операнд
     * \param [in] rightOperand - правый операнд
     */
    void setRightOperand(const QString& rightOperand);

    /*!
     * \brief Устанавливает индекс символа
     * \param [in] symbolIndex - индекс символа
     */
    void setSymbolIndex(int symbolIndex);

    /*!
     * \brief Устанавливает операцию
     * \param [in] operation - операция
     */
    void setOperation(const QString& operation);

    /*!
     * \brief Генерирует сообщение об ошибке
     * \return сообщение об ошибке
     */
    QString message() const;

    /*!
     * \brief Перегрузка оператора сравнения двух ошибок
     * \param [in] other - ошибка, с которой сравнивается текущая
     * \return true - равны, false - не равны
     */
    bool operator==(const Error& other) const;

    /*!
     * \brief Функция хеширования для класса Error
     * \param [in] error - объект ошибки, для которого вычисляется хеш
     * \param [in] seed - начальное значение хеша (используется для комбинирования хешей)
     * \return Уникальное хеш-значение для объекта ошибки
     */
    friend uint qHash(const Error& error, uint seed = 0) {
        uint hash = qHash(error.type, seed);
        switch (error.type) {
        case Error::Type::unknownSymbolsSequence:
            hash ^= qHash(error.symbolsSequence, seed);
            break;
        case Error::Type::divisionByZero:
        case Error::Type::incorrectRoot:
            hash ^= qHash(error.expression, seed);
            break;
        case Error::Type::noLeftOperand:
        case Error::Type::noRightOperand:
        case Error::Type::noBothOperands:
            hash ^= qHash(error.operation, seed) ^ qHash(error.indexSymbol, seed);
            break;
        case Error::Type::noClosingParenthesis:
        case Error::Type::noOpeningParenthesis:
            hash ^= qHash(error.indexSymbol, seed);
            break;
        case Error::Type::noCalcOperation:
            hash ^= qHash(error.leftOperand, seed) ^ qHash(error.rightOperand, seed);
            break;
        case Error::Type::inputFileWay:
        case Error::Type::outputFileWay:
        case Error::Type::countStrings:
        case Error::Type::operandOutOfRange:
        default:
            // Для этих типов достаточно type
            break;
        }
        return hash;
    }

private:
    Type type;                  //!< Тип ошибки
    QString symbolsSequence;    //!< Последовательность неизвестных символов
    QString expression;         //!< Выражение
    QString leftOperand;        //!< Левый операнд
    QString rightOperand;       //!< Правый операнд
    int indexSymbol;            //!< Индекс символа
    QString operation;          //!< Операция
};

#endif // ERROR_H
