#ifndef TEST_DETERMINENODETYPE_H
#define TEST_DETERMINENODETYPE_H

#include <QObject>
#include <QtTest/QtTest>
#include "calculationtools.h"
#include "expressiontree.h"

class Test_DetermineNodeType : public QObject
{
    Q_OBJECT

private slots:
    void testDetermineNodeType();
    void testDetermineNodeType_data();

private:
    QString compareErrors(const QSet<Error>& actual, const QSet<Error>& expected);
};

#endif // TEST_DETERMINENODETYPE_H
