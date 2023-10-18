#ifndef TURING_H
#define TURING_H

#include <fstream>
#include <iostream>
#include <string>

constexpr size_t kMaxTransitions = 1024;
// keywords and enums to parse file
const std::string kKwAlphabet        = "-- alphabet";
const std::string kKwTape            = "-- tape alphabet";
const std::string kKwStates          = "-- states";
const std::string kKwInitial         = "-- initial";
const std::string kKwAccept          = "-- accept";
const std::string kKwReject          = "-- reject";
const std::string kKwTransitions     = "-- transitions";
const std::string kKwTransitionFrom  = " -- ";
const std::string kKwTransitionDelim = ",";
const std::string kKwTransitionTo    = " --> ";
const std::string kKwDelim           = " ";
const std::string kKwBlankMarker     = "w";
const std::string kKwLeftMarker      = "E";
const std::string kMoveLeft          = "L";
const std::string kMoveRight         = "R";
const std::string kOutputReject      = "REJECT";

enum FileParseState {
	Alphabet,
	Tape,
	States,
	Initial,
	Accept,
	Reject,
	Transitions,
};

struct Transition {
		std::string from;
		std::string input;
		std::string output;
		std::string move;
		std::string to;
};

// class to store Turing Machine
class TuringMachine {
	public:
		TuringMachine(std::string file_name);
		~TuringMachine();

		std::string ProcessInput(std::string input);

	private:
		std::string alphabet_;
		std::string tape_;
		std::string states_;
		std::string initial_;
		std::string accept_;
		std::string reject_;
		std::string transitions_[kMaxTransitions];
		size_t      num_transitions_;

		void        parse_file(std::string file_name);
		std::string clean_input(std::string input);
		std::string run(std::string input);
		Transition  process_transition(std::string transition);
		bool        exists_in_set(std::string state, std::string set);
};

#endif