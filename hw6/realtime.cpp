#include <iostream>
#include <string>

// have to use this weird array size helper because vectors arent allowed :(
#define ARRAY_LENGTH(array) (sizeof((array)) / sizeof((array)[0]))
#define UINT_MAX            0xffffffff // have to use this because limits.h is not allowed :(

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

		// tasks are printed as name(period, work_remaining/work_length)
		friend std::ostream& operator<<(std::ostream& os, const Task& t) {
			os << t.name << "(" << t.period << ", " << t.work_remaining << "/" << t.work_length << ")" << (t.done ? "*" : "");
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
	Task test0[] = {
		Task("t1", 4, 1),
		Task("t2", 5, 2),
		Task("t3", 7, 2),
	};
	runScheduler(test0, ARRAY_LENGTH(test0), 10, RM);

	Task test1[] = {
		Task("t1", 4, 1),
		Task("t2", 5, 2),
		Task("t3", 7, 2),
	};
	runScheduler(test1, ARRAY_LENGTH(test1), 20, EDF);
	return 0;
}

void runScheduler(Task tasks[], size_t num_tasks, uint total_time, SchedulerType type) {
	uint   time          = 1; // start from 1 so the tasks execute at t=1
	size_t next_task_idx = 0;
	std::cout << "\nScheduling algorithm: " << (type == RM ? "Rate Monotonic" : "Earliest Deadline First") << '\n';
	std::cout << "Total time: " << total_time << '\n';
	std::cout << "Tasks(" << num_tasks << "): ";
	for (size_t i = 0; i < num_tasks; i++) {
		std::cout << tasks[i];
		if (i != num_tasks - 1) {
			std::cout << ", ";
		}
	}
	std::cout << "\nTime\tTask name(period, work_remaining/work_length)\t(* = task done)\n";
	while (time <= total_time) {
		// find the next task to run
		if (type == RM) {
			next_task_idx = nextTaskRM(tasks, num_tasks);
		} else if (type == EDF) {
			next_task_idx = nextTaskEDF(tasks, num_tasks);
		}

		// run the task
		tasks[next_task_idx].run();

		// print the time and the tasks
		std::cout << time << '\t' << tasks[next_task_idx] << '\n';

		// reset if time is a modulus of the period
		for (size_t i = 0; i < num_tasks; i++) {
			if (time % tasks[i].period == 0) {
				// report if the task missed its deadline
				if (!tasks[i].done) {
					std::cout << "** " << tasks[i] << " missed its deadline **\n";
				}
				tasks[i].restart();
			}
		}
		++time;
	}
}

size_t nextTaskRM(Task tasks[], size_t num_tasks) {
	size_t smallest_period_idx = 0;
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
	size_t earliest_deadline_idx = 0;
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
