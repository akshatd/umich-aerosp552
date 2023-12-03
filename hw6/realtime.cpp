#include <iostream>
#include <string>
#include <cmath>

// have to use this weird array size helper because vectors arent allowed :(
#define ARRAY_LENGTH(array) (sizeof((array)) / sizeof((array)[0]))
#define UINT_MAX            0xffffffff // have to use this because limits is not allowed :(
#define SIZE_T_MAX          0xffff     // have to use this because limits is not allowed :(

// task struct
struct Task {
		std::string name;
		uint        period;
		uint        deadline;
		uint        work_length;
		uint        work_remaining;
		bool        done;

		Task(std::string t_name, uint t_period, uint t_work_length) {
			name           = t_name;
			period         = t_period;
			deadline       = t_period;
			work_length    = t_work_length;
			work_remaining = t_work_length;
			done           = false;
		}

		void run() {
			work_remaining--;
			if (work_remaining == 0) {
				work_remaining = work_length;
				deadline += period;
				done = true;
			}
		}

		void restart() { done = false; }

		void reset() {
			deadline       = period;
			work_remaining = work_length;
			done           = false;
		}

		// tasks are printed as name(period, work_length)
		friend std::ostream& operator<<(std::ostream& os, const Task& t) {
			os << t.name << "(" << t.period << ", " << t.work_length << ")";
			return os;
		}
};

enum SchedulerType { RM, EDF };

// runs the scheduler on the tasks with a specific scheduling algorithm
void runScheduler(Task tasks[], size_t num_tasks, uint total_time, SchedulerType type);

// returns the index of next task based on Rate Monotonic scheduling
size_t nextTaskRM(Task tasks[], size_t num_tasks);

// returns the index of next task based on Earliest Deadline First scheduling
size_t nextTaskEDF(Task tasks[], size_t num_tasks);

int main(void) {
	std::cout << "--- Test 0 (class ex 1): fail with RM ---\n";
	Task test0[] = {
		Task("T1", 4, 1),
		Task("T2", 5, 2),
		Task("T3", 7, 2),
	};
	runScheduler(test0, ARRAY_LENGTH(test0), 15, RM);
	runScheduler(test0, ARRAY_LENGTH(test0), 15, EDF);

	std::cout << "\n--- Test 1 (class ex 2, overscheduled): fail ---\n";
	Task test1[] = {
		Task("T1", 4, 3),
		Task("T2", 5, 3),
		Task("T3", 6, 3),
		Task("T3", 7, 3),
	};
	runScheduler(test1, ARRAY_LENGTH(test1), 15, RM);
	runScheduler(test1, ARRAY_LENGTH(test1), 15, EDF);

	std::cout << "\n--- Test 2 (underscheduled): pass with idle ---\n";
	Task test2[] = {
		Task("T1", 4, 2),
		Task("T2", 5, 1),
	};
	runScheduler(test2, ARRAY_LENGTH(test2), 10, RM);
	runScheduler(test2, ARRAY_LENGTH(test2), 10, EDF);

	std::cout << "\n--- Test 3 (one task is very taxing, 100% utilization): pass ---\n";
	Task test3[] = {
		Task("T1", 3, 2),
		Task("T2", 9, 3),
	};
	runScheduler(test3, ARRAY_LENGTH(test3), 15, RM);
	runScheduler(test3, ARRAY_LENGTH(test3), 15, EDF);

	std::cout << "\n--- Test 4 (one task): pass ---\n";
	Task test4[] = {
		Task("T1", 5, 5),
	};
	runScheduler(test4, ARRAY_LENGTH(test4), 10, RM);
	runScheduler(test4, ARRAY_LENGTH(test4), 10, EDF);

	return 0;
}

void runScheduler(Task tasks[], size_t num_tasks, uint total_time, SchedulerType type) {
	uint   time          = 1; // start from 1 so the tasks execute at t=1
	size_t next_task_idx = 0;
	std::cout << "Scheduling algorithm: " << (type == RM ? "Rate Monotonic" : "Earliest Deadline First") << '\n';
	std::cout << "Total time          : " << total_time << '\n';
	std::cout << "Tasks(" << num_tasks << ")            : ";
	double utilization = 0;
	for (size_t i = 0; i < num_tasks; i++) {
		utilization += static_cast<double>(tasks[i].work_length) / static_cast<double>(tasks[i].period);
		std::cout << tasks[i];
		if (i != num_tasks - 1) {
			std::cout << ", ";
		} else {
			std::cout << '\n';
		}
	}
	double utilization_limit = 1;
	double n                 = static_cast<double>(num_tasks);
	if (type == RM) {
		utilization_limit = n * (std::pow(2., (1. / n)) - 1);
	}
	std::cout << "Utilization         : " << utilization << "/" << utilization_limit;

	if (utilization > utilization_limit) {
		if (type == RM) std::cout << " *** Warning: The system MAY NOT be schedulable ***\n";
		if (type == EDF) std::cout << " *** Warning: The system is NOT schedulable ***\n";
	} else {
		std::cout << '\n';
	}

	std::cout << "Time\tTask name(period, work_remaining/work_length)\t(* = work done)\n";
	while (time <= total_time) {
		// find the next task to run
		if (type == RM) {
			next_task_idx = nextTaskRM(tasks, num_tasks);
		} else if (type == EDF) {
			next_task_idx = nextTaskEDF(tasks, num_tasks);
		}

		// check if there is any task to be run
		if (next_task_idx != SIZE_T_MAX) {
			// run the task
			tasks[next_task_idx].run();

			// print the time and the tasks with progress bar
			std::cout << time << '\t' << tasks[next_task_idx] << " [";
			// print progress bar
			if (tasks[next_task_idx].done) {
				for (size_t i = 0; i < tasks[next_task_idx].work_length; i++) {
					std::cout << "*";
				}
			} else {
				for (size_t i = 0; i < tasks[next_task_idx].work_length - tasks[next_task_idx].work_remaining; i++) {
					std::cout << "*";
				}
				for (size_t i = 0; i < tasks[next_task_idx].work_remaining; i++) {
					std::cout << "-";
				}
			}
			std::cout << "]\n";
		} else {
			std::cout << time << '\t' << "idle\n";
		}

		// reset if time is a modulus of the period
		for (size_t i = 0; i < num_tasks; i++) {
			if (time % tasks[i].period == 0) {
				// report if the task missed its deadline
				if (!tasks[i].done) {
					std::cout << "*** " << tasks[i].name << " missed its deadline ***\n";
				}
				tasks[i].restart();
			}
		}
		++time;
	}

	// reset all tasks so they can be reused
	for (size_t i = 0; i < num_tasks; i++) {
		tasks[i].reset();
	}
}

size_t nextTaskRM(Task tasks[], size_t num_tasks) {
	size_t smallest_period_idx = SIZE_T_MAX;
	uint   smallest_period     = UINT_MAX;
	for (size_t i = 0; i < num_tasks; i++) {
		// take the smallest period that is not done
		if (!tasks[i].done && tasks[i].period < smallest_period) {
			smallest_period     = tasks[i].period;
			smallest_period_idx = i;
		}
	}
	return smallest_period_idx;
}

size_t nextTaskEDF(Task tasks[], const size_t num_tasks) {
	size_t earliest_deadline_idx = SIZE_T_MAX;
	uint   earliest_deadline     = UINT_MAX;
	for (size_t i = 0; i < num_tasks; i++) {
		// take the earliest deadline that is not done
		if (!tasks[i].done && tasks[i].deadline < earliest_deadline) {
			earliest_deadline     = tasks[i].deadline;
			earliest_deadline_idx = i;
		}
	}
	return earliest_deadline_idx;
}
