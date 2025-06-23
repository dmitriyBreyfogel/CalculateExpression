/*!
* \file
* \brief Реализация методов класса тестирования функции ExpressionTree::calculate
*/

#include "test_calculate.h"

QList<Token> Test_Calculate::testTokens;

void Test_Calculate::testCalculate() {
    QFETCH(ExpressionTree*, root);
    QFETCH(double, result);
    QFETCH(QSet<Error>, errors);
    QFETCH(QList<Token>, tokens);


    ExpressionTree::expressionTokens = tokens;

    QSet<Error> actualErrors;
    double actualResult = 0.0;
    try {
        actualResult = root->calculate(actualErrors);
    } catch (const QSet<Error>& thrownErrors) {
        actualErrors = thrownErrors;
    }

    QString errorComparison = compareErrors(actualErrors, errors);
    QVERIFY2(errorComparison.isEmpty(), qPrintable(errorComparison));

    if (errors.isEmpty()) {
        const double epsilon = 1e-8;
        bool resultMatch = qAbs(actualResult - result) < epsilon;

        QVERIFY2(resultMatch, qPrintable(
                                  QString("Result mismatch. Expected: %1, Actual: %2")
                                      .arg(result)
                                      .arg(actualResult)
                                  ));
    }

    delete root;
}

void Test_Calculate::testCalculate_data() {
    QTest::addColumn<ExpressionTree*>("root");
    QTest::addColumn<double>("result");
    QTest::addColumn<QSet<Error>>("errors");
    QTest::addColumn<QList<Token>>("tokens");

    QSet<Error> emptyErrors = {};

    // Тест 1
    testTokens.clear();
    testTokens.append(Token("1", 0, 0));

    ExpressionTree* node_1 = new ExpressionTree("1", nullptr, nullptr, 0, 0);
    double result_1 = 1;

    QTest::newRow("1. Single number (integer)") << node_1 << result_1 << emptyErrors << testTokens;

    // Тест 2
    testTokens.clear();
    testTokens.append(Token("1.234", 0, 4));

    ExpressionTree* node_2 = new ExpressionTree("1.234", nullptr, nullptr, 0, 4);
    double result_2 = 1.234;

    QTest::newRow("2. Single number (float)") << node_2 << result_2 << emptyErrors << testTokens;

    // Тест 3
    testTokens.clear();
    testTokens.append(Token("1", 0, 0));
    testTokens.append(Token("+", 1, 1));
    testTokens.append(Token("4", 2, 2));

    ExpressionTree* node_3_1 = new ExpressionTree("1", nullptr, nullptr, 0, 0);
    ExpressionTree* node_3_2 = new ExpressionTree("4", nullptr, nullptr, 2, 2);
    ExpressionTree* node_3 = new ExpressionTree("+", node_3_1, node_3_2, 0, 2);

    double result_3 = 1 + 4;

    QTest::newRow("3. Simple addition (integers)") << node_3 << result_3 << emptyErrors << testTokens;

    // Тест 4
    testTokens.clear();
    testTokens.append(Token("1.234", 0, 4));
    testTokens.append(Token("+", 5, 5));
    testTokens.append(Token("5.678", 6, 10));

    ExpressionTree* node_4_1 = new ExpressionTree("1.234", nullptr, nullptr, 0, 4);
    ExpressionTree* node_4_2 = new ExpressionTree("5.678", nullptr, nullptr, 6, 10);
    ExpressionTree* node_4 = new ExpressionTree("+", node_4_1, node_4_2, 0, 10);

    double result_4 = 1.234 + 5.678;

    QTest::newRow("4. Simple addition (floats)") << node_4 << result_4 << emptyErrors << testTokens;

    // Тест 5
    testTokens.clear();
    testTokens.append(Token("1", 0, 0));
    testTokens.append(Token("-", 1, 1));
    testTokens.append(Token("4", 2, 2));

    ExpressionTree* node_5_1 = new ExpressionTree("1", nullptr, nullptr, 0, 0);
    ExpressionTree* node_5_2 = new ExpressionTree("4", nullptr, nullptr, 2, 2);
    ExpressionTree* node_5 = new ExpressionTree("-", node_5_1, node_5_2, 0, 2);

    double result_5 = 1 - 4;

    QTest::newRow("5. Simple subtraction (integers)") << node_5 << result_5 << emptyErrors << testTokens;

    // Тест 6
    testTokens.clear();
    testTokens.append(Token("1.234", 0, 4));
    testTokens.append(Token("-", 5, 5));
    testTokens.append(Token("5.678", 6, 10));

    ExpressionTree* node_6_1 = new ExpressionTree("1.234", nullptr, nullptr, 0, 4);
    ExpressionTree* node_6_2 = new ExpressionTree("5.678", nullptr, nullptr, 6, 10);
    ExpressionTree* node_6 = new ExpressionTree("-", node_6_1, node_6_2, 0, 10);

    double result_6 = 1.234 - 5.678;

    QTest::newRow("6. Simple subtraction (floats)") << node_6 << result_6 << emptyErrors << testTokens;

    // Тест 7
    testTokens.clear();
    testTokens.append(Token("1", 0, 0));
    testTokens.append(Token("/", 1, 1));
    testTokens.append(Token("4", 2, 2));

    ExpressionTree* node_7_1 = new ExpressionTree("1", nullptr, nullptr, 0, 0);
    ExpressionTree* node_7_2 = new ExpressionTree("4", nullptr, nullptr, 2, 2);
    ExpressionTree* node_7 = new ExpressionTree("/", node_7_1, node_7_2, 0, 2);

    double result_7 = 1.0 / 4.0;

    QTest::newRow("7. Simple division (integers)") << node_7 << result_7 << emptyErrors << testTokens;

    // Тест 8
    testTokens.clear();
    testTokens.append(Token("1.234", 0, 4));
    testTokens.append(Token("/", 5, 5));
    testTokens.append(Token("5.678", 6, 10));

    ExpressionTree* node_8_1 = new ExpressionTree("1.234", nullptr, nullptr, 0, 4);
    ExpressionTree* node_8_2 = new ExpressionTree("5.678", nullptr, nullptr, 6, 10);
    ExpressionTree* node_8 = new ExpressionTree("/", node_8_1, node_8_2, 0, 10);

    double result_8 = 1.234 / 5.678;

    QTest::newRow("8. Simple division (floats)") << node_8 << result_8 << emptyErrors << testTokens;

    // Тест 9
    testTokens.clear();
    testTokens.append(Token("1", 0, 0));
    testTokens.append(Token("*", 1, 1));
    testTokens.append(Token("4", 2, 2));

    ExpressionTree* node_9_1 = new ExpressionTree("2", nullptr, nullptr, 0, 0);
    ExpressionTree* node_9_2 = new ExpressionTree("4", nullptr, nullptr, 2, 2);
    ExpressionTree* node_9 = new ExpressionTree("*", node_9_1, node_9_2, 0, 2);

    double result_9 = 2 * 4;

    QTest::newRow("9. Simple multiplication (integers)") << node_9 << result_9 << emptyErrors << testTokens;

    // Тест 10
    testTokens.clear();
    testTokens.append(Token("1.234", 0, 4));
    testTokens.append(Token("*", 5, 5));
    testTokens.append(Token("5.678", 6, 10));

    ExpressionTree* node_10_1 = new ExpressionTree("1.234", nullptr, nullptr, 0, 4);
    ExpressionTree* node_10_2 = new ExpressionTree("5.678", nullptr, nullptr, 6, 10);
    ExpressionTree* node_10 = new ExpressionTree("*", node_10_1, node_10_2, 0, 10);

    double result_10 = 1.234 * 5.678;

    QTest::newRow("10. Simple multiplication (floats)") << node_10 << result_10 << emptyErrors << testTokens;

    // Тест 11
    testTokens.clear();
    testTokens.append(Token("1", 0, 0));
    testTokens.append(Token("^", 1, 1));
    testTokens.append(Token("4", 2, 2));

    ExpressionTree* node_11_1 = new ExpressionTree("2", nullptr, nullptr, 0, 0);
    ExpressionTree* node_11_2 = new ExpressionTree("4", nullptr, nullptr, 2, 2);
    ExpressionTree* node_11 = new ExpressionTree("^", node_11_1, node_11_2, 0, 2);

    double result_11 = 16;

    QTest::newRow("11. Simple exponentiation (integer base and exponent)") << node_11 << result_11 << emptyErrors << testTokens;

    // Тест 12
    testTokens.clear();
    testTokens.append(Token("1.234", 0, 4));
    testTokens.append(Token("^", 5, 5));
    testTokens.append(Token("5.678", 6, 10));

    ExpressionTree* node_12_1 = new ExpressionTree("1.234", nullptr, nullptr, 0, 4);
    ExpressionTree* node_12_2 = new ExpressionTree("5.678", nullptr, nullptr, 6, 10);
    ExpressionTree* node_12 = new ExpressionTree("^", node_12_1, node_12_2, 0, 10);

    double result_12 = 3.29979893;

    QTest::newRow("12. Simple exponentiation (float base and exponent)") << node_12 << result_12 << emptyErrors << testTokens;

    // Тест 13
    testTokens.clear();
    testTokens.append(Token("4", 0, 0));
    testTokens.append(Token("^", 1, 1));
    testTokens.append(Token("0.5", 2, 4));

    ExpressionTree* node_13_1 = new ExpressionTree("4", nullptr, nullptr, 0, 0);
    ExpressionTree* node_13_2 = new ExpressionTree("0.5", nullptr, nullptr, 2, 4);
    ExpressionTree* node_13 = new ExpressionTree("^", node_13_1, node_13_2, 0, 4);

    double result_13 = 2;

    QTest::newRow("13. Square root (exponent 0.5)") << node_13 << result_13 << emptyErrors << testTokens;

    // Тест 14
    testTokens.clear();
    testTokens.append(Token("-u", 0, 0));
    testTokens.append(Token("2", 1, 1));
    testTokens.append(Token("^", 2, 2));
    testTokens.append(Token("(", 3, 3));
    testTokens.append(Token("1", 4, 4));
    testTokens.append(Token("/", 5, 5));
    testTokens.append(Token("3", 6, 6));
    testTokens.append(Token(")", 7, 7));

    ExpressionTree* node_14_1_1 = new ExpressionTree("2", nullptr, nullptr, 1, 1);
    ExpressionTree* node_14_1 = new ExpressionTree("-u", node_14_1_1, nullptr, 0, 1);
    ExpressionTree* node_14_2_1 = new ExpressionTree("1", nullptr, nullptr, 4, 4);
    ExpressionTree* node_14_2_2 = new ExpressionTree("3", nullptr, nullptr, 6, 6);
    ExpressionTree* node_14_2 = new ExpressionTree("/", node_14_2_1, node_14_2_2, 3, 7);
    ExpressionTree* node_14 = new ExpressionTree("^", node_14_1, node_14_2, 0, 7);

    double result_14 = -1.25992104989;

    QTest::newRow("14. Complex expression with unary minus and exponentiation") << node_14 << result_14 << emptyErrors << testTokens;

    // Тест 15
    testTokens.clear();
    testTokens.append(Token("1", 0, 0));
    testTokens.append(Token("+", 1, 1));
    testTokens.append(Token("2", 2, 2));
    testTokens.append(Token("-", 3, 3));
    testTokens.append(Token("3", 4, 4));
    testTokens.append(Token("*", 5, 5));
    testTokens.append(Token("4", 6, 6));
    testTokens.append(Token("/", 7, 7));
    testTokens.append(Token("5", 8, 8));
    testTokens.append(Token("^", 9, 9));
    testTokens.append(Token("6", 10, 10));

    ExpressionTree* node_15_1_1 = new ExpressionTree("1", nullptr, nullptr, 0, 0);
    ExpressionTree* node_15_1_2 = new ExpressionTree("2", nullptr, nullptr, 2, 2);
    ExpressionTree* node_15_1 = new ExpressionTree("+", node_15_1_1, node_15_1_2, 0, 2);
    ExpressionTree* node_15_2_1_1 = new ExpressionTree("3", nullptr, nullptr, 4, 4);
    ExpressionTree* node_15_2_1_2 = new ExpressionTree("4", nullptr, nullptr, 6, 6);
    ExpressionTree* node_15_2_1 = new ExpressionTree("*", node_15_2_1_1, node_15_2_1_2, 4, 6);
    ExpressionTree* node_15_2_2_1 = new ExpressionTree("5", nullptr, nullptr, 8, 8);
    ExpressionTree* node_15_2_2_2 = new ExpressionTree("6", nullptr, nullptr, 10, 10);
    ExpressionTree* node_15_2_2 = new ExpressionTree("^", node_15_2_2_1, node_15_2_2_2, 8, 10);
    ExpressionTree* node_15_2 = new ExpressionTree("/", node_15_2_1, node_15_2_2, 4, 10);
    ExpressionTree* node_15 = new ExpressionTree("-", node_15_1, node_15_2, 0, 10);

    double result_15 = 2.999232;

    QTest::newRow("15. Complex nested operations") << node_15 << result_15 << emptyErrors << testTokens;

    // Тест 16
    testTokens.clear();
    testTokens.append(Token("-u", 0, 0));
    testTokens.append(Token("1", 1, 1));
    testTokens.append(Token("+", 2, 2));
    testTokens.append(Token("2", 3, 3));
    testTokens.append(Token("*", 4, 4));
    testTokens.append(Token("3", 5, 5));
    testTokens.append(Token("^", 6, 6));
    testTokens.append(Token("5", 7, 7));

    ExpressionTree* node_16_1_1 = new ExpressionTree("1", nullptr, nullptr, 1, 1);
    ExpressionTree* node_16_1 = new ExpressionTree("-u", node_16_1_1, nullptr, 0, 1);
    ExpressionTree* node_16_2_2_1 = new ExpressionTree("3", nullptr, nullptr, 5, 5);
    ExpressionTree* node_16_2_2_2 = new ExpressionTree("5", nullptr, nullptr, 7, 7);
    ExpressionTree* node_16_2_2 = new ExpressionTree("^", node_16_2_2_1, node_16_2_2_2, 5, 7);
    ExpressionTree* node_16_2_1 = new ExpressionTree("2", nullptr, nullptr, 3, 3);
    ExpressionTree* node_16_2 = new ExpressionTree("*", node_16_2_1, node_16_2_2, 3, 7);
    ExpressionTree* node_16 = new ExpressionTree("+", node_16_1, node_16_2, 0, 7);

    double result_16 = 485;

    QTest::newRow("16. Unary minus with exponentiation") << node_16 << result_16 << emptyErrors << testTokens;

    // Тест 17
    testTokens.clear();
    testTokens.append(Token("-u", 0, 0));
    testTokens.append(Token("(", 1, 1));
    testTokens.append(Token("1", 2, 2));
    testTokens.append(Token("+", 3, 3));
    testTokens.append(Token("5", 4, 4));
    testTokens.append(Token(")", 5, 5));
    testTokens.append(Token("^", 6, 6));
    testTokens.append(Token("2", 7, 7));

    ExpressionTree* node_17_1_1_1 = new ExpressionTree("1", nullptr, nullptr, 2, 2);
    ExpressionTree* node_17_1_1_2 = new ExpressionTree("5", nullptr, nullptr, 4, 4);
    ExpressionTree* node_17_1_1 = new ExpressionTree("+", node_17_1_1_1, node_17_1_1_2, 1, 5);
    ExpressionTree* node_17_1_2 = new ExpressionTree("2", nullptr, nullptr, 7, 7);
    ExpressionTree* node_17_1 = new ExpressionTree("^", node_17_1_1, node_17_1_2, 1, 7);
    ExpressionTree* node_17 = new ExpressionTree("-u", node_17_1, nullptr, 0, 7);

    double result_17 = -36;

    QTest::newRow("17. Nested unary minus with exponentiation") << node_17 << result_17 << emptyErrors << testTokens;

    // Тест 18
    testTokens.clear();
    testTokens.append(Token("(", 0, 0));
    testTokens.append(Token("-u", 1, 1));
    testTokens.append(Token("(", 2, 2));
    testTokens.append(Token("1", 3, 3));
    testTokens.append(Token("+", 4, 4));
    testTokens.append(Token("5", 5, 5));
    testTokens.append(Token(")", 6, 6));
    testTokens.append(Token(")", 7, 7));
    testTokens.append(Token("^", 8, 8));
    testTokens.append(Token("2", 9, 9));

    ExpressionTree* node_18_1_1_1 = new ExpressionTree("1", nullptr, nullptr, 3, 3);
    ExpressionTree* node_18_1_1_2 = new ExpressionTree("5", nullptr, nullptr, 5, 5);
    ExpressionTree* node_18_1_1 = new ExpressionTree("+", node_18_1_1_1, node_18_1_1_2, 2, 6);
    ExpressionTree* node_18_1 = new ExpressionTree("-u", node_18_1_1, nullptr, 1, 6);
    ExpressionTree* node_18_2 = new ExpressionTree("2", nullptr, nullptr, 9, 9);
    ExpressionTree* node_18 = new ExpressionTree("^", node_18_1, node_18_2, 0, 9);

    double result_18 = 36;

    QTest::newRow("18. Unary minus in base of exponentiation") << node_18 << result_18 << emptyErrors << testTokens;

    // Тест 19
    testTokens.clear();
    testTokens.append(Token("-u", 0, 0));
    testTokens.append(Token("-u", 1, 1));
    testTokens.append(Token("-u", 2, 2));
    testTokens.append(Token("(", 3, 3));
    testTokens.append(Token("1", 4, 4));
    testTokens.append(Token("+", 5, 5));
    testTokens.append(Token("5", 6, 6));
    testTokens.append(Token(")", 7, 7));

    ExpressionTree* node_19_1_1_1_1 = new ExpressionTree("1", nullptr, nullptr, 4, 4);
    ExpressionTree* node_19_1_1_1_2 = new ExpressionTree("5", nullptr, nullptr, 6, 6);
    ExpressionTree* node_19_1_1_1 = new ExpressionTree("+", node_19_1_1_1_1, node_19_1_1_1_2, 3, 7);
    ExpressionTree* node_19_1_1 = new ExpressionTree("-u", node_19_1_1_1, nullptr, 2, 7);
    ExpressionTree* node_19_1 = new ExpressionTree("-u", node_19_1_1, nullptr, 1, 7);
    ExpressionTree* node_19 = new ExpressionTree("-u", node_19_1, nullptr, 0, 7);

    double result_19 = -6;

    QTest::newRow("19. Multiple unary minuses") << node_19 << result_19 << emptyErrors << testTokens;

    // Тест 20
    testTokens.clear();
    testTokens.append(Token("2", 0, 0));
    testTokens.append(Token("/", 1, 1));
    testTokens.append(Token("0", 2, 2));
    testTokens.append(Token("+", 3, 3));
    testTokens.append(Token("4", 4, 4));

    ExpressionTree* node_20_1_1 = new ExpressionTree("2", nullptr, nullptr, 0, 0);
    ExpressionTree* node_20_1_2 = new ExpressionTree("0", nullptr, nullptr, 2, 2);
    ExpressionTree* node_20_1 = new ExpressionTree("/", node_20_1_1, node_20_1_2, 0, 2);
    ExpressionTree* node_20_2 = new ExpressionTree("4", nullptr, nullptr, 4, 4);
    ExpressionTree* node_20 = new ExpressionTree("+", node_20_1, node_20_2, 0, 4);

    double result_20 = 0;

    Error error_20;
    error_20.setType(Error::Type::divisionByZero);
    error_20.setExpression("2/0");

    QSet<Error> errors_20 = { error_20 };

    QTest::newRow("20. Division by zero (simple case)") << node_20 << result_20 << errors_20 << testTokens;

    // Тест 21
    testTokens.clear();
    testTokens.append(Token("(", 0, 0));
    testTokens.append(Token("1", 1, 1));
    testTokens.append(Token("+", 2, 2));
    testTokens.append(Token("2", 3, 3));
    testTokens.append(Token(")", 4, 4));
    testTokens.append(Token("/", 5, 5));
    testTokens.append(Token("0", 6, 6));
    testTokens.append(Token("+", 7, 7));
    testTokens.append(Token("4", 8, 8));

    ExpressionTree* node_21_1_1 = new ExpressionTree("1", nullptr, nullptr, 1, 1);
    ExpressionTree* node_21_1_2 = new ExpressionTree("2", nullptr, nullptr, 3, 3);
    ExpressionTree* node_21_1 = new ExpressionTree("+", node_21_1_1, node_21_1_2, 0, 4);
    ExpressionTree* node_21_2 = new ExpressionTree("0", nullptr, nullptr, 6, 6);
    ExpressionTree* node_21_inner = new ExpressionTree("/", node_21_1, node_21_2, 0, 6);
    ExpressionTree* node_21_3 = new ExpressionTree("4", nullptr, nullptr, 9, 9);
    ExpressionTree* node_21 = new ExpressionTree("+", node_21_inner, node_21_3, 0, 9);

    Error error_21;
    error_21.setType(Error::Type::divisionByZero);
    error_21.setExpression("(1+2)/0");

    QSet<Error> errors_21 = { error_21 };
    QTest::newRow("21. Division by zero in parenthesized expression") << node_21 << 0.0 << errors_21 << testTokens;

    // Тест 22
    testTokens.clear();
    testTokens.append(Token("3", 0, 0));
    testTokens.append(Token("/", 1, 1));
    testTokens.append(Token("(", 2, 2));
    testTokens.append(Token("1", 3, 3));
    testTokens.append(Token("-", 4, 4));
    testTokens.append(Token("1", 5, 5));
    testTokens.append(Token(")", 6, 6));
    testTokens.append(Token("+", 7, 7));
    testTokens.append(Token("4", 8, 8));

    ExpressionTree* node_22_1_1 = new ExpressionTree("1", nullptr, nullptr, 2, 2);
    ExpressionTree* node_22_1_2 = new ExpressionTree("1", nullptr, nullptr, 4, 4);
    ExpressionTree* node_22_1 = new ExpressionTree("-", node_22_1_1, node_22_1_2, 1, 5);
    ExpressionTree* node_22_2 = new ExpressionTree("3", nullptr, nullptr, 0, 0);
    ExpressionTree* node_22_inner = new ExpressionTree("/", node_22_2, node_22_1, 0, 6);
    ExpressionTree* node_22_3 = new ExpressionTree("4", nullptr, nullptr, 8, 8);
    ExpressionTree* node_22 = new ExpressionTree("+", node_22_inner, node_22_3, 0, 8);

    Error error_22;
    error_22.setType(Error::Type::divisionByZero);
    error_22.setExpression("3/(1-1)");

    QSet<Error> errors_22 = { error_22 };
    QTest::newRow("22. Division by zero with subtraction in denominator") << node_22 << 0.0 << errors_22 << testTokens;

    // Тест 23
    testTokens.clear();
    testTokens.append(Token("(", 0, 0));
    testTokens.append(Token("2", 1, 1));
    testTokens.append(Token("+", 2, 2));
    testTokens.append(Token("3", 3, 3));
    testTokens.append(Token(")", 4, 4));
    testTokens.append(Token("/", 5, 5));
    testTokens.append(Token("(", 6, 6));
    testTokens.append(Token("4", 7, 7));
    testTokens.append(Token("-", 8, 8));
    testTokens.append(Token("5", 9, 9));
    testTokens.append(Token("+", 10, 10));
    testTokens.append(Token("1", 11, 11));
    testTokens.append(Token(")", 12, 12));
    testTokens.append(Token("+", 13, 13));
    testTokens.append(Token("6", 14, 14));

    ExpressionTree* node_23_1_1 = new ExpressionTree("2", nullptr, nullptr, 1, 1);
    ExpressionTree* node_23_1_2 = new ExpressionTree("3", nullptr, nullptr, 3, 3);
    ExpressionTree* node_23_1 = new ExpressionTree("+", node_23_1_1, node_23_1_2, 0, 4);
    ExpressionTree* node_23_2_1 = new ExpressionTree("4", nullptr, nullptr, 7, 7);
    ExpressionTree* node_23_2_2 = new ExpressionTree("5", nullptr, nullptr, 9, 9);
    ExpressionTree* node_23_2_3 = new ExpressionTree("1", nullptr, nullptr, 11, 11);
    ExpressionTree* node_23_2_inner = new ExpressionTree("-", node_23_2_1, node_23_2_2, 6, 9);
    ExpressionTree* node_23_2 = new ExpressionTree("+", node_23_2_inner, node_23_2_3, 6, 12);
    ExpressionTree* node_23_inner = new ExpressionTree("/", node_23_1, node_23_2, 0, 12);
    ExpressionTree* node_23_3 = new ExpressionTree("6", nullptr, nullptr, 14, 14);
    ExpressionTree* node_23 = new ExpressionTree("+", node_23_inner, node_23_3, 0, 14);

    Error error_23;
    error_23.setType(Error::Type::divisionByZero);
    error_23.setExpression("(2+3)/(4-5+1)");

    QSet<Error> errors_23 = { error_23 };
    QTest::newRow("23. Division by zero with complex denominator") << node_23 << 0.0 << errors_23 << testTokens;

    // Тест 24
    testTokens.clear();
    testTokens.append(Token("(", 0, 0));
    testTokens.append(Token("1", 1, 1));
    testTokens.append(Token("+", 2, 2));
    testTokens.append(Token("2", 3, 3));
    testTokens.append(Token(")", 4, 4));
    testTokens.append(Token("/", 5, 5));
    testTokens.append(Token("0", 6, 6));
    testTokens.append(Token("+", 7, 7));
    testTokens.append(Token("1", 8, 8));
    testTokens.append(Token("/", 9, 9));
    testTokens.append(Token("(", 10, 10));
    testTokens.append(Token("2", 11, 11));
    testTokens.append(Token("-", 12, 12));
    testTokens.append(Token("2", 13, 13));
    testTokens.append(Token(")", 14, 14));

    ExpressionTree* node_24_1_1 = new ExpressionTree("1", nullptr, nullptr, 1, 1);
    ExpressionTree* node_24_1_2 = new ExpressionTree("2", nullptr, nullptr, 3, 3);
    ExpressionTree* node_24_1 = new ExpressionTree("+", node_24_1_1, node_24_1_2, 0, 4);
    ExpressionTree* node_24_2 = new ExpressionTree("0", nullptr, nullptr, 6, 6);
    ExpressionTree* node_24_left = new ExpressionTree("/", node_24_1, node_24_2, 0, 6);
    ExpressionTree* node_24_3 = new ExpressionTree("1", nullptr, nullptr, 8, 8);
    ExpressionTree* node_24_4_1 = new ExpressionTree("2", nullptr, nullptr, 11, 11);
    ExpressionTree* node_24_4_2 = new ExpressionTree("2", nullptr, nullptr, 13, 13);
    ExpressionTree* node_24_4 = new ExpressionTree("-", node_24_4_1, node_24_4_2, 10, 14);
    ExpressionTree* node_24_right = new ExpressionTree("/", node_24_3, node_24_4, 8, 15);
    ExpressionTree* node_24 = new ExpressionTree("+", node_24_left, node_24_right, 0, 15);

    Error error_24_1;
    error_24_1.setType(Error::Type::divisionByZero);
    error_24_1.setExpression("(1+2)/0");

    QSet<Error> errors_24 = { error_24_1 };
    QTest::newRow("24. Multiple division by zero cases") << node_24 << 0.0 << errors_24 << testTokens;

    // Тест 25
    testTokens.clear();
    testTokens.append(Token("(", 0, 0));
    testTokens.append(Token("-u", 1, 1));
    testTokens.append(Token("2", 2, 2));
    testTokens.append(Token(")", 3, 3));
    testTokens.append(Token("^", 4, 4));
    testTokens.append(Token("0.5", 5, 7));
    testTokens.append(Token("+", 8, 8));
    testTokens.append(Token("3", 9, 9));

    ExpressionTree* node_25_1_1 = new ExpressionTree("2", nullptr, nullptr, 2, 2);
    ExpressionTree* node_25_1 = new ExpressionTree("-u", node_25_1_1, nullptr, 0, 3);
    ExpressionTree* node_25_2 = new ExpressionTree("0.5", nullptr, nullptr, 5, 7);
    ExpressionTree* node_25_inner = new ExpressionTree("^", node_25_1, node_25_2, 0, 7);
    ExpressionTree* node_25_3 = new ExpressionTree("3", nullptr, nullptr, 9, 9);
    ExpressionTree* node_25 = new ExpressionTree("+", node_25_inner, node_25_3, 0, 9);

    Error error_25;
    error_25.setType(Error::Type::incorrectRoot);
    error_25.setExpression("(-2)^0.5");

    QSet<Error> errors_25 = { error_25 };
    QTest::newRow("25. Root of negative number (decimal exponent)") << node_25 << 0.0 << errors_25 << testTokens;

    // Тест 26
    testTokens.clear();
    testTokens.append(Token("(", 0, 0));
    testTokens.append(Token("-u", 1, 1));
    testTokens.append(Token("2", 2, 2));
    testTokens.append(Token(")", 3, 3));
    testTokens.append(Token("^", 4, 4));
    testTokens.append(Token("(", 5, 5));
    testTokens.append(Token("1", 6, 6));
    testTokens.append(Token("/", 7, 7));
    testTokens.append(Token("2", 8, 8));
    testTokens.append(Token(")", 9, 9));
    testTokens.append(Token("+", 10, 10));
    testTokens.append(Token("3", 11, 11));

    ExpressionTree* node_26_1_1 = new ExpressionTree("2", nullptr, nullptr, 2, 2);
    ExpressionTree* node_26_1 = new ExpressionTree("-u", node_26_1_1, nullptr, 0, 3);
    ExpressionTree* node_26_2_1 = new ExpressionTree("1", nullptr, nullptr, 5, 5);
    ExpressionTree* node_26_2_2 = new ExpressionTree("2", nullptr, nullptr, 7, 7);
    ExpressionTree* node_26_2 = new ExpressionTree("/", node_26_2_1, node_26_2_2, 4, 8);
    ExpressionTree* node_26_inner = new ExpressionTree("^", node_26_1, node_26_2, 0, 8);
    ExpressionTree* node_26_3 = new ExpressionTree("3", nullptr, nullptr, 10, 10);
    ExpressionTree* node_26 = new ExpressionTree("+", node_26_inner, node_26_3, 0, 10);

    Error error_26;
    error_26.setType(Error::Type::incorrectRoot);
    error_26.setExpression("(-2)^(1/2)");

    QSet<Error> errors_26 = { error_26 };
    QTest::newRow("26. Root of negative number (fractional exponent)") << node_26 << 0.0 << errors_26 << testTokens;

    // Тест 27
    testTokens.clear();
    testTokens.append(Token("(", 0, 0));
    testTokens.append(Token("-u", 1, 1));
    testTokens.append(Token("3", 2, 2));
    testTokens.append(Token("+", 3, 3));
    testTokens.append(Token("1", 4, 4));
    testTokens.append(Token(")", 5, 5));
    testTokens.append(Token("^", 6, 6));
    testTokens.append(Token("(", 7, 7));
    testTokens.append(Token("1", 8, 8));
    testTokens.append(Token("/", 9, 9));
    testTokens.append(Token("2", 10, 10));
    testTokens.append(Token(")", 11, 11));
    testTokens.append(Token("+", 12, 12));
    testTokens.append(Token("3", 13, 13));

    ExpressionTree* node_27_1_1 = new ExpressionTree("3", nullptr, nullptr, 2, 2);
    ExpressionTree* node_27_1 = new ExpressionTree("-u", node_27_1_1, nullptr, 1, 2);
    ExpressionTree* node_27_2 = new ExpressionTree("1", nullptr, nullptr, 4, 4);
    ExpressionTree* node_27_left = new ExpressionTree("+", node_27_1, node_27_2, 0, 5);
    ExpressionTree* node_27_3_1 = new ExpressionTree("1", nullptr, nullptr, 8, 8);
    ExpressionTree* node_27_3_2 = new ExpressionTree("2", nullptr, nullptr, 10, 10);
    ExpressionTree* node_27_3 = new ExpressionTree("/", node_27_3_1, node_27_3_2, 7, 11);
    ExpressionTree* node_27_inner = new ExpressionTree("^", node_27_left, node_27_3, 0, 11);
    ExpressionTree* node_27_4 = new ExpressionTree("3", nullptr, nullptr, 13, 13);
    ExpressionTree* node_27 = new ExpressionTree("+", node_27_inner, node_27_4, 0, 13);

    Error error_27;
    error_27.setType(Error::Type::incorrectRoot);
    error_27.setExpression("(-3+1)^(1/2)");

    QSet<Error> errors_27 = { error_27 };
    QTest::newRow("27. Root of negative expression") << node_27 << 0.0 << errors_27 << testTokens;

    // Тест 28
    testTokens.clear();
    testTokens.append(Token("(", 0, 0));
    testTokens.append(Token("-u", 1, 1));
    testTokens.append(Token("2", 2, 2));
    testTokens.append(Token(")", 3, 3));
    testTokens.append(Token("^", 4, 4));
    testTokens.append(Token("0.5", 5, 7));
    testTokens.append(Token("+", 8, 8));
    testTokens.append(Token("(", 9, 9));
    testTokens.append(Token("-u", 10, 10));
    testTokens.append(Token("3", 11, 11));
    testTokens.append(Token(")", 12, 12));
    testTokens.append(Token("^", 13, 13));
    testTokens.append(Token("0.5", 14, 16));

    ExpressionTree* node_28_1_1 = new ExpressionTree("2", nullptr, nullptr, 2, 2);
    ExpressionTree* node_28_1 = new ExpressionTree("-u", node_28_1_1, nullptr, 0, 3);
    ExpressionTree* node_28_2 = new ExpressionTree("0.5", nullptr, nullptr, 5, 7);
    ExpressionTree* node_28_left = new ExpressionTree("^", node_28_1, node_28_2, 0, 7);
    ExpressionTree* node_28_3_1 = new ExpressionTree("3", nullptr, nullptr, 11, 11);
    ExpressionTree* node_28_3 = new ExpressionTree("-u", node_28_3_1, nullptr, 9, 12);
    ExpressionTree* node_28_4 = new ExpressionTree("0.5", nullptr, nullptr, 14, 16);
    ExpressionTree* node_28_right = new ExpressionTree("^", node_28_3, node_28_4, 9, 16);
    ExpressionTree* node_28 = new ExpressionTree("+", node_28_left, node_28_right, 0, 16);

    Error error_28_1;
    error_28_1.setType(Error::Type::incorrectRoot);
    error_28_1.setExpression("(-2)^0.5");

    QSet<Error> errors_28 = { error_28_1 };
    QTest::newRow("28. Multiple roots of negative numbers") << node_28 << 0.0 << errors_28 << testTokens;
}

QString Test_Calculate::compareErrors(const QSet<Error>& actual, const QSet<Error>& expected) {
    QSet<Error> missing = expected - actual;
    QSet<Error> unexpected = actual - expected;

    QString result;

    if (!missing.empty() || !unexpected.empty()) {
        if (!missing.empty()) {
            result += "Missing errors:\n";
            for (const auto& error : missing) {
                result += error.message() + "\n";
            }
        }

        if (!unexpected.empty()) {
            if (!result.isEmpty()) result += "\n";
            result += "Unexpected errors:\n";
            for (const auto& error : unexpected) {
                result += error.message() + "\n";
            }
        }
    }

    return result;
}
