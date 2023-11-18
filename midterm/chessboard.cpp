#include <iostream>

#include "chessboard.h"

ChessBoard::ChessBoard(uint size) : size_(size) {}

uint ChessBoard::getSize() const { return size_; }

std::vector<Queen> ChessBoard::getQueens() const { return queens_; }

// adds a queen and returns true if the queen can be added
bool ChessBoard::addQueen(const Queen &queen) {
	if (queen.row >= size_ || queen.col >= size_) {
		return false;
	}
	for (auto q : queens_) {
		// check straight lines
		if (q.row == queen.row || q.col == queen.col) {
			return false;
		}
		// check diagonal tl to br
		if (q.row - q.col == queen.row - queen.col) {
			return false;
		}
		// check diagonal tr to bl
		if (q.row + q.col == queen.row + queen.col) {
			return false;
		}
	}
	queens_.push_back(queen);
	return true;
}

std::ostream &operator<<(std::ostream &os, const ChessBoard &board) {
	std::vector<Queen> queens = board.getQueens();
	for (uint row = 0; row < board.getSize(); row++) {
		for (uint col = 0; col < board.getSize(); col++) {
			bool queen = false;
			for (auto &q : queens) {
				if (q.row == row && q.col == col) {
					queen = true;
					break;
				}
			}
			if (queen) {
				os << "|Q|";
			} else {
				os << "|_|";
			}
		}
		os << '\n';
	}
	return os;
}
