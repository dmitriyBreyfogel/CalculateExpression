#ifndef EXPRESSIONTREE_H
#define EXPRESSIONTREE_H

#include <QString>
#include <QMap>
#include <QList>
#include <QStack>
#include "calculationtools.h"
#include "error.h"

class ExpressionTree
{
public:
    enum NodeType {
        Operand,
        Plus,
        BinaryMinus,
        Multiplication,
        Division,
        Power,
        UnaryMinus
    };

    ExpressionTree(const QString& operation, ExpressionTree* left, ExpressionTree* right, int start, int end);
    ~ExpressionTree();

    static const QMap<QString, int> operatorsPrecedence;

    int getStart() const;
    int getEnd() const;
    double getValue() const;
    NodeType getNodeType() const;

    static int countOperands(NodeType type);
    static ExpressionTree* build(const QList<Token>& tokens, QSet<Error>& errors);
    double calculate(QSet<Error>& errors) const;

private:
    ExpressionTree* left;
    ExpressionTree* right;
    NodeType nodeType;
    double value;
    int start;
    int end;

    static bool processOperatorFromStack(QStack<Token>& operators, QStack<ExpressionTree*>& nodes, QSet<Error>& errors);
    bool isValidNumber(double number) const;
    static NodeType determineNodeType(const QString& token);
};

#endif // EXPRESSIONTREE_H
