#include <iostream>

constexpr size_t kNumBoatStates = 2;
// yes this is a magic number for the length of the array that keeps track of the visited states but im not allowed to
// use a hashmap to keep track of all the IDs that have been visited so i have to resort to this kinda thing.
// i cant really implement a hashmap or a vector from scratch just for this homework
constexpr size_t kMaxStates = 1024;

struct State {
		uint num_missionary[kNumBoatStates];
		uint num_cannibal[kNumBoatStates];
		bool boat_loc;
		char padding_unused[3]; // padding to align struct
};

std::ostream &operator<<(std::ostream &os, const State &state) {
	os << "A: " << state.num_missionary[0] << "M " << state.num_cannibal[0] << "C / B: " << state.num_missionary[1]
		 << "M " << state.num_cannibal[1] << "C / Boat: " << ((state.boat_loc) ? "B" : "A");
	return os;
}

bool solveMandSProblem(const State initial_state, State &final_state, State visited[], size_t &len_visited);
bool isStateValid(const State state);
bool isProblemSolved(const State state);
bool isVisited(const State state, const State visited[], const size_t len_visited);

int main(void) {
	State  initial_state{3, 0, 3, 0, 0, 0};
	State  visited[kMaxStates];
	size_t len_visited = 0;
	std::cout << "Welcome to Missionaries and Cannibals, starting with " << initial_state << '\n';
	State final_state;
	bool  is_solved = solveMandSProblem(initial_state, final_state, visited, len_visited);
	std::cout << "\nSolution for the problem found: " << is_solved << " with final state: " << final_state << '\n';
	return 0;
}

bool solveMandSProblem(const State initial_state, State &final_state, State visited[], size_t &len_visited) {
	// std::cout << "Visiting " << initial_state << '\n';
	if (isProblemSolved(initial_state)) {
		std::cout << "Solved, printing states and actions in reverse:\n";
		std::cout << "State " << initial_state << '\n';
		final_state = initial_state;
		return true;
	}

	if (isVisited(initial_state, visited, len_visited)) {
		// std::cout << "Already visited\n";
		return false;
	}

	// visit this state
	visited[len_visited] = initial_state;
	++len_visited;

	if (!isStateValid(initial_state)) {
		// std::cout << "Invalid state\n";
		return false;
	}

	// try all possible actions
	State new_state;
	bool  result;
	if (initial_state.num_missionary[initial_state.boat_loc] >= 1) {
		// std::cout << "Action 1, move 1 missionary\n";
		new_state = initial_state;
		new_state.num_missionary[new_state.boat_loc] -= 1;
		new_state.num_missionary[!new_state.boat_loc] += 1;
		new_state.boat_loc = !new_state.boat_loc;
		result             = solveMandSProblem(new_state, final_state, visited, len_visited);
		if (result) {
			std::cout << "State " << initial_state << " , Action: 1, move 1 missionary\n";
			return true;
		}
	}

	if (initial_state.num_cannibal[initial_state.boat_loc] >= 1) {
		// std::cout << "Action 2, move 1 cannibal\n";
		new_state = initial_state;
		new_state.num_cannibal[new_state.boat_loc] -= 1;
		new_state.num_cannibal[!new_state.boat_loc] += 1;
		new_state.boat_loc = !new_state.boat_loc;
		result             = solveMandSProblem(new_state, final_state, visited, len_visited);
		if (result) {
			std::cout << "State " << initial_state << " , Action: 2, move 1 cannibal\n";
			return true;
		}
	}

	if (initial_state.num_missionary[initial_state.boat_loc] >= 2) {
		// std::cout << "Action 3, move 2 missionaries\n";
		new_state = initial_state;
		new_state.num_missionary[new_state.boat_loc] -= 2;
		new_state.num_missionary[!new_state.boat_loc] += 2;
		new_state.boat_loc = !new_state.boat_loc;
		result             = solveMandSProblem(new_state, final_state, visited, len_visited);
		if (result) {
			std::cout << "State " << initial_state << " , Action: 3, move 2 missionaries\n";
			return true;
		}
	}

	if (initial_state.num_cannibal[initial_state.boat_loc] >= 2) {
		// std::cout << "Action 4, move 2 cannibals\n";
		new_state = initial_state;
		new_state.num_cannibal[new_state.boat_loc] -= 2;
		new_state.num_cannibal[!new_state.boat_loc] += 2;
		new_state.boat_loc = !new_state.boat_loc;
		result             = solveMandSProblem(new_state, final_state, visited, len_visited);
		if (result) {
			std::cout << "State " << initial_state << " , Action: 4, move 2 cannibals\n";
			return true;
		}
	}

	if (
		initial_state.num_missionary[initial_state.boat_loc] >= 1 && initial_state.num_cannibal[initial_state.boat_loc] >= 1) {
		// std::cout << "Action 5, move 1 missionary, 1 cannibal\n";
		new_state = initial_state;
		new_state.num_missionary[new_state.boat_loc] -= 1;
		new_state.num_cannibal[new_state.boat_loc] -= 1;
		new_state.num_missionary[!new_state.boat_loc] += 1;
		new_state.num_cannibal[!new_state.boat_loc] += 1;
		new_state.boat_loc = !new_state.boat_loc;
		result             = solveMandSProblem(new_state, final_state, visited, len_visited);
		if (result) {
			std::cout << "State " << initial_state << " , Action: 5, move 1 missionary, 1 cannibal\n";
			return true;
		}
	}

	return false;
}

bool isStateValid(const State state) {
	return (state.num_missionary[0] >= state.num_cannibal[0] || state.num_missionary[0] == 0) &&
	       (state.num_missionary[1] >= state.num_cannibal[1] || state.num_missionary[1] == 0);
}

bool isProblemSolved(const State state) { return state.num_missionary[1] == 3 && state.num_cannibal[1] == 3; }

bool isVisited(const State state, const State visited[], const size_t len_visited) {
	for (size_t i = 0; i < len_visited && i < kMaxStates; ++i) {
		if (
			state.num_missionary[0] == visited[i].num_missionary[0] &&
			state.num_missionary[1] == visited[i].num_missionary[1] && state.num_cannibal[0] == visited[i].num_cannibal[0] &&
			state.num_cannibal[1] == visited[i].num_cannibal[1] && state.boat_loc == visited[i].boat_loc) {
			return true;
		}
	}
	return false;
}
