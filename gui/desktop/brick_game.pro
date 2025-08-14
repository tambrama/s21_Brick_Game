# brick_game.pro — главный проект

TEMPLATE = subdirs
CONFIG += ordered

# Путь к исходникам игр
BRICK_GAME_DIR = $$PWD/../../brick_game

# Корневая папка для всех артефактов
BUILD_DIR = $$PWD/build

# Устанавливаем выходные директории
BIN_DIR = $$OUT_PWD/bin
LIB_DIR = $$OUT_PWD/lib

export(BRICK_GAME_DIR)
export(BUILD_DIR)

# Список подпроектов
SUBDIRS =

# Условия сборки
build_snake {
    SUBDIRS += snake_game.pro
}

build_tetris {
    SUBDIRS += tetris_game.pro
}

# по умолчанию
isEmpty(SUBDIRS) {
    SUBDIRS += snake_game.pro tetris_game.pro
}

# Передаём переменные в подпроекты
CONFIG += ordered
QMAKE_SUBDIRS += $$SUBDIRS

# Общие переменные для подпроектов
DEFINES_COMMON = \
    QT_DEPRECATED_WARNINGS \
    QT_USE_QSTRINGBUILDER

INCLUDEPATH_COMMON = \
    $$PWD \
    $$BRICK_GAME_DIR \
    $$BRICK_GAME_DIR/controller

# Можно добавить в каждый подпроект