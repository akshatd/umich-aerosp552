#include <iostream>

#include "chessboard.h"

std::vector<ChessBoard> solveNQueens(const uint n_queens, uint depth);
void                    findSolutions(ChessBoard &board, uint depth, uint column, std::vector<ChessBoard> &solutions);

int main(void) {
	int input;
	while (true) {
		// tests
		std::cout << "-- TESTS -- \n";
		// ChessBoard board(1);
		// board.addQueen({0, 0});
		// board.addQueen({0, 3});
		// board.addQueen({3, 3});
		// board.addQueen({3, 0});
		// board.addQueen({1, 2});
		std::cin.clear();
		std::cout << "-- USER INPUT -- \n";
		std::cout << "Enter the number of Queens (integer > 0): ";
		if (std::cin >> input) {
			if (input < 1) {
				std::cout << "** Error, number of Queens is not > 0\n";
				continue;
			}
			break;
		} else {
			std::cout << "** Error, bad input\n";
			std::cin.clear();
			std::cin.ignore();
		}
	}

	bool solutions_found = false;
	// for (uint depth = 1; depth <= static_cast<uint>(input); ++depth) {
	for (uint depth = input; depth <= static_cast<uint>(input); ++depth) {
		std::cout << "Searching depth " << depth << " ... ";
		std::vector<ChessBoard> solutions = solveNQueens(static_cast<uint>(input), depth);
		std::cout << "Solutions found: " << solutions.size() << '\n';
		for (size_t i = 0; i < solutions.size(); ++i) {
			solutions_found = true;
			std::cout << "*** Press enter to view solution " << i + 1 << " ***\n";
			std::cin.clear();
			std::cin.ignore();
			std::cout << solutions[i] << '\n';
		}
	}
	if (solutions_found) std::cout << "No more solutions left to display, exiting\n";

	return 0;
}

std::vector<ChessBoard> solveNQueens(const uint n_queens, uint depth) {
	std::vector<ChessBoard> solutions;

	// create a board with queen in each first row and start search
	for (uint row = 0; row < n_queens; ++row) {
		ChessBoard board(n_queens);
		board.addQueen({row, 0});
		// std::cout << "New board, added queen at " << row << ", " << 0 << '\n';
		// std::cout << '\n' << board << '\n';
		findSolutions(board, depth, 1, solutions);
	}

	return solutions;
}

void findSolutions(ChessBoard &board, uint depth, uint column, std::vector<ChessBoard> &solutions) {
	// Here depth = columns = no of queens left to place, so I could check those instead
	// However doing it the traditional way here to make it look more like a iterative deepening search
	if (depth == 0) {
		return;
	}
	if (board.getQueens().size() == board.getSize()) {
		solutions.push_back(board);
		return;
	}
	for (uint row = 0; row < board.getSize(); ++row) {
		// making a copy here for each board so multiple solutions can be found
		ChessBoard board_next = board;
		if (board_next.addQueen({row, column})) {
			// std::cout << "depth: " << depth << ". added queen at " << row << ", " << column << '\n';
			// std::cout << '\n' << board_next << '\n';
			findSolutions(board_next, depth - 1, column + 1, solutions);
		}
	}
}
