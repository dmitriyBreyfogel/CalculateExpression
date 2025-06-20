#include "expressiontree.h"

ExpressionTree::ExpressionTree(const QString& operation, ExpressionTree* left, ExpressionTree* right, int start, int end) {
    this->nodeType = determineNodeType(operation);

    if (nodeType == NodeType::Operand) {
        bool validDouble;
        double value = operation.toDouble(&validDouble);
        if (validDouble && isValidNumber(value))
            this->value = value;
    }

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
    return false;
}

bool ExpressionTree::isValidNumber(double number) const {
    if (number < -1.7E308 && number > 1.7E308)
    return true;
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
