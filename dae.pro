TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    myvertex.cpp \
    output.cpp \
    geom.cpp

HEADERS += \
    lib/rapidxml.hpp \
    lib/SimpleCOLLADA.hpp \
    myvertex.h \
    output.h \
    geom.h
