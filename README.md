This project has been created as part
of the 42 curriculum by lyanga.

# philosophers

### Description
The philosophers project aims to present a solution to the Dining Philosophers problem, given the project's restrictions.

With multithreading and mutex objects, the project implements a simulation where N philosophers alternate between eating, sleeping and thinking while sharing N forks.

The simulation has the following restrictions:
- Philosophers have access to only the forks on their left and right.
- Philosophers need 2 forks to eat.
- Forks cannot be taken away from a philosopher holding it.
- Philosophers do not communicate with one another.
- Philosophers do not know if another philosopher is about to die.

The project addresses the issues such as synchronisation, deadlocks, mutual exclusion, resource starvation, etc., that can be present in the Dining Philosophers problem.

The simulation ends when a philosopher dies (fails to eat within time_to_die) or when every philosopher has eaten the required number of times.

Given reasonable parameters, the simulation should not end with a dead philosopher.

### Instructions

#### Building
- Build the main program:
	- cd into the philo directory and run make:
		```
		cd philo
		make
		```

#### Usage
- Run the program from the philo directory:
	```
	./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
	```
	- Examples:
		- 5 philosophers, die after 800ms, eat 200ms, sleep 200ms:
		```
		./philo 5 800 200 200
		```
		- Same with minimum eats (e.g. 3):
		```
		./philo 5 800 200 200 3
		```

- Cleanup
	- To remove only objects:
		```
		cd philo
		make clean
		```
	- To remove objects and binary:
		```
		cd philo
		make fclean
		```

### Notes

Several measures were made to make ensure that the program does not result in deadlocks:

- Even-numbered philosophers are delayed at the start of the simulation (to ensure odd-numbered philosophers get the forks first)
- Even-numbered philosophers will pick up the right fork first, while odd-numbered philosophers will pick up the left fork first.
- If time_to_eat is longer than time_to_sleep, then the philosopher will spend the difference in time while in the thinking state.
- Philosophers will spend a portion of their remaining time (before death) thinking.

### Resources
- The Dining Philosophers problem wiki page: https://en.wikipedia.org/wiki/Dining_philosophers_problem
- nafuka11's philosopher visualiser: https://nafuka11.github.io/philosophers-visualizer/