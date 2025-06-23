/*!
 * \file
 * \brief Реализация функции запуска тестирования основных функций
 */

#include "testrunner.h"

void runTests() {
    // Тестирование функции tokenize
    Test_Tokenize test_tokenize;
    QTest::qExec(&test_tokenize);

    // Тестирование функции ExpressionTree::build
    Test_Build test_build;
    QTest::qExec(&test_build);

    // Тестирование функции ExpressionTree::calculate
    Test_Calculate test_calculate;
    QTest::qExec(&test_calculate);

    // Тестирование функции ExpressionTree::processOperatorFromStack
    Test_ProcessOperatorFromStack test_processOperatorFromStack;
    QTest::qExec(&test_processOperatorFromStack);

    // Тестирование функции ExpressionTree::determineNodeType
    Test_DetermineNodeType test_determineNodeType;
    QTest::qExec(&test_determineNodeType);
}
