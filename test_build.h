#ifndef TEST_BUILD_H
#define TEST_BUILD_H

#include <QObject>
#include <QtTest/QtTest>
#include "calculationtools.h"
#include "expressiontree.h"

class Test_Build : public QObject
{
    Q_OBJECT

private slots:
    void testBuild();
    void testBuild_data();

private:
    QString compareErrors(const QSet<Error>& actual, const QSet<Error>& expected);
    bool compareTrees(const ExpressionTree* expected, const ExpressionTree* actual, QStringList& tracePath, QString& error);
};

#endif // TEST_BUILD_H
