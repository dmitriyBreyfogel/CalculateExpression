/*!
* \file
* \brief Реализация методов класса дерева выражения ExpressionTree
*/

#include "expressiontree.h"
#include "calculationtools.h"

QList<Token> ExpressionTree::expressionTokens;

ExpressionTree::ExpressionTree(const QString& operation, ExpressionTree* left, ExpressionTree* right, int start, int end) {
    this->nodeType = determineNodeType(operation);  // Определяем тип узла

    if (nodeType == NodeType::Operand) {    // Если это операнд
        // Получаем из строки значение операнда
        bool validDouble;
        double value = operation.toDouble(&validDouble);
        if (validDouble && isValidNumber(value))
            this->value = value;
    }
    else this->value = 0;   // Иначе значение равно нулю

    // Устанавливаем остальные параметры из конструктора
    this->left = left;
    this->right = right;
    this->start = start;
    this->end = end;
}

ExpressionTree::~ExpressionTree() {
    delete left;
    delete right;
}

const QMap<QString, int> ExpressionTree::operatorsPrecedence = {
    {"+", 1},
    {"-", 1},
    {"*", 2},
    {"/", 2},
    {"^", 3},
    {"-u", 4},
    {"(", 5},
    {")", 5}
};

void ExpressionTree::addOperandsError(QSet<Error>& errors, Error::Type type, const QString& operation, int symbolIndex) {
    Error error;
    error.setType(type);
    error.setOperation(operation);
    error.setSymbolIndex(symbolIndex);
    errors.insert(error);
}

bool ExpressionTree::isValidNumber(double number) {
    if (number >= -1.7E308 && number <= 1.7E308)
        return true;

    return false;
}

ExpressionTree::NodeType ExpressionTree::determineNodeType(const QString& value) {
    if (value == "+") return NodeType::Plus;
    if (value == "-") return NodeType::BinaryMinus;
    if (value == "-u") return NodeType::UnaryMinus;
    if (value == "/") return NodeType::Division;
    if (value == "*") return NodeType::Multiplication;
    if (value == "^") return NodeType::Power;
    return NodeType::Operand;
}

ExpressionTree* ExpressionTree::getLeft() const {
    return left;
}

ExpressionTree* ExpressionTree::getRight() const {
    return right;
}

int ExpressionTree::getStart() const {
    return start;
}

int ExpressionTree::getEnd() const {
    return end;
}

double ExpressionTree::getValue() const {
    return value;
}

ExpressionTree::NodeType ExpressionTree::getNodeType() const {
    return nodeType;
}

QString ExpressionTree::toString() const {
    if (nodeType == NodeType::Plus) return "+";
    if (nodeType == NodeType::BinaryMinus) return "-";
    if (nodeType == NodeType::UnaryMinus) return "-u";
    if (nodeType == NodeType::Division) return "/";
    if (nodeType == NodeType::Multiplication) return "*";
    if (nodeType == NodeType::Power) return "^";
    return QString::number(value);
}

ExpressionTree* ExpressionTree::build(const QList<Token>& tokens, QSet<Error>& errors) {
    expressionTokens = tokens;  // Запоминаем список токенов исходного выражения

    QStack<Token> operators;                // Стек операторов
    QStack<ExpressionTree*> nodes;          // Стек узлов
    QStack<int> positionsOpenParenthesis;   // Стек начальных позиций открывающихся скобок

    // Построение дерева
    for (const Token& token : tokens) { // Для каждого токена
        // Обработка открывающей скобки
        if (token.value == "(") {   // Если токен - открывающая скобка
            positionsOpenParenthesis.push(token.start); // Запоминаем её начальную позицию
            operators.push(token);  // Добавляем в стек операторов
        }
        // Обработка токена пробелов
        else if (token.value.trimmed().isEmpty()) { // Если токен - токен пробелов
            continue;   // Переходим к следующей итерации
        }
        // Обработка закрывающей скобки
        else if (token.value == ")") {  // Если токен - закрывающая скобка
            // Обрабатываем закрывающую скобку
            if (!handleCloseParenthesis(token, operators, nodes, positionsOpenParenthesis, errors)) // Если обработка не удалась
                throw errors; // Дерево не построено
        }
        // Обработка оператора
        else if (operatorsPrecedence.contains(token.value)) {   // Если токен - оператор
            // Обрабатываем оператор
            if (!handleOperator(token, operators, nodes, errors))   // Если обработка не удалась
                throw errors; // Дерево не построено
        }
        // Обработка операнда
        else if (determineNodeType(token.value) == NodeType::Operand) { // Если токен - операнд
            // Обрабатываем операнд
            if (!handleOperand(token, operators, nodes, errors))    // Если обработка не удалась
                throw errors; // Дерево не построено
        }
    }

    // Обрабатываем оставшиеся операторы в стеке
    while (!operators.isEmpty()) {  // Пока есть операторы в стеке
        if (operators.top().value == "(") { // Если на вершине стека операторов открывающая скобка
            // Обрабатываем ошибку
            Error error;
            error.setType(Error::Type::noClosingParenthesis);   // Отсутствует закрывающая скобка
            error.setSymbolIndex(operators.top().start);
            errors.insert(error);
            throw errors; // Дерево не построено
        }
        if (!nodes.isEmpty() && nodes.top()->getEnd() < operators.top().start) {    // Если в стеке узлов остался узел, который до оператора
            // Обрабатываем ошибку
            addOperandsError(errors, Error::Type::noRightOperand, operators.top().value, operators.top().start);    // Отсутствует правый операнд
            throw errors; // Дерево не построено
        }
        else {  // Иначе
            // Обрабатываем оператор
            if (!processOperatorFromStack(operators, nodes, errors)) {  // Если обработка не удалась
                // Если есть ошибки
                if (!errors.isEmpty()) throw errors;  // Дерево не построено
            }
        }
    }

    // Проверяем узлы
    if (nodes.size() == 1) {    // Если остался один узел
        return nodes.pop(); // Дерево построено, возвращаем корень дерева
    }
    else if (nodes.size() > 1) {    // Если в стеке есть несколько узлов
        // Обрабатываем ошибку
        Error error;
        error.setType(Error::Type::noCalcOperation);    // Отсутствует оператор вычисления

        // Получаем первые два узла из стека
        ExpressionTree* firstNode = nodes[0];
        ExpressionTree* secondNode = nodes[1];

        // Формируем строки подвыражений для первого и второго узлов
        QString leftOperand = createSubexpressionString(firstNode);
        QString rightOperand = createSubexpressionString(secondNode);

        // Устанавливаем строки операндов в ошибки
        error.setLeftOperand(leftOperand);
        error.setRightOperand(rightOperand);

        errors.insert(error);
        throw errors; // Дерево не построено
    }
    else    // Иначе
        throw errors; // Дерево не построено
}

bool ExpressionTree::processOperatorFromStack(QStack<Token>& operators, QStack<ExpressionTree*>& nodes, QSet<Error>& errors) {
    // Если стек операторов пуст
    if (operators.isEmpty()) {
        return false;   // Обработка не удалась
    }

    Token operatorToken = operators.pop();  // Извлекаем токен из стека операторов

    // Проверяем специальные случаи
    if (operatorToken.value == " " || operatorToken.value == "(" || operatorToken.value == ")") {   // Если токен - пробел или скобка
        operators.push(operatorToken);  // Возвращаем токен в стек операторов
        return false;   // Обработка не удалась
    }

    NodeType typeOperator = determineNodeType(operatorToken.value); // Определяем тип оператора

    // Обработка операнда (если токен - число)
    if (typeOperator == NodeType::Operand) {
        // Создаём узел операнда и добавляем его в стек узлов
        ExpressionTree* node = new ExpressionTree(operatorToken.value, nullptr, nullptr, operatorToken.start, operatorToken.end);
        nodes.push(node);
        return true;    // Обработка удалась
    }

    // Обработка унарного минуса
    if (typeOperator == NodeType::UnaryMinus) { // Если тип оператора - унарный минус
        if (nodes.size() >= 1) {    // Если стек узлов содержит хотя бы один узел
            // Получаем одного ребёнка для унарного минуса
            ExpressionTree* operand = nodes.pop();

            // Создаём узел для унарного минуса и добавляем его в стек узлов
            ExpressionTree* node = new ExpressionTree(operatorToken.value, operand, nullptr, operatorToken.start, operand->getEnd());
            nodes.push(node);
            return true;    // Обработка удалась
        }
        else {
            operators.push(operatorToken);  // Возвращаем токен в стек операторов
            // Обрабатываем ошибку
            addOperandsError(errors, Error::Type::noRightOperand, "-", operatorToken.start);    // Отсутствует правый операнд
            return false;   // Обработка не удалась
        }
    }

    // Обработка бинарных операторов
    int requiredOperands = countOperands(typeOperator);
    if (nodes.size() >= requiredOperands) { // Если узлов в стеке хватает для создания нового узла
        // Получаем детей из стека
        ExpressionTree* right = nodes.pop();
        ExpressionTree* left = nodes.pop();

        // Создаём узел с детьми и добавляем его в стек узлов
        ExpressionTree* node = new ExpressionTree(operatorToken.value, left, right, left->getStart(), right->getEnd());
        nodes.push(node);
        return true;    // Обработка удалась
    }
    else {
        operators.push(operatorToken);  // Возвращаем токен в стек операторов

        // Обрабатываем ошибку
        if (nodes.size() == 1) {    // Если в стеке узлов остался только один узел
            // Определяем какой операнд отсутствует
            if (operatorToken.start < nodes.top()->getStart())  // Если узел справа от оператора
                // Обрабатываем ошибку
                addOperandsError(errors, Error::Type::noLeftOperand, operatorToken.value, operatorToken.start); // Отсутствует левый операнд
            else    // Иначе
                // Обрабатываем ошибку
                addOperandsError(errors, Error::Type::noRightOperand, operatorToken.value, operatorToken.start);    // Отсутствует правый операнд
        }
        else {    // Иначе
            // Обрабатываем ошибку
            addOperandsError(errors, Error::Type::noBothOperands, operatorToken.value, operatorToken.start);    // Отсутствуют оба операнда
        }
        return false;   // Обработка не удалась
    }
}

bool ExpressionTree::handleCloseParenthesis(const Token& token, QStack<Token>& operators, QStack<ExpressionTree*>& nodes, QStack<int>& positionsOpenParenthesis, QSet<Error>& errors) {
    // Обрабатываем все операторы в стеке до открывающей скобки
    while (!operators.isEmpty() && operators.top().value != "(") {  // Пока в стеке операторов не нашли открывающую скобку
        // Обрабатываем унарный оператор
        if (operators.top().value == "-u") {    // Если токен - унарный минус
            if (!nodes.isEmpty()) { // Если есть узлы
                // Проверяем валидность узла, как правого операнда для унарного минуса
                ExpressionTree* rightOperand = nodes.top();
                bool validRightOperand = rightOperand->getStart() > positionsOpenParenthesis.top() &&
                                         rightOperand->getStart() > operators.top().end;

                if (validRightOperand) {    // Если узел является валидным правым операндом
                    // Обрабтываем узел
                    if (!processOperatorFromStack(operators, nodes, errors)) {  // Если узел не обработался
                        // Если есть ошибки
                        if (!errors.isEmpty()) return false; // Дерево не построено
                    }
                }
                else {  // Иначе
                    // Обрабатываем ошибку отсутствия правого операнда
                    addOperandsError(errors, Error::Type::noRightOperand, operators.top().value, operators.top().start);
                    return false; // Дерево не построено
                }
            }
            else {  // Иначе (если узлов нет)
                // Обрабатываем ошибку отсутствия правого опреанда
                addOperandsError(errors, Error::Type::noRightOperand, operators.top().value, operators.top().start);
                return false; // Дерево не построено
            }
        }
        else {  // Иначе (обрабатываем бинарный оператор)
            ExpressionTree* leftOperand = nullptr;
            ExpressionTree* rightOperand = nullptr;

            bool hasValidLeftOperand;
            bool hasValidRightOperand;

            if (nodes.size() > 1) { // Если в стеке есть несколько узлов
                // Получаем два последних узла (левый и правый операнд)
                leftOperand = nodes[nodes.size() - 2];
                rightOperand = nodes.top();

                // Проверяем валидность операндов
                if (!positionsOpenParenthesis.isEmpty()) {  // Если есть хотя бы одна позиция открывающей скобки
                    // Проверяем валидность левого операнда (находится внутри блока скобок и перед оператором)
                    hasValidLeftOperand = (leftOperand->getStart() > positionsOpenParenthesis.top() &&
                                           leftOperand->getEnd() < operators.top().start)
                                          ||
                                          (rightOperand->getStart() > positionsOpenParenthesis.top() &&
                                           rightOperand->getEnd() < operators.top().start);

                    // Проверяем валидность правого операнда (находится внутри блока скобок и после оператора)
                    hasValidRightOperand = (leftOperand->getStart() > positionsOpenParenthesis.top() &&
                                            leftOperand->getStart() > operators.top().end)
                                           ||
                                           (rightOperand->getStart() > positionsOpenParenthesis.top() &&
                                            rightOperand->getStart() > operators.top().end);
                }
                else    // Иначе
                    break;  // Обработка закончилась

                if (hasValidLeftOperand && hasValidRightOperand) {  // Если оба операнда валидны
                    // Обрабатываем оператор
                    if (!processOperatorFromStack(operators, nodes, errors)) {  // Если обработка не удалась
                        // Если есть ошибки
                        if (!errors.isEmpty()) return false; // Обработка закрывающей скобки не удалась
                    }
                }
                else {  // Иначе
                    // Обрабатываем ошибку
                    Error error;
                    if (hasValidLeftOperand)    // Если есть левый валидный операнд
                        addOperandsError(errors, Error::Type::noRightOperand, operators.top().value, operators.top().start);  // Отсутствует правый операнд
                    else if (hasValidRightOperand)  // Если есть правый валидный операнд
                        addOperandsError(errors, Error::Type::noLeftOperand, operators.top().value, operators.top().start);   // Отсутствует левый операнд
                    else    // Иначе
                        addOperandsError(errors, Error::Type::noBothOperands, operators.top().value, operators.top().start);  // Отсутствуют оба операнда
                    return false;   // Обработка закрывающей скобки не удалась
                }
            }
            else if (nodes.size() == 1) {   // Если в стеке узлов всего один узел
                rightOperand = nodes.top(); // Получаем данный узел (правый операнд)

                // Проверяем валидность правого операнда (находится внутри блока скобок и после оператора)
                hasValidRightOperand = rightOperand->getStart() > positionsOpenParenthesis.top() &&
                                       rightOperand->getStart() > operators.top().end;

                // Обрабатываем ошибку
                Error error;
                if (hasValidRightOperand)   // Если правый операнд является валидным
                    addOperandsError(errors, Error::Type::noLeftOperand, operators.top().value, operators.top().start); // Отсутствует левый операнд
                else    // Иначе
                    addOperandsError(errors, Error::Type::noBothOperands, operators.top().value, operators.top().start);    // Отсутствуют оба операнда
                return false;   // Обработка закрывающей скобки не удалась
            }
            else {  // Иначе
                // Обработать ошибку
                addOperandsError(errors, Error::Type::noBothOperands, operators.top().value, operators.top().start);    // Отсутствуют оба операнда
                return false;   // Обработка закрывающей скобки не удалась
            }
        }
    }
    if (!operators.isEmpty() && operators.top().value == "(") { // Если на верху стека операторов - открывающая скобка
        operators.pop(); // Удаляем открывающую скобку
        // Удаляем позицию данной скобки из стека позиций
        if (!positionsOpenParenthesis.isEmpty()) {
            positionsOpenParenthesis.pop();
        }
    }
    else {  // Иначе
        // Обрабатываем ошибку
        Error error;
        error.setType(Error::Type::noOpeningParenthesis);   // Отсутствует открывающая скобка к закрывающей
        error.setSymbolIndex(token.start);
        errors.insert(error);
        return false;   // Обработка закрывающей скобки не удалась
    }
    return true;    // Обработка закрывающей скобки удалась
}

bool ExpressionTree::handleOperator(const Token& token, QStack<Token>& operators, QStack<ExpressionTree*>& nodes, QSet<Error>& errors) {
    // Обрабатываем оператор
    if (token.value != "(" && token.value != ")") { // Если токен - оператор (не скобка)
        // Сохраняем унарные минусы до появления операнда
        if (token.value == "-u" && (operators.isEmpty() || operators.top().value == "(" || nodes.isEmpty())) {  // Если токен - унарный минус и соблюдаются условия его унарности
            operators.push(token);  // Добавляем унарный минус к стеку операторов
        }
        else {  // Иначе
            while (!operators.isEmpty() && operators.top().value != "(" &&
                   operatorsPrecedence.value(operators.top().value) >= operatorsPrecedence.value(token.value)) {    // Пока в стеке операторов есть операторы с приоритетом больше, чем приоритет токена
                // Обрабатываем оператор
                if (!processOperatorFromStack(operators, nodes, errors)) {  // Если обработка не удалась
                    // Если есть ошибки
                    if (!errors.isEmpty()) return false;    // Обработка оператора не удалась
                }
            }
            operators.push(token);  // Добавляем токен в стек операторов
        }
    }
    return true;    // Обработка оператора удалась
}

bool ExpressionTree::handleOperand(const Token& token, QStack<Token>& operators, QStack<ExpressionTree*>& nodes, QSet<Error>& errors) {
    bool correctValue;
    double value = token.value.toDouble(&correctValue);
    // Обработка операнда
    if (correctValue && isValidNumber(value)) { // Если из токена корректно извлекается число и число в необходимом диапазоне
        ExpressionTree* node = new ExpressionTree(token.value, nullptr, nullptr, token.start, token.end);   // Создаём узел с данным операндом
        nodes.push(node);   // Добавляем текущий узел к стеку узлов
        // Обрабатываем накопленные унарные минусы
        while (!operators.isEmpty() && operators.top().value == "-u") { // Пока не достигнем унарного минуса в стеке операторов
            // Обрабатываем оператор
            if (!processOperatorFromStack(operators, nodes, errors)) {  // Если обработка не удалась
                // Если есть ошибки
                if (!errors.isEmpty()) return false;    // Обработка операнда не удалась
            }
        }
    }
    else {  // Иначе
        // Обрабатываем ошибку
        Error error;
        error.setType(Error::Type::operandOutOfRange);  // Число вышло за допустимые значения
        errors.insert(error);
        return false;   // Обработка операнда не удалась
    }
    return true;    // Обработка операнда удалась
}

QString ExpressionTree::createSubexpressionString(const ExpressionTree* node) {
    // Проверяем наличие узла
    if (!node)  // Если узла нет
        return ""; // Возвращаем пустую строку

    // Обрабатываем операнд
    if (node->nodeType == NodeType::Operand)    // Если это операнд
        return QString::number(node->value); // Возвращаем строковое представление значения операнда

    // Получаем границы покрытия узла
    int start = node->getStart();
    int end = node->getEnd();

    // Находим индексы первого и последнего токенов в диапазоне
    int firstTokenIndex, lastTokenIndex;
    findIndexesFirstAndLastTokensInRange(expressionTokens, start, end, firstTokenIndex, lastTokenIndex);

    // Проверяем нашлись ли индексы токенов
    if (firstTokenIndex == -1 || lastTokenIndex == -1) {    // Если хотя бы один токен не найден
        return ""; // Возвращаем пустую строку
    }

    QList<Token> subTokens = getTokensInRange(expressionTokens, start, end);    // Собираем токены в диапазоне покрытия узла
    QString result = tokensToString(subTokens); // Формируем строку из токенов покрытия

    // Считаем количество открывающих и закрывающих скобок
    int countOpenParenthesis = result.count('(');
    int countClosingParenthesis = result.count(')');

    // Проверяем баланс скобок
    if (countOpenParenthesis != countClosingParenthesis) {  // Если скобки не сбалансированы
        balanceParenthesesRange(expressionTokens, start, end, subTokens, result);   // Балансируем скобки в диапазоне
    }

    result = extendToOuterParentheses(expressionTokens, start, end, subTokens); // Расширяем диапазон до внешних парных скобок
    return result.trimmed();    // Возвращаем строку подвыражения без лишних пробелов
}

void ExpressionTree::findIndexesFirstAndLastTokensInRange(const QList<Token>& tokens, int start, int end, int& firstTokenIndex, int& lastTokenIndex) {
    // Считаем, что индексы не найдены
    firstTokenIndex = -1;
    lastTokenIndex = -1;

    // Ищем индексы
    for (int i = 0; i < tokens.size(); ++i) {   // Для каждого токена
        if (tokens[i].start == start)   // Если начальная позиция токена равна начальной позиции диапазона
            firstTokenIndex = i;    // Индекс начального токена найден
        if (tokens[i].end == end)   // Если конечная позиция токена равна конечной позиции диапазона
            lastTokenIndex = i;     // Индекс конечного токена найден
    }
}

QList<Token> ExpressionTree::getTokensInRange(const QList<Token>& tokens, int start, int end) {
    QList<Token> subTokens; // Считаем, что токены не найдены

    // Ищем токены в нужном диапазоне
    for (const Token& token : tokens) { // Для каждого токена
        if (token.start >= start && token.end <= end) { // Если позиции токена в нужнем диапазоне
            subTokens.append(token);    // Добавляем токен в список необходимых
        }
    }

    return subTokens;   // Возвращаем список полученных токенов
}

QString ExpressionTree::tokensToString(const QList<Token>& tokens) {
    QString result; // Считаем, что строка пуста

    // Строим строку на основе токенов
    for (const Token& token : tokens) { // Для каждого токена
        result += (token.value == "-u") ? "-" : token.value;    // Добавляем к строке значение токена
    }
    return result;  // Возвращаем построенную строку
}

void ExpressionTree::balanceParenthesesRange(const QList<Token>& tokens, int& start, int& end, QList<Token>& subTokens, QString& result) {
    // Находим индексы первого и последнего токенов в диапазоне
    int firstTokenIndex, lastTokenIndex;
    findIndexesFirstAndLastTokensInRange(tokens, start, end, firstTokenIndex, lastTokenIndex);

    // Устанавливаем начальные индексы для поиска скобок
    int leftIndex = firstTokenIndex - 1;
    int rightIndex = lastTokenIndex + 1;

    // Считаем количество открывающих и закрывающих скобок
    int openParCount = result.count('(');
    int closeParCount = result.count(')');

    // Расширяем диапазон, пока скобки не сбалансированы
    while (openParCount != closeParCount && (leftIndex >= 0 || rightIndex < tokens.size())) { // Пока скобки не сбалансированы и есть токены для проверки
        bool expanded = false;  // Инициализируем флаг расширения диапазона

        // Добавляем открывающую скобку слева, если нужно
        if (openParCount < closeParCount && leftIndex >= 0 && tokens[leftIndex].value == "(") { // Если не хватает открывающих скобок
            start = tokens[leftIndex].start;    // Обновляем начальную позицию диапазона
            expanded = true;    // Указываем, что диапазон расширен
            leftIndex--;    // Сдвигаем индекс влево
            openParCount++; // Увеличиваем счётчик открывающих скобок
        }
        // Добавляем закрывающую скобку справа, если нужно
        else if (openParCount > closeParCount && rightIndex < tokens.size() && tokens[rightIndex].value == ")") { // Если не хватает закрывающих скобок
            end = tokens[rightIndex].end;   // Обновляем конечную позицию диапазона
            expanded = true;    // Указываем, что диапазон расширен
            rightIndex++;   // Сдвигаем индекс вправо
            closeParCount++;    // Увеличиваем счётчик закрывающих скобок
        }

        // Обновляем токены и строку, если диапазон расширен
        if (expanded) { // Если добавлены скобки
            subTokens.clear();  // Очищаем текущий список токенов
            result.clear(); // Очищаем текущую строку
            subTokens = getTokensInRange(tokens, start, end);   // Собираем токены в новом диапазоне
            result = tokensToString(subTokens); // Формируем строку из новых токенов
        }
        else {  // Иначе
            break; // Завершаем цикл, если скобки не добавлены
        }
    }
}

QString ExpressionTree::extendToOuterParentheses(const QList<Token>& tokens, int& start, int& end, QList<Token>& subTokens) {
    // Устанавливаем начальные границы диапазона
    int newStart = start;
    int newEnd = end;

    // Расширяем диапазон, пока есть парные скобки
    while (true) { // Пока можно добавить парные скобки
        // Инициализируем флаги наличия скобок
        bool hasLeftParen = false;
        bool hasRightParen = false;

        // Проверяем токен слева от newStart
        for (int i = 0; i < tokens.size(); ++i) { // Для каждого токена
            if (tokens[i].end == newStart - 1 && tokens[i].value == "(") { // Если найден токен '(' слева
                hasLeftParen = true;    // Указываем наличие открывающей скобки
                newStart = tokens[i].start; // Обновляем начальную позицию
                break;  // Прерываем поиск
            }
        }

        // Проверяем токен справа от newEnd
        for (int i = 0; i < tokens.size(); ++i) { // Для каждого токена
            if (tokens[i].start == newEnd + 1 && tokens[i].value == ")") { // Если найден токен ')' справа
                hasRightParen = true;   // Указываем наличие закрывающей скобки
                newEnd = tokens[i].end; // Обновляем конечную позицию
                break;  // Прерываем поиск
            }
        }

        // Обновляем диапазон, если найдены обе скобки
        if (hasLeftParen && hasRightParen) { // Если есть пара скобок
            start = newStart;   // Устанавливаем новую начальную позицию
            end = newEnd;   // Устанавливаем новую конечную позицию
            subTokens = getTokensInRange(tokens, start, end);   // Собираем токены в новом диапазоне
        }
        else {    // Иначе
            break; // Завершаем цикл, если хотя бы одной скобки нет
        }
    }

    // Формируем строку из токенов
    return tokensToString(subTokens); // Возвращаем строку подвыражения
}

double ExpressionTree::calculate(QSet<Error>& errors) const {
    // Обрабатываем операнд
    if (nodeType == NodeType::Operand)  // Если текущий узел - операнд
        return value;   // Возвращаем его значение
    // Обрабатываем унарный минус
    else if (nodeType == NodeType::UnaryMinus) {    // Если текущий узел - унарный минус
        double leftOperand = left->calculate(errors);   // Получаем значение операнда
        return leftOperand * (-1);  // Возвращаем значение операнда умноженное на (-1)
    }
    // Обрабатываем бинарные операторы
    else {  // Иначе
        double leftOperand = left->calculate(errors);   // Получаем значение левого операнда
        double rightOperand = right->calculate(errors); // Получаем значение правого операнда

        // Обрабатываем сумму
        if (nodeType == NodeType::Plus) // Если текущий узел - плюс
            return leftOperand + rightOperand;  // Возвращаем сумму операндов
        // Обрабатываем бинарный минус
        else if (nodeType == NodeType::BinaryMinus) // Если текущий узел - бинарный минус
            return leftOperand - rightOperand;  // Возвращаем разность левого и правого операндов
        // Обрабатываем произведение
        else if (nodeType == NodeType::Multiplication)  // Если текущий узел - произведение
            return leftOperand * rightOperand;  // Возвращаем произведение левого и правого операндов
        // Обрабатываем частное
        else if (nodeType == NodeType::Division) {  // Если текущий узел - частное
            // Обрабатываем деление на ноль
            if (rightOperand == 0) {    // Если значение правого операнда равно нулю
                // Обрабатываем ошибку
                Error error;
                error.setType(Error::Type::divisionByZero);
                QString expression = createSubexpressionString(this);
                error.setExpression(expression);
                errors.insert(error);
                throw errors;   // Расчёт окончен с ошибкой
            }
            return leftOperand / rightOperand;  // Возвращаем частное левого и правого операндов
        }
        // Обрабатываем возведение в степень
        else if (nodeType == NodeType::Power) { // Если текущий узел - возведение в степень
            // Обрабатываем возведение в степень отрицательного числа
            if (leftOperand < 0) {  // Если левый операнд - отрицательный
                // Проверяем, является ли показатель дробью 1/n, где n — целое
                double inverse = 1.0 / rightOperand;    // Получаем n
                double intPart;
                double fractPart = std::modf(inverse, &intPart);
                if (std::abs(fractPart) < 1e-10) { // Если это показатель вида 1/n
                    int n = static_cast<int>(intPart);
                    // Проверяем чётность корня
                    if (n % 2 == 0) {  // Если корень чётный
                        // Обрабатываем ошибку
                        Error error;
                        error.setType(Error::Type::incorrectRoot);  // Некорректное извлечение корня
                        QString expression = createSubexpressionString(this);
                        error.setExpression(expression);
                        errors.insert(error);
                        throw errors;   // Расчёт окончен с ошибкой
                    } else { // Иначе
                        return -std::pow(-leftOperand, rightOperand);   // Возвращаем отрицательное значение извлечённого корня
                    }
                } else if (std::abs(rightOperand - std::round(rightOperand)) < 1e-10) { // Если это целый показатель
                    return std::pow(leftOperand, rightOperand); // Возвращаем значение возведения в степень
                } else { // Если это дробный показатель, не 1/n
                    // Обрабатываем ошибку
                    Error error;
                    error.setType(Error::Type::incorrectRoot);  // Некорректное извлечение корня
                    QString expression = createSubexpressionString(this);
                    error.setExpression(expression);
                    errors.insert(error);
                    throw errors;   // Расчёт окончен с ошибкой
                }
            }
            return std::pow(leftOperand, rightOperand); // Возвращаем значение возведения в степень
        }
    }
    return 0;
}

int ExpressionTree::countOperands(NodeType type) {
    switch(type) {
        case NodeType::Plus:
        case NodeType::BinaryMinus:
        case NodeType::Multiplication:
        case NodeType::Division:
        case NodeType::Power: return 2;
        case NodeType::UnaryMinus: return 1;
        default: return 0;
    }
}
