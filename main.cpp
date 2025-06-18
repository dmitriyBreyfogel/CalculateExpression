#include "test_tokenize.h"
#include "test_build.h"
#include "test_calculate.h"
#include "test_processoperatorfromstack.h"
#include "test_determinenodetype.h"
#include "calculationtools.h"
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    Test_Tokenize test_tokenize;
    QTest::qExec(&test_tokenize);

    Test_Build test_build;
    QTest::qExec(&test_build);

    Test_Calculate test_calculate;
    QTest::qExec(&test_calculate);

    Test_ProcessOperatorFromStack test_processOperatorFromStack;
    QTest::qExec(&test_processOperatorFromStack);

    Test_DetermineNodeType test_determineNodeType;
    QTest::qExec(&test_determineNodeType);
}
