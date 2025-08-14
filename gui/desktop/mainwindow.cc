#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
  controller = new s21::GameController(this);

  gameView = new GameView(this);
  setCentralWidget(gameView);

  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &MainWindow::onTimer);
  timer->start(100);

  connect(controller, &s21::GameController::stateChanged, gameView,
          &GameView::updateState);
  controller->startGame();
  restoreFocus();
}

void MainWindow::onTimer() {
  controller->getCurrentState();
  restoreFocus();
}

void MainWindow::onStateUpdated(const GameInfo_t& info) {
  gameView->updateState(info);
  restoreFocus();
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
  processUserInput(event->key());
  restoreFocus();
}

void MainWindow::restoreFocus() {
  setFocus();
  activateWindow();
  raise();
}
void MainWindow::processUserInput(int key) {
  UserAction_t action = getActionForKey(key);
  if (action == Terminate) {
    userInput(action, false);
    QApplication::quit();
    return;
  }
  if (action == Start) {
    userInput(action, false);
    userInput(START, false);
    return;
  }
  userInput(action, false);
}

UserAction_t MainWindow::getActionForKey(int key) {
  UserAction_t action;
  switch (key) {
    case Qt::Key_Up:
      action = Up;
      break;
    case Qt::Key_Down:
      action = Down;
      break;
    case Qt::Key_Left:
      action = Left;
      break;
    case Qt::Key_Right:
      action = Right;
      break;
    case Qt::Key_Space:
      action = Action;
      break;
    case Qt::Key_Return:
      action = Start;
      break;
    case Qt::Key_P:
      action = Pause;
      break;
    case Qt::Key_Escape:
      action = Terminate;
      break;
    default:
      break;
  }

  return action;
}