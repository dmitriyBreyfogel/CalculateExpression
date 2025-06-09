#ifndef CALCULATIONTOOLS_H
#define CALCULATIONTOOLS_H

#include <QString>
#include "error.h"

struct Token {
    QString value;
    int start;
    int end;

    Token() {
        value = "";
        start = -1;
        end = -1;
    }

    Token(QString value, int start, int end) {
        this->value = value;
        this->start = start;
        this->end = end;
    }
};

bool readInputFile(const QString& filePath, QString& expression, QSet<Error>& errors);
QList<Token> tokenize(const QString& expression, QSet<Error>& errors);
bool writeOutputFile(const QString& filePath, QString& content, QSet<Error>& errors);

#endif // CALCULATIONTOOLS_H
