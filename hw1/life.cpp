#include <iostream>
#include <fstream>
#include <string>

#define kAliveCell 'O'
#define kDeadCell  ' '
#define kQuitInput "quit"

// constexpr int kNumFiles = 5;
constexpr int kMaxRows = 1000;
constexpr int kMaxCols = 1000;

const std::string kInitialPattern("life.dat");
// const std::string kInitialPatternFiles[] = {
// 	"life.dat", "life1.dat", "life_block.dat", "life_beehive.dat", "life_beacon.dat",
// };

void   playGameOfLife(std::string pattern_filename);
char **readFile(std::string pattern_filename, bool &success, int &rows, int &cols);
int    getNumNbrs(char **data, int rows, int cols, int row_idx, int col_idx);

int main(void) {
	// for (int i = 0; i < kNumFiles; ++i) {
	// 	playGameOfLife(kInitialPatternFiles[i]);
	// }
	playGameOfLife(kInitialPattern);
}

void playGameOfLife(std::string pattern_filename) {
	// std::cout << "Using " << pattern_filename << " as initial pattern \n";
	int    rows, cols;
	bool   success;
	char **data = readFile(pattern_filename, success, rows, cols);
	if (!success) return;

	char **data_next = new char *[rows];
	for (int row = 0; row < rows; ++row) {
		data_next[row] = new char[cols];
		for (int col = 0; col < cols; ++col) {
			data_next[row][col] = data[row][col];
		}
	}

	bool        can_continue = true;
	std::string user_input;
	while (can_continue) {
		for (int row = 0; row < rows; ++row) {
			for (int col = 0; col < rows; ++col) {
				int neighbours = getNumNbrs(data, rows, cols, row, col);
				// std::cout << row << ',' << col << " has " << neighbours << " neighbours\n";
				if (neighbours < 2) {
					data_next[row][col] = kDeadCell;
				} else if (neighbours == 2 && data[row][col] == kAliveCell) {
					data_next[row][col] = kAliveCell;
				} else if (neighbours == 3) {
					data_next[row][col] = kAliveCell;
					// } else if (neighbours >= 4) {
					// 	data_next[row][col] = kDeadCell;
				} else {
					data_next[row][col] = kDeadCell;
				}
			}
		}
		char **data_temp = data;
		data             = data_next;
		data_next        = data_temp;
		for (int row = 0; row < rows; ++row) {
			for (int col = 0; col < cols; ++col) {
				std::cout << data[row][col];
			}
			std::cout << '\n';
		}
		std::cout << "Press <Enter> to continue or type quit\n";
		std::getline(std::cin, user_input);
		if (user_input.size() != 0 && user_input == kQuitInput) { // user hit enter, continue
			can_continue = false;
		}
	}
	for (int row = 0; row < rows; ++row) {
		delete data[row];
		delete data_next[row];
	}
	delete data;
	delete data_next;
}

char **readFile(std::string pattern_filename, bool &success, int &rows, int &cols) {
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
		} else if (cols != static_cast<int>(line.size())) {
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
	char        **temp_data = new char *[rows];
	int           line_idx  = 0;
	for (std::string line; std::getline(input_file_again, line); ++line_idx) {
		temp_data[line_idx] = new char[cols];
		for (int j = 0; j < cols; ++j) {
			temp_data[line_idx][j] = line[j];
		}
	}
	success = true;
	return temp_data;
}

int getNumNbrs(char **data, int rows, int cols, int row_idx, int col_idx) {
	int row_low    = std::max(0, row_idx - 1);
	int row_high   = std::min(rows - 1, row_idx + 1);
	int col_low    = std::max(0, col_idx - 1);
	int col_high   = std::min(cols - 1, col_idx + 1);
	int neighbours = 0;
	for (int row = row_low; row <= row_high; ++row) {
		for (int col = col_low; col <= col_high; ++col) {
			if (row == row_idx && col == col_idx) continue;
			if (data[row][col] == kAliveCell) ++neighbours;
		}
	}
	return neighbours;
}
