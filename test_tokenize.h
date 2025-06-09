#ifndef TEST_TOKENIZE_H
#define TEST_TOKENIZE_H

#include <QObject>
#include <QtTest/QtTest>
#include "calculationtools.h"

class Test_Tokenize : public QObject
{
    Q_OBJECT

private slots:
    void testTokenize();
    void testTokenize_data();

private:
    QStringList compareTokenLists(const QList<Token>& actual, const QList<Token>& expected);
    QString compareErrors(const QSet<Error>& actual, const QSet<Error>& expected);
};

#endif // TEST_TOKENIZE_H
