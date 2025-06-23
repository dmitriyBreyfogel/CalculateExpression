/*!
* \file
* \brief Данный файл содержит главную функцию программы CalculateExpression.
*
* \mainpage Документация для программы "CalculateExpression"
Программа предназначена для вычисления математических выражений.
Для функционирования программы необходима операционная система Windows 7 или выше.
Программа разработана на языке С++ с использованием стандартных библиотек C++, библиотеки Qt и фреймворка Qt Creator.
Программа должна получать два аргумента командной строки: имя входного файла и имя выходного файла в формате (.txt),
или один аргумент командной строки для запуска модульных тестов: (--test).

Пример команды запуска программы:
* \code
./CalculateExpression.exe ./input.txt ./output.txt
* \endcode

* \author Брейфогель Дмитрий
* \date 23 Июня 2025
* \version 1.0
*/

#include <QCoreApplication>
#include "calculationtools.h"
#include "testrunner.h"

/*!
 * \brief Главная функция программы CalculateExpression
 * \param [in] argc - количество переданных аргументов командной строки
 * \param [in] argv - переданные аргументы командной строки
 * \param [in] argv[0] - аргумент запуска программы
 * \param [in] argv[1] - путь к входному файлу (.txt) с выражением (или запрос запуска тестов)
 * \param [in] argv[2] - путь к выходному файлу (.txt) для вывода результата работы программы
 * \return 0 - программа завершилась успешно; 1 - программа завершилась аварийно (с ошибкой)
 */
int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "Russian");

    QCoreApplication a(argc, argv);

    // Проверка аргументов командной строки
    if (argc == 2 && QString(argv[1]) == "--test") {   // Если передан запрос запуска тестов
        runTests(); // Запускаем тесты
        return 0;   // Программа завершена успешна
    }
    else if (argc != 3) {   // Иначе если аргументов не три
        qDebug() << "Программа должна принимать три аргумента: <путь к программе> <путь к входному файлу> <путь к выходному файлу>";
        return 1;   // Программа завершена с ошибкой
    }

    // Чтение файла
    QString expression;        // Строка с выражением
    QSet<Error> errors;        // Множество ошибок

    // Обработка чтения
    if (!readInputFile(argv[1], expression, errors)) { // Если прочитать не удалось
        // Обрабатываем ошибки
        for (const Error& err : errors) {   // Для каждой ошибки
            qDebug() << err.message();  // Выводим сообщение
        }
        return 1;   // Программа завершена с ошибкой
    }

    // Создание дерева выражения
    ExpressionTree* root = nullptr; // Считаем, что корня искомого дерева нет
    try {
        QList<Token> tokens = tokenize(expression, errors); // Разбиваем строку выражения на токены
        root = ExpressionTree::build(tokens, errors);   // Строим дерево выражения
    }
    catch (QSet<Error>& errors) {   // Если были выброшены исключения
        // Обрабатываем ошибки
        for (const Error& err : errors) {   // Для каждой ошибки
            qDebug() << err.message();  // Выводим сообщение
        }
        return 1;   // Программа завершена с ошибкой
    }

    // Вычисление результата
    double result = 0;
    try {
        result = root->calculate(errors);   // Считаем выражение
    }
    catch (QSet<Error>& errors) {   // Если были выброшены исключения
        // Обрабатываем ошибки
        for (const Error& err : errors) {   // Для каждой ошибки
            qDebug() << err.message();  // Выводим сообщение
        }
        delete root;    // Очищаем память
        return 1;   // Программа завершена с ошибкой
    }

    // Обработка ошибок вычисления
    if (!errors.isEmpty()) {    // Если есть ошибки
        // Обрабатываешь ошибки
        for (const Error& err : errors) {   // Для каждой ошибки
            qDebug() << err.message();  // Выводим сообщение
        }
        delete root;    // Очищаем память
        return 1;   // Программа завершена с ошибкой
    }

    QString outputContent = QString::number(result);    // Формирование строки результата

    // Запись в выходной файл
    if (!writeOutputFile(argv[2], outputContent, errors)) { // Если запись не удалась
        // Обрабатываем ошибки
        for (const Error& err : errors) {   // Для каждой ошибки
            qDebug() << err.message();  // Выводим сообщение
        }
        delete root;    // Очищаем память
        return 1;   // Программа завешена с ошибкой
    }

    delete root;    // Очищаем память
    return 0;   // Программа завершена успешно
}
