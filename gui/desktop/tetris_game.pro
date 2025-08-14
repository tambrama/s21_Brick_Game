# tetris_game.pro

TARGET = tetris_game_qt
DESTDIR = ../../build//bin
BUILD_DIR = ../../build/tetris

OBJECTS_DIR = $$BUILD_DIR/tetris/obj
MOC_DIR     = $$BUILD_DIR/tetris/moc
UI_DIR      = $$BUILD_DIR/tetris/ui
RCC_DIR     = $$BUILD_DIR/tetris/rcc

TEMPLATE = app
QT += widgets
CONFIG += c++20
QMAKE_CXX_STANDARD = 20
QMAKE_CXXFLAGS += -Wall -Wextra -std=c++20

CONFIG += qt warn_on


SOURCES = \
    main.cc \
    mainwindow.cc \
    view.cc \
    ../../brick_game/tetris/backend.c \
    ../../brick_game/tetris/fsm.c \
     ../../brick_game/controller/controller.cc

HEADERS = \
    mainwindow.h \
    view.h \
    ../../brick_game/tetris/backend.h \
    ../../brick_game/tetris/fsm.h \
     ../../brick_game/controller/controller.h


RESOURCES += $$PWD/resources.qrc

INCLUDEPATH += $$INCLUDEPATH_COMMON \
    $$BRICK_GAME_DIR/tetris

DEFINES += TETRIS_GAME

 # QMAKE_LFLAGS += -static