/*!
* \file
* \brief Реализация методов класса тестирования функции ExpressionTree::determineNodeType
*/

#include "test_determinenodetype.h"

void Test_DetermineNodeType::testDetermineNodeType() {
    QFETCH(QString, inputString);
    QFETCH(ExpressionTree::NodeType, type);

    ExpressionTree::NodeType actualType = ExpressionTree::determineNodeType(inputString);

    QVERIFY2(actualType == type,
             QString("Node type mismatch for input '%1': Expected %2, but got %3")
                 .arg(qPrintable(inputString))
                 .arg(static_cast<int>(type))
                 .arg(static_cast<int>(actualType))
                 .toUtf8().constData());
}

void Test_DetermineNodeType::testDetermineNodeType_data() {
    QTest::addColumn<QString>("inputString");
    QTest::addColumn<ExpressionTree::NodeType>("type");

    // Тест 1
    QString input_1 = " ";
    ExpressionTree::NodeType type_1 = ExpressionTree::NodeType::Operand;

    QTest::newRow("1. Space") << input_1 << type_1;

    // Тест 2
    QString input_2 = "abc";
    ExpressionTree::NodeType type_2 = ExpressionTree::NodeType::Operand;

    QTest::newRow("2. Unknown symbols sequence") << input_2 << type_2;

    // Тест 3
    QString input_3 = "(";
    ExpressionTree::NodeType type_3 = ExpressionTree::NodeType::Operand;

    QTest::newRow("3. Opening parenthesis") << input_3 << type_3;

    // Тест 4
    QString input_4 = ")";
    ExpressionTree::NodeType type_4 = ExpressionTree::NodeType::Operand;

    QTest::newRow("4. Closing parenthesis") << input_4 << type_4;

    // Тест 5
    QString input_5 = "3";
    ExpressionTree::NodeType type_5 = ExpressionTree::NodeType::Operand;

    QTest::newRow("5. Integer number") << input_5 << type_5;

    // Тест 6
    QString input_6 = "0.345";
    ExpressionTree::NodeType type_6 = ExpressionTree::NodeType::Operand;

    QTest::newRow("6. Float number") << input_6 << type_6;

    // Тест 7
    QString input_7 = "+";
    ExpressionTree::NodeType type_7 = ExpressionTree::NodeType::Plus;

    QTest::newRow("7. Plus") << input_7 << type_7;

    // Тест 8
    QString input_8 = "-";
    ExpressionTree::NodeType type_8 = ExpressionTree::NodeType::BinaryMinus;

    QTest::newRow("8. Binary minus") << input_8 << type_8;

    // Тест 9
    QString input_9 = "-u";
    ExpressionTree::NodeType type_9 = ExpressionTree::NodeType::UnaryMinus;

    QTest::newRow("9. Unary minus") << input_9 << type_9;

    // Тест 10
    QString input_10 = "*";
    ExpressionTree::NodeType type_10 = ExpressionTree::NodeType::Multiplication;

    QTest::newRow("10. Multiplication") << input_10 << type_10;

    // Тест 11
    QString input_11 = "/";
    ExpressionTree::NodeType type_11 = ExpressionTree::NodeType::Division;

    QTest::newRow("11. Division") << input_11 << type_11;

    // Тест 12
    QString input_12 = "^";
    ExpressionTree::NodeType type_12 = ExpressionTree::NodeType::Power;

    QTest::newRow("12. Power") << input_12 << type_12;
}

