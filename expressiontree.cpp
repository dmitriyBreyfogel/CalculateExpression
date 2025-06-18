#include "expressiontree.h"

ExpressionTree::ExpressionTree(const QString& operation, ExpressionTree* left, ExpressionTree* right, int start, int end) {
    this->nodeType = determineNodeType(operation);
    if (nodeType == NodeType::Operand) {
        this->value = operation.toDouble();
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

const QMap<QString, int> operatorsPrecedence = {
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
    return "";
}

ExpressionTree* ExpressionTree::build(const QList<Token>& tokens, QSet<Error>& errors) {
    return nullptr;
}

double ExpressionTree::calculate(QSet<Error>& errors) const {
    return 0;
}

int ExpressionTree::countOperands(NodeType type) {
    return -1;
}
