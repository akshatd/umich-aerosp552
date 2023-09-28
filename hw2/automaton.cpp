#include <fstream>
#include <iostream>
#include <string>

const std::string kInputFile = "file.txt";

constexpr size_t kMaxTransitions = 1024;
// keywords and enums to parse file
const std::string kKwAlphabet       = "-- alphabet";
const std::string kKwStates         = "-- states";
const std::string kKwInitial        = "-- initial";
const std::string kKwFinal          = "-- final";
const std::string kKwTransitions    = "-- transitions";
const std::string kKwTransitionFrom = " -- ";
const std::string kKwTransitionTo   = " --> ";
const std::string kKwDelim          = " ";

enum FileParseState {
	Alphabet,
	States,
	Initial,
	Final,
	Transitions,
};

struct Transition {
		std::string from;
		std::string input;
		std::string to;
};

// class to store automaton
class Automaton {
	public:
		Automaton(std::string file_name);
		~Automaton();

		bool ProcessInput(std::string input);

	private:
		std::string alphabet_;
		std::string states_;
		std::string initial_;
		std::string final_;
		std::string transitions_[kMaxTransitions];
		size_t      num_transitions_;

		void        parse_file(std::string file_name);
		std::string clean_input(std::string input);
		bool        run(std::string input);
		Transition  process_transition(std::string transition);
		bool        exists_in_set(std::string state, std::string set);
};

/*
  ideally:
  - this function would be part of the automaton class and be called input
  - return the current state of the automaton as an enum/string/struct
*/
bool automaton(std::string file, std::string input);

int main() {
	std::cout << "hello\n";

	std::string input;
	while (true) {
		std::cout << "Enter input: ";
		if (std::cin >> input) {
			break;
		} else {
			std::cout << "** Error, bad input\n";
			std::cin.clear();
			std::cin.ignore();
		}
	}
	if (automaton(kInputFile, input)) {
		std::cout << "Automaton accepts \"" << input << "\"\n";
	} else {
		std::cout << "Automaton rejects \"" << input << "\"\n";
	}

	return 0;
}

bool automaton(std::string file, std::string input) {
	try {
		Automaton aut(file);
		return aut.ProcessInput(input);
	} catch (...) {
		std::cout << "** Error occurred :(\n";
		return false;
	}
}

// Automaton function implementations

Automaton::Automaton(std::string file_name) { parse_file(file_name); }

Automaton::~Automaton() {}

bool Automaton::ProcessInput(std::string input) {
	std::string cleaned_input = clean_input(input);
	return run(cleaned_input);
}

void Automaton::parse_file(std::string file_name) {
	std::ifstream input_file(file_name);
	if (!input_file.is_open()) {
		// idealy would have thrown a proper exception but cant :(
		std::cout << "** Error: Could not open " << file_name << '\n';
		throw("");
	}

	FileParseState state    = Alphabet;
	bool           kw_found = false;
	num_transitions_        = 0;
	for (std::string line; std::getline(input_file, line);) {
		switch (state) {
			case Alphabet:
				if (line == kKwAlphabet) {
					kw_found = true;
				} else if (kw_found) {
					alphabet_ = line;
					std::cout << "Alphabet found: " << alphabet_ << '\n';
					kw_found = false;
					state    = States;
				} else {
					std::cout << "** Error parsing Alphabet\n";
				}
				break;

			case States:
				if (line == kKwStates) {
					kw_found = true;
				} else if (kw_found) {
					states_ = line;
					std::cout << "States found: " << states_ << '\n';
					kw_found = false;
					state    = Initial;
				} else {
					std::cout << "** Error parsing States\n";
				}
				break;

			case Initial:
				if (line == kKwInitial) {
					kw_found = true;
				} else if (kw_found) {
					initial_ = line;
					std::cout << "Initial found: " << initial_ << '\n';
					kw_found = false;
					state    = Final;
				} else {
					std::cout << "** Error parsing Initial\n";
				}
				break;

			case Final:
				if (line == kKwFinal) {
					kw_found = true;
				} else if (kw_found) {
					final_ = line;
					std::cout << "Final found: " << final_ << '\n';
					kw_found = false;
					state    = Transitions;
				} else {
					std::cout << "** Error parsing Final\n";
				}
				break;

			case Transitions:
				if (line == kKwTransitions) {
					kw_found = true;
				} else if (kw_found) {
					transitions_[num_transitions_++] = line;
					std::cout << "Transition found: " << line << '\n';
				} else {
					std::cout << "** Error parsing Transitions\n";
				}
				break;

			default:
				break;
		}
	}
	if (state != Transitions) {
		std::cout << "** Error parsing file, missing keywords. Stuck at " << std::to_string(state) << '\n';
		throw("");
	}
}

std::string Automaton::clean_input(std::string input) {
	std::cout << "Input received: \"" << input << "\"\n";

	std::string cleaned_input;
	for (char i : input) {
		// check if the input is a number or letter
		// letters are case sensitive
		if (
			(i >= '0' && i <= '9') || //
			(i >= 'A' && i <= 'Z') || //
			(i >= 'a' && i <= 'z')) {
			std::cout << "Adding to input: \"" << std::string(1, i) << "\"\n";
			cleaned_input += i;
		} else {
			std::cout << "** Ignoring \"" << std::string(1, i) << "\", input must be 0-9, a-z or A-Z\n";
		}
	}
	std::cout << "Clean input: " << cleaned_input << '\n';
	return cleaned_input;
}

bool Automaton::run(std::string input) {
	std::string state = initial_;
	std::string letter;
	// , end, input;
	Transition transition;
	// Transition transition = process_transition(transitions_[0]);
	for (char l : input) {
		letter = std::string(1, l);
		std::cout << "Processing letter: " << letter << '\n';
		// check if letter is in alphabet
		if (!exists_in_set(letter, alphabet_)) {
			std::cout << "** Error: Letter " << letter << " does not exist in the alphabet\n";
			return false;
		}
		// ideally done during file parsing but i dont want to deal with all the string arrays
		// using vectors here would be more appropriate
		for (size_t i = 0; i < num_transitions_; ++i) {
			transition = process_transition(transitions_[i]);
			// make the transition if possible
			if (state == transition.from && letter == transition.input) {
				// check if to state exists
				if (!exists_in_set(transition.to, states_)) {
					std::cout << "** Error: State " << transition.to << " does not exist\n";
					return false;
				}
				// make the transition
				std::cout << "Transitioning from " << transition.from << " to " << transition.to << '\n';
				state = transition.to;
				break;
			}
		}
	}

	// check if state is in final
	if (exists_in_set(state, final_)) {
		return true;
	} else {
		std::cout << "** Error: State " << transition.to << " is not a final state\n";
		return false;
	}
}

Transition Automaton::process_transition(std::string transition) {
	std::string from  = transition.substr(0, transition.find(kKwTransitionFrom));
	std::string input = transition.substr(
		from.length() + kKwTransitionFrom.length(),
		transition.find(kKwTransitionTo) - (from.length() + kKwTransitionFrom.length()));
	std::string to = transition.substr(
		from.length() + kKwTransitionFrom.length() + input.length() + kKwTransitionTo.length(),
		transition.length() - (from.length() + kKwTransitionFrom.length() + input.length() + kKwTransitionTo.length()));
	return {from, input, to};
}

bool Automaton::exists_in_set(std::string state, std::string set) {
	size_t      pos = 0;
	std::string state_in_set;
	while ((pos = set.find(kKwDelim)) != std::string::npos) {
		state_in_set = set.substr(0, pos);
		if (state == state_in_set) return true;
		set.erase(0, pos + kKwDelim.length());
	}

	// check if the remaining string of set has the final state
	if (state == set) {
		return true;
	} else {
		return false;
	}
}
