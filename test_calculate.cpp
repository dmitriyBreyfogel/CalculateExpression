#include "test_calculate.h"

void Test_Calculate::testCalculate() {
    // Получаем тестовые данные
    QFETCH(ExpressionTree*, root);
    QFETCH(double, result);
    QFETCH(QSet<Error>, errors);

    // Вычисляем результат
    QSet<Error> actualErrors;
    double actualResult = root->calculate(actualErrors);

    // Сравниваем ошибки
    QString errorComparison = compareErrors(actualErrors, errors);
    QVERIFY2(errorComparison.isEmpty(), qPrintable(errorComparison));

    // Сравниваем результаты (только если нет ошибок)
    if (errors.isEmpty()) {
        const double epsilon = 1e-8; // Точность для сравнения вещественных чисел
        bool resultMatch = qAbs(actualResult - result) < epsilon;

        QVERIFY2(resultMatch, qPrintable(
                                  QString("Result mismatch. Expected: %1, Actual: %2")
                                      .arg(result)
                                      .arg(actualResult)
                                  ));
    }

    // Очищаем память
    delete root;
}

void Test_Calculate::testCalculate_data() {
    QTest::addColumn<ExpressionTree*>("root");
    QTest::addColumn<double>("result");
    QTest::addColumn<QSet<Error>>("errors");

    QSet<Error> emptyErrors = {};

    // Тест 1
    ExpressionTree* node_1 = new ExpressionTree("1", nullptr, nullptr, 0, 0);
    double result_1 = 1;

    QTest::newRow("1. Single number (integer)") << node_1 << result_1 << emptyErrors;

    // Тест 2
    ExpressionTree* node_2 = new ExpressionTree("1.234", nullptr, nullptr, 0, 4);
    double result_2 = 1.234;

    QTest::newRow("2. Single number (float)") << node_2 << result_2 << emptyErrors;

    // Тест 3
    ExpressionTree* node_3_1 = new ExpressionTree("1", nullptr, nullptr, 0, 0);
    ExpressionTree* node_3_2 = new ExpressionTree("4", nullptr, nullptr, 2, 2);
    ExpressionTree* node_3 = new ExpressionTree("+", node_3_1, node_3_2, 0, 2);

    double result_3 = 1 + 4;

    QTest::newRow("3. Simple addition (integers)") << node_3 << result_3 << emptyErrors;

    // Тест 4
    ExpressionTree* node_4_1 = new ExpressionTree("1.234", nullptr, nullptr, 0, 4);
    ExpressionTree* node_4_2 = new ExpressionTree("5.678", nullptr, nullptr, 6, 10);
    ExpressionTree* node_4 = new ExpressionTree("+", node_4_1, node_4_2, 0, 10);

    double result_4 = 1.234 + 5.678;

    QTest::newRow("4. Simple addition (floats)") << node_4 << result_4 << emptyErrors;

    // Тест 5
    ExpressionTree* node_5_1 = new ExpressionTree("1", nullptr, nullptr, 0, 0);
    ExpressionTree* node_5_2 = new ExpressionTree("4", nullptr, nullptr, 2, 2);
    ExpressionTree* node_5 = new ExpressionTree("-", node_5_1, node_5_2, 0, 2);

    double result_5 = 1 - 4;

    QTest::newRow("5. Simple subtraction (integers)") << node_5 << result_5 << emptyErrors;

    // Тест 6
    ExpressionTree* node_6_1 = new ExpressionTree("1.234", nullptr, nullptr, 0, 4);
    ExpressionTree* node_6_2 = new ExpressionTree("5.678", nullptr, nullptr, 6, 10);
    ExpressionTree* node_6 = new ExpressionTree("-", node_6_1, node_6_2, 0, 10);

    double result_6 = 1.234 - 5.678;

    QTest::newRow("6. Simple subtraction (floats)") << node_6 << result_6 << emptyErrors;

    // Тест 7
    ExpressionTree* node_7_1 = new ExpressionTree("1", nullptr, nullptr, 0, 0);
    ExpressionTree* node_7_2 = new ExpressionTree("4", nullptr, nullptr, 2, 2);
    ExpressionTree* node_7 = new ExpressionTree("/", node_7_1, node_7_2, 0, 2);

    double result_7 = 1.0 / 4.0;

    QTest::newRow("7. Simple division (integers)") << node_7 << result_7 << emptyErrors;

    // Тест 8
    ExpressionTree* node_8_1 = new ExpressionTree("1.234", nullptr, nullptr, 0, 4);
    ExpressionTree* node_8_2 = new ExpressionTree("5.678", nullptr, nullptr, 6, 10);
    ExpressionTree* node_8 = new ExpressionTree("/", node_8_1, node_8_2, 0, 10);

    double result_8 = 1.234 / 5.678;

    QTest::newRow("8. Simple division (floats)") << node_8 << result_8 << emptyErrors;

    // Тест 9
    ExpressionTree* node_9_1 = new ExpressionTree("2", nullptr, nullptr, 0, 0);
    ExpressionTree* node_9_2 = new ExpressionTree("4", nullptr, nullptr, 2, 2);
    ExpressionTree* node_9 = new ExpressionTree("*", node_9_1, node_9_2, 0, 2);

    double result_9 = 2 * 4;

    QTest::newRow("9. Simple multiplication (integers)") << node_9 << result_9 << emptyErrors;

    // Тест 10
    ExpressionTree* node_10_1 = new ExpressionTree("1.234", nullptr, nullptr, 0, 4);
    ExpressionTree* node_10_2 = new ExpressionTree("5.678", nullptr, nullptr, 6, 10);
    ExpressionTree* node_10 = new ExpressionTree("*", node_10_1, node_10_2, 0, 10);

    double result_10 = 1.234 * 5.678;

    QTest::newRow("10. Simple multiplication (floats)") << node_10 << result_10 << emptyErrors;

    // Тест 11
    ExpressionTree* node_11_1 = new ExpressionTree("2", nullptr, nullptr, 0, 0);
    ExpressionTree* node_11_2 = new ExpressionTree("4", nullptr, nullptr, 2, 2);
    ExpressionTree* node_11 = new ExpressionTree("^", node_11_1, node_11_2, 0, 2);

    double result_11 = 16;

    QTest::newRow("11. Simple exponentiation (integer base and exponent)") << node_11 << result_11 << emptyErrors;

    // Тест 12
    ExpressionTree* node_12_1 = new ExpressionTree("1.234", nullptr, nullptr, 0, 4);
    ExpressionTree* node_12_2 = new ExpressionTree("5.678", nullptr, nullptr, 6, 10);
    ExpressionTree* node_12 = new ExpressionTree("^", node_12_1, node_12_2, 0, 10);

    double result_12 = 3.29979893;

    QTest::newRow("12. Simple exponentiation (float base and exponent)") << node_12 << result_12 << emptyErrors;

    // Тест 13
    ExpressionTree* node_13_1 = new ExpressionTree("4", nullptr, nullptr, 0, 0);
    ExpressionTree* node_13_2 = new ExpressionTree("0.5", nullptr, nullptr, 2, 4);
    ExpressionTree* node_13 = new ExpressionTree("^", node_13_1, node_13_2, 0, 4);

    double result_13 = 2;

    QTest::newRow("13. Square root (exponent 0.5)") << node_13 << result_13 << emptyErrors;

    // Тест 14
    ExpressionTree* node_14_1_1 = new ExpressionTree("2", nullptr, nullptr, 1, 1);
    ExpressionTree* node_14_1 = new ExpressionTree("-u", node_14_1_1, nullptr, 0, 1);
    ExpressionTree* node_14_2_1 = new ExpressionTree("1", nullptr, nullptr, 4, 4);
    ExpressionTree* node_14_2_2 = new ExpressionTree("3", nullptr, nullptr, 6, 6);
    ExpressionTree* node_14_2 = new ExpressionTree("/", node_14_2_1, node_14_2_2, 3, 7);
    ExpressionTree* node_14 = new ExpressionTree("^", node_14_1, node_14_2, 0, 7);

    double result_14 = -1.25992104989;

    QTest::newRow("14. Complex expression with unary minus and exponentiation") << node_14 << result_14 << emptyErrors;

    // Тест 15
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

    QTest::newRow("15. Complex nested operations") << node_15 << result_15 << emptyErrors;

    // Тест 16
    ExpressionTree* node_16_1_1 = new ExpressionTree("1", nullptr, nullptr, 1, 1);
    ExpressionTree* node_16_1 = new ExpressionTree("-u", node_16_1_1, nullptr, 0, 1);
    ExpressionTree* node_16_2_2_1 = new ExpressionTree("3", nullptr, nullptr, 5, 5);
    ExpressionTree* node_16_2_2_2 = new ExpressionTree("5", nullptr, nullptr, 7, 7);
    ExpressionTree* node_16_2_2 = new ExpressionTree("^", node_16_2_2_1, node_16_2_2_2, 5, 7);
    ExpressionTree* node_16_2_1 = new ExpressionTree("2", nullptr, nullptr, 3, 3);
    ExpressionTree* node_16_2 = new ExpressionTree("*", node_16_2_1, node_16_2_2, 3, 7);
    ExpressionTree* node_16 = new ExpressionTree("+", node_16_1, node_16_2, 0, 7);

    double result_16 = 485;

    QTest::newRow("16. Unary minus with exponentiation") << node_16 << result_16 << emptyErrors;

    // Тест 17
    ExpressionTree* node_17_1_1_1 = new ExpressionTree("1", nullptr, nullptr, 2, 2);
    ExpressionTree* node_17_1_1_2 = new ExpressionTree("5", nullptr, nullptr, 4, 4);
    ExpressionTree* node_17_1_1 = new ExpressionTree("+", node_17_1_1_1, node_17_1_1_2, 1, 5);
    ExpressionTree* node_17_1_2 = new ExpressionTree("2", nullptr, nullptr, 7, 7);
    ExpressionTree* node_17_1 = new ExpressionTree("^", node_17_1_1, node_17_1_2, 1, 7);
    ExpressionTree* node_17 = new ExpressionTree("-u", node_17_1, nullptr, 0, 7);

    double result_17 = -36;

    QTest::newRow("17. Nested unary minus with exponentiation") << node_17 << result_17 << emptyErrors;

    // Тест 18
    ExpressionTree* node_18_1_1_1 = new ExpressionTree("1", nullptr, nullptr, 3, 3);
    ExpressionTree* node_18_1_1_2 = new ExpressionTree("5", nullptr, nullptr, 5, 5);
    ExpressionTree* node_18_1_1 = new ExpressionTree("+", node_18_1_1_1, node_18_1_1_2, 2, 6);
    ExpressionTree* node_18_1 = new ExpressionTree("-u", node_18_1_1, nullptr, 1, 6);
    ExpressionTree* node_18_2 = new ExpressionTree("2", nullptr, nullptr, 9, 9);
    ExpressionTree* node_18 = new ExpressionTree("^", node_18_1, node_18_2, 0, 9);

    double result_18 = 36;

    QTest::newRow("18. Unary minus in base of exponentiation") << node_18 << result_18 << emptyErrors;

    // Тест 19
    ExpressionTree* node_19_1_1_1_1 = new ExpressionTree("1", nullptr, nullptr, 4, 4);
    ExpressionTree* node_19_1_1_1_2 = new ExpressionTree("5", nullptr, nullptr, 6, 6);
    ExpressionTree* node_19_1_1_1 = new ExpressionTree("+", node_19_1_1_1_1, node_19_1_1_1_2, 3, 7);
    ExpressionTree* node_19_1_1 = new ExpressionTree("-u", node_19_1_1_1, nullptr, 2, 7);
    ExpressionTree* node_19_1 = new ExpressionTree("-u", node_19_1_1, nullptr, 1, 7);
    ExpressionTree* node_19 = new ExpressionTree("-u", node_19_1, nullptr, 0, 7);

    double result_19 = 36;

    QTest::newRow("19. Multiple unary minuses") << node_19 << result_19 << emptyErrors;

    // Тест 20
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

    QTest::newRow("20. Division by zero (simple case)") << node_20 << result_20 << errors_20;

    // Тест 21
    ExpressionTree* node_21_1_1 = new ExpressionTree("1", nullptr, nullptr, 1, 1);
    ExpressionTree* node_21_1_2 = new ExpressionTree("2", nullptr, nullptr, 3, 3);
    ExpressionTree* node_21_1 = new ExpressionTree("+", node_21_1_1, node_21_1_2, 0, 4);
    ExpressionTree* node_21_2 = new ExpressionTree("0", nullptr, nullptr, 6, 6);
    ExpressionTree* node_21_inner = new ExpressionTree("/", node_21_1, node_21_2, 0, 7);
    ExpressionTree* node_21_3 = new ExpressionTree("4", nullptr, nullptr, 9, 9);
    ExpressionTree* node_21 = new ExpressionTree("+", node_21_inner, node_21_3, 0, 9);

    Error error_21;
    error_21.setType(Error::Type::divisionByZero);
    error_21.setExpression("(1+2)/0");

    QSet<Error> errors_21 = { error_21 };
    QTest::newRow("21. Division by zero in parenthesized expression") << node_21 << 0.0 << errors_21;

    // Тест 22
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
    QTest::newRow("22. Division by zero with subtraction in denominator") << node_22 << 0.0 << errors_22;

    // Тест 23
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
    QTest::newRow("23. Division by zero with complex denominator") << node_23 << 0.0 << errors_23;

    // Тест 24
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
    QTest::newRow("24. Multiple division by zero cases") << node_24 << 0.0 << errors_24;

    // Тест 25
    ExpressionTree* node_25_1_1 = new ExpressionTree("2", nullptr, nullptr, 2, 2);
    ExpressionTree* node_25_1 = new ExpressionTree("-u", node_25_1_1, nullptr, 0, 3);
    ExpressionTree* node_25_2 = new ExpressionTree("0.5", nullptr, nullptr, 5, 7);
    ExpressionTree* node_25_inner = new ExpressionTree("^", node_25_1, node_25_2, 0, 7);
    ExpressionTree* node_25_3 = new ExpressionTree("3", nullptr, nullptr, 9, 9);
    ExpressionTree* node_25 = new ExpressionTree("+", node_25_inner, node_25_3, 0, 9);

    Error error_25;
    error_25.setType(Error::Type::incorrectRoot);
    error_25.setExpression("(-2)^0.5");

    QSet<Error> errors_5 = { error_25 };
    QTest::newRow("25. Root of negative number (decimal exponent)") << node_25 << 0.0 << errors_5;

    // Тест 26
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
    QTest::newRow("26. Root of negative number (fractional exponent)") << node_26 << 0.0 << errors_26;

    // Тест 27
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
    QTest::newRow("27. Root of negative expression") << node_27 << 0.0 << errors_27;

    // Тест 28
    ExpressionTree* node_28_1_1 = new ExpressionTree("2", nullptr, nullptr, 2, 2);
    ExpressionTree* node_28_1 = new ExpressionTree("-u", node_28_1_1, nullptr, 0, 3);
    ExpressionTree* node_28_2 = new ExpressionTree("0.5", nullptr, nullptr, 5, 7);
    ExpressionTree* node_28_left = new ExpressionTree("^", node_28_1, node_28_2, 0, 7);
    ExpressionTree* node_28_3_1 = new ExpressionTree("3", nullptr, nullptr, 13, 13);
    ExpressionTree* node_28_3 = new ExpressionTree("-u", node_28_3_1, nullptr, 11, 14);
    ExpressionTree* node_28_4 = new ExpressionTree("0.5", nullptr, nullptr, 16, 18);
    ExpressionTree* node_28_right = new ExpressionTree("^", node_28_3, node_28_4, 11, 18);
    ExpressionTree* node_28 = new ExpressionTree("+", node_28_left, node_28_right, 0, 18);

    Error error_28_1;
    error_28_1.setType(Error::Type::incorrectRoot);
    error_28_1.setExpression("(-2)^0.5");

    QSet<Error> errors_28 = { error_28_1 };
    QTest::newRow("28. Multiple roots of negative numbers") << node_28 << 0.0 << errors_28;
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
