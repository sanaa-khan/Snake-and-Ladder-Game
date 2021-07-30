#include "SnakesAndLadders.h"

int main() {

	Game game("map.txt", 4);

	cout << game.IsSnake(32);
}