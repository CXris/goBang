#include "Game.h"

int main() {
	Game gobang;
	while (gobang.window.isOpen()) {
		gobang.Run();
	}
	return 0;
}