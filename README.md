# Emergency-service-project

# Description

This C project simulates a 112 emergency dispatch system that prioritizes incidents (high, medium, low) and allocates response units. Using doubly linked lists, priority queues, and a dispatch history stack, it manages incident tracking, dispatching, undo actions, and system inspection via file I/O (tema1.in/tema1.out).

# Data Structures Used

In this project, several data structures were used to store information about each incident, crew, intervention, and overall system state, using dynamic data structures for efficient memory allocation. Active incidents and interventions are stored using sentinel-node doubly linked lists (`incidents`, `last_incident`, `interventions`, `last_intervention`). Queues are also used: one for available crew units (`queue_available_units`) and three for priority-based incident management (`queue_high`, `queue_medium`, `queue_low`). Additionally, to enable undoing the last action and restoring the system to its previous state, a stack is used to store previous interventions (`prev_interventions`). All queues and stacks store `void*` pointers rather than copies of the `unit`, `incident`, or `intervention` structures.

Node structures created for these data structures include:

* `Node_1`: Incident list node
* `Node_2`: Intervention list node
* `Node_s_q`: Stack and queue node

# Operating Mechanism

Information is read from the input file `tema1.in` and outputted to `tema1.out`. The file contains the total number of units, details about each unit, followed by various commands and parameters where applicable:

1. `ADD_INCIDENT <id> <priority> <description>` – Adds a new incident to the system by invoking the `add_incident` function, which appends the incident to the list and places it into the queue corresponding to its priority (`low`, `medium`, `high`) using the `priority_check` function to evaluate urgency.
2. `CHECK_UNITS_AVAILABILITY` – Displays the number of currently available response units by calling the `check_available_units` function.
3. `DISPATCH` – Assigns the first available unit to the highest-priority pending incident from the priority queues and creates an intervention recorded in the history stack (`prev_interventions`). The incident status is updated to "intervened". Displays an error message if no units are available or if no pending incidents exist in the priority queues.
4. `UNDO_LAST_DISPATCH` – Reverts the most recent dispatch operation. The unit is returned to the available queue, and the incident is re-inserted at the front of its original queue with the status "queued". If the history stack is empty, an error message is displayed.
5. `SOLVED_INCIDENT <incident_id>` – Marks an incident currently in "intervened" status as "solved" and frees the assigned response unit, making it available again. Displays an error message if the ID does not exist or is not currently in "intervened" status.
6. `SHOW_UNIT <unit_id>` – Displays details about a specific unit (type and availability status). Displays an error message if the unit ID is not found.
7. `SHOW_INCIDENT <incident_id>` – Displays details about an incident (priority, description, status). Displays an error message if the incident ID is not found.
8. `SHOW_INTERVENTIONS` – Displays all initiated interventions and their current status. If no interventions exist, it outputs "No intervention has been initiated".

Additionally, dedicated functions manage the underlying data structures:

* **Queues**: `init_queue`, `enqueue`, `enqueue_first`, `dequeue`, `queue_is_empty`, `queue_free`
* **Lists**: `init_intervention`, `init_incident`, `add_incident`, `add_intervention`, `free_incidents`, `free_interventions`
* **Stack**: `init_stack`, `stack_is_empty`, `push`, `pop`, `free_stack`

# Compilation and Execution

To compile the program using `gcc`, run `make build` in the terminal. To run the program, execute `make run`.
