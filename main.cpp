#include "test_tokenize.h"
#include "test_build.h"
#include "test_calculate.h"
#include "calculationtools.h"
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    Test_Tokenize test;
    QTest::qExec(&test);

    Test_Build test_build;
    QTest::qExec(&test_build);

    Test_Calculate test_calculate;
    QTest::qExec(&test_calculate);
}
