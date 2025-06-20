#include "expressiontree.h"
#include "calculationtools.h"

ExpressionTree::ExpressionTree(const QString& operation, ExpressionTree* left, ExpressionTree* right, int start, int end) {
    this->nodeType = determineNodeType(operation);

    if (nodeType == NodeType::Operand) {
        bool validDouble;
        double value = operation.toDouble(&validDouble);
        if (validDouble && isValidNumber(value))
            this->value = value;
    }
    else this->value = 0;

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
            // Обрабатываем ошибку (Отсутствует правый операнд)
            Error error;
            error.setType(Error::Type::noRightOperand);
            error.setOperation("-");
            error.setSymbolIndex(operatorToken.start);
            errors.insert(error);
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
        Error error;
        if (nodes.size() == 1) {    // Если в стеке узлов остался только один узел
            // Отсутствует правый операнд
            error.setType(Error::Type::noRightOperand);
            error.setOperation(operatorToken.value);
        } else {    // Иначе
            // Отсутствуют оба операнда
            error.setType(Error::Type::noBothOperands);
            error.setOperation(operatorToken.value);
        }
        error.setSymbolIndex(operatorToken.start);
        errors.insert(error);
        return false;   // Обработка не удалась
    }
}

bool ExpressionTree::isValidNumber(double number) const {
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
    return nullptr;
}

double ExpressionTree::calculate(QSet<Error>& errors) const {
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
