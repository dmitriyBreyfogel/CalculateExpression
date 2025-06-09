#include "test_tokenize.h"
#include "calculationtools.h"
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    Test_Tokenize test;
    QTest::qExec(&test);
}
