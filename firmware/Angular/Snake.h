#ifndef SNAKE_H
#define SNAKE_H

#include <cstdlib> // Для rand() и srand()
#include <ctime>   // Для инициализации генератора случайных чисел

// Размеры поля
const int FIELD_WIDTH = 8;
const int FIELD_HEIGHT = 8;

// Режим игры
enum GameMode {
  MODE_BORDER,  // Ограниченное поле
  MODE_WRAP     // Закольцованное поле
};

// Направления змейки
enum Direction {
  UP,
  DOWN,
  LEFT,
  RIGHT
};

// Структура для координат
struct Point {
  int x;
  int y;
};

// Класс игры "Змейка"
class SnakeGame {
private:
  GameMode gameMode;         // Режим игры (граница или закольцованное поле)
  Direction currentDir;      // Текущее направление
  Direction nextDir;         // Направление, в которое нужно двигаться
  Point snake[FIELD_WIDTH * FIELD_HEIGHT]; // Координаты тела змейки
  int snakeLength;           // Длина змейки
  Point food;                // Координаты еды
  bool gameOver;             // Флаг окончания игры

  void spawnFood() {
    bool validPosition;
    do {
      validPosition = true;
      food.x = rand() % FIELD_WIDTH;
      food.y = rand() % FIELD_HEIGHT;
      for (int i = 0; i < snakeLength; ++i) {
        if (snake[i].x == food.x && snake[i].y == food.y) {
          validPosition = false;
          break;
        }
      }
    } while (!validPosition);
  }

  bool checkCollision() {
    for (int i = 1; i < snakeLength; ++i) {
      if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
        return true;
      }
    }

    if (gameMode == MODE_BORDER) {
      if (snake[0].x < 0 || snake[0].x >= FIELD_WIDTH ||
          snake[0].y < 0 || snake[0].y >= FIELD_HEIGHT) {
        return true;
      }
    }

    return false;
  }

  void moveSnake() {
    // Сохраняем текущее направление
    currentDir = nextDir;

    // Смещение головы змейки
    Point nextHead = snake[0];
    switch (currentDir) {
      case UP:    nextHead.y -= 1; break;
      case DOWN:  nextHead.y += 1; break;
      case LEFT:  nextHead.x -= 1; break;
      case RIGHT: nextHead.x += 1; break;
    }

    // Обработка закольцованного режима
    if (gameMode == MODE_WRAP) {
      if (nextHead.x < 0) nextHead.x = FIELD_WIDTH - 1;
      if (nextHead.x >= FIELD_WIDTH) nextHead.x = 0;
      if (nextHead.y < 0) nextHead.y = FIELD_HEIGHT - 1;
      if (nextHead.y >= FIELD_HEIGHT) nextHead.y = 0;
    }

    // Перемещение тела змейки
    for (int i = snakeLength - 1; i > 0; --i) {
      snake[i] = snake[i - 1];
    }
    snake[0] = nextHead;

    // Проверка на столкновение с едой
    if (snake[0].x == food.x && snake[0].y == food.y) {
      if (snakeLength < FIELD_WIDTH * FIELD_HEIGHT) {
        ++snakeLength;
      }
      spawnFood();
    }

    // Проверка на столкновение
    if (checkCollision()) {
      gameOver = true;
    }
  }

  void changeDirection(Direction dir) {
    if ((dir == UP && currentDir != DOWN) ||
        (dir == DOWN && currentDir != UP) ||
        (dir == LEFT && currentDir != RIGHT) ||
        (dir == RIGHT && currentDir != LEFT)) {
      nextDir = dir;
    }
  }

public:
  SnakeGame(GameMode mode = MODE_WRAP) : gameMode(mode) {
    srand(static_cast<unsigned>(time(0)));
    resetGame();
  }

  void resetGame() {
    snakeLength = 1;
    snake[0] = {FIELD_WIDTH / 2, FIELD_HEIGHT / 2};
    currentDir = RIGHT;
    nextDir = RIGHT;
    gameOver = false;
    spawnFood();
  }

  bool updateGameField(Direction direct) {
    if (gameOver) {
      return false;
    }

    changeDirection(direct);
    moveSnake();

    return !gameOver;
  }

  bool isWin() const {
    return snakeLength == FIELD_WIDTH * FIELD_HEIGHT;
  }

  uint8_t* getGameField() const {
    static uint8_t field[FIELD_HEIGHT] = {0};

    // Очистка поля
    for (int i = 0; i < FIELD_HEIGHT; ++i) {
      field[i] = 0;
    }

    // Добавление тела змейки
    for (int i = 0; i < snakeLength; ++i) {
      field[snake[i].y] |= (1 << snake[i].x);
    }

    // Добавление еды
    field[food.y] |= (1 << food.x);

    return field;
  }
};

#endif
