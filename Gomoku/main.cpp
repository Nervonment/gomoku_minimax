#include "Game.h"

int main() {
	Game game;

	//for (int i = 0; i < 15; ++i) {
	//	for (int j = 0; j < 15; ++j)
	//		std::cout << game.point_weight(i, j) << ", ";
	//	std::cout << "\n";
	//}

	auto hwnd = GetHWnd();
	while (IsWindow(hwnd)) {
		game.Draw();
		game.ProcInput();
		game.Update();

		Sleep(5);
	}
}