/*!
 * \file
 * \brief Заголовочный файл класса дерева выражения ExpressionTree
 */

#ifndef EXPRESSIONTREE_H
#define EXPRESSIONTREE_H

#include <QString>
#include <QMap>
#include <QList>
#include <QStack>
#include <math.h>
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
     * \brief Построение дерева математического выражения
     * \param [in] tokens - списко валидных токенов выражения
     * \param [in, out] errors - список ошибок
     * \return корень построенного дерева
     */
    static ExpressionTree* build(const QList<Token>& tokens, QSet<Error>& errors);

    /*!
     * \brief Определение необходимого количества операндов для определённого типа узла
     * \param [in] type - тип узла
     * \return количестно необходимых операндов
     */
    static int countOperands(NodeType type);

    /*!
     * \brief Функция расчёта математического выражения в виде дерева
     * \param [in, out] errors - список ошибок
     * \return резлутат математического выражения
     */
    double calculate(QSet<Error>& errors) const;

private:
    friend class Test_ProcessOperatorFromStack; //!< Дружественный класс для тестирования функции processOperatorFromStack
    friend class Test_DetermineNodeType;        //!< Дружественный класс для тестирования функции determineNodeType
    friend class Test_Calculate;                //!< Дружественный класс для тестирования функции calculate

    static QList<Token> expressionTokens;

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
    static bool isValidNumber(double number);

    /*!
     * \brief Определяет тип узла по значению узла
     * \param [in] token - значение узла
     * \return тип узла
     */
    static NodeType determineNodeType(const QString& token);

    /*!
     * \brief Обработка закрывающей скобки при построении дерева выражения
     * \param [in] token - токен закрывающей скобки
     * \param [in, out] operators - стек операторов
     * \param [in, out] nodes - стек узлов
     * \param [in, out] positionsOpenParenthesis - стек позиций открывающих скобок
     * \param [in, out] errors - список ошибок
     * \return true - обработка удалась, false - обработка не удалась
     */
    static bool handleCloseParenthesis(const Token& token, QStack<Token>& operators, QStack<ExpressionTree*>& nodes, QStack<int>& positionsOpenParenthesis, QSet<Error>& errors);

    /*!
     * \brief Обработка оператора при построении дерева выражения
     * \param [in] token - токен оператора
     * \param [in, out] operators - стек операторов
     * \param [in, out] nodes - стек узлов
     * \param [in, out] errors - список ошибок
     * \return true - обработка удалась, false - обработка не удалась
     */
    static bool handleOperator(const Token& token, QStack<Token>& operators, QStack<ExpressionTree*>& nodes, QSet<Error>& errors);

    /*!
     * \brief Обработка операнда при построении дерева выражения
     * \param [in] token - токен операнда
     * \param [in, out] operators - стек операторов
     * \param [in, out] nodes - стек узлов
     * \param [in, out] errors - список ошибок
     * \return true - обработка удалась, false - обработка не удалась
     */
    static bool handleOperand(const Token& token, QStack<Token>& operators, QStack<ExpressionTree*>& nodes, QSet<Error>& errors);

    /*!
     * \brief Создание строки подвыражения для узла
     * \param [in] node - узел дерева выражения
     * \return строка подвыражения
     */
    static QString createSubexpressionString(const ExpressionTree* node);

    /*!
     * \brief Балансировка диапазона скобок для корректного отображения подвыражения
     * \param [in] tokens - список всех токенов выражения
     * \param [in, out] start - начальная позиция диапазона
     * \param [in, out] end - конечная позиция диапазона
     * \param [in, out] subTokens - список токенов в диапазоне
     * \param [in, out] result - результирующая строка
     */
    static void balanceParenthesesRange(const QList<Token>& tokens, int& start, int& end, QList<Token>& subTokens, QString& result);

    /*!
     * \brief Расширение диапазона до внешних скобок
     * \param [in] tokens - список всех токенов выражения
     * \param [in, out] start - начальная позиция диапазона
     * \param [in, out] end - конечная позиция диапазона
     * \param [in, out] subTokens - список токенов в диапазоне
     * \return строка с обрамляющими скобками
     */
    static QString extendToOuterParentheses(const QList<Token>& tokens, int& start, int& end, QList<Token>& subTokens);

    /*!
     * \brief Поиск индексов первого и последнего токенов в заданном диапазоне
     * \param [in] tokens - список всех токенов выражения
     * \param [in] start - начальная позиция диапазона
     * \param [in] end - конечная позиция диапазона
     * \param [out] firstTokenIndex - индекс первого токена в диапазоне
     * \param [out] lastTokenIndex - индекс последнего токена в диапазоне
     */
    static void findIndexesFirstAndLastTokensInRange(const QList<Token>& tokens, int start, int end, int& firstTokenIndex, int& lastTokenIndex);

    /*!
     * \brief Получение списка токенов в заданном диапазоне
     * \param [in] tokens - список всех токенов выражения
     * \param [in] start - начальная позиция диапазона
     * \param [in] end - конечная позиция диапазона
     * \return список токенов в заданном диапазоне
     */
    static QList<Token> getTokensInRange(const QList<Token>& tokens, int start, int end);

    /*!
     * \brief Преобразование списка токенов в строку
     * \param [in] tokens - список токенов
     * \return строка, составленная из значений токенов
     */
    static QString tokensToString(const QList<Token>& tokens);

    /*!
     * \brief Добавление ошибки, связанной с операндами, в список ошибок
     * \param [in, out] errors - список ошибок
     * \param [in] type - тип ошибки
     * \param [in] operation - оператор, для которого отсутствуют операнды
     * \param [in] symbolIndex - индекс символа в выражении
     */
    static void addOperandsError(QSet<Error>& errors, Error::Type type, const QString& operation, int symbolIndex);
};

#endif // EXPRESSIONTREE_H
