#include <iostream>
#include <fstream>
#include <string>

#define kAliveCell 'O'
#define kDeadCell  ' '
#define kQuitInput "quit"

constexpr int kNumFiles = 1;
constexpr int kMaxRows  = 1000;
constexpr int kMaxCols  = 1000;

const std::string kInitialPatternFiles[] = {
	"life.dat",
};

void  playGameOfLife(std::string pattern_filename);
int **readFile(std::string pattern_filename, bool &success, int &rows, int &cols);
int   getNumNbrs(int **data, int rows, int cols, int row_idx, int col_idx);

int main(void) {
	for (int i = 0; i < kNumFiles; ++i) {
		playGameOfLife(kInitialPatternFiles[i]);
	}
}

void playGameOfLife(std::string pattern_filename) {
	std::cout << "Using " << pattern_filename << " as initial pattern \n";
	int   rows, cols;
	bool  success;
	int **data      = readFile(pattern_filename, success, rows, cols);
	int **data_next = new int *[rows];
	for (int row = 0; row < rows; ++row) {
		data_next[row] = new int[cols];
		for (int col = 0; col < cols; ++col) {
			data_next[row][col] = data[row][col];
		}
	}
	if (!success) return;
	// check the data
	// for (int i = 0; i < rows; ++i) {
	// 	for (int j = 0; j < cols; ++j) {
	// 		std::cout << data[i][j];
	// 	}
	// 	std::cout << '\n';
	// }
	bool        can_continue = true;
	std::string user_input;
	while (can_continue) {
		for (int row = 0; row < rows; ++row) {
			for (int col = 0; col < cols; ++col) {
				std::cout << data[row][col];
			}
			std::cout << '\n';
		}
		for (int row = 0; row < rows; ++row) {
			for (int col = 0; col < rows; ++col) {
				int neighbours = getNumNbrs(data, rows, cols, row, col);
				// std::cout << row << ',' << col << " has " << neighbours << " neighbours\n";
				if (neighbours < 2) {
					data_next[row][col] = 0;
				} else if (neighbours == 2 && data[row][col]) {
					data_next[row][col] = 1;
				} else if (neighbours == 3) {
					data_next[row][col] = 1;
					// } else if (neighbours >= 4) {
					// 	data_next[row][col] = 0;
				} else {
					data_next[row][col] = 0;
				}
			}
		}
		int **data_temp = data;
		data            = data_next;
		data_next       = data_temp;
		std::cout << "Press <Enter> to continue or type quit\n";
		std::getline(std::cin, user_input);
		if (user_input.size() != 0 && user_input == kQuitInput) { // user hit enter, continue
			can_continue = false;
		}
	}
	for (int row = 0; row < rows; ++row) {
		delete data[row];
	}
	delete data;
}

int **readFile(std::string pattern_filename, bool &success, int &rows, int &cols) {
	std::ifstream input_file(pattern_filename);
	if (!input_file.is_open()) {
		std::cout << "** Error: Could not open " << pattern_filename << '\n';
		success = false;
		return nullptr;
	}

	// sanity check the file and determine its size
	cols = -1;
	rows = 0;
	for (std::string line; std::getline(input_file, line); rows++) {
		// std::cout << line << '\n';
		if (cols < 0) {
			cols = line.size();
		} else if (cols != line.size()) {
			std::cout << "**Error: Lines do not have the same number of characters\n";
			success = false;
			return nullptr;
		}
		for (char c : line) {
			if (c != kAliveCell && c != kDeadCell) {
				std::cout << "** Error: Invalid characters in file\n";
				success = false;
				return nullptr;
			}
		}
	}
	// std::cout << "file has " << rows << " rows and " << cols << " cols\n";
	// have to do it this ugly way instead of using a nicer STL dynamic mem container
	input_file.close();

	// put data in data struct
	std::ifstream input_file_again(pattern_filename);
	int         **temp_data = new int *[rows];
	int           line_idx  = 0;
	for (std::string line; std::getline(input_file_again, line); ++line_idx) {
		temp_data[line_idx] = new int[cols];
		for (int j = 0; j < cols; ++j) {
			temp_data[line_idx][j] = (line[j] == kAliveCell);
		}
	}
	success = true;
	return temp_data;
}

int getNumNbrs(int **data, int rows, int cols, int row_idx, int col_idx) {
	int row_low    = std::max(0, row_idx - 1);
	int row_high   = std::min(rows - 1, row_idx + 1);
	int col_low    = std::max(0, col_idx - 1);
	int col_high   = std::min(cols - 1, col_idx + 1);
	int neighbours = 0;
	for (int row = row_low; row <= row_high; ++row) {
		for (int col = col_low; col <= col_high; ++col) {
			if (row == row_idx && col == col_idx) continue;
			if (data[row][col] == 1) ++neighbours;
		}
	}
	return neighbours;
}
