#ifndef EXPRESSIONTREE_H
#define EXPRESSIONTREE_H

#include <QString>
#include <QMap>
#include <QList>
#include <QStack>
#include "error.h"

struct Token;

/*!
 * \brief Класс узла дерева
 */
class ExpressionTree
{
public:
    /*!
     * \brief Перечисление типов узла
     */
    enum NodeType {
        Operand,            //!< Операнд
        Plus,               //!< Плюс
        BinaryMinus,        //!< Бинарный минус
        Multiplication,     //!< Умножение
        Division,           //!< Деление
        Power,              //!< Возведение в степень
        UnaryMinus          //!< Унарный минус
    };

    /*!
     * \brief Конструктор для узла
     * \param [in] operation - значение создаваемого узла
     * \param [in] left - левый ребёнок
     * \param [in] right - правый ребёнок
     * \param [in] start - индекс начала покрытия узла
     * \param [in] end - индекс конца покрытия узла
     */
    ExpressionTree(const QString& operation, ExpressionTree* left, ExpressionTree* right, int start, int end);

    /*!
     * \brief Деструктор
     */
    ~ExpressionTree();

    static const QMap<QString, int> operatorsPrecedence;    //!< Мапа для операторов выражения и их приоритета

    /*!
     * \brief Получение левого ребёнка
     */
    ExpressionTree* getLeft() const;

    /*!
     * \brief Получение правого ребёнка
     */
    ExpressionTree* getRight() const;

    /*!
     * \brief Получение индекса начала покрытия узла
     */
    int getStart() const;

    /*!
     * \brief Получение индекса конца покрытия узла
     */
    int getEnd() const;

    /*!
     * \brief Получение значения в узле
     */
    double getValue() const;

    /*!
     * \brief Получение типа узла
     */
    NodeType getNodeType() const;

    /*!
     * \brief Преобразование узла в строку
     */
    QString toString() const;

    /*!
     * \brief Определение необходимого количества операндов для определённого типа узла
     *
     */
    static int countOperands(NodeType type);

    /*!
     * \brief Построение дерева математического выражения
     * \param [in] tokens - списко валидных токенов выражения
     * \param [in, out] errors - список ошибок
     * \return корень построенного дерева
     */
    static ExpressionTree* build(const QList<Token>& tokens, QSet<Error>& errors);

    /*!
     * \brief Функция расчёта математического выражения в виде дерева
     * \param [in, out] errors - список ошибок
     * \return резлутат математического выражения
     */
    double calculate(QSet<Error>& errors) const;

private:
    friend class Test_ProcessOperatorFromStack; //!< Дружественный класс для тестирования функции processOperatorFromStack
    friend class Test_DetermineNodeType;        //!< Дружественный класс для тестирования функции determineNodeType

    ExpressionTree* left;   //!< Левый ребёнок
    ExpressionTree* right;  //!< Правый ребёнок
    NodeType nodeType;      //!< Тип узла
    double value;           //!< Значение в узле
    int start;              //!< Индекс начала покрытия узла
    int end;                //!< Индекс конца покрытия узла

    /*!
     * \brief Обрабатывает оператор из стека с созданием соответствующего узла
     * \param [in, out] operators - стек операторов
     * \param [in, out] nodes - стек узлов
     * \param [in, out] errors - список ошибок
     * \return true - обработка удалась, false - обработка не удалась
     */
    static bool processOperatorFromStack(QStack<Token>& operators, QStack<ExpressionTree*>& nodes, QSet<Error>& errors);

    /*!
     * \brief Определяет, находится ли число в необходимом диапазоне
     * \param [in] number - число
     * \return true - число находится в необходимом диапазоне, false - не находится
     */
    bool isValidNumber(double number) const;

    /*!
     * \brief Определяет тип узла по значению узла
     * \param [in] token - значение узла
     * \return тип узла
     */
    static NodeType determineNodeType(const QString& token);
};

#endif // EXPRESSIONTREE_H
