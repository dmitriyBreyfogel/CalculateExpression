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

private:
    Type type;
    QString symbolsSequence;
    QString expression;
    QString leftOperand;
    QString rightOperand;
    int indexSymbol;
    QString operation;
};

// Функция хеширования для QSet
inline uint qHash(const Error &error, uint seed = 0) {
    return qHash(error.message(), seed);
}

#endif // ERROR_H
