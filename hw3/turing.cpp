#include <fstream>
#include <iostream>
#include <string>

#include "turing.h"

const std::string kTMDefinition = "turing.txt";

std::string turing(std::string file, std::string input);

int main() {
	// test cases
	std::cout << " -- TESTS -- \n";
	// std::cout << "Expected 1: Accepted by automaton: " << automaton(kTMDefinition, "aa") << '\n';
	// std::cout << "Expected 1: Accepted by automaton: " << automaton(kTMDefinition, "bb") << '\n';
	// std::cout << "Expected 0: Accepted by automaton: " << automaton(kTMDefinition, "ab") << '\n';
	// std::cout << "Expected 1: Accepted by automaton: " << automaton(kTMDefinition, "bbbbabbbbba") << '\n';
	// std::cout << "Expected 1: Accepted by automaton: " << automaton(kTMDefinition, "`;'aa[]") << '\n';
	// std::cout << "Expected 1: Accepted by automaton: " << automaton("file1.txt", "aa") << '\n';
	// std::cout << "Expected 0: Accepted by automaton: " << automaton("file2.txt", "aa") << '\n';
	// std::cout << "Expected 0: Accepted by automaton: " << automaton("file3.txt", "aa") << '\n';
	// std::cout << "Expected 0: Accepted by automaton: " << automaton("file4.txt", "aa") << '\n';
	// std::cout << "Expected 0: Accepted by automaton: " << automaton("file5.txt", "aa") << '\n';

	std::string input;
	while (true) {
		std::cin.clear();
		std::cout << "-- USER INPUT -- \n";
		std::cout << "Enter input: ";
		if (std::cin >> input) {
			break;
		} else {
			std::cout << "** Error, bad input\n";
			std::cin.clear();
			std::cin.ignore();
		}
	}
	std::cout << "Accepted by automaton: " << turing(kTMDefinition, input) << '\n';

	return 0;
}

std::string turing(std::string file, std::string input) {
	try {
		TuringMachine tm(file);
		return tm.ProcessInput(input);
	} catch (...) {
		std::cout << "** Error occurred :(\n";
		return kOutputReject;
	}
}

// Automaton function implementations

TuringMachine::TuringMachine(std::string file_name) { parse_file(file_name); }

TuringMachine::~TuringMachine() {}

std::string TuringMachine::ProcessInput(std::string input) {
	std::string cleaned_input = clean_input(input);
	return run(cleaned_input);
}

void TuringMachine::parse_file(std::string file_name) {
	std::ifstream input_file(file_name);
	if (!input_file.is_open()) {
		// idealy would have thrown a proper exception but cant :(
		std::cout << "** Error: Could not open " << file_name << '\n';
		throw("");
	}

	FileParseState state    = Alphabet;
	bool           kw_found = false;
	num_transitions_        = 0;
	bool can_continue       = true;
	for (std::string line; std::getline(input_file, line) && can_continue;) {
		switch (state) {
			case Alphabet:
				if (line == kKwAlphabet) {
					kw_found = true;
				} else if (kw_found) {
					if (line.empty()) {
						std::cout << "** Error parsing Alphabet, cannot be empty\n";
						can_continue = false;
					} else {
						alphabet_ = line;
						std::cout << "Alphabet found: " << alphabet_ << '\n';
						kw_found = false;
						state    = Tape;
					}
				} else {
					std::cout << "** Error parsing Alphabet\n";
					can_continue = false;
				}
				break;

			case Tape:
				if (line == kKwTape) {
					kw_found = true;
				} else if (kw_found) {
					if (line.empty()) {
						std::cout << "** Error parsing Tape Alphabet, cannot be empty\n";
						can_continue = false;
					} else {
						tape_ = line;
						std::cout << "Tape Alphabet found: " << tape_ << '\n';
						kw_found = false;
						state    = States;
					}
				} else {
					std::cout << "** Error parsing Tape Alphabet\n";
					can_continue = false;
				}
				break;

			case States:
				if (line == kKwStates) {
					kw_found = true;
				} else if (kw_found) {
					if (line.empty()) {
						std::cout << "** Error parsing States, cannot be empty\n";
						can_continue = false;
					} else {
						states_ = line;
						std::cout << "States found: " << states_ << '\n';
						kw_found = false;
						state    = Initial;
					}
				} else {
					std::cout << "** Error parsing States\n";
					can_continue = false;
				}
				break;

			case Initial:
				if (line == kKwInitial) {
					kw_found = true;
				} else if (kw_found) {
					if (line.find(kKwDelim) != std::string::npos) {
						std::cout << "** Error parsing Inital, cannot have multiple\n";
						can_continue = false;
					} else {
						initial_ = line;
						std::cout << "Initial found: " << initial_ << '\n';
						if (!exists_in_set(initial_, states_)) {
							std::cout << "** Error: State " << initial_ << " does not exist in states\n";
							can_continue = false;
						} else {
							kw_found = false;
							state    = Accept;
						}
					}
				} else {
					std::cout << "** Error parsing Initial\n";
					can_continue = false;
				}
				break;

			case Accept:
				if (line == kKwAccept) {
					kw_found = true;
				} else if (kw_found) {
					accept_ = line;
					std::cout << "Accept state found: " << accept_ << '\n';
					kw_found = false;
					state    = Reject;
				} else {
					std::cout << "** Error parsing Accept\n";
					can_continue = false;
				}
				break;

			case Reject:
				if (line == kKwReject) {
					kw_found = true;
				} else if (kw_found) {
					reject_ = line;
					std::cout << "Reject state found: " << reject_ << '\n';
					kw_found = false;
					state    = Transitions;
				} else {
					std::cout << "** Error parsing Reject\n";
					can_continue = false;
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
					can_continue = false;
				}
				break;

			default:
				break;
		}
	}
	if (state != Transitions || !can_continue) {
		std::cout << "** Error parsing file, maybe missing keywords? Stuck at " << std::to_string(state) << '\n';
		throw("");
	}
}

std::string TuringMachine::clean_input(std::string input) {
	// std::cout << "Input received: \"" << input << "\"\n";

	std::string cleaned_input;
	for (char i : input) {
		// check if the input is a number or letter
		// letters are case sensitive
		if (
			(i >= '0' && i <= '9') || //
			(i >= 'A' && i <= 'Z') || //
			(i >= 'a' && i <= 'z')) {
			// std::cout << "Adding to input: \"" << std::string(1, i) << "\"\n";
			cleaned_input += i;
		} else {
			std::cout << "** Ignoring \"" << std::string(1, i) << "\", input must be 0-9, a-z or A-Z\n";
		}
	}
	// std::cout << "Clean input: " << cleaned_input << '\n';
	return cleaned_input;
}

std::string TuringMachine::run(std::string input) {
	std::string state = initial_;
	std::string letter;
	// , end, input;
	Transition transition;
	// Transition transition = process_transition(transitions_[0]);
	for (char l : input) {
		letter = std::string(1, l);
		// std::cout << "Processing letter: " << letter << '\n';
		// check if letter is in alphabet
		if (!exists_in_set(letter, alphabet_)) {
			std::cout << "** Error: Letter " << letter << " does not exist in the alphabet\n";
			return kOutputReject;
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
					return kOutputReject;
				}
				// make the transition
				// std::cout << "Transitioning from " << transition.from << " to " << transition.to << '\n';
				state = transition.to;
				break;
			}
		}
	}

	// check if state is in final
	if (exists_in_set(state, reject_)) {
		std::cout << "** Error: State " << transition.to << " is not a final state\n";
		return kOutputReject;
	} else {
		return tape_;
	}
}

Transition TuringMachine::process_transition(std::string transition) {
	// find from
	size_t      pos  = 0;
	std::string from = transition.substr(pos, transition.find(kKwTransitionFrom));
	// find input
	pos += from.length() + kKwTransitionFrom.length();
	std::string input = transition.substr(pos, transition.find(kKwTransitionDelim) - pos);
	// find output
	pos += input.length() + kKwTransitionDelim.length();
	std::string output = transition.substr(pos, transition.find(kKwTransitionDelim) - pos);
	// find move
	pos += output.length() + kKwTransitionDelim.length();
	std::string move = transition.substr(pos, transition.find(kKwTransitionTo) - pos);
	// find to
	pos += move.length() + kKwTransitionTo.length();
	std::string to = transition.substr(pos, transition.length() - pos);
	return {clean_input(from), clean_input(input), clean_input(output), clean_input(move), clean_input(to)};
}

bool TuringMachine::exists_in_set(std::string state, std::string set) {
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
