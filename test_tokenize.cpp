#include "test_tokenize.h"

void Test_Tokenize::testTokenize() {
    // Получаем данные из тестовой строки
    QFETCH(QString, inputString);
    QFETCH(QList<Token>, tokens);
    QFETCH(QSet<Error>, errors);

    // Вызываем тестируемую функцию
    QSet<Error> actualErrors;
    QList<Token> actualTokens = tokenize(inputString, actualErrors);

    // Сравниваем токены
    QStringList tokenDifferences = compareTokenLists(actualTokens, tokens);
    QVERIFY2(tokenDifferences.isEmpty(),
             qPrintable(QString("Token mismatch:\n%1")
                            .arg(tokenDifferences.join("\n"))));

    // Сравниваем ошибки
    QString differences = compareErrors(errors, actualErrors);
    QVERIFY2(differences.isEmpty(), qPrintable(differences));
}

void Test_Tokenize::testTokenize_data() {
    // Столбцы
    QTest::addColumn<QString>("inputString");
    QTest::addColumn<QList<Token>>("tokens");
    QTest::addColumn<QSet<Error>>("errors");

    QSet<Error> emptyErrors = {};

    // Тест 1
    // Выражение без пробелов, скобок и унарных минусов. Числа целые.
    QString input_1 = "1+2-3";

    QList<Token> tokens_1 = {};
    tokens_1.append(Token("1", 0, 0));
    tokens_1.append(Token("+", 1, 1));
    tokens_1.append(Token("2", 2, 2));
    tokens_1.append(Token("-", 3, 3));
    tokens_1.append(Token("3", 4, 4));

    QTest::newRow("no_spaces_no_parentheses_no_unary_minus_integers") << input_1 << tokens_1 << emptyErrors;

    // Тест 2
    // Выражение с пробелами, без скобок и унарных минусов. Числа целые
    QString input_2 = "1 + 2 - 3";

    QList<Token> tokens_2 = {};
    tokens_2.append(Token("1", 0, 0));
    tokens_2.append(Token(" ", 1, 1));
    tokens_2.append(Token("2", 2, 2));
    tokens_2.append(Token(" ", 3, 3));
    tokens_2.append(Token("-", 4, 4));
    tokens_2.append(Token(" ", 5, 5));
    tokens_2.append(Token("3", 6, 6));

    QTest::newRow("with_spaces_no_parentheses_no_unary_minus_integers") << input_2 << tokens_2 << emptyErrors;

    // Тест 3
    // Выражение без пробелов, скобок и унарных минусов. Числа вещественные
    QString input_3 = "1.5+2.3-3.7";

    QList<Token> tokens_3 = {};
    tokens_3.append(Token("1.5", 0, 2));
    tokens_3.append(Token("+", 3, 3));
    tokens_3.append(Token("2.3", 4, 6));
    tokens_3.append(Token("-", 7, 7));
    tokens_3.append(Token("3.7", 8, 10));

    QTest::newRow("no_spaces_no_parentheses_no_unary_minus_floats") << input_3 << tokens_3 << emptyErrors;

    // Тест 4
    // Выражение с пробелами, без скобок и унарных минусов. Числа вещественные
    QString input_4 = "1.5 + 2.3 - 3.7";

    QList<Token> tokens_4 = {};
    tokens_4.append(Token("1.5", 0, 2));
    tokens_4.append(Token(" ", 3, 3));
    tokens_4.append(Token("+", 4, 4));
    tokens_4.append(Token(" ", 5, 5));
    tokens_4.append(Token("2.3", 6, 8));
    tokens_4.append(Token(" ", 9, 9));
    tokens_4.append(Token("-", 10, 10));
    tokens_4.append(Token(" ", 11, 11));
    tokens_4.append(Token("3.7", 12, 14));

    QTest::newRow("with_spaces_no_parentheses_no_unary_minus_floats") << input_4 << tokens_4 << emptyErrors;

    // Тест 5
    // Охват всех типов операций
    QString input_5 = "1 – 2 + 3 * 4 / 2 ^ 3";

    QList<Token> tokens_5 = {};
    tokens_5.append(Token("1", 0, 0));
    tokens_5.append(Token(" ", 1, 1));
    tokens_5.append(Token("-", 2, 2));
    tokens_5.append(Token(" ", 3, 3));
    tokens_5.append(Token("2", 4, 4));
    tokens_5.append(Token(" ", 5, 5));
    tokens_5.append(Token("+", 6, 6));
    tokens_5.append(Token(" ", 7, 7));
    tokens_5.append(Token("3", 8, 8));
    tokens_5.append(Token(" ", 9, 9));
    tokens_5.append(Token("*", 10, 10));
    tokens_5.append(Token(" ", 11, 11));
    tokens_5.append(Token("4", 12, 12));
    tokens_5.append(Token(" ", 13, 13));
    tokens_5.append(Token("/", 14, 14));
    tokens_5.append(Token(" ", 15, 15));
    tokens_5.append(Token("2", 16, 16));
    tokens_5.append(Token(" ", 17, 17));
    tokens_5.append(Token("^", 18, 18));
    tokens_5.append(Token(" ", 19, 19));
    tokens_5.append(Token("3", 20, 20));

    QTest::newRow("all_operation_types_coverage") << input_5 << tokens_5 << emptyErrors;

    // Тест 6
    // Выражение содержит корректный блок круглых скобок
    QString input_6 = "(1+2)";

    QList<Token> tokens_6 = {};
    tokens_6.append(Token("(", 0, 0));
    tokens_6.append(Token("1", 1, 1));
    tokens_6.append(Token("+", 2, 2));
    tokens_6.append(Token("2", 3, 3));
    tokens_6.append(Token(")", 4, 4));

    QTest::newRow("valid_parentheses_block") << input_6 << tokens_6 << emptyErrors;

    // Тест 7
    // Выражение содержит некорректный блок круглых скобок
    QString input_7 = ")1+2(";

    QList<Token> tokens_7 = {};
    tokens_7.append(Token(")", 0, 0));
    tokens_7.append(Token("1", 1, 1));
    tokens_7.append(Token("+", 2, 2));
    tokens_7.append(Token("2", 3, 3));
    tokens_7.append(Token("(", 4, 4));

    QTest::newRow("invalid_parentheses_block") << input_7 << tokens_7 << emptyErrors;

    // Тест 8
    // Отсутствуют операции
    QString input_8 = "4 3 4.12 5";

    QList<Token> tokens_8 = {};
    tokens_8.append(Token("4", 0, 0));
    tokens_8.append(Token(" ", 1, 1));
    tokens_8.append(Token("3", 2, 2));
    tokens_8.append(Token(" ", 3, 3));
    tokens_8.append(Token("4.12", 4, 7));
    tokens_8.append(Token(" ", 8, 8));
    tokens_8.append(Token("5", 9, 9));

    QTest::newRow("missing_operations") << input_8 << tokens_8 << emptyErrors;

    // Тест 9
    // Отсутствуют операнды и пробелы
    QString input_9 = "++--//^^**";

    QList<Token> tokens_9 = {};
    tokens_9.append(Token("+", 0, 0));
    tokens_9.append(Token("+", 1, 1));
    tokens_9.append(Token("-", 2, 2));
    tokens_9.append(Token("-", 3, 3));
    tokens_9.append(Token("/", 4, 4));
    tokens_9.append(Token("/", 5, 5));
    tokens_9.append(Token("^", 6, 6));
    tokens_9.append(Token("^", 7, 7));
    tokens_9.append(Token("*", 8, 8));
    tokens_9.append(Token("*", 9, 9));

    QTest::newRow("missing_operands_and_spaces") << input_9 << tokens_9 << emptyErrors;

    // Тест 10
    // Унарный минус с целым числом
    QString input_10 = "-42";

    QList<Token> tokens_10 = {};
    tokens_10.append(Token("-u", 0, 0));
    tokens_10.append(Token("42", 1, 2));

    QTest::newRow("unary_minus_with_integer") << input_10 << tokens_10 << emptyErrors;

    // Тест 11
    // Унарный минус с вещественным числом
    QString input_11 = "-3.14";

    QList<Token> tokens_11 = {};
    tokens_11.append(Token("-u", 0, 0));
    tokens_11.append(Token("3.14", 1, 4));

    QTest::newRow("unary_minus_with_float") << input_11 << tokens_11 << emptyErrors;

    // Тест 12
    // Унарный минус после оператора
    QString input_12 = "2*-3";

    QList<Token> tokens_12 = {};
    tokens_12.append(Token("2", 0, 0));
    tokens_12.append(Token("*", 1, 1));
    tokens_12.append(Token("-u", 2, 2));
    tokens_12.append(Token("3", 3, 3));

    QTest::newRow("unary_minus_after_operator") << input_12 << tokens_12 << emptyErrors;

    // Тест 13
    // Несколько унарных минусов подряд
    QString input_13 = "--5";

    QList<Token> tokens_13 = {};
    tokens_13.append(Token("-u", 0, 0));
    tokens_13.append(Token("-u", 1, 1));
    tokens_13.append(Token("5", 2, 2));

    QTest::newRow("multiple_consecutive_unary_minuses") << input_13 << tokens_13 << emptyErrors;

    // Тест 14
    // Унарный минус в скобках
    QString input_14 = "(-5)";

    QList<Token> tokens_14 = {};
    tokens_14.append(Token("(", 0, 0));
    tokens_14.append(Token("-u", 1, 1));
    tokens_14.append(Token("5", 2, 2));
    tokens_14.append(Token(")", 3, 3));

    QTest::newRow("unary_minus_inside_parentheses") << input_14 << tokens_14 << emptyErrors;

    // Тест 15
    // Унарный минус в начале выражения
    QString input_15 = "-1+2";

    QList<Token> tokens_15 = {};
    tokens_15.append(Token("-u", 0, 0));
    tokens_15.append(Token("1", 1, 1));
    tokens_15.append(Token("+", 2, 2));
    tokens_15.append(Token("2", 3, 3));

    QTest::newRow("unary_minus_at_expression_start") << input_15 << tokens_15 << emptyErrors;

    // Тест 16
    // Унарный минус перед скобкой
    QString input_16 = "-(1+2)";

    QList<Token> tokens_16 = {};
    tokens_16.append(Token("-u", 0, 0));
    tokens_16.append(Token("(", 1, 1));
    tokens_16.append(Token("1", 2, 2));
    tokens_16.append(Token("+", 3, 3));
    tokens_16.append(Token("2", 4, 4));
    tokens_16.append(Token(")", 5, 5));

    QTest::newRow("unary_minus_before_parenthesis") << input_16 << tokens_16 << emptyErrors;

    // Тест 17
    // Унарный минус перед числом с пробелом
    QString input_17 = "- 5";

    QList<Token> tokens_17 = {};
    tokens_17.append(Token("-u", 0, 0));
    tokens_17.append(Token(" ", 1, 1));
    tokens_17.append(Token("5", 2, 2));

    QTest::newRow("unary_minus_before_number_with_space") << input_17 << tokens_17 << emptyErrors;

    // Тест 18
    // Унарный минус после операции, отличной от минуса
    QString input_18 = "1+-2";

    QList<Token> tokens_18 = {};
    tokens_18.append(Token("1", 0, 0));
    tokens_18.append(Token("+", 1, 1));
    tokens_18.append(Token("-u", 2, 2));
    tokens_18.append(Token("2", 3, 3));

    QTest::newRow("unary_minus_after_non_minus_operator") << input_18 << tokens_18 << emptyErrors;

    // Тест 19
    // Несколько унарных минусов после операции, отличной от минуса
    QString input_19 = "1+---2";

    QList<Token> tokens_19 = {};
    tokens_19.append(Token("1", 0, 0));
    tokens_19.append(Token("+", 1, 1));
    tokens_19.append(Token("-u", 2, 2));
    tokens_19.append(Token("-u", 3, 3));
    tokens_19.append(Token("-u", 4, 4));
    tokens_19.append(Token("2", 5, 5));

    QTest::newRow("multiple_unary_minuses_after_non_minus_operator") << input_19 << tokens_19 << emptyErrors;

    // Тест 20
    // Имеется неизвестная последовательность символов
    QString input_20 = "abc + 3.14 – 3.15";

    Error error_20;
    error_20.setType(Error::Type::unknownSymbolsSequence);
    error_20.setSymbolsSequence("abc");
    QSet<Error> errors_20 = { error_20 };

    QList<Token> tokens_20 = {};

    QTest::newRow("single_unknown_symbols_sequence") << input_20 << tokens_20 << errors_20;

    // Тест 21
    // Имеется несколько неизвестных последовательностей символов
    QString input_21 = "abc + 3.14 – def";

    Error error_21_1;
    error_21_1.setType(Error::Type::unknownSymbolsSequence);
    error_21_1.setSymbolsSequence("abc");

    Error error_21_2;
    error_21_2.setType(Error::Type::unknownSymbolsSequence);
    error_21_2.setSymbolsSequence("def");

    QSet<Error> errors_21 = { error_21_1, error_21_2 };

    QList<Token> tokens_21 = {};

    QTest::newRow("multiple_unknown_symbols_sequences") << input_21 << tokens_21 << errors_21;

    // Тест 22
    // Неизвестная последовательность включает в себя корректное целое число
    QString input_22 = "ab3c + 3.14";

    Error error_22_1;
    error_22_1.setType(Error::Type::unknownSymbolsSequence);
    error_22_1.setSymbolsSequence("ab");

    Error error_22_2;
    error_22_2.setType(Error::Type::unknownSymbolsSequence);
    error_22_2.setSymbolsSequence("c");

    QSet<Error> errors_22 = { error_22_1, error_22_2 };

    QList<Token> tokens_22 = {};

    QTest::newRow("unknown_sequence_contains_valid_integer") << input_22 << tokens_22 << errors_22;

    // Тест 23
    // Корректное целое число перед неизвестной последовательностью символов
    QString input_23 = "3abc + 3.14";

    Error error_23;
    error_23.setType(Error::Type::unknownSymbolsSequence);
    error_23.setSymbolsSequence("abc");
    QSet<Error> errors_23 = { error_23 };

    QList<Token> tokens_23 = {};

    QTest::newRow("valid_integer_before_unknown_sequence") << input_23 << tokens_23 << errors_23;

    // Тест 24
    // Корректное целое число в конце неизвестной последовательности символов
    QString input_24 = "abc3 + 3.14";

    Error error_24;
    error_24.setType(Error::Type::unknownSymbolsSequence);
    error_24.setSymbolsSequence("abc");
    QSet<Error> errors_24 = { error_24 };

    QList<Token> tokens_24 = {};

    QTest::newRow("valid_integer_after_unknown_sequence") << input_24 << tokens_24 << errors_24;

    // Тест 25
    // Неизвестная последовательность символов включает в себя корректное вещественное число
    QString input_25 = "ab3.14c + 3.15";

    Error error_25_1;
    error_25_1.setType(Error::Type::unknownSymbolsSequence);
    error_25_1.setSymbolsSequence("ab");

    Error error_25_2;
    error_25_2.setType(Error::Type::unknownSymbolsSequence);
    error_25_2.setSymbolsSequence("c");

    QSet<Error> errors_25 = { error_25_1, error_25_2 };

    QList<Token> tokens_25 = {};

    QTest::newRow("unknown_sequence_contains_valid_float") << input_25 << tokens_25 << errors_25;

    // Тест 26
    // Неизвестная последовательность символов включает в себя некорректное число
    QString input_26 = "ab3...14c + 3.15";

    Error error_26;
    error_26.setType(Error::Type::unknownSymbolsSequence);
    error_26.setSymbolsSequence("ab3...14c");
    QSet<Error> errors_26 = { error_26 };

    QList<Token> tokens_26 = {};

    QTest::newRow("unknown_sequence_contains_invalid_number") << input_26 << tokens_26 << errors_26;

    // Тест 27
    // Неизвестная последовательность символов включает в себя символ поддерживаемой операции
    QString input_27 = "ab+c – 3.14";

    Error error_27_1;
    error_27_1.setType(Error::Type::unknownSymbolsSequence);
    error_27_1.setSymbolsSequence("ab");

    Error error_27_2;
    error_27_2.setType(Error::Type::unknownSymbolsSequence);
    error_27_2.setSymbolsSequence("c");

    QSet<Error> errors_27 = { error_27_1, error_27_2 };

    QList<Token> tokens_27 = {};

    QTest::newRow("unknown_sequence_contains_supported_operator") << input_27 << tokens_27 << errors_27;

    // Тест 28
    // Поддерживаемый символ операции перед неизвестной последовательностью символов
    QString input_28 = "-abc – 3.14";

    Error error_28;
    error_28.setType(Error::Type::unknownSymbolsSequence);
    error_28.setSymbolsSequence("abc");
    QSet<Error> errors_28 = { error_28 };

    QList<Token> tokens_28 = {};

    QTest::newRow("supported_operator_before_unknown_sequence") << input_28 << tokens_28 << errors_28;

    // Тест 29
    // Поддерживаемый символ операции после неизвестной последовательности символов
    QString input_29 = "abc- – 3.14";

    Error error_29;
    error_29.setType(Error::Type::unknownSymbolsSequence);
    error_29.setSymbolsSequence("abc");
    QSet<Error> errors_29 = { error_29 };

    QList<Token> tokens_29 = {};

    QTest::newRow("supported_operator_after_unknown_sequence") << input_29 << tokens_29 << errors_29;

    // Тест 30
    // В вещественном числе несколько последовательно идущих плавающих точек
    QString input_30 = "1..2 + 3.4";

    Error error_30;
    error_30.setType(Error::Type::unknownSymbolsSequence);
    error_30.setSymbolsSequence("1..2");
    QSet<Error> errors_30 = { error_30 };

    QList<Token> tokens_30 = {};

    QTest::newRow("float_with_multiple_consecutive_points") << input_30 << tokens_30 << errors_30;

    // Тест 31
    // В нескольких вещественных числах несколько последовательно идущих плавающих точек
    QString input_31 = "1..2 + 3...4";

    Error error_31_1;
    error_31_1.setType(Error::Type::unknownSymbolsSequence);
    error_31_1.setSymbolsSequence("1..2");

    Error error_31_2;
    error_31_2.setType(Error::Type::unknownSymbolsSequence);
    error_31_2.setSymbolsSequence("3...4");

    QSet<Error> errors_31 = { error_31_1, error_31_2 };

    QList<Token> tokens_31 = {};

    QTest::newRow("multiple_floats_with_consecutive_points") << input_31 << tokens_31 << errors_31;

    // Тест 32
    // В вещественном числе несколько не последовательных плавающих точек
    QString input_32 = "1.2.3 + 3.4";

    Error error_32;
    error_32.setType(Error::Type::unknownSymbolsSequence);
    error_32.setSymbolsSequence("1.2.3");
    QSet<Error> errors_32 = { error_32 };

    QList<Token> tokens_32 = {};

    QTest::newRow("float_with_non_consecutive_points") << input_32 << tokens_32 << errors_32;

    // Тест 33
    // В нескольких вещественных числах несколько не последовательных плавающих точек
    QString input_33 = "1.2.3 + 3.4.5";

    Error error_33_1;
    error_33_1.setType(Error::Type::unknownSymbolsSequence);
    error_33_1.setSymbolsSequence("1.2.3");

    Error error_33_2;
    error_33_2.setType(Error::Type::unknownSymbolsSequence);
    error_33_2.setSymbolsSequence("3.4.5");

    QSet<Error> errors_33 = { error_33_1, error_33_2 };

    QList<Token> tokens_33 = {};

    QTest::newRow("multiple_floats_with_non_consecutive_points") << input_33 << tokens_33 << errors_33;

    // Тест 34
    // В вещественном числе есть несколько последовательных и непоследовательных плавающих точек
    QString input_34 = "1.2.3...4 + 5.6";

    Error error_34;
    error_34.setType(Error::Type::unknownSymbolsSequence);
    error_34.setSymbolsSequence("1.2.3...4");
    QSet<Error> errors_34 = { error_34 };

    QList<Token> tokens_34 = {};

    QTest::newRow("float_with_mixed_point_sequences") << input_34 << tokens_34 << errors_34;

    // Тест 35
    // Отсутствует целая часть у вещественного числа
    QString input_35 = ".4 + 3.14";

    Error error_35;
    error_35.setType(Error::Type::unknownSymbolsSequence);
    error_35.setSymbolsSequence(".4");
    QSet<Error> errors_35 = { error_35 };

    QList<Token> tokens_35 = {};

    QTest::newRow("float_missing_integer_part") << input_35 << tokens_35 << errors_35;

    // Тест 36
    // Отсутствует вещественная часть у вещественного числа
    QString input_36 = "4. + 3.14";

    Error error_36;
    error_36.setType(Error::Type::unknownSymbolsSequence);
    error_36.setSymbolsSequence("4.");
    QSet<Error> errors_36 = { error_36 };

    QList<Token> tokens_36 = {};

    QTest::newRow("float_missing_fractional_part") << input_36 << tokens_36 << errors_36;

    // Тест 37
    // Имеется только одна плавающая точка
    QString input_37 = ". + 3.14";

    Error error_37;
    error_37.setType(Error::Type::unknownSymbolsSequence);
    error_37.setSymbolsSequence(".");
    QSet<Error> errors_37 = { error_37 };

    QList<Token> tokens_37 = {};

    QTest::newRow("single_floating_point_only") << input_37 << tokens_37 << errors_37;
}

QStringList Test_Tokenize::compareTokenLists(const QList<Token>& actual, const QList<Token>& expected) {
    QStringList differences;

    if (actual.size() != expected.size()) {
        differences.append(QString("Token list size mismatch. Actual: %1, Expected: %2")
                               .arg(actual.size())
                               .arg(expected.size()));
        return differences;
    }

    for (int i = 0; i < actual.size(); i++) {
        const Token actualToken = actual[i];
        const Token expectedToken = expected[i];

        if (actualToken.value != expectedToken.value) {
            differences.append(QString("Token #%1: Value mismatch (Actual: '%2', Expected: '%3')")
                                   .arg(i)
                                   .arg(actualToken.value)
                                   .arg(expectedToken.value));
        }
        if (actualToken.start != expectedToken.start) {
            differences.append(QString("Token #%1: Start position mismatch (Actual: %2, Expected: %3)")
                                   .arg(i)
                                   .arg(actualToken.start)
                                   .arg(expectedToken.start));
        }
        if (actualToken.end != expectedToken.end) {
            differences.append(QString("Token #%1: End position mismatch (Actual: %2, Expected: %3)")
                                   .arg(i)
                                   .arg(actualToken.end)
                                   .arg(expectedToken.end));
        }
    }

    return differences;
}

QString Test_Tokenize::compareErrors(const QSet<Error>& actual, const QSet<Error>& expected) {
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
