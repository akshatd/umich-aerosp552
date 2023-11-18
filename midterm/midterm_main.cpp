#include <iostream>

#include "chessboard.h"

std::vector<ChessBoard> solveNQueens(const uint n_queens, uint depth);
void                    findSolutions(ChessBoard &board, uint depth, uint column, std::vector<ChessBoard> &solutions);

int main(void) {
	int input;
	while (true) {
		// tests
		std::cout << "-- TESTS -- \n";
		// testing chessboard class
		std::cout << "Trying to add queens to a 0 board ... ";
		ChessBoard board0(0);
		board0.addQueen({0, 0});
		board0.getQueens().size() == 0 ? std::cout << "Passed\n" : std::cout << "Failed\n";
		std::cout << "Trying to add more than 1 queen to a 1 board ... ";
		ChessBoard board1(1);
		board1.addQueen({0, 0});
		board1.addQueen({0, 0});
		board1.getQueens().size() == 1 ? std::cout << "Passed\n" : std::cout << "Failed\n";
		std::cout << "Trying to add a queen outside the board ... ";
		ChessBoard board2(1);
		board2.addQueen({5, 5});
		board2.getQueens().size() == 0 ? std::cout << "Passed\n" : std::cout << "Failed\n";
		std::cout << "Trying to add a queen in the same row ... ";
		ChessBoard board3(5);
		board3.addQueen({2, 2});
		board3.addQueen({2, 0});
		board3.getQueens().size() == 1 ? std::cout << "Passed\n" : std::cout << "Failed\n";
		std::cout << "Trying to add a queen in the same column ... ";
		board3.addQueen({0, 2});
		board3.getQueens().size() == 1 ? std::cout << "Passed\n" : std::cout << "Failed\n";
		std::cout << "Trying to add a queen in the same diagonal 1 ... ";
		board3.addQueen({0, 0});
		board3.getQueens().size() == 1 ? std::cout << "Passed\n" : std::cout << "Failed\n";
		std::cout << "Trying to add a queen in the same diagonal 2 ... ";
		board3.addQueen({4, 4});
		board3.getQueens().size() == 1 ? std::cout << "Passed\n" : std::cout << "Failed\n";

		// testing solveNQueens num of solutions generated
		std::cout << "Testing that n_queens=0 generates 0 solutions  ... ";
		solveNQueens(0, 0).size() == 0 ? std::cout << "Passed\n" : std::cout << "Failed\n";
		std::cout << "Testing that n_queens=1 generates 1 solution  ... ";
		solveNQueens(1, 1).size() == 1 ? std::cout << "Passed\n" : std::cout << "Failed\n";
		std::cout << "Testing that n_queens=2 generates 0 solutions  ... ";
		solveNQueens(2, 2).size() == 0 ? std::cout << "Passed\n" : std::cout << "Failed\n";
		std::cout << "Testing that n_queens=3 generates 0 solutions  ... ";
		solveNQueens(3, 3).size() == 0 ? std::cout << "Passed\n" : std::cout << "Failed\n";
		std::cout << "Testing that n_queens=4 generates 2 solutions  ... ";
		solveNQueens(4, 4).size() == 2 ? std::cout << "Passed\n" : std::cout << "Failed\n";
		std::cout << "Testing that n_queens=5 generates 10 solutions  ... ";
		solveNQueens(5, 5).size() == 10 ? std::cout << "Passed\n" : std::cout << "Failed\n";
		std::cout << "Testing that n_queens=6 generates 4 solutions  ... ";
		solveNQueens(6, 6).size() == 4 ? std::cout << "Passed\n" : std::cout << "Failed\n";
		std::cout << "Testing that n_queens=7 generates 40 solutions  ... ";
		solveNQueens(7, 7).size() == 40 ? std::cout << "Passed\n" : std::cout << "Failed\n";
		std::cout << "Testing that n_queens=8 generates 92 solutions  ... ";
		solveNQueens(8, 8).size() == 92 ? std::cout << "Passed\n" : std::cout << "Failed\n";
		std::cout << "Testing that n_queens=9 generates 352 solutions  ... ";
		solveNQueens(9, 9).size() == 352 ? std::cout << "Passed\n" : std::cout << "Failed\n";
		std::cout << "Testing that n_queens=10 generates 724 solutions  ... ";
		solveNQueens(10, 10).size() == 724 ? std::cout << "Passed\n" : std::cout << "Failed\n";

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
	for (uint depth = 1; depth <= static_cast<uint>(input); ++depth) {
		std::cout << "Searching depth " << depth << " ... ";
		std::vector<ChessBoard> solutions = solveNQueens(static_cast<uint>(input), depth);
		std::cout << "Solutions found: " << solutions.size() << '\n';
		for (size_t i = 0; i < solutions.size(); ++i) {
			solutions_found = true;
			std::cout << "*** Press enter to view solution " << i + 1 << '/' << solutions.size() << " ***\n";
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
