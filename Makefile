CXX = g++ -g
CC = gcc
CXXFLAGS = -std=c++20 -Wall -Wextra -Iinclude -fPIC
CFLAGS = -Wall -Werror -Wextra -std=c11 -fPIC
LNCURSES = -lncurses
COVERAGE_FLAGS := -fprofile-arcs -ftest-coverage
QT_LIB := -lQt5Widgets -lQt5Gui -lQt5Core
RPATH_FLAG = -Wl,-rpath,'$$ORIGIN/../lib'
# Исходные файлы для змейки
SNAKE_SRC = \
		brick_game/snake/snake_model.cc \
		brick_game/snake/api.cc \
		brick_game/snake/snake_fsm.cc

# Исходные файлы для тетриса
TETRIS_SRC = \
    brick_game/tetris/backend.c\
    brick_game/tetris/fsm.c

MINE_NCURSES = gui/cli/main.c

# Фронтенд
FRONTEND_SRC = gui/cli/frontend.c
GUI_SOURCE_DIR = gui/desktop/mainwindow.cc \
        gui/desktop/view.cc \
        gui/desktop/main.cc

# Объектные файлы
SNAKE_OBJ = $(SNAKE_SRC:.cc=.o)
TETRIS_OBJ = $(TETRIS_SRC:.c=.o)

FRONTEND_OBJ = $(FRONTEND_SRC:.c=.o)

# Директории сборки
BUILD_DIR = build
LIB_DIR = $(BUILD_DIR)/lib
BIN_DIR = $(BUILD_DIR)/bin
TETRIS_DIR := $(BIN_DIR)/tetris
SNAKE_DIR := $(BIN_DIR)/snake

# Имена библиотек и исполняемых файлов
SNAKE_LIB = $(LIB_DIR)/libsnake.so
TETRIS_LIB = $(LIB_DIR)/libtetris.so
SNAKE_EXE = $(BIN_DIR)/snake_game_cli
TETRIS_EXE = $(BIN_DIR)/tetris_game_cli

###############################################################################
# Основные цели
###############################################################################
.PHONY: all snake tetris clean install uninstall test brick_game

all: install

# Установка
install: tetris-ncurses snake-ncurses tetris-qt snake-qt
	@echo "Installed to $(BIN_DIR)"

# Удаление
uninstall:
	rm -rf $(BIN_DIR)

# Очистка
clean:
	find . -name '*.a' -delete
	find . -name '*.o' -delete
	find . -name '*.gcda' -delete
	find . -name '*.gcno' -delete
	find . -name '*.info' -delete
	find . -name '*.gz' -delete
	rm -rf docs *.txt $(BUILD_DIR)

# Правило для сборки объектных файлов
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Документация
dvi:
	mkdir -p docs
	doxygen Doxyfile 2>/dev/null || echo "Install doxygen to generate documentation"
	@echo "Generating LaTeX documentation..."
	@if [ -d "docs/latex" ] && [ -f "docs/latex/Makefile" ]; then \
        cd docs/latex && \
        make clean && \
        make pdfsilent 2>/dev/null || \
        (make 2>/dev/null && echo "PDF generated with warnings") || \
        echo "LaTeX tools not found - install texlive-latex-recommended texlive-latex-extra"; \
        if [ -f "refman.pdf" ]; then \
            echo "PDF documentation generated: docs/latex/refman.pdf"; \
        else \
            echo "PDF generation failed - LaTeX may not be installed"; \
        fi; \
	else \
        echo "LaTeX documentation not generated - check Doxyfile settings"; \
	fi
	@echo "Documentation generation complete!"
	@echo "HTML: docs/html/index.html"
	@echo "PDF:  docs/latex/refman.pdf (if LaTeX is installed)"
# Архив с проектом
dist: clean
	mkdir -p brickgame_dist
	cp -r Makefile Doxyfile test brick_game gui brickgame_dist/
	tar -czvf brickgame_dist.tar.gz brickgame_dist
	rm -rf brickgame_dist
###############################################################################
# Сборки игр
###############################################################################
# Сборка библиотеки змейки
$(SNAKE_LIB): $(SNAKE_OBJ)
	@mkdir -p $(LIB_DIR)
	$(CXX) -shared $^ -o $@

# Сборка библиотеки тетриса
$(TETRIS_LIB): $(TETRIS_OBJ)
	@mkdir -p $(LIB_DIR)
	$(CC) -shared $^ -o $@

snake.o: $(MINE_NCURSES)
	$(CC) $(CFLAGS) -DSNAKE_GAME -c $< -o $@

tetris.o: $(MINE_NCURSES)
	$(CC) $(CFLAGS) -DTETRIS_GAME -c $< -o $@

# Сборка исполняемого файла змейки
$(SNAKE_EXE): $(SNAKE_LIB) snake.o $(FRONTEND_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -DSNAKE_GAME snake.o $(FRONTEND_OBJ) -L$(LIB_DIR) -l:libsnake.so -o $@ $(LNCURSES) $(RPATH_FLAG)
	echo 0 > high_score_sn.txt

# Сборка исполняемого файла тетриса
$(TETRIS_EXE): $(TETRIS_LIB) tetris.o $(FRONTEND_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -DTETRIS_GAME tetris.o $(FRONTEND_OBJ) -L$(LIB_DIR) -l:libtetris.so -o $@ $(LNCURSES) $(RPATH_FLAG)
	echo 0 > high_score_t.txt

###############################################################################
# Интерактивный выбор игры и фронтенда
###############################################################################
# Меню выбора игры
brick_game: 
	@echo "Выберите игру:"
	@choice_game=$$(whiptail --title "Brick Game" --menu "Выберите игру:" 15 40 2 \
        "1" "Змейка" \
        "2" "Тетрис" \
        3>&1 1>&2 2>&3); \
	if [ $$? -ne 0 ]; then \
        echo "Выбор игры отменён"; \
        exit 1; \
	fi; \
	if [ "$$choice_game" = "1" ]; then \
        GAME_TARGET=snake; \
	elif [ "$$choice_game" = "2" ]; then \
        GAME_TARGET=tetris; \
	else \
        echo "Неизвестная игра"; \
        exit 1; \
	fi; \
    \
	echo "Выберите фронтенд:"; \
	choice_frontend=$$(whiptail --title "Brick Game" --menu "Выберите фронтенд" 12 40 2 \
        "1" "Консольный (ncurses)" \
        "2" "Графический (Qt)" \
        3>&1 1>&2 2>&3); \
	if [ $$? -ne 0 ]; then \
        echo "Выбор фронтенда отменён"; \
        exit 1; \
	fi; \
	if [ "$$choice_frontend" = "1" ]; then \
        FRONTEND_TARGET=-ncurses; \
	elif [ "$$choice_frontend" = "2" ]; then \
        FRONTEND_TARGET=-qt; \
	else \
        echo "Неизвестный фронтенд"; \
        exit 1; \
	fi; \
    \
	TARGET=run-$$GAME_TARGET$$FRONTEND_TARGET; \
	echo "Запуск: $$TARGET"; \
	$(MAKE) $$TARGET

# Сборка змейки
snake-ncurses: $(SNAKE_EXE)

# Сборка тетриса
tetris-ncurses: $(TETRIS_EXE)

# Запуск змейки
run-snake-ncurses: 
	$(SNAKE_EXE)

# Запуск тетриса
run-tetris-ncurses:
	$(TETRIS_EXE)

# Змейка + Qt
snake-qt:
	@mkdir -p $(BUILD_DIR)/snake
	cd $(BUILD_DIR)/snake && \
	qmake ../../gui/desktop/brick_game.pro CONFIG+=build_snake && \
	make
	echo 0 > high_score_sn.txt

# Тетрис + Qt
tetris-qt:
	@mkdir -p $(BUILD_DIR)/tetris
	cd $(BUILD_DIR)/tetris && \
	qmake ../../gui/desktop/brick_game.pro CONFIG+=build_tetris && \
	make
	echo 0 > high_score_t.txt

run-snake-qt: 
	./$(BIN_DIR)/snake_game_qt


run-tetris-qt: 
	./$(BIN_DIR)/tetris_game_qt
	
