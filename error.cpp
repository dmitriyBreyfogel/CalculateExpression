#include "error.h"

void Error::setType(Type type) {
    this->type = type;
}

void Error::setSymbolsSequence(const QString& symbolsSequence) {
    this->symbolsSequence = symbolsSequence;
}

void Error::setExpression(const QString& expression) {
    this->expression = expression;
}

void Error::setLeftOperand(const QString& leftOperand) {
    this->leftOperand = leftOperand;
}

void Error::setRightOperand(const QString& rightOperand) {
    this->rightOperand = rightOperand;
}

void Error::setSymbolIndex(int indexSymbol) {
    this->indexSymbol = indexSymbol;
}

void Error::setOperation(const QString& operation) {
    this->operation = operation;
}

QString Error::message() const {
    switch (type) {
    case Type::inputFileWay:
        return "Неверно указан файл с входными данными. Возможно, файл не существует";
    case Type::outputFileWay:
        return "Неверно указан файл для выходных данных. Возможно, указанного расположения не существует или нет прав на запись";
    case Type::countStrings:
        return "Программа принимает на вход файлы из одной строки. Разместите каждую строчку в отдельном файле";
    case Type::unknownSymbolsSequence:
        return "Неизвестная последовательность символов: «" + symbolsSequence + "»";
    case Type::divisionByZero:
        return "Деление на ноль невозможно, выражение: «" + expression + "»";
    case Type::noLeftOperand:
        return "Отсутствует левый операнд у операции \"" + operation + "\" на позиции " + QString::number(indexSymbol);
    case Type::noRightOperand:
        return "Отсутствует правый операнд у операции \"" + operation + "\" на позиции " + QString::number(indexSymbol);
    case Type::noBothOperands:
        return "Отсутствуют операнды у операции \"" + operation + "\" на позиции " + QString::number(indexSymbol);
    case Type::noClosingParenthesis:
        return "Отсутствует закрывающая скобка к открывающей на позиции " + QString::number(indexSymbol);
    case Type::noOpeningParenthesis:
        return "Отсутствует открывающая скобка к закрывающей на позиции " + QString::number(indexSymbol);
    case Type::noCalcOperation:
        return "Отсутствует оператор вычисления между операндами: " + leftOperand + " и " + rightOperand;
    case Type::incorrectRoot:
        return "Невозможно вычислить выражение: " + expression + ". Извлекается степень из отрицательного числа, которая является правильной дробью";
    case Type::operandOutOfRange:
        return "Один из операндов выходит за пределы обрабатываемых чисел. Диапазон обрабатываемых чисел: [-1.7E308..1.7E308]";
    default:
        return "Неизвестная ошибка";
    }
}

bool Error::operator==(const Error& other) const {
    return type == other.type && symbolsSequence == other.symbolsSequence &&
            expression == other.expression && leftOperand == other.leftOperand &&
            rightOperand == other.rightOperand && indexSymbol == other.indexSymbol &&
           operation == other.operation;
}
