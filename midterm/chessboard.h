#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <iostream>
#include <vector>

struct Queen {
		uint row;
		uint col;
};

class ChessBoard {
	public:
		ChessBoard(uint size);

		uint                 getSize() const;
		std::vector<Queen>   getQueens() const;
		bool                 addQueen(const Queen& queen);
		friend std::ostream& operator<<(std::ostream& os, const ChessBoard& t);

	private:
		uint               size_;
		std::vector<Queen> queens_;
};

std::vector<ChessBoard> solveNQueens(const uint n_queens);

#endif