#include "Game.h"

//	一个勉强能用的棋局评估函数
//	评估（对于side方而言当前棋局的形势）
int Game::situation(char side) {
	int self = 0;
	int oppo = 0;
	const char chess_self = side;
	const char chess_oppo = side == WHITE_PIECE ? BLACK_PIECE : WHITE_PIECE;

	for (int i = 0; i < board_height; ++i)
		for (int j = 0; j < board_width; ++j) {
			if (board[i][j] == chess_self) {
				self += point_score(i, j, chess_self);
			}
			else if (board[i][j] == chess_oppo) {
				oppo += point_score(i, j, chess_oppo);
			}
		}

	return self - oppo;
}

//	一点的权重
//	越靠近中心权重越大（与该点到中心的曼哈顿距离负相关）
//int Game::point_weight(int i, int j) {
//	return board_height / 2 + board_width / 2 - abs(i - board_height / 2) - abs(j - board_width / 2);
//}

//	以该位置为中心2*2的区域内是否有其他棋子
//	用此函数进行筛选能够大大地减少分支的数量
bool Game::is_nearby(int i, int j) {
	for (int di = -1; di < 2; ++di)
		for (int dj = -1; dj < 2; ++dj)
			if (i + di >= 0 && i + di < board_height && j + dj >= 0 && j + dj < board_width)
				if (board[i + di][j + dj] != NONE)
					return true;
	return false;
}

//	某一点的得分
int Game::point_score(int i, int j, char side) {
	int score = 1;

	//	*   *   *
	//	  * * *
	//  * * * * *
	//	  * * *
	//	*   *   *
	const POINT lines[4][9] = {
		{{-4,0},{-3,0},{-2,0},{-1,0},{0,0},{1,0},{2,0},{3,0},{4,0}},
		{{0,-4}, {0,-3}, {0,-2},{0,-1},{0,0},{0,1},{0,2},{0,3},{0,4}},
		{{-4,-4}, {-3,-3}, {-2,-2},{-1,-1},{0,0},{1,1},{2,2},{3,3},{4,4}},
		{{-4,4},{-3,3}, {-2,2},{-1,1},{0,0},{1,-1},{2,-2},{3,-3},{4,-4}}
	};

	//	五子棋术语
	//	参考：https://zhuanlan.zhihu.com/p/361201277
	const std::string CHENG5 = "sssss";
	const std::string HUO4 = " ssss ";
	const std::string CHONG4[] = {
		" sssso",
		"s sss",
		"ss ss",
		"sss s",
		"ossss ",
	};
	const std::string LIANHUO3[] = {
		" sss  ",
		"  sss ",
	};
	const std::string TIAOHUO3[] = {
		" s ss ",
		" ss s ",
	};
	const std::string MIAN3[] = {
		"  ssso",
		" s sso",
		" ss so",
		"osss  ",
		"oss s ",
		"os ss ",
		"ss  s",
		"s  ss",
		"s s s",
		"o sss o",
	};
	const std::string HUO2[] = {
		"   ss ",
		"  ss  ",
		" ss   ",
		"  s s ",
		" s s  ",
	};
	const std::string MIAN2[] = {
		"   sso",
		"  s so",
		" s  so",
		"s   s",
		"oss   ",
		"os s  ",
		"os  s ",

		"o  ss o",
		"o ss  o",
		"o s s o",
	};

	for (auto& line : lines) {
		std::string line_str = "";
		for (auto& point : line) {
			int x = j - point.x;
			int y = i - point.y;
			if (x < 0 || x >= board_width || y < 0 || y >= board_height)
				line_str += 'n';
			else if (board[y][x] == side)
				line_str += 's';
			else if (board[y][x] == NONE)
				line_str += ' ';
			else line_str += 'o';
		}

		if (line_str.find(CHENG5) != std::string::npos)
			score += 5000000;
		if (line_str.find(HUO4) != std::string::npos)
			score += 100000;
		for (auto& chong4 : CHONG4)
			if (line_str.find(chong4) != std::string::npos) {
				score += 16000;
				break;
			}
		for (auto& lianhuo3 : LIANHUO3)
			if (line_str.find(lianhuo3) != std::string::npos) {
				score += 8000;
				break;
			}
		for (auto& tiaohuo3 : TIAOHUO3)
			if (line_str.find(tiaohuo3) != std::string::npos) {
				score += 2000;
				break;
			}
		for (auto& mian3 : MIAN3)
			if (line_str.find(mian3) != std::string::npos) {
				score += 300;
				break;
			}
		for (auto& huo2 : HUO2)
			if (line_str.find(huo2) != std::string::npos) {
				score += 20;
				break;
			}
		for (auto& mian2 : MIAN2)
			if (line_str.find(mian2) != std::string::npos) {
				score += 2;
				break;
			}
	}

	return score;// *point_weight(i, j);
}

//	带alpha-beta剪枝的minmax搜索
int Game::search_max(int depth, char side, int parent_beta) {
	int alpha = INT_MIN;
	for (int i = 0; i < board_height; ++i)
		for (int j = 0; j < board_width; ++j) {
			if (board[i][j] == NONE && is_nearby(i, j)) {
				board[i][j] = side;
				int score =
					depth < max_search_depth ?
					search_min(depth + 1, side == WHITE_PIECE ? BLACK_PIECE : WHITE_PIECE, alpha) :
					situation(WHITE_PIECE);
				board[i][j] = NONE;
				if (score > alpha) {
					alpha = score;
					if (depth == 0)
						ai_drop_pos = { j,i };
					if (alpha >= parent_beta)
						return alpha;
				}
			}
		}
	return alpha;
}

int Game::search_min(int depth, char side, int parent_alpha) {
	int beta = INT_MAX;
	for (int i = 0; i < board_height; ++i)
		for (int j = 0; j < board_width; ++j) {
			if (board[i][j] == NONE && is_nearby(i, j)) {
				board[i][j] = side;
				int score =
					depth < max_search_depth ?
					search_max(depth + 1, side == WHITE_PIECE ? BLACK_PIECE : WHITE_PIECE, beta) :
					situation(WHITE_PIECE);
				board[i][j] = NONE;
				if (score < beta) {
					beta = score;
					if (beta <= parent_alpha)
						return beta;
				}
			}
		}
	return beta;
}

void Game::AIDrop() {
	int score = search_max(0, WHITE_PIECE, INT_MAX);
	Drop(ai_drop_pos.y, ai_drop_pos.x);

#ifdef DEBUG
	std::cout << "AI认为当前的局势它有" << score << "分的把握能赢\n";
#endif
}
