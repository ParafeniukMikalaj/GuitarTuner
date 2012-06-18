#-------------------------------------------------
#
# Project created by QtCreator 2012-05-27T23:32:19
#
#-------------------------------------------------

QT       += core gui

TARGET = GuitarTuner
TEMPLATE = app

LIBS += -L"$$_PRO_FILE_PWD_/libs/" -lportaudio -lasound

SOURCES += main.cpp\
        mainwindow.cpp \
    noteanalyzer.cpp \
    fftransformer.cpp \
    audiowrapper.cpp \
    logicproxy.cpp \
    workerthreadin.cpp \
    workerthreadout.cpp

HEADERS  += mainwindow.h \
    portaudio.h \
    noteanalyzer.h \
    logicproxy.h \
    fftransformer.h \
    audiowrapper.h \
    workerthreadin.h \
    workerthreadout.h

FORMS    += mainwindow.ui

RESOURCES += \
    appres.qrc

unix:!macx:!symbian: LIBS += -L$$PWD/../../../../../usr/local/qwt-6.0.1/lib/ -lqwt

INCLUDEPATH += $$PWD/../../../../../usr/local/qwt-6.0.1/include
DEPENDPATH += $$PWD/../../../../../usr/local/qwt-6.0.1/include
