#ifndef TEST_PROCESSOPERATORFROMSTACK_H
#define TEST_PROCESSOPERATORFROMSTACK_H

#include <QObject>
#include <QtTest/QtTest>
#include "calculationtools.h"
#include "expressiontree.h"

class Test_ProcessOperatorFromStack : public QObject
{
    Q_OBJECT

private slots:
    void testProcessOperatorFromStack();
    void testProcessOperatorFromStack_data();

private:
    QString compareTokenStacks(const QStack<Token>& actual, const QStack<Token>& expected);
    QString compareNodeStacks(const QStack<ExpressionTree*>& actual, const QStack<ExpressionTree*>& expected);
    QString compareExpressionTrees(ExpressionTree* actual, ExpressionTree* expected);
    QString compareErrors(const QSet<Error>& actual, const QSet<Error>& expected);
};

#endif // TEST_PROCESSOPERATORFROMSTACK_H
