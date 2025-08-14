#include "view.h"

GameView::GameView(QWidget* parent) : QWidget(parent) {
  setFixedSize(440, 640);
  setWindowTitle("Brick Game");
  setFocusPolicy(Qt::StrongFocus);
  setFocus();

  memset(&currentInfo, 0, sizeof(GameInfo_t));
  initializeField();

  QFontDatabase::addApplicationFont(":/fonts/PressStart2P-Regular.ttf");
  QFontDatabase::addApplicationFont(":/fonts/VT323-Regular.ttf");
  QFontDatabase::addApplicationFont(":/fonts/Orbitron-Regular.ttf");
}

void GameView::paintEvent(QPaintEvent*) {
  QPainter painter(this);

  painter.setBrush(QColor("#2B2B2B"));
  painter.drawRect(0, 0, width(), height());

  const int cellSize = 32;

  if (currentInfo.field == nullptr) {
    return;
  }
  for (int y = 0; y < 20; ++y) {
    for (int x = 0; x < 10; ++x) {
      int cell = currentInfo.field[y][x];
      if (cell == 0) continue;

      QColor color = getCellColor(cell);
      painter.setBrush(color);

      //  Тень (нижняя и правая границы)
      painter.setPen(QColor(0, 0, 0, 80));  // полупрозрачная тень
      painter.drawLine(x * cellSize + 1, y * cellSize + cellSize,
                       x * cellSize + cellSize - 1, y * cellSize + cellSize);
      painter.drawLine(x * cellSize + cellSize, y * cellSize + 1,
                       x * cellSize + cellSize, y * cellSize + cellSize - 1);

      painter.setPen(QPen(color.darker(150), 1));  // тёмная рамка
      painter.drawRect(x * cellSize, y * cellSize, cellSize, cellSize);
    }
  }

  // инфо панель
  painter.setBrush(QColor("#E1A6F5"));
  painter.drawRect(320, 0, 120, 640);

  // текст
  painter.setPen(QColor("#000000"));
  painter.setFont(QFont("Orbitron", 12, QFont::Bold));

  painter.drawText(330, 50, "Information:");

  painter.setFont(QFont("Orbitron", 8, QFont::Normal));

  painter.drawText(330, 70,
                   QString("High score: %1").arg(currentInfo.high_score));
  painter.drawText(330, 90, QString("Score: %1").arg(currentInfo.score));
  painter.drawText(330, 110, QString("Level: %1").arg(currentInfo.level));
  painter.drawText(330, 130, QString("Speed: %1").arg(currentInfo.speed));

  drawNextFigure(painter);
  drawControls(painter);
  drawOverlay(painter);

  painter.setPen(QColor("#000000"));
  painter.setFont(QFont("Orbitron", 12, QFont::Bold));

  painter.drawText(332, 550, "Tambrama");
  painter.drawText(335, 570, "Shcool 21");
  painter.drawText(350, 590, "2025");
}

void GameView::initializeField() {
  // Инициализируем field, если ещё не инициализировано
  if (currentInfo.field == nullptr) {
    currentInfo.field = new int*[20];
    for (int i = 0; i < 20; ++i) {
      currentInfo.field[i] = new int[10]{0};  // обнуляем
    }
    // currentInfo.height = 20;
    // currentInfo.width = 10;
  }
}
void GameView::drawControls(QPainter& painter) {
  int yPos = 380;
  int xPos = 330;

  painter.setFont(QFont("Orbitron", 12, QFont::Bold));
  painter.setPen(QColor("#000000"));
  painter.drawText(332, yPos, "Controls:");

  painter.setFont(QFont("Orbitron", 8, QFont::Normal));
  yPos += 20;
  painter.drawText(xPos, yPos, "↑↓←→  -  Move");
  yPos += 20;
  painter.drawText(xPos, yPos, "Enter  -  Start");
  yPos += 20;
  painter.drawText(xPos, yPos, "Space  -  Action");
  yPos += 20;
  painter.drawText(xPos, yPos, "P  -  Pause");
  yPos += 20;
  painter.drawText(xPos, yPos, "Esc  -  Exit");
}

void GameView::drawOverlay(QPainter& painter) {
  QString message;
  QColor bgColor = QColor(0, 0, 0, 180);  // затемнение
  QColor textColor = Qt::white;

  switch (currentInfo.pause) {
    case 2:
      message = "PRESS ENTER\nto START";
      textColor = QColor("#FFB6C1");
      break;
    case 1:
      message = "PAUSED\n(P)";
      textColor = QColor("#FFC0CB");
      break;
    case 3:
      message = "GAME OVER\nENTER to Restart";
      textColor = QColor("#FFA0B0");
      break;
    case 4:
      message = "YOU WIN!\nENTER to Restart";
      textColor = QColor("#FFC0CB");
      break;
    default:
      return;
  }

  painter.fillRect(0, 0, width(), height(), bgColor);
  painter.setPen(textColor);
  painter.setFont(QFont("Press Start 2P", 18, QFont::Bold));
  painter.drawText(rect(), Qt::AlignCenter, message);
}

void GameView::updateState(const GameInfo_t& info) {
  currentInfo = info;
  update();
  setFocus();
}

void GameView::drawNextFigure(QPainter& painter) {
  if (!currentInfo.next) return;

  painter.setPen(QColor("#000000"));  // чёрный текст
  painter.setFont(QFont("Orbitron", 12, QFont::Bold));
  painter.drawText(350, 200, "Next:");

  const int nextX = 350;
  const int nextY = 220;
  const int cellSize = 20;

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      int cell = currentInfo.next[i][j];
      if (cell == 0) continue;

      QColor color = getCellColor(cell);
      painter.setBrush(color);

      painter.setPen(QColor(0, 0, 0, 80));
      painter.drawLine(
          nextX + j * cellSize + 1, nextY + i * cellSize + cellSize,
          nextX + j * cellSize + cellSize - 1, nextY + i * cellSize + cellSize);
      painter.drawLine(
          nextX + j * cellSize + cellSize, nextY + i * cellSize + 1,
          nextX + j * cellSize + cellSize, nextY + i * cellSize + cellSize - 1);

      painter.setPen(QPen(color.darker(150), 1));
      painter.drawRect(nextX + j * cellSize, nextY + i * cellSize, cellSize,
                       cellSize);
    }
  }
}

QColor GameView::getCellColor(int cellValue) {
  switch (cellValue) {
    case 1:
      return QColor("#E1A6F5");  // змейка
    case 2:
      return QColor("#DB3CC6");  // статика
    case 3:
      return QColor("#FF6B6B");  // яблоко
    case 4:
      return QColor("#8A2BE2");
    case 5:
      return QColor("#e040c5ff");
    case 6:
      return QColor("#ff0055ff");
    case 7:
      return QColor("#9753c5ff");
    case 8:
      return QColor("#BA55D3");
    case 9:
      return QColor("#FFD700");
    default:
      return QColor("#E1A6F5");
  }
}