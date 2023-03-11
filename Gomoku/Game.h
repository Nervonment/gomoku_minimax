#pragma once
#include <graphics.h>
#include <iostream>
#include <limits>

#include "utils.hpp"

#define DEBUG


class Game {

	char** board;	//	����

	const int	board_width		= 15;	//	����·��
	const int	board_height	= 15;	//	����·��
	const int	grid_size		= 40;	//	ÿһ��Ŀ��/px
	const char	WHITE_PIECE		= 'w';	//	����
	const char	BLACK_PIECE		= 'b';	//	����
	const char	NONE			= 'n';	//	��
	bool		current_is_black	= true;		//	�����Ƿ��ֵ� ����
	enum {
		BLACK_WIN,
		WHITE_WIN,
		TIED,
		NOT_END
	} status = NOT_END;		//	��ǰ���״̬

public:
	Game() {
		board = new char* [board_height];
		for (int i = 0; i < board_height; ++i)
			board[i] = new char[board_width];

		for (int i = 0; i < board_height; ++i)
			for (int j = 0; j < board_width; ++j)
				board[i][j] = NONE;

		initgraph(board_width * grid_size, board_height * grid_size);
		setbkcolor(RGB(245, 238, 225));
		BeginBatchDraw();
	}

	void Draw();
	void ProcInput();
	void Update();

private:
	bool Drop(int i, int j);
	
	//	�����㷨����

	int		max_search_depth	= 1;
	POINT	ai_drop_pos			= { 0,0 };

	int situation(char side);
	int point_weight(int i, int j);
	bool is_nearby(int i, int j);
	int point_score(int i, int j, char side);
	int search_max(int depth, char side, int parent_beta);
	int search_min(int depth, char side, int parent_alpha);
	void AIDrop();

public:
	~Game() {
		for (int i = 0; i < board_width; ++i)
			delete board[i];
		delete board;
	}
};