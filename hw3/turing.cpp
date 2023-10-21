#include <fstream>
#include <iostream>
#include <string>

#include "turing.h"

const std::string kTMDefinition = "lowercase.dat";

std::string turing(std::string file, std::string input);

int main() {
	// test cases
	// std::cout << " -- TESTS -- \n";
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
	std::cout << turing(kTMDefinition, input) << '\n';

	return 0;
}

std::string turing(std::string file, std::string input) {
	try {
		TuringMachine tm(file);
		return std::string("Output from Turing Machine: ") + tm.ProcessInput(input);
	} catch (...) {
		std::cout << "** Error occurred :(\n";
		return std::string("Output from Turing Machine: ") + kOutputReject;
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
						// std::cout << "Alphabet found: " << alphabet_ << '\n';
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
						tape_alphabet_ = line;
						// std::cout << "Tape Alphabet found: " << tape_alphabet_ << '\n';
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
						// std::cout << "States found: " << states_ << '\n';
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
						std::cout << "** Error parsing Inital state, cannot have multiple\n";
						can_continue = false;
					} else {
						initial_ = line;
						// std::cout << "Initial state found: " << initial_ << '\n';
						if (!exists_in_set(initial_, states_)) {
							std::cout << "** Error: Initial state " << initial_ << " does not exist in states\n";
							can_continue = false;
						} else {
							kw_found = false;
							state    = Accept;
						}
					}
				} else {
					std::cout << "** Error parsing Initial state\n";
					can_continue = false;
				}
				break;

			case Accept:
				if (line == kKwAccept) {
					kw_found = true;
				} else if (kw_found) {
					accept_ = line;
					// std::cout << "Accept state found: " << accept_ << '\n';
					kw_found = false;
					state    = Reject;
				} else {
					std::cout << "** Error parsing Accept state\n";
					can_continue = false;
				}
				break;

			case Reject:
				if (line == kKwReject) {
					kw_found = true;
				} else if (kw_found) {
					reject_ = line;
					// std::cout << "Reject state found: " << reject_ << '\n';
					kw_found = false;
					state    = Transitions;
				} else {
					std::cout << "** Error parsing Reject state\n";
					can_continue = false;
				}
				break;

			case Transitions:
				if (line == kKwTransitions) {
					kw_found = true;
				} else if (kw_found) {
					transitions_[num_transitions_++] = line;
					// std::cout << "Transition found: " << line << '\n';
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
	std::string state    = initial_;
	size_t      tape_pos = 0;
	std::string letter;
	Transition  transition;

	while (!exists_in_set(state, reject_) && !exists_in_set(state, accept_)) {
		// if tape position exceeds the length, add a space to simulate infinite spaces
		if (tape_pos > input.size() - 1) input += "w";
		letter = input.at(tape_pos);
		std::cout << "Processing letter: " << letter << " at position " << tape_pos << '\n';
		// check if letter is in alphabet
		if (!exists_in_set(letter, alphabet_) && !exists_in_set(letter, tape_alphabet_)) {
			std::cout << "** Error: Processing Letter: " << letter << " does not exist in the alphabet\n";
			return kOutputReject;
		}
		// ideally done during file parsing but i dont want to deal with all the string arrays
		// using vectors here would be more appropriate
		for (size_t i = 0; i < num_transitions_; ++i) {
			transition = process_transition(transitions_[i]);
			// make the transition if possible
			if (letter == transition.input && state == transition.from) {
				// check if to state exists
				if (!exists_in_set(transition.to, states_)) {
					std::cout << "** Error: State " << transition.to << " does not exist\n";
					return kOutputReject;
				}
				// check if trying to move past left end marker
				if (tape_pos == 0 && transition.move == kMoveLeft) {
					std::cout << "** Error: Trying to transition left of left end marker\n";
					return kOutputReject;
				}
				// check if trying to overwrite left end marker
				if (tape_pos == 0 && transition.output != kKwLeftMarker) {
					std::cout << "** Error: Trying to overwrite left end marker\n";
					return kOutputReject;
				}
				// check if trying to write something not part of the full alphabet
				if (!exists_in_set(transition.output, alphabet_) && !exists_in_set(transition.output, tape_alphabet_)) {
					std::cout << "** Error: Trying to write " << transition.output << " which is not part of the alphabet\n";
					return kOutputReject;
				}

				// write to tape
				std::cout << "Writing " << transition.output << " to tape at position " << tape_pos << '\n';
				input.replace(tape_pos, transition.output.size(), transition.output);
				// move
				if (transition.move == kMoveLeft) {
					std::cout << "Moving left\n";
					tape_pos--;
				} else if (transition.move == kMoveRight) {
					std::cout << "Moving right\n";
					tape_pos++;
				} else {
					std::cout << "** Error: Move " << transition.move << " is not a valid move\n";
					return kOutputReject;
				}
				// make the transition
				std::cout << "Transitioning from " << transition.from << " to " << transition.to << '\n';
				state = transition.to;

				break;
			}
		}
	}

	// check if state is in accept
	if (!exists_in_set(state, accept_)) {
		std::cout << "** Error: State " << transition.to << " is not an accepting state\n";
		return kOutputReject;
	} else {
		return input;
	}
}

Transition TuringMachine::process_transition(std::string transition) {
	// find from
	std::string from = transition.substr(0, transition.find(kKwTransitionFrom));
	transition.erase(0, from.length() + kKwTransitionFrom.length());
	// find input
	std::string input = transition.substr(0, transition.find(kKwTransitionDelim));
	transition.erase(0, input.length() + kKwTransitionDelim.length());
	// find output
	std::string output = transition.substr(0, transition.find(kKwTransitionDelim));
	transition.erase(0, output.length() + kKwTransitionDelim.length());
	// find move
	std::string move = transition.substr(0, transition.find(kKwTransitionTo));
	transition.erase(0, move.length() + kKwTransitionTo.length());
	// find to
	std::string to = transition;
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
