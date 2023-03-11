#include "Game.h"

int main() {
	Game game;

	auto hwnd = GetHWnd();
	while (IsWindow(hwnd)) {
		game.Draw();
		game.ProcInput();
		game.Update();

		Sleep(5);
	}
}