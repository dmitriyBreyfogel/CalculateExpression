#ifndef TEST_CALCULATE_H
#define TEST_CALCULATE_H

#include <QObject>
#include <QtTest/QtTest>
#include "calculationtools.h"
#include "expressiontree.h"

class Test_Calculate : public QObject
{
    Q_OBJECT

private slots:
    void testCalculate();
    void testCalculate_data();

private:
    static QList<Token> testTokens;
    QString compareErrors(const QSet<Error>& actual, const QSet<Error>& expected);
};

#endif // TEST_CALCULATE_H
