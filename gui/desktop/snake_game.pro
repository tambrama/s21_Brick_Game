# snake_game.pro

TARGET = snake_game_qt
DESTDIR = ../../build//bin
BUILD_DIR = ../../build/snake

OBJECTS_DIR = $$BUILD_DIR/snake/obj
MOC_DIR     = $$BUILD_DIR/snake/moc
UI_DIR      = $$BUILD_DIR/snake/ui
RCC_DIR     = $$BUILD_DIR/snake/rcc

TEMPLATE = app
QT += widgets
CONFIG += c++20
QMAKE_CXX_STANDARD = 20
QMAKE_CXXFLAGS += -Wall -Wextra -std=c++20


# Включаем автоматические инструменты Qt
CONFIG += qt warn_on

# Источники
SOURCES = \
    $$PWD/main.cc \
    $$PWD/mainwindow.cc \
    $$PWD/view.cc \
    $$PWD/../../brick_game/snake/api.cc \
    $$PWD/../../brick_game/snake/snake_fsm.cc \
    $$PWD/../../brick_game/snake/snake_model.cc \
    $$PWD/../../brick_game/controller/controller.cc

HEADERS = \
    $$PWD/mainwindow.h \
    $$PWD/view.h \
    $$PWD/../../brick_game/snake/snake_fsm.h \
    $$PWD/../../brick_game/snake/snake.h \
    $$PWD/../../brick_game/snake/snake_wrapper.h \
    $$PWD/../../brick_game/brick_games.h \
    $$PWD/../../brick_game/controller/controller.h


RESOURCES += $$PWD/resources.qrc

# Include пути
INCLUDEPATH += $$INCLUDEPATH_COMMON \
    $$PWD/../../brick_game/snake

# Макрос для сборки
DEFINES += SNAKE_GAME

# Дополнительно: статическая сборка (опционально)
  #QMAKE_LFLAGS += -static