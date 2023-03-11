#include "Game.h"

bool Game::Drop(int i, int j) {
	if (i < 0 || i >= board_height || j < 0 || j >= board_width)
		return false;
	if (board[i][j] != NONE)
		return false;
	if (status != NOT_END)
		return false;
	board[i][j] = current_is_black ? BLACK_PIECE : WHITE_PIECE;

#ifdef DEBUG
	std::cout << (current_is_black ? "黑" : "白")
		<< "方在 (" << i << ", " << j << ") 位置落子\n";
#endif

	current_is_black = !current_is_black;
	return true;
}

void Game::Draw() {
	cleardevice();
	setlinecolor(RGB(55, 57, 61));
	for (int i = 0; i < board_width; ++i) {
		line(grid_size / 2 + i * grid_size, grid_size / 2, grid_size / 2 + i * grid_size, grid_size / 2 + (board_height - 1) * grid_size);
		line(grid_size / 2, grid_size / 2 + i * grid_size, grid_size / 2 + (board_width - 1) * grid_size, grid_size / 2 + i * grid_size);
	}
	for (int i = 0; i < board_height; ++i)
		for (int j = 0; j < board_width; ++j) {
			if (board[i][j] == BLACK_PIECE) {
				setfillcolor(RGB(55, 57, 61));
				solidcircle(grid_size / 2 + grid_size * j, grid_size / 2 + grid_size * i, grid_size / 3);
			}
			else if (board[i][j] == WHITE_PIECE) {
				setfillcolor(RGB(246, 247, 248));
				setlinecolor(RGB(55, 57, 61));
				fillcircle(grid_size / 2 + grid_size * j, grid_size / 2 + grid_size * i, grid_size / 3);
			}
		}

	FlushBatchDraw();
}

void Game::ProcInput() {
	ExMessage msg;

	while (peekmessage(&msg, EX_CHAR | EX_KEY | EX_MOUSE)) {
		if (msg.message == WM_LBUTTONDOWN) {
			bool continue_ = true;
			for (int i = 0; i < board_height && continue_; ++i)
				for (int j = 0; j < board_width && continue_; ++j) {
					int y = grid_size / 2 + i * grid_size;
					int x = grid_size / 2 + j * grid_size;
					if (x - grid_size / 3 < msg.x && msg.x < x + grid_size / 3 &&
						y - grid_size / 3 < msg.y && msg.y < y + grid_size / 3) {
						Drop(i, j);
						continue_ = false;
					}
				}
		}
	}
}

void Game::Update() {
	if (status != NOT_END)
		return;

	for (int i = 0; i < board_height; ++i)
		for (int j = 0; j <= board_width - 5; ++j)
			if (board[i][j] != NONE && equal(board[i][j], board[i][j + 1], board[i][j + 2], board[i][j + 3], board[i][j + 4])) {
				if (board[i][j] == WHITE_PIECE)
					status = WHITE_WIN;
				else status = BLACK_WIN;
			}

	for (int i = 0; i < board_width; ++i)
		for (int j = 0; j <= board_height - 5; ++j)
			if (board[j][i] != NONE && equal(board[j][i], board[j + 1][i], board[j + 2][i], board[j + 3][i], board[j + 4][i])) {
				if (board[j][i] == WHITE_PIECE)
					status = WHITE_WIN;
				else status = BLACK_WIN;
			}

	for (int i = 0; i <= board_height - 5; ++i)
		for (int j = 0; j <= board_width - 5; ++j)
			if (
				(board[i][j] != NONE && equal(board[i][j], board[i + 1][j + 1], board[i + 2][j + 2], board[i + 3][j + 3], board[i + 4][j + 4]))
				|| (board[i + 4][j] != NONE && equal(board[i + 4][j], board[i + 3][j + 1], board[i + 2][j + 2], board[i + 1][j + 3], board[i][j + 4]))) {
				if (board[i + 2][j + 2] == WHITE_PIECE)
					status = WHITE_WIN;
				else status = BLACK_WIN;
			}

#ifdef DEBUG
	if (status == WHITE_WIN)
		std::cout << "游戏结束，白方获胜\n";
	else if (status == BLACK_WIN)
		std::cout << "游戏结束，黑方获胜\n";
#endif
	
	if (status == NOT_END && !current_is_black)
		AIDrop();
}