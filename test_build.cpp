#include "test_build.h"

void Test_Build::testBuild() {
    // Получаем данные из тестовой строки
    QFETCH(QList<Token>, tokens);
    QFETCH(ExpressionTree*, root);
    QFETCH(QSet<Error>, errors);

    // Вызываем тестируемую функцию
    QSet<Error> actualErrors;
    ExpressionTree* actualTree = ExpressionTree::build(tokens, actualErrors);

    // Сравниваем ошибки
    QString errorDifferences = compareErrors(actualErrors, errors);
    QVERIFY2(errorDifferences.isEmpty(), qPrintable(errorDifferences));

    QStringList tracePath;
    QString treeCompareError;
    bool treesMatch = compareTrees(root, actualTree, tracePath, treeCompareError);
    QVERIFY2(treesMatch, qPrintable(treeCompareError));

    // Очищаем память
    delete root;
    delete actualTree;
}

void Test_Build::testBuild_data() {
    QTest::addColumn<QList<Token>>("tokens");
    QTest::addColumn<ExpressionTree*>("root");
    QTest::addColumn<QSet<Error>>("errors");

    QSet<Error> emptyErrors = {};

    // Тест 1
    QList<Token> tokens_1 = {};
    tokens_1.append(Token("1", 0, 0));

    ExpressionTree* node_1 = new ExpressionTree("1", nullptr, nullptr, 0, 0);

    QTest::newRow("01. Single operand (integer)") << tokens_1 << node_1 << emptyErrors;

    // Тест 2
    QList<Token> tokens_2 = {};
    tokens_2.append(Token("1.234", 0, 4));

    ExpressionTree* node_2 = new ExpressionTree("1.234", nullptr, nullptr, 0, 4);

    QTest::newRow("02. Single operand (real number)") << tokens_2 << node_2 << emptyErrors;

    // Тест 3
    QList<Token> tokens_3 = {};
    tokens_3.append(Token("1", 0, 0));
    tokens_3.append(Token("+", 1, 1));
    tokens_3.append(Token("4", 2, 2));

    ExpressionTree* node_3_1 = new ExpressionTree("1", nullptr, nullptr, 0, 0);
    ExpressionTree* node_3_2 = new ExpressionTree("4", nullptr, nullptr, 2, 2);
    ExpressionTree* node_3 = new ExpressionTree("+", node_3_1, node_3_2, 0, 2);

    QTest::newRow("03. Addition of two integers") << tokens_3 << node_3 << emptyErrors;

    // Тест 4
    QList<Token> tokens_4 = {};
    tokens_4.append(Token("1.234", 0, 4));
    tokens_4.append(Token("+", 5, 5));
    tokens_4.append(Token("5.678", 6, 10));

    ExpressionTree* node_4_1 = new ExpressionTree("1.234", nullptr, nullptr, 0, 4);
    ExpressionTree* node_4_2 = new ExpressionTree("5.678", nullptr, nullptr, 6, 10);
    ExpressionTree* node_4 = new ExpressionTree("+", node_4_1, node_4_2, 0, 10);

    QTest::newRow("04. Addition of two real numbers") << tokens_4 << node_4 << emptyErrors;

    // Тест 5
    QList<Token> tokens_5 = {};
    tokens_5.append(Token("1", 0, 0));
    tokens_5.append(Token("-", 1, 1));
    tokens_5.append(Token("4", 2, 2));

    ExpressionTree* node_5_1 = new ExpressionTree("1", nullptr, nullptr, 0, 0);
    ExpressionTree* node_5_2 = new ExpressionTree("4", nullptr, nullptr, 2, 2);
    ExpressionTree* node_5 = new ExpressionTree("-", node_5_1, node_5_2, 0, 2);

    QTest::newRow("05. Subtraction of two integers") << tokens_5 << node_5 << emptyErrors;

    // Тест 6
    QList<Token> tokens_6 = {};
    tokens_6.append(Token("1.234", 0, 4));
    tokens_6.append(Token("-", 5, 5));
    tokens_6.append(Token("5.678", 6, 10));

    ExpressionTree* node_6_1 = new ExpressionTree("1.234", nullptr, nullptr, 0, 4);
    ExpressionTree* node_6_2 = new ExpressionTree("5.678", nullptr, nullptr, 6, 10);
    ExpressionTree* node_6 = new ExpressionTree("-", node_6_1, node_6_2, 0, 10);

    QTest::newRow("06. Subtraction of two real numbers") << tokens_6 << node_6 << emptyErrors;

    // Тест 7
    QList<Token> tokens_7 = {};
    tokens_7.append(Token("1", 0, 0));
    tokens_7.append(Token("/", 1, 1));
    tokens_7.append(Token("4", 2, 2));

    ExpressionTree* node_7_1 = new ExpressionTree("1", nullptr, nullptr, 0, 0);
    ExpressionTree* node_7_2 = new ExpressionTree("4", nullptr, nullptr, 2, 2);
    ExpressionTree* node_7 = new ExpressionTree("/", node_7_1, node_7_2, 0, 2);

    QTest::newRow("07. Division of two integers") << tokens_7 << node_7 << emptyErrors;

    // Тест 8
    QList<Token> tokens_8 = {};
    tokens_8.append(Token("1.234", 0, 4));
    tokens_8.append(Token("/", 5, 5));
    tokens_8.append(Token("5.678", 6, 10));

    ExpressionTree* node_8_1 = new ExpressionTree("1.234", nullptr, nullptr, 0, 4);
    ExpressionTree* node_8_2 = new ExpressionTree("5.678", nullptr, nullptr, 6, 10);
    ExpressionTree* node_8 = new ExpressionTree("/", node_8_1, node_8_2, 0, 10);

    QTest::newRow("08. Division of two real numbers") << tokens_8 << node_8 << emptyErrors;

    // Тест 9
    QList<Token> tokens_9 = {};
    tokens_9.append(Token("2", 0, 0));
    tokens_9.append(Token("*", 1, 1));
    tokens_9.append(Token("4", 2, 2));

    ExpressionTree* node_9_1 = new ExpressionTree("2", nullptr, nullptr, 0, 0);
    ExpressionTree* node_9_2 = new ExpressionTree("4", nullptr, nullptr, 2, 2);
    ExpressionTree* node_9 = new ExpressionTree("*", node_9_1, node_9_2, 0, 2);

    QTest::newRow("09. Multiplication of two integers") << tokens_9 << node_9 << emptyErrors;

    // Тест 10
    QList<Token> tokens_10 = {};
    tokens_10.append(Token("1.234", 0, 4));
    tokens_10.append(Token("*", 5, 5));
    tokens_10.append(Token("5.678", 6, 10));

    ExpressionTree* node_10_1 = new ExpressionTree("1.234", nullptr, nullptr, 0, 4);
    ExpressionTree* node_10_2 = new ExpressionTree("5.678", nullptr, nullptr, 6, 10);
    ExpressionTree* node_10 = new ExpressionTree("*", node_10_1, node_10_2, 0, 10);

    QTest::newRow("10. Multiplication of two real numbers") << tokens_10 << node_10 << emptyErrors;

    // Тест 11
    QList<Token> tokens_11 = {};
    tokens_11.append(Token("2", 0, 0));
    tokens_11.append(Token("^", 1, 1));
    tokens_11.append(Token("4", 2, 2));

    ExpressionTree* node_11_1 = new ExpressionTree("2", nullptr, nullptr, 0, 0);
    ExpressionTree* node_11_2 = new ExpressionTree("4", nullptr, nullptr, 2, 2);
    ExpressionTree* node_11 = new ExpressionTree("^", node_11_1, node_11_2, 0, 2);

    QTest::newRow("11. Exponentiation with integers") << tokens_11 << node_11 << emptyErrors;

    // Тест 12
    QList<Token> tokens_12 = {};
    tokens_12.append(Token("1.234", 0, 4));
    tokens_12.append(Token("^", 5, 5));
    tokens_12.append(Token("5.678", 6, 10));

    ExpressionTree* node_12_1 = new ExpressionTree("1.234", nullptr, nullptr, 0, 4);
    ExpressionTree* node_12_2 = new ExpressionTree("5.678", nullptr, nullptr, 6, 10);
    ExpressionTree* node_12 = new ExpressionTree("^", node_12_1, node_12_2, 0, 10);

    QTest::newRow("12. Exponentiation with real numbers") << tokens_12 << node_12 << emptyErrors;

    // Тест 13
    QList<Token> tokens_13 = {};
    tokens_13.append(Token("1", 0, 0));
    tokens_13.append(Token("+", 1, 1));
    tokens_13.append(Token("2", 2, 2));
    tokens_13.append(Token("-", 3, 3));
    tokens_13.append(Token("3", 4, 4));

    ExpressionTree* node_13_1_1 = new ExpressionTree("1", nullptr, nullptr, 0, 0);
    ExpressionTree* node_13_1_2 = new ExpressionTree("2", nullptr, nullptr, 2, 2);
    ExpressionTree* node_13_1 = new ExpressionTree("+", node_13_1_1, node_13_1_2, 0, 2);
    ExpressionTree* node_13_2 = new ExpressionTree("3", nullptr, nullptr, 4, 4);
    ExpressionTree* node_13 = new ExpressionTree("-", node_13_1, node_13_2, 0, 4);

    QTest::newRow("13. Multiple same-precedence operations without spaces") << tokens_13 << node_13 << emptyErrors;

    // Тест 14
    QList<Token> tokens_14 = {};
    tokens_14.append(Token("1", 0, 0));
    tokens_14.append(Token(" ", 1, 1));
    tokens_14.append(Token("+", 2, 2));
    tokens_14.append(Token(" ", 3, 3));
    tokens_14.append(Token("2", 4, 4));

    ExpressionTree* node_14_1 = new ExpressionTree("1", nullptr, nullptr, 0, 0);
    ExpressionTree* node_14_2 = new ExpressionTree("2", nullptr, nullptr, 4, 4);
    ExpressionTree* node_14 = new ExpressionTree("+", node_14_1, node_14_2, 0, 4);

    QTest::newRow("14. Single operation with spaces") << tokens_14 << node_14 << emptyErrors;

    // Тест 15
    QList<Token> tokens_15 = {};
    tokens_15.append(Token("1", 0, 0));
    tokens_15.append(Token(" ", 1, 1));
    tokens_15.append(Token("+", 2, 2));
    tokens_15.append(Token(" ", 3, 3));
    tokens_15.append(Token("2", 4, 4));
    tokens_15.append(Token(" ", 5, 5));
    tokens_15.append(Token("-", 6, 6));
    tokens_15.append(Token(" ", 7, 7));
    tokens_15.append(Token("3", 8, 8));

    ExpressionTree* node_15_1_1 = new ExpressionTree("1", nullptr, nullptr, 0, 0);
    ExpressionTree* node_15_1_2 = new ExpressionTree("2", nullptr, nullptr, 4, 4);
    ExpressionTree* node_15_1 = new ExpressionTree("+", node_15_1_1, node_15_1_2, 0, 4);
    ExpressionTree* node_15_2 = new ExpressionTree("3", nullptr, nullptr, 8, 8);
    ExpressionTree* node_15 = new ExpressionTree("-", node_15_1, node_15_2, 0, 8);

    QTest::newRow("15. Multiple same-precedence operations with spaces") << tokens_15 << node_15 << emptyErrors;

    // Тест 16
    QList<Token> tokens_16 = {};
    tokens_16.append(Token("1", 0, 0));
    tokens_16.append(Token("+", 1, 1));
    tokens_16.append(Token("2", 2, 2));
    tokens_16.append(Token("*", 3, 3));
    tokens_16.append(Token("3", 4, 4));
    tokens_16.append(Token("-", 5, 5));
    tokens_16.append(Token("5", 6, 6));

    ExpressionTree* node_16_1_1_1 = new ExpressionTree("2", nullptr, nullptr, 2, 2);
    ExpressionTree* node_16_1_1_2 = new ExpressionTree("3", nullptr, nullptr, 4, 4);
    ExpressionTree* node_16_1_2 = new ExpressionTree("*", node_16_1_1_1, node_16_1_1_2, 2, 4);
    ExpressionTree* node_16_1_1 = new ExpressionTree("1", nullptr, nullptr, 0, 0);
    ExpressionTree* node_16_1 = new ExpressionTree("+", node_16_1_1, node_16_1_2, 0, 4);
    ExpressionTree* node_16_2 = new ExpressionTree("5", nullptr, nullptr, 6, 6);
    ExpressionTree* node_16 = new ExpressionTree("-", node_16_1, node_16_2, 0, 6);

    QTest::newRow("16. Mixed precedence operations") << tokens_16 << node_16 << emptyErrors;

    // Тест 17
    QList<Token> tokens_17 = {};
    tokens_17.append(Token("1", 0, 0));
    tokens_17.append(Token("+", 1, 1));
    tokens_17.append(Token("2", 2, 2));
    tokens_17.append(Token("*", 3, 3));
    tokens_17.append(Token("3", 4, 4));
    tokens_17.append(Token("^", 5, 5));
    tokens_17.append(Token("5", 6, 6));

    ExpressionTree* node_17_1_1_1 = new ExpressionTree("3", nullptr, nullptr, 4, 4);
    ExpressionTree* node_17_1_1_2 = new ExpressionTree("5", nullptr, nullptr, 6, 6);
    ExpressionTree* node_17_1_2 = new ExpressionTree("^", node_17_1_1_1, node_17_1_1_2, 4, 6);
    ExpressionTree* node_17_1_1 = new ExpressionTree("2", nullptr, nullptr, 2, 2);
    ExpressionTree* node_17_2 = new ExpressionTree("*", node_17_1_1, node_17_1_2, 2, 6);
    ExpressionTree* node_17_1 = new ExpressionTree("1", nullptr, nullptr, 0, 0);
    ExpressionTree* node_17 = new ExpressionTree("+", node_17_1, node_17_2, 0, 6);

    QTest::newRow("17. Mixed precedence with exponentiation") << tokens_17 << node_17 << emptyErrors;

    // Тест 18
    QList<Token> tokens_18 = {};
    tokens_18.append(Token("-u", 0, 0));
    tokens_18.append(Token("1", 1, 1));
    tokens_18.append(Token("+", 2, 2));
    tokens_18.append(Token("2", 3, 3));
    tokens_18.append(Token("*", 4, 4));
    tokens_18.append(Token("3", 5, 5));
    tokens_18.append(Token("^", 6, 6));
    tokens_18.append(Token("5", 7, 7));

    ExpressionTree* node_18_1_1_1 = new ExpressionTree("3", nullptr, nullptr, 5, 5);
    ExpressionTree* node_18_1_1_2 = new ExpressionTree("5", nullptr, nullptr, 7, 7);
    ExpressionTree* node_18_1_2 = new ExpressionTree("^", node_18_1_1_1, node_18_1_1_2, 5, 7);
    ExpressionTree* node_18_1_1 = new ExpressionTree("2", nullptr, nullptr, 3, 3);
    ExpressionTree* node_18_2 = new ExpressionTree("*", node_18_1_1, node_18_1_2, 3, 7);
    ExpressionTree* node_18_1_u = new ExpressionTree("1", nullptr, nullptr, 1, 1);
    ExpressionTree* node_18_1 = new ExpressionTree("-u", node_18_1_u, nullptr, 0, 1);
    ExpressionTree* node_18 = new ExpressionTree("+", node_18_1, node_18_2, 0, 7);

    QTest::newRow("18. Mixed operations with exponentiation and unary minus") << tokens_18 << node_18 << emptyErrors;

    // Тест 19
    QList<Token> tokens_19 = {};
    tokens_19.append(Token("(", 0, 0));
    tokens_19.append(Token("1", 1, 1));
    tokens_19.append(Token("+", 2, 2));
    tokens_19.append(Token("2", 3, 3));
    tokens_19.append(Token(")", 4, 4));

    ExpressionTree* node_19_1 = new ExpressionTree("1", nullptr, nullptr, 1, 1);
    ExpressionTree* node_19_2 = new ExpressionTree("2", nullptr, nullptr, 3, 3);
    ExpressionTree* node_19 = new ExpressionTree("+", node_19_1, node_19_2, 0, 4);

    QTest::newRow("19. Parentheses without priority change") << tokens_19 << node_19 << emptyErrors;

    // Тест 20
    QList<Token> tokens_20 = {};
    tokens_20.append(Token("(", 0, 0));
    tokens_20.append(Token("1", 1, 1));
    tokens_20.append(Token("+", 2, 2));
    tokens_20.append(Token("2", 3, 3));
    tokens_20.append(Token(")", 4, 4));
    tokens_20.append(Token("^", 5, 5));
    tokens_20.append(Token("3", 6, 6));

    ExpressionTree* node_20_1_1 = new ExpressionTree("1", nullptr, nullptr, 1, 1);
    ExpressionTree* node_20_1_2 = new ExpressionTree("2", nullptr, nullptr, 3, 3);
    ExpressionTree* node_20_1 = new ExpressionTree("+", node_20_1_1, node_20_1_2, 0, 4);
    ExpressionTree* node_20_2 = new ExpressionTree("3", nullptr, nullptr, 6, 6);
    ExpressionTree* node_20 = new ExpressionTree("^", node_20_1, node_20_2, 0, 6);

    QTest::newRow("20. Parentheses changing operation priority") << tokens_20 << node_20 << emptyErrors;

    // Тест 21
    QList<Token> tokens_21 = {};
    tokens_21.append(Token("(", 0, 0));
    tokens_21.append(Token("1", 1, 1));
    tokens_21.append(Token("/", 2, 2));
    tokens_21.append(Token("(", 3, 3));
    tokens_21.append(Token("3", 4, 4));
    tokens_21.append(Token("-", 5, 5));
    tokens_21.append(Token("2", 6, 6));
    tokens_21.append(Token(")", 7, 7));
    tokens_21.append(Token(")", 8, 8));
    tokens_21.append(Token("^", 9, 9));
    tokens_21.append(Token("4", 10, 10));

    ExpressionTree* node_21_1_2_1 = new ExpressionTree("3", nullptr, nullptr, 4, 4);
    ExpressionTree* node_21_1_2_2 = new ExpressionTree("2", nullptr, nullptr, 6, 6);
    ExpressionTree* node_21_1_2 = new ExpressionTree("-", node_21_1_2_1, node_21_1_2_2, 3, 7);
    ExpressionTree* node_21_1_1 = new ExpressionTree("1", nullptr, nullptr, 1, 1);
    ExpressionTree* node_21_1 = new ExpressionTree("/", node_21_1_1, node_21_1_2, 0, 8);
    ExpressionTree* node_21_2 = new ExpressionTree("4", nullptr, nullptr, 10, 10);
    ExpressionTree* node_21 = new ExpressionTree("^", node_21_1, node_21_2, 0, 10);

    QTest::newRow("21. Nested parentheses") << tokens_21 << node_21 << emptyErrors;

    // Тест 22
    QList<Token> tokens_22 = {};
    tokens_22.append(Token("-u", 0, 0));
    tokens_22.append(Token("(", 1, 1));
    tokens_22.append(Token("1", 2, 2));
    tokens_22.append(Token("+", 3, 3));
    tokens_22.append(Token("5", 4, 4));
    tokens_22.append(Token(")", 5, 5));
    tokens_22.append(Token("^", 6, 6));
    tokens_22.append(Token("2", 7, 7));

    ExpressionTree* node_22_1_1_1 = new ExpressionTree("1", nullptr, nullptr, 2, 2);
    ExpressionTree* node_22_1_1_2 = new ExpressionTree("5", nullptr, nullptr, 4, 4);
    ExpressionTree* node_22_1_1 = new ExpressionTree("+", node_22_1_1_1, node_22_1_1_2, 1, 5);
    ExpressionTree* node_22_1_2 = new ExpressionTree("2", nullptr, nullptr, 7, 7);
    ExpressionTree* node_22_1 = new ExpressionTree("-u", node_22_1_1, nullptr, 0, 5);
    ExpressionTree* node_22 = new ExpressionTree("^", node_22_1, node_22_1_2, 0, 7);

    QTest::newRow("22. Unary minus before parenthesized expression") << tokens_22 << node_22 << emptyErrors;

    // Тест 23
    QList<Token> tokens_23 = {};
    tokens_23.append(Token("(", 0, 0));
    tokens_23.append(Token("-u", 1, 1));
    tokens_23.append(Token("(", 2, 2));
    tokens_23.append(Token("1", 3, 3));
    tokens_23.append(Token("+", 4, 4));
    tokens_23.append(Token("5", 5, 5));
    tokens_23.append(Token(")", 6, 6));
    tokens_23.append(Token(")", 7, 7));
    tokens_23.append(Token("^", 8, 8));
    tokens_23.append(Token("2", 9, 9));

    ExpressionTree* node_23_1_1_1 = new ExpressionTree("1", nullptr, nullptr, 3, 3);
    ExpressionTree* node_23_1_1_2 = new ExpressionTree("5", nullptr, nullptr, 5, 5);
    ExpressionTree* node_23_1_1 = new ExpressionTree("+", node_23_1_1_1, node_23_1_1_2, 2, 6);
    ExpressionTree* node_23_1 = new ExpressionTree("-u", node_23_1_1, nullptr, 1, 6);
    ExpressionTree* node_23_2 = new ExpressionTree("2", nullptr, nullptr, 9, 9);
    ExpressionTree* node_23 = new ExpressionTree("^", node_23_1, node_23_2, 0, 9);

    QTest::newRow("23. Unary minus inside parentheses") << tokens_23 << node_23 << emptyErrors;

    // Тест 24
    QList<Token> tokens_24 = {};
    tokens_24.append(Token("-u", 0, 0));
    tokens_24.append(Token("-u", 1, 1));
    tokens_24.append(Token("-u", 2, 2));
    tokens_24.append(Token("(", 3, 3));
    tokens_24.append(Token("1", 4, 4));
    tokens_24.append(Token("+", 5, 5));
    tokens_24.append(Token("5", 6, 6));
    tokens_24.append(Token(")", 7, 7));

    ExpressionTree* node_24_1_1_1_1 = new ExpressionTree("1", nullptr, nullptr, 4, 4);
    ExpressionTree* node_24_1_1_1_2 = new ExpressionTree("5", nullptr, nullptr, 6, 6);
    ExpressionTree* node_24_1_1_1 = new ExpressionTree("+", node_24_1_1_1_1, node_24_1_1_1_2, 3, 7);
    ExpressionTree* node_24_1_1 = new ExpressionTree("-u", node_24_1_1_1, nullptr, 2, 7);
    ExpressionTree* node_24_1 = new ExpressionTree("-u", node_24_1_1, nullptr, 1, 7);
    ExpressionTree* node_24 = new ExpressionTree("-u", node_24_1, nullptr, 0, 7);

    QTest::newRow("24. Multiple consecutive unary minuses") << tokens_24 << node_24 << emptyErrors;

    // Тест 25
    QList<Token> tokens_25 = {};
    tokens_25.append(Token("(", 0, 0));
    tokens_25.append(Token(")", 1, 1));
    tokens_25.append(Token("1", 2, 2));
    tokens_25.append(Token("+", 3, 3));
    tokens_25.append(Token("2", 4, 4));
    tokens_25.append(Token("/", 5, 5));
    tokens_25.append(Token("(", 6, 6));
    tokens_25.append(Token(")", 7, 7));
    tokens_25.append(Token("3", 8, 8));

    ExpressionTree* node_25_2_1 = new ExpressionTree("2", nullptr, nullptr, 4, 4);
    ExpressionTree* node_25_2_2 = new ExpressionTree("3", nullptr, nullptr, 8, 8);
    ExpressionTree* node_25_2 = new ExpressionTree("/", node_25_2_1, node_25_2_2, 4, 8);
    ExpressionTree* node_25_1 = new ExpressionTree("1", nullptr, nullptr, 2, 2);
    ExpressionTree* node_25 = new ExpressionTree("+", node_25_1, node_25_2, 2, 8);

    QTest::newRow("25. Empty insignificant parentheses blocks") << tokens_25 << node_25 << emptyErrors;

    // Тест 26
    QList<Token> tokens_26 = {};
    tokens_26.append(Token("*", 0 , 0));
    tokens_26.append(Token("3", 1, 1));
    tokens_26.append(Token("+", 2, 2));
    tokens_26.append(Token("4", 3, 3));

    ExpressionTree* node_26 = nullptr;

    Error error_26;
    error_26.setType(Error::Type::noLeftOperand);
    error_26.setOperation("*");
    error_26.setSymbolIndex(0);

    QSet<Error> errors_26 = { error_26 };

    QTest::newRow("26. Missing left operand for specific operation") << tokens_26 << node_26 << errors_26;

    // Тест 27
    QList<Token> tokens_27 = {};
    tokens_27.append(Token("*", 0 , 0));
    tokens_27.append(Token("3", 1, 1));
    tokens_27.append(Token("+", 2, 2));
    tokens_27.append(Token("(", 3, 3));
    tokens_27.append(Token("/", 4, 4));
    tokens_27.append(Token("4", 5, 5));
    tokens_27.append(Token(")", 6, 6));

    ExpressionTree* node_27 = nullptr;

    Error error_27;
    error_27.setType(Error::Type::noLeftOperand);
    error_27.setOperation("*");
    error_27.setSymbolIndex(0);

    QSet<Error> errors_27 = { error_27 };

    QTest::newRow("27. Missing left operand for multiple operations") << tokens_27 << node_27 << errors_27;

    // Тест 28
    QList<Token> tokens_28 = {};
    tokens_28.append(Token("3", 0, 0));
    tokens_28.append(Token("+", 1, 1));
    tokens_28.append(Token("2", 2, 2));
    tokens_28.append(Token("*", 3, 3));

    ExpressionTree* node_28 = nullptr;

    Error error_28;
    error_28.setType(Error::Type::noRightOperand);
    error_28.setOperation("*");
    error_28.setSymbolIndex(3);

    QSet<Error> errors_28 = { error_28 };

    QTest::newRow("28. Missing right operand for specific operation") << tokens_28 << node_28 << errors_28;

    // Тест 29
    QList<Token> tokens_29 = {};
    tokens_29.append(Token("3", 0, 0));
    tokens_29.append(Token("/", 1, 1));
    tokens_29.append(Token("(", 2, 2));
    tokens_29.append(Token("4", 3, 3));
    tokens_29.append(Token("^", 4, 4));
    tokens_29.append(Token(")", 5, 5));
    tokens_29.append(Token("+", 6, 6));
    tokens_29.append(Token("2", 7, 7));
    tokens_29.append(Token("*", 8, 8));

    ExpressionTree* node_29 = nullptr;

    Error error_29;
    error_29.setType(Error::Type::noRightOperand);
    error_29.setOperation("^");
    error_29.setSymbolIndex(4);

    QSet<Error> errors_29 = { error_29 };

    QTest::newRow("29. Missing right operand for multiple operations") << tokens_29 << node_29 << errors_29;

    // Тест 30
    QList<Token> tokens_30 = {};
    tokens_30.append(Token("*", 0, 0));

    ExpressionTree* node_30 = nullptr;

    Error error_30;
    error_30.setType(Error::Type::noBothOperands);
    error_30.setOperation("*");
    error_30.setSymbolIndex(0);

    QSet<Error> errors_30 = { error_30 };

    QTest::newRow("30. Missing both operands for specific operation") << tokens_30 << node_30 << errors_30;

    // Тест 31
    QList<Token> tokens_31 = {};
    tokens_31.append(Token("*", 0, 0));
    tokens_31.append(Token("/", 1, 1));

    ExpressionTree* node_31 = nullptr;

    Error error_31;
    error_31.setType(Error::Type::noBothOperands);
    error_31.setOperation("*");
    error_31.setSymbolIndex(0);

    QSet<Error> errors_31 = { error_31 };

    QTest::newRow("31. Missing both operands for multiple operations") << tokens_31 << node_31 << errors_31;

    // Тест 32
    QList<Token> tokens_32 = {};
    tokens_32.append(Token("(", 0, 0));
    tokens_32.append(Token("1", 1, 1));
    tokens_32.append(Token("+", 2, 2));
    tokens_32.append(Token("2", 3, 3));

    ExpressionTree* node_32 = nullptr;

    Error error_32;
    error_32.setType(Error::Type::noClosingParenthesis);
    error_32.setSymbolIndex(0);

    QSet<Error> errors_32 = { error_32 };

    QTest::newRow("32. Missing single closing parenthesis") << tokens_32 << node_32 << errors_32;

    // Тест 33
    QList<Token> tokens_33 = {};
    tokens_33.append(Token("(", 0, 0));
    tokens_33.append(Token("1", 1, 1));
    tokens_33.append(Token("+", 2, 2));
    tokens_33.append(Token("2", 3, 3));
    tokens_33.append(Token("(", 4, 4));
    tokens_33.append(Token(")", 5, 5));

    ExpressionTree* node_33 = nullptr;

    Error error_33;
    error_33.setType(Error::Type::noClosingParenthesis);
    error_33.setSymbolIndex(0);

    QSet<Error> errors_33 = { error_33 };

    QTest::newRow("33. Missing closing parenthesis with valid pairs present") << tokens_33 << node_33 << errors_33;

    // Тест 34
    QList<Token> tokens_34 = {};
    tokens_34.append(Token("(", 0, 0));
    tokens_34.append(Token("(", 1, 1));
    tokens_34.append(Token("1", 2, 2));
    tokens_34.append(Token("+", 3, 3));
    tokens_34.append(Token("2", 4, 4));

    ExpressionTree* node_34 = nullptr;

    Error error_34;
    error_34.setType(Error::Type::noClosingParenthesis);
    error_34.setSymbolIndex(1);

    QSet<Error> errors_34 = { error_34 };

    QTest::newRow("34. Multiple missing closing parentheses") << tokens_34 << node_34 << errors_34;

    // Тест 35
    QList<Token> tokens_35 = {};
    tokens_35.append(Token("1", 0, 0));
    tokens_35.append(Token("+", 1, 1));
    tokens_35.append(Token("2", 2, 2));
    tokens_35.append(Token(")", 3, 3));

    ExpressionTree* node_35 = nullptr;

    Error error_35;
    error_35.setType(Error::Type::noOpeningParenthesis);
    error_35.setSymbolIndex(3);

    QSet<Error> errors_35 = { error_35 };

    QTest::newRow("35. Missing single opening parenthesis") << tokens_35 << node_35 << errors_35;

    // Тест 36
    QList<Token> tokens_36 = {};
    tokens_36.append(Token("(", 0, 0));
    tokens_36.append(Token("1", 1, 1));
    tokens_36.append(Token("+", 2, 2));
    tokens_36.append(Token("2", 3, 3));
    tokens_36.append(Token(")", 4, 4));
    tokens_36.append(Token(")", 5, 5));

    ExpressionTree* node_36 = nullptr;

    Error error_36;
    error_36.setType(Error::Type::noOpeningParenthesis);
    error_36.setSymbolIndex(5);

    QSet<Error> errors_36 = { error_36 };

    QTest::newRow("36. Missing opening parenthesis with valid pairs present") << tokens_36 << node_36 << errors_36;

    // Тест 37
    QList<Token> tokens_37 = {};
    tokens_37.append(Token("1", 0, 0));
    tokens_37.append(Token("+", 1, 1));
    tokens_37.append(Token("2", 2, 2));
    tokens_37.append(Token(")", 3, 3));
    tokens_37.append(Token(")", 4, 4));

    ExpressionTree* node_37 = nullptr;

    Error error_37;
    error_37.setType(Error::Type::noOpeningParenthesis);
    error_37.setSymbolIndex(3);

    QSet<Error> errors_37 = { error_37 };

    QTest::newRow("37. Multiple missing opening parentheses") << tokens_37 << node_37 << errors_37;

    // Тест 38
    QList<Token> tokens_38 = {};
    tokens_38.append(Token("1", 0, 0));
    tokens_38.append(Token(" ", 1, 1));
    tokens_38.append(Token("2", 2, 2));

    ExpressionTree* node_38 = nullptr;

    Error error_38;
    error_38.setType(Error::Type::noCalcOperation);
    error_38.setLeftOperand("1");
    error_38.setRightOperand("2");

    QSet<Error> errors_38 = { error_38 };

    QTest::newRow("38. Missing operator between numeric operands") << tokens_38 << node_38 << errors_38;

    // Тест 39
    QList<Token> tokens_39 = {};
    tokens_39.append(Token("(", 0, 0));
    tokens_39.append(Token("1", 1, 1));
    tokens_39.append(Token("+", 2, 2));
    tokens_39.append(Token("2", 3, 3));
    tokens_39.append(Token(")", 4, 4));
    tokens_39.append(Token("(", 5, 5));
    tokens_39.append(Token("3", 6, 6));
    tokens_39.append(Token("+", 7, 7));
    tokens_39.append(Token("4", 8, 8));
    tokens_39.append(Token(")", 9, 9));

    ExpressionTree* node_39 = nullptr;

    Error error_39;
    error_39.setType(Error::Type::noCalcOperation);
    error_39.setLeftOperand("(1+2)");
    error_39.setRightOperand("(3+4)");

    QSet<Error> errors_39 = { error_39 };

    QTest::newRow("39. Missing operator between parenthesized operands") << tokens_39 << node_39 << errors_39;
}

QString Test_Build::compareErrors(const QSet<Error>& actual, const QSet<Error>& expected) {
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

bool Test_Build::compareTrees(const ExpressionTree* expected, const ExpressionTree* actual, QStringList& tracePath, QString& error) {
    if (!expected && !actual) {
        return true;
    }
    if (!expected || !actual) {
        error = "Null node encountered at path: " + tracePath.join("");
        return false;
    }

    if (!tracePath.empty()) {
        tracePath += " -> ";
    }
    tracePath << QString("Node(%1)").arg(expected ? expected->toString() : "null");

    if (expected->getNodeType() != actual->getNodeType()) {
        error = QString("Node type mismatch. Expected: %1, Found: %2. Path: %3")
        .arg(expected ? expected->toString() : "null")
            .arg(actual ? actual->toString() : "null")
            .arg(tracePath.join(""));
        return false;
    }

    if (expected->getNodeType() == ExpressionTree::Operand &&
        (expected->getValue() != actual->getValue() ||
         expected->getStart() != actual->getStart() ||
         expected->getEnd() != actual->getEnd())) {
        error = QString("Value or range mismatch. Expected: %1 (%2-%3), Found: %4 (%5-%6). Path: %7")
        .arg(expected->getValue()).arg(expected->getStart()).arg(expected->getEnd())
            .arg(actual->getValue()).arg(actual->getStart()).arg(actual->getEnd())
            .arg(tracePath.join(""));
        return false;
    }

    if (expected->getLeft() || actual->getLeft()) {
        if (!compareTrees(expected->getLeft(), actual->getLeft(), tracePath, error)) {
            return false;
        }
    }
    if (expected->getRight() || actual->getRight()) {
        if (!compareTrees(expected->getRight(), actual->getRight(), tracePath, error)) {
            return false;
        }
    }

    if (!tracePath.isEmpty()) {
        tracePath.removeLast();
        if (!tracePath.isEmpty()) {
            tracePath.removeLast();
        }
    }

    return true;
}
