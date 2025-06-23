/*!
* \file
* \brief Реализация функций, использующихся в ходе работы программы CalculateExpression.
*/

#include "calculationtools.h"
#include "expressiontree.h"

bool readInputFile(const QString& filePath, QString& expression, QSet<Error>& errors) {
    QFile inputFile(filePath);

    // Если открыть не удалось
    if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // Обрабатываем ошибку
        Error error;
        error.setType(Error::Type::inputFileWay);
        errors.insert(error);
        return false;   // Считать не удалось
    }

    QTextStream in(&inputFile);
    QStringList lines;

    // Прочитать строку из файла
    while (!in.atEnd()) {
        lines << in.readLine();
    }

    // Если в файле больше одной строки
    if (lines.size() > 1) {
        // Обрабатываем ошибку
        Error error;
        error.setType(Error::Type::countStrings);
        errors.insert(error);
        inputFile.close();  // Закрываем файл
        return false;   // Считать не удалось
    }

    inputFile.close();  // Закрываем файл

    // Если файл не пустой
    if (!lines.isEmpty()) {
        expression = lines.first(); // Получаем строку
        return true;    // Считать удалось
    }

    return false;   // Считать не удалось
}

bool TokenizerUtils::isMathOperator(const QChar& symbol) {
    return ExpressionTree::operatorsPrecedence.contains(QString(symbol));
}

bool TokenizerUtils::isInvalidMathSymbol(const QChar& symbol) {
    return !isMathOperator(symbol) && symbol != ' ' &&
           symbol != '.' && !symbol.isDigit();
}

bool TokenizerUtils::isWellFormedNumber(const QString& number) {
    // Считаем точки
    int pointCount = number.count('.');

    // Если в числе больше одной точки
    if (pointCount > 1) return false;   // Число невалидно

    // Если в числе только одна точка
    if (pointCount == 1) {
        // Проверяем наличие целой и вещественной части числа
        QStringList parts = number.split('.');
        // Если нет хотя бы одной, то число невалидно
        return !parts[0].isEmpty() && !parts[1].isEmpty();
    }
    return true;    // Число валидно
}

Token TokenizerUtils::extractSpaces(const QString& expression, int& index) {
    int start = index;  // Запоминаем стартовый индекс последовательности
    QString buffer;     // Создаём строку для накопления пробелов
    buffer.append(expression[index]);   // Добавляем текущий пробел к строке накопления

    // Собираем последовательность пробелов
    while (++index < expression.size() && expression[index] == ' ') {   // Пока пробелы идут последовательно
        buffer.append(expression[index]);   // Добавляем пробел к строке накопления
    }

    index--;    // Корректируем индекс (указывает на индекс последнего символа в собранной последовательности)
    return Token(buffer, start, index);     // Формируем и возвращаем токен
}

Token TokenizerUtils::extractNumberLiteral(const QString& expression, int& index) {
    int start = index;  // Запоминаем стартовый индекс последовательности
    QString buffer;     // Создаём строку для накопления цифр и точек
    buffer.append(expression[index]);   // Добавляем текущий символ к строке накопления

    // Собираем последовательность цифр и точек
    while (++index < expression.size()) {   // Пока есть следующие символы в строке
        QChar symbol = expression[index];
        if (symbol.isDigit() || symbol == '.') {    // Если символ - цифра или точка
            buffer.append(symbol);  // Добавляем символ к строке накопления
        }
        else {  // Иначе
            break;  // Сборка последовательности закончена
        }
    }

    index--;    // Корректируем индекс (указывает на индекс последнего символа в собранной последовательности)
    return Token(buffer, start, index);     // Формируем и возвращаем токен
}

bool TokenizerUtils::isUnaryMinus(const QString& expression, int index) {
    // Если минус в начале выражения
    if (index == 0) return true;    // Это унарный минус

    int prevIndex = index - 1;  // Получаем индекс символа перед минусом
    // Пропускаем незначащие пробелы между минусом и первым значащим символом перед ним
    while (prevIndex >= 0 && expression[prevIndex] == ' ') prevIndex--;

    // Если минус - первый значащий символ в выражении
    if (prevIndex < 0) return true; // Это унарный минус

    QChar prevChar = expression[prevIndex];
    // Если перед минусом - оператор и не закрывающая скобка, то минус - унарный, иначе - бинарный
    return isMathOperator(prevChar) && prevChar != ')';
}

Token TokenizerUtils::extractInvalidSymbols(const QString& expression, int& index) {
    int start = index;  // Запоминаем стартовый индекс последовательности
    QString buffer;     // Создаём строку для накопления неизвестных символов
    buffer.append(expression[index]);   // Добавляем текущий символ к строке накопления

    // Собираем последовательность неизвестных символов
    while (++index < expression.size()) {   // Пока есть следующие символы в строке
        QChar symbol = expression[index];
        if (isInvalidMathSymbol(symbol)) {  // Если символ является неизвестным
            buffer.append(symbol);  // Добавляем символ к строке накопления
        }
        else {  // Иначе
            break;  // Сборка последовательности закончена
        }
    }

    index--;    // Корректируем индекс (указывает на индекс последнего символа в собранной последовательности)
    return Token(buffer, start, index); // Формируем и возвращаем токен
}

Token TokenizerUtils::collectInvalidToken(const QString& expression, int& index, const QString& initialValue, int initialStart) {
    QString invalidSequence = initialValue; // Запоминаем начальную невалидную последовательность символов
    int start = initialStart;   // Запоминаем стартовый индекс начальной невалидной последовательности
    int end = index;    // Создаём индекс конца полной невалидной последовательности

    // Собираем полную невалидную последовательность
    while (index + 1 < expression.size()) { // Пока есть следующие символы в строке
        QChar nextSymbol = expression[index + 1];   // Получаем следующий символ
        if (isMathOperator(nextSymbol) || nextSymbol.isSpace()) {   // Если следующий символ - оператор или пробел
            break;  // Сборка невалидной последовательности закончена
        }
        else if (isInvalidMathSymbol(nextSymbol)) { // Если следующий символ - невалидный
            index++;    // Переходим к нему
            Token unknownToken = extractInvalidSymbols(expression, index);  // Собираем последовательность невалидных символов
            invalidSequence += unknownToken.value;  // Добавляем к невалидной последовательности новую полученную
            end = unknownToken.end; // Запоминаем новый конечный индекс невалидной последовательности
        }
        else if (nextSymbol.isDigit() || nextSymbol == '.') {   // Если следующий символ - число или точка
            index++;    // Переходим к нему
            Token nextNumberToken = extractNumberLiteral(expression, index);   // Собираем последовательность цифр и точек
            if (isWellFormedNumber(nextNumberToken.value)) {    // Если данная последовательность - валидное число
                break;  // Сборка невалидной последовательности закончена
            }
            else {  // Иначе
                invalidSequence += nextNumberToken.value;   // Добавляем к невалидной последовательности новую полученную
                end = nextNumberToken.end;  // Запоминаем новый конечный индекс невалидной последовательности
            }
        }
        else {  // Иначе
            break;  // Сборка невалидной последовательности закончена
        }
    }

    return Token(invalidSequence, start, end);  // Возвращаем сформированный невалидный токен
}

QList<Token> tokenize(const QString& expression, QSet<Error>& errors) {
    QList<Token> validTokens;   // Считаем, что валидных токенов нет
    QList<Token> invalidTokens; // Считаем, что невалидных токенов нет
    using namespace TokenizerUtils;

    // Разбиваем строку на токены
    for (int i = 0; i < expression.size(); i++) {   // Для каждого символа в строке
        QChar symbol = expression[i];   // Получаем текущий символ

        if (symbol.isSpace()) { // Если символ - пробел
            validTokens.append(extractSpaces(expression, i));   // Добавляем токен последовательности пробелов и добавляем к валидным
            continue;   // Переходим к следующей итерации цикла
        }
        else if (isMathOperator(symbol)) {  // Если символ - оператор
            if (symbol == '-') {    // Если символ - минус
                bool unary = isUnaryMinus(expression, i);   // Проверяем, является ли минус унарным
                if (unary)  // Если минус унарный
                    validTokens.append(Token("-u", i, i));  // Добавляем валидный токен с унарным минусом
                else    // Иначе
                    validTokens.append(Token("-", i, i));   // Добавляем валидный токен с бинарным минусом
            }
            else    // Иначе
                validTokens.append(Token(symbol, i, i));    // Добавляем валидный токен с текущим символом операции
            continue;   // Переходим к следующей итерации
        }
        else if (symbol.isDigit()) {    // Если символ - число
            Token numberToken = extractNumberLiteral(expression, i);    // Формируем токен последовательности цифр и точек
            // Проверяем число на валидность
            if (isWellFormedNumber(numberToken.value))  // Если число валидное
                validTokens.append(numberToken);    // Добавляем валидный токен с данным числом
            else    // Иначе
                invalidTokens.append(collectInvalidToken(expression, i, numberToken.value, numberToken.start)); // Добавляем невалидный токен, предварительно собрав его полноценно
            continue;   // Переходим к следующей итерации
        }
        else if (symbol == '.') {   // Если символ - точка
            Token dotToken = extractNumberLiteral(expression, i);   // Формируем токен последовательности цифр и точек
            invalidTokens.append(collectInvalidToken(expression, i, dotToken.value, dotToken.start));   // Добавляем невалидный токен, предварительно собрав его полноценно
            continue;   // Переходим к следующей итерации
        }
        else {  // Иначе
            Token unknownToken = extractInvalidSymbols(expression, i);  // Формируем токен неизвестной последовательности символов
            invalidTokens.append(collectInvalidToken(expression, i, unknownToken.value, unknownToken.start));   // Добавляем невалидный токен, предварительно собрав его полноценно
            continue;   // Переходим к следующей итерации
        }
    }

    // Формируем ошибки невалидных токенов
    for (Token& token : invalidTokens) {    // Для каждого невалидного токена
        // Обрабатываем ошибку
        Error error;
        error.setType(Error::Type::unknownSymbolsSequence);
        error.setSymbolsSequence(token.value);
        errors.insert(error);
    }

    if (!errors.isEmpty()) {    // Если список ошибок не пустой
        throw errors;   // throw список ошибок
    }
    else {  // Иначе
        return validTokens; // Возвращаем список валидных токенов
    }
}

bool writeOutputFile(const QString& filePath, QString& content, QSet<Error>& errors) {
    QFile outputFile(filePath);

    // Если не удалось открыть файл
    if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        // Обрабатываем ошибку
        Error error;
        error.setType(Error::Type::outputFileWay);
        errors.insert(error);
        return false;   // Записать не удалось
    }

    // Записываем результат в файл
    QTextStream out(&outputFile);
    out << content;
    outputFile.close(); // Закрываем файл

    return true;    // Записать удалось
}
