#include <iostream>
#include <string>

int  get_value_from_cin(std::string value_name, int min, int max);
void hanoi(int n);

#define kFrom "A"
#define kTemp "B"
#define kTo   "C"
void hanoi_rec(int n, std::string from, std::string temp, std::string to);

int main(void) {
	// tests: should return in (2^n)-1 moves
	hanoi(0); // returns immediately
	hanoi(1); // returns in 1 move
	hanoi(2); // returns in 3 moves
	hanoi(3); // returns in 7 moves

	std::cout << '\n';
	int num_disks = get_value_from_cin("number of disks", 0, __INT_MAX__);
	hanoi(num_disks);
	return 0;
}

// tested with inputs out of range and garbage inputs like strings
int get_value_from_cin(std::string value_name, int min, int max) {
	bool input_valid = false;
	int  input;
	while (!input_valid) {
		std::cout << "Enter " << value_name << ": ";
		if (std::cin >> input) {
			if (input < min) {
				std::cout << "** Error, " << value_name << " must be >= " << min << "\n";
			} else if (input >= max) {
				std::cout << "** Error, " << value_name << " must be < " << max << "\n";
			} else {
				input_valid = true;
			}
		} else {
			std::cout << "** Error, bad input\n";
			std::cin.clear();
			std::cin.ignore();
		}
	}
	return input;
}

void hanoi(int n) {
	std::cout << "\nMoving " << n << " disks\n";
	hanoi_rec(n, kFrom, kTemp, kTo);
}

void hanoi_rec(int n, std::string from, std::string temp, std::string to) {
	if (n == 0) return;
	// move all smaller disks from "from" to "temp" rod using "to" as temp
	hanoi_rec(n - 1, from, to, temp);
	// move biggest disk to "to"
	std::cout << "Make a move from " << from << " to " << to << '\n';
	// move the remaining disks from the "temp" to "to" rod using "from as a temp"
	hanoi_rec(n - 1, temp, from, to);
}
