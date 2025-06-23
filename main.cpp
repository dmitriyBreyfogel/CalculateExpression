#include "test_tokenize.h"
#include "test_build.h"
#include "test_calculate.h"
#include "test_processoperatorfromstack.h"
#include "test_determinenodetype.h"
#include "calculationtools.h"
#include <QCoreApplication>

/*!
 * \brief Главная функция программы CalculateExpression
 * \param [in] argc - количество переданных аргументов командной строки
 * \param [in] argv - переданные аргументы командной строки
 * \param [in] argv[0] - аргумент запуска программы
 * \param [in] argv[1] - путь к входному файлу (.txt) с выражением (или запрос запуска тестов)
 * \param [in] argv[2] - путь к выходному файлу (.txt) для вывода результата работы программы
 * \return 0 - программа завершилась успешно; 1 - была найдена ошибка
 */
int main(int argc, char *argv[])
{
    /*
    Test_Tokenize test_tokenize;
    QTest::qExec(&test_tokenize);

    Test_Build test_build;
    QTest::qExec(&test_build);

    Test_Calculate test_calculate;
    QTest::qExec(&test_calculate);

    Test_ProcessOperatorFromStack test_processOperatorFromStack;
    QTest::qExec(&test_processOperatorFromStack);

    Test_DetermineNodeType test_determineNodeType;
    QTest::qExec(&test_determineNodeType);*/

    QSet<Error> errors;
    QString input = "(((1 + 2)(3 + 4 * (5 - 6))))";
    double result = 0;

    try {
        QList<Token> tokens = tokenize(input, errors);

        ExpressionTree* node;
        node = ExpressionTree::build(tokens, errors);


        result = node->calculate(errors);
    }
    catch (QSet<Error>& errors) {
        if (!errors.isEmpty()) {
            for (Error error : errors) {
                qDebug() << error.message();
            }
            return 0;
        }
    }

    qDebug() << result;
}
