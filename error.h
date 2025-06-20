#ifndef ERROR_H
#define ERROR_H

#include <QString>
#include <QHash>

class Error
{
public:
    enum Type {
        inputFileWay,
        outputFileWay,
        countStrings,
        unknownSymbolsSequence,
        divisionByZero,
        noLeftOperand,
        noRightOperand,
        noBothOperands,
        noClosingParenthesis,
        noOpeningParenthesis,
        noCalcOperation,
        incorrectRoot,
        operandOutOfRange
    };

    void setType(Type type);
    void setSymbolsSequence(const QString& symbolsSequence);
    void setExpression(const QString& expression);
    void setLeftOperand(const QString& leftOperand);
    void setRightOperand(const QString& rightOperand);
    void setSymbolIndex(int symbolIndex);
    void setOperation(const QString& operation);

    QString message() const;
    bool operator==(const Error& other) const;

    friend uint qHash(const Error& error, uint seed = 0) {
        uint hash = qHash(error.type, seed);
        switch (error.type) {
        case Error::Type::unknownSymbolsSequence:
            hash ^= qHash(error.symbolsSequence, seed);
            break;
        case Error::Type::divisionByZero:
        case Error::Type::incorrectRoot:
            hash ^= qHash(error.expression, seed);
            break;
        case Error::Type::noLeftOperand:
        case Error::Type::noRightOperand:
        case Error::Type::noBothOperands:
            hash ^= qHash(error.operation, seed) ^ qHash(error.indexSymbol, seed);
            break;
        case Error::Type::noClosingParenthesis:
        case Error::Type::noOpeningParenthesis:
            hash ^= qHash(error.indexSymbol, seed);
            break;
        case Error::Type::noCalcOperation:
            hash ^= qHash(error.leftOperand, seed) ^ qHash(error.rightOperand, seed);
            break;
        case Error::Type::inputFileWay:
        case Error::Type::outputFileWay:
        case Error::Type::countStrings:
        case Error::Type::operandOutOfRange:
        default:
            // Для этих типов достаточно type
            break;
        }
        return hash;
    }

private:
    Type type;
    QString symbolsSequence;
    QString expression;
    QString leftOperand;
    QString rightOperand;
    int indexSymbol;
    QString operation;
};

#endif // ERROR_H
