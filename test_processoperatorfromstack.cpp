#include "test_processoperatorfromstack.h"

void Test_ProcessOperatorFromStack::testProcessOperatorFromStack()
{
    QFETCH(QStack<Token>, inputOperators);
    QFETCH(QStack<Token>, outputOperators);
    QFETCH(QStack<ExpressionTree*>, inputNodes);
    QFETCH(QStack<ExpressionTree*>, outputNodes);
    QFETCH(bool, processed);
    QFETCH(QSet<Error>, errors);

    QStack<Token> actualOperators = inputOperators;
    QStack<ExpressionTree*> actualNodes = inputNodes;
    QSet<Error> actualErrors;
    bool actualProcessed = ExpressionTree::processOperatorFromStack(actualOperators, actualNodes, actualErrors);

    QVERIFY2(actualProcessed == processed,
             qPrintable(QString("Expected processing status: %1, Actual: %2")
                            .arg(processed).arg(actualProcessed)));

    QString opStackDiff = compareTokenStacks(actualOperators, outputOperators);
    QVERIFY2(opStackDiff.isEmpty(),
             qPrintable(QString("Operator stack mismatch:\n%1").arg(opStackDiff)));

    QString nodeStackDiff = compareNodeStacks(actualNodes, outputNodes);
    QVERIFY2(nodeStackDiff.isEmpty(),
             qPrintable(QString("Node stack mismatch:\n%1").arg(nodeStackDiff)));

    QString errorDiff = compareErrors(actualErrors, errors);
    QVERIFY2(errorDiff.isEmpty(),
             qPrintable(QString("Error mismatch:\n%1").arg(errorDiff)));
}

void Test_ProcessOperatorFromStack::testProcessOperatorFromStack_data() {
    QTest::addColumn<QStack<Token>>("inputOperators");
    QTest::addColumn<QStack<Token>>("outputOperators");
    QTest::addColumn<QStack<ExpressionTree*>>("inputNodes");
    QTest::addColumn<QStack<ExpressionTree*>>("outputNodes");
    QTest::addColumn<bool>("processed");
    QTest::addColumn<QSet<Error>>("errors");

    QSet<Error> emptyErrors = {};

    // Тест 1
    QStack<Token> inputOperators_1;
    inputOperators_1.push(Token("4", 5, 5));
    QStack<Token> outputOperators_1;

    ExpressionTree* node_1_1_1 = new ExpressionTree("1", nullptr, nullptr, 0, 0);
    ExpressionTree* node_1_1_2 = new ExpressionTree("2", nullptr, nullptr, 2, 2);
    ExpressionTree* node_1_1 = new ExpressionTree("+", node_1_1_1, node_1_1_2, 0, 2);
    ExpressionTree* node_1_2 = new ExpressionTree("4", nullptr, nullptr, 5, 5);

    QStack<ExpressionTree*> inputNodes_1;
    inputNodes_1.push(node_1_1);
    QStack<ExpressionTree*> outputNodes_1;
    outputNodes_1.push(node_1_1);
    outputNodes_1.push(node_1_2);
    bool processed_1 = true;

    QTest::newRow("1. Integer token at the top of operator stack") << inputOperators_1 << outputOperators_1 << inputNodes_1 << outputNodes_1 << processed_1 << emptyErrors;

    // Тест 2
    QStack<Token> inputOperators_2;
    inputOperators_2.push(Token("4.56", 5, 8));
    QStack<Token> outputOperators_2;

    ExpressionTree* node_2_1_1 = new ExpressionTree("1", nullptr, nullptr, 0, 0);
    ExpressionTree* node_2_1_2 = new ExpressionTree("2", nullptr, nullptr, 2, 2);
    ExpressionTree* node_2_1 = new ExpressionTree("+", node_2_1_1, node_2_1_2, 0, 2);
    ExpressionTree* node_2_2 = new ExpressionTree("4.56", nullptr, nullptr, 5, 8);

    QStack<ExpressionTree*> inputNodes_2;
    inputNodes_2.push(node_2_1);
    QStack<ExpressionTree*> outputNodes_2;
    outputNodes_2.push(node_2_1);
    outputNodes_2.push(node_2_2);
    bool processed_2 = true;

    QTest::newRow("2. Real number token at the top of operator stack") << inputOperators_2 << outputOperators_2 << inputNodes_2 << outputNodes_2 << processed_2 << emptyErrors;

    // Тест 3
    QStack<Token> inputOperators_3;
    inputOperators_3.push(Token("/", 3, 3));
    QStack<Token> outputOperators_3;

    ExpressionTree* node_3_1_1 = new ExpressionTree("1", nullptr, nullptr, 0, 0);
    ExpressionTree* node_3_1_2 = new ExpressionTree("2", nullptr, nullptr, 2, 2);
    ExpressionTree* node_3_1 = new ExpressionTree("-", node_3_1_1, node_3_1_2, 0, 2);
    ExpressionTree* node_3_2 = new ExpressionTree("3", nullptr, nullptr, 4, 4);
    ExpressionTree* node_3 = new ExpressionTree("/", node_3_1, node_3_2, 0, 4);

    QStack<ExpressionTree*> inputNodes_3;
    inputNodes_3.push(node_3_1);
    inputNodes_3.push(node_3_2);
    QStack<ExpressionTree*> outputNodes_3;
    outputNodes_3.push(node_3);
    bool processed_3 = true;

    QTest::newRow("3. Only one node in data stacks") << inputOperators_3 << outputOperators_3 << inputNodes_3 << outputNodes_3 << processed_3 << emptyErrors;

    // Тест 4
    QStack<Token> inputOperators_4;
    inputOperators_4.push(Token("/", 7, 7));
    QStack<Token> outputOperators_4;

    ExpressionTree* node_4_1_1 = new ExpressionTree("1", nullptr, nullptr, 1, 1);
    ExpressionTree* node_4_1_2 = new ExpressionTree("1.2", nullptr, nullptr, 3, 5);
    ExpressionTree* node_4_1 = new ExpressionTree("-", node_4_1_1, node_4_1_2, 1, 5);
    ExpressionTree* node_4_2_1 = new ExpressionTree("3.1", nullptr, nullptr, 9, 11);
    ExpressionTree* node_4_2_2 = new ExpressionTree("4", nullptr, nullptr, 13, 13);
    ExpressionTree* node_4_2 = new ExpressionTree("+", node_4_2_1, node_4_2_2, 9, 13);
    ExpressionTree* node_4 = new ExpressionTree("/", node_4_1, node_4_2, 1, 13);

    QStack<ExpressionTree*> inputNodes_4;
    inputNodes_4.push(node_4_1);
    inputNodes_4.push(node_4_2);
    QStack<ExpressionTree*> outputNodes_4;
    outputNodes_4.push(node_4);
    bool processed_4 = true;

    QTest::newRow("4. Division at top of operator stack with both operands in parentheses") << inputOperators_4 << outputOperators_4 << inputNodes_4 << outputNodes_4 << processed_4 << emptyErrors;

    // Тест 5
    QStack<Token> inputOperators_5;
    inputOperators_5.push(Token("^", 7, 7));
    QStack<Token> outputOperators_5;

    ExpressionTree* node_5_1_1 = new ExpressionTree("1", nullptr, nullptr, 1, 1);
    ExpressionTree* node_5_1_2 = new ExpressionTree("1.2", nullptr, nullptr, 3, 5);
    ExpressionTree* node_5_1 = new ExpressionTree("-", node_5_1_1, node_5_1_2, 1, 5);
    ExpressionTree* node_5_2_1 = new ExpressionTree("2.1", nullptr, nullptr, 9, 11);
    ExpressionTree* node_5_2_2 = new ExpressionTree("3", nullptr, nullptr, 13, 13);
    ExpressionTree* node_5_2 = new ExpressionTree("+", node_5_2_1, node_5_2_2, 9, 13);
    ExpressionTree* node_5 = new ExpressionTree("^", node_5_1, node_5_2, 1, 13);

    QStack<ExpressionTree*> inputNodes_5;
    inputNodes_5.push(node_5_1);
    inputNodes_5.push(node_5_2);
    QStack<ExpressionTree*> outputNodes_5;
    outputNodes_5.push(node_5);
    bool processed_5 = true;

    QTest::newRow("5. Exponentiation at top of operator stack with both operands in parentheses") << inputOperators_5 << outputOperators_5 << inputNodes_5 << outputNodes_5 << processed_5 << emptyErrors;

    // Тест 6
    QStack<Token> inputOperators_6;
    inputOperators_6.push(Token("(", 1, 1));
    inputOperators_6.push(Token("^", 3, 3));
    QStack<Token> outputOperators_6;
    outputOperators_6.push(Token("(", 1, 1));

    ExpressionTree* node_6_1 = new ExpressionTree("1", nullptr, nullptr, 0, 0);
    ExpressionTree* node_6_2 = new ExpressionTree("2", nullptr, nullptr, 2, 2);
    ExpressionTree* node_6_3 = new ExpressionTree("3", nullptr, nullptr, 4, 4);
    ExpressionTree* node_6 = new ExpressionTree("^", node_6_2, node_6_3, 2, 4);

    QStack<ExpressionTree*> inputNodes_6;
    inputNodes_6.push(node_6_1);
    inputNodes_6.push(node_6_2);
    inputNodes_6.push(node_6_3);
    QStack<ExpressionTree*> outputNodes_6;
    outputNodes_6.push(node_6_1);
    outputNodes_6.push(node_6);
    bool processed_6 = true;

    QTest::newRow("6. Open parenthesis present in operator stack") << inputOperators_6 << outputOperators_6 << inputNodes_6 << outputNodes_6 << processed_6 << emptyErrors;

    // Тест 7
    QStack<Token> operators_7;
    operators_7.push(Token("(", 0, 0));

    ExpressionTree* node_7_1_1 = new ExpressionTree("1", nullptr, nullptr, 1, 1);
    ExpressionTree* node_7_1_2 = new ExpressionTree("2", nullptr, nullptr, 3, 3);
    ExpressionTree* node_7_1 = new ExpressionTree("*", node_7_1_1, node_7_1_2, 1, 3);
    ExpressionTree* node_7_2_1 = new ExpressionTree("3", nullptr, nullptr, 7, 7);
    ExpressionTree* node_7_2_2 = new ExpressionTree("4", nullptr, nullptr, 9, 9);
    ExpressionTree* node_7_2 = new ExpressionTree("/", node_7_2_1, node_7_2_2, 7, 9);

    QStack<ExpressionTree*> nodes_7;
    nodes_7.push(node_7_1);
    nodes_7.push(node_7_2);
    bool processed_7 = false;

    QTest::newRow("7. Open parenthesis at top of operator stack") << operators_7 << operators_7 << nodes_7 << nodes_7 << processed_7 << emptyErrors;

    // Тест 8
    QStack<Token> operators_8;
    operators_8.push(Token(")", 11, 11));

    ExpressionTree* node_8_1_1 = new ExpressionTree("1", nullptr, nullptr, 1, 1);
    ExpressionTree* node_8_1_2 = new ExpressionTree("2", nullptr, nullptr, 3, 3);
    ExpressionTree* node_8_1 = new ExpressionTree("*", node_8_1_1, node_8_1_2, 1, 3);
    ExpressionTree* node_8_2_1 = new ExpressionTree("3", nullptr, nullptr, 7, 7);
    ExpressionTree* node_8_2_2 = new ExpressionTree("4", nullptr, nullptr, 9, 9);
    ExpressionTree* node_8_2 = new ExpressionTree("/", node_8_2_1, node_8_2_2, 7, 9);

    QStack<ExpressionTree*> nodes_8;
    nodes_8.push(node_8_1);
    nodes_8.push(node_8_2);
    bool processed_8 = false;

    QTest::newRow("8. Close parenthesis at top of operator stack") << operators_8 << operators_8 << nodes_8 << nodes_8 << processed_8 << emptyErrors;

    // Тест 9
    QStack<Token> operators_9;
    operators_9.push(Token(" ", 0, 0));

    ExpressionTree* node_9_1_1 = new ExpressionTree("1", nullptr, nullptr, 1, 1);
    ExpressionTree* node_9_1_2 = new ExpressionTree("2", nullptr, nullptr, 3, 3);
    ExpressionTree* node_9_1 = new ExpressionTree("*", node_9_1_1, node_9_1_2, 1, 3);
    ExpressionTree* node_9_2_1 = new ExpressionTree("3", nullptr, nullptr, 8, 8);
    ExpressionTree* node_9_2_2 = new ExpressionTree("4", nullptr, nullptr, 10, 10);
    ExpressionTree* node_9_2 = new ExpressionTree("/", node_9_2_1, node_9_2_2, 8, 10);

    QStack<ExpressionTree*> nodes_9;
    nodes_9.push(node_9_1);
    nodes_9.push(node_9_2);
    bool processed_9 = false;

    QTest::newRow("9. Space at top of operator stack") << operators_9 << operators_9 << nodes_9 << nodes_9 << processed_9 << emptyErrors;

    // Тест 10
    QStack<Token> operators_10;
    QStack<ExpressionTree*> nodes_10;
    bool processed_10 = false;

    QTest::newRow("10. Both operator and node stacks are empty") << operators_10 << operators_10 << nodes_10 << nodes_10 << processed_10 << emptyErrors;

    // Тест 11
    QStack<Token> inputOperators_11;
    inputOperators_11.push(Token("-u", 0, 0));
    QStack<Token> outputOperators_11;

    ExpressionTree* node_11_1 = new ExpressionTree("1", nullptr, nullptr, 2, 2);
    ExpressionTree* node_11_2 = new ExpressionTree("2", nullptr, nullptr, 4, 4);
    ExpressionTree* node_11 = new ExpressionTree("*", node_11_1, node_11_2, 2, 4);
    ExpressionTree* node_11_u = new ExpressionTree("-u", node_11, nullptr, 0, 4);

    QStack<ExpressionTree*> inputNodes_11;
    inputNodes_11.push(node_11);
    QStack<ExpressionTree*> outputsNodes_11;
    outputsNodes_11.push(node_11_u);
    bool processed_11 = true;

    QTest::newRow("11. Unary minus at top of operator stack with one node in node stack") << inputOperators_11 << outputOperators_11 << inputNodes_11 << outputsNodes_11 << processed_11 << emptyErrors;

    // Тест 12
    QStack<Token> operators_12;
    operators_12.push(Token("+", 3, 3));

    ExpressionTree* node_12_1 = new ExpressionTree("1", nullptr, nullptr, 8, 8);
    ExpressionTree* node_12_2 = new ExpressionTree("2", nullptr, nullptr, 10, 10);
    ExpressionTree* node_12 = new ExpressionTree("*", node_12_1, node_12_2, 8, 10);

    QStack<ExpressionTree*> nodes_12;
    nodes_12.push(node_12);
    bool processed_12 = false;

    Error error_12;
    error_12.setType(Error::Type::noRightOperand);
    error_12.setOperation("+");
    error_12.setSymbolIndex(3);

    QSet<Error> errors_12 = { error_12 };

    QTest::newRow("12. Non-unary operator at top of stack with one node in node stack") << operators_12 << operators_12 << nodes_12 << nodes_12 << processed_12 << errors_12;

    // Тест 13
    QStack<Token> operators_13;
    operators_13.push(Token("+", 3, 3));

    QStack<ExpressionTree*> nodes_13;
    bool processed_13 = false;

    Error error_13;
    error_13.setType(Error::Type::noBothOperands);
    error_13.setOperation("+");
    error_13.setSymbolIndex(3);

    QSet<Error> errors_13 = { error_13 };

    QTest::newRow("13. Non-unary operator at top of stack with empty node stack") << operators_13 << operators_13 << nodes_13 << nodes_13 << processed_13 << errors_13;

    // Тест 14
    QStack<Token> operators_14;
    operators_14.push(Token("-u", 0, 0));

    QStack<ExpressionTree*> nodes_14;
    bool processed_14 = false;

    Error error_14;
    error_14.setType(Error::Type::noRightOperand);
    error_14.setOperation("-");
    error_14.setSymbolIndex(0);

    QSet<Error> errors_14 = { error_14 };

    QTest::newRow("14. Unary minus at top of stack with empty node stack") << operators_14 << operators_14 << nodes_14 << nodes_14 << processed_14 << errors_14;
}

QString Test_ProcessOperatorFromStack::compareTokenStacks(const QStack<Token>& actual, const QStack<Token>& expected) {
    if (actual.size() != expected.size()) {
        return QString("Stack size mismatch. Expected: %1, Actual: %2")
        .arg(expected.size())
            .arg(actual.size());
    }

    QList<Token> actualList = actual.toList();
    QList<Token> expectedList = expected.toList();

    for (int i = 0; i < actualList.size(); i++) {
        if (actualList[i].value != expectedList[i].value ||
            actualList[i].start != expectedList[i].start ||
            actualList[i].end != expectedList[i].end) {
            return QString("Token mismatch at position %1:\nExpected: '%2' (%3-%4)\nActual: '%5' (%6-%7)")
            .arg(i)
                .arg(expectedList[i].value)
                .arg(expectedList[i].start)
                .arg(expectedList[i].end)
                .arg(actualList[i].value)
                .arg(actualList[i].start)
                .arg(actualList[i].end);
        }
    }

    return QString();
}

QString Test_ProcessOperatorFromStack::compareNodeStacks(const QStack<ExpressionTree*>& actual, const QStack<ExpressionTree*>& expected) {
    if (actual.size() != expected.size()) {
        return QString("Node stack size mismatch. Expected: %1, Actual: %2")
        .arg(expected.size()).arg(actual.size());
    }

    for (int i = 0; i < actual.size(); i++) {
        QString nodeDiff = compareExpressionTrees(actual[i], expected[i]);
        if (!nodeDiff.isEmpty()) {
            return QString("Node mismatch at position %1:\n%2")
            .arg(i).arg(nodeDiff);
        }
    }

    return QString();
}

QString Test_ProcessOperatorFromStack::compareExpressionTrees(ExpressionTree* actual, ExpressionTree* expected) {
    if (!actual && !expected) return QString();
    if (!actual || !expected) {
        return QString("Node existence mismatch. Expected: %1, Actual: %2")
        .arg(expected ? "exists" : "null")
            .arg(actual ? "exists" : "null");
    }

    if (actual->getValue() != expected->getValue() ||
        actual->getStart() != expected->getStart() ||
        actual->getEnd() != expected->getEnd()) {
        return QString("Node data mismatch:\nExpected: '%1' (%2-%3)\nActual: '%4' (%5-%6)")
        .arg(expected->getValue())
            .arg(expected->getStart())
            .arg(expected->getEnd())
            .arg(actual->getValue())
            .arg(actual->getStart())
            .arg(actual->getEnd());
    }

    QString leftDiff = compareExpressionTrees(actual->getLeft(), expected->getLeft());
    if (!leftDiff.isEmpty()) {
        return QString("Left child mismatch for node '%1':\n%2")
        .arg(actual->getValue()).arg(leftDiff);
    }

    QString rightDiff = compareExpressionTrees(actual->getRight(), expected->getRight());
    if (!rightDiff.isEmpty()) {
        return QString("Right child mismatch for node '%1':\n%2")
        .arg(actual->getValue()).arg(rightDiff);
    }

    return QString();
}

QString Test_ProcessOperatorFromStack::compareErrors(const QSet<Error>& actual, const QSet<Error>& expected) {
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
