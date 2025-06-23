QT = core testlib

CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        calculationtools.cpp \
        error.cpp \
        expressiontree.cpp \
        main.cpp \
        test_build.cpp \
        test_calculate.cpp \
        test_determinenodetype.cpp \
        test_processoperatorfromstack.cpp \
        test_tokenize.cpp \
        testrunner.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    calculationtools.h \
    error.h \
    expressiontree.h \
    test_build.h \
    test_calculate.h \
    test_determinenodetype.h \
    test_processoperatorfromstack.h \
    test_tokenize.h \
    testrunner.h
