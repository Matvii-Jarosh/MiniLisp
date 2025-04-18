TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        environment.cpp \
        evaluator.cpp \
        lambda.cpp \
        listobject.cpp \
        main.cpp \
        primitive.cpp \
        tokenstream.cpp \
        utils.cpp \
        value.cpp

DISTFILES += \
    LICENSE \
    doc/basic_check.md \
    doc/basic_primitive.md \
    doc/math.md \
    doc/system.md \
    doc/type_check.md \
    run.bat

HEADERS += \
    environment.h \
    evaluator.h \
    lambda.h \
    listobject.h \
    primitive.h \
    tokenstream.h \
    utils.h \
    value.h
