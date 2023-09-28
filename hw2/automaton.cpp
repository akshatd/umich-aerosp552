#include <fstream>
#include <iostream>
#include <string>

const std::string kInputFile   = "file.txt";
constexpr size_t  kmaxInputLen = 8;

class Automaton {
	public:
		Automaton() {}
		~Automaton() {}

	private:
		void construct_from_file(std::string file_name) {}

		// these should be vectors of strings :(
		char **alphabet_;
		char **states_;
		char **initial_;
		char **final_;
		char **transitions_;
};

Automaton global_automaton;

std::string get_input();

/*
  ideally:
  - this function would be part of the automaton class and be called input
  - return the current state of the automaton as an enum/string/struct
  i am now forced to create a global that this function operates on
*/
bool automaton(std::string file, std::string input);

int main() {
	std::cout << "hell\n";
	std::string input;
	while (true) {
		input = get_input();
		if (automaton(kInputFile, "world")) {
			std::cout << "Automaton accepts \"" << input << "\"\n";
		} else {
			std::cout << "Automaton rejects \"" << input << "\"\n";
		}
	}

	return 0;
}

// tested with garbage inputs
std::string get_input() {
	std::string input;

	while (true) {
		std::cout << "Enter next input: ";
		if (std::cin >> input) {
			// check if the input is a number or letter
			// letters are case sensitive
			for (char i : input) {
				if (
					(i >= '0' && i <= '9') || //
					(i >= 'A' && i <= 'Z') || //
					(i >= 'a' && i <= 'z')) {
					std::cout << "Input received: \"" << std::string(1, i) << "\"\n";
					return std::string(1, i);
				} else {
					std::cout << "** Ignoring \"" << std::string(1, i) << "\", input must be 0-9, a-z or A-Z\n";
				}
			}
		} else {
			std::cout << "** Error, bad input\n";
			std::cin.clear();
			std::cin.ignore();
		}
	}
	return input;
}

bool automaton(std::string file, std::string input) {
	static int magic = 0;
	return magic++ % 2;
}
