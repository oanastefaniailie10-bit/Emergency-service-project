/*ILIE Oana Ștefania*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct unit{
	int id;
	char type;
	int availability;
}unit;

typedef struct incident{
	int id;
	char priority[7];
	char *description;
	char status[11];
}incident;

typedef struct intervention{
	incident *incident;
	unit *unit;
}intervention;

typedef struct Node_incidents {
	incident *incident;
	struct Node_incidents *next, *prev;
}Node_1;

typedef struct Node_interventions {
	intervention *intervention;
	struct Node_interventions *next, *prev;
}Node_2;

typedef struct Node_stack_and_queue{
	void *p;
	struct Node_stack_and_queue *next;
}Node_s_q;

typedef struct System{
	unit *units;
	Node_1 *incidents;
	Node_2 *interventions;
}System;

typedef struct Queue{
	Node_s_q *head, *tail;
	int len;
}Queue;

typedef struct Stack{
	Node_s_q *head, *top;
	int len;
}Stack;

void init_queue(Queue **q)
{
	*q = malloc(sizeof(Queue));
	if (!(*q))
		return;
	Node_s_q *sentinel = malloc(sizeof(Node_s_q));
	if (!sentinel) {
		return;
	}
	sentinel->p = NULL;
	sentinel->next = NULL;

	(*q)->head = sentinel;
	(*q)->tail = sentinel;
	(*q)->len = 0;
}

int queue_is_empty(Queue *q)
{
	if (q->tail == q->head)
		return 1;
	return 0;
}

void enqueue(Queue *q, void *p)
{
	Node_s_q *elem = malloc(sizeof(Node_s_q));
	if(!elem)
		return;
	elem->p = p;
	elem->next = NULL;
	q->tail->next = elem;
	q->tail = elem;
	q->len = q->len + 1;
}

void enqueue_first(Queue *q, void *p)
{
	Node_s_q *elem = malloc(sizeof(Node_s_q));
	if(!elem)
		return;
	elem->p = p;
	elem->next = q->head->next;
	q->head->next = elem;
	if (q->tail == q->head)
		q->tail = elem;
	q->len = q->len + 1;
}

void dequeue(Queue *q)
{
	if (queue_is_empty(q))
		return;
	Node_s_q* elem = q->head->next;
	if (q->tail == elem) {
		q->tail = q->head;
		q->head->next = NULL;
	}
	else
		q->head->next = elem->next;
	q->len = q->len - 1;
	free(elem);
}

void queue_free(Queue **q)
{
	if (!q || !(*q))
		return;
	Node_s_q *elem = (*q)->head;
	while (elem)
	{
		Node_s_q* next = elem->next;
		free(elem);
		elem = next;
	}
	free(*q);
	*q = NULL;
}

int stack_is_empty(Stack *s)
{
	if (s->head == s->top)
		return 1;
	return 0;
}

void init_stack(Stack **s)
{
	(*s) = malloc(sizeof(Stack));
	if (!(*s))
		return;
	(*s)->head = malloc(sizeof(Node_s_q));
	if (!(*s)->head)
	{
		free(*s);
		return;
	}
	(*s)->head->p = NULL;
	(*s)->head->next = NULL;
	(*s)->top = (*s)->head;
	(*s)->len = 0;
}

void push(Stack *s, intervention *n)
{
	Node_s_q *elem = malloc(sizeof(Node_s_q));
	elem->p = n;
	elem->next = NULL;
	s->len = s->len + 1;
	elem->next = s->top;
	s->top = elem;
}

void pop(Stack *s)
{
	if (stack_is_empty(s) == 1)
		return;
	Node_s_q *elem = s->top;
	s->len = s->len - 1;
	s->top = s->top->next;
	free(elem);
}

void stack_free(Stack **s)
{
	if (!s || !*s)
		return;
	Node_s_q* elem = (*s)->top;
	while (elem)
	{
		Node_s_q* next = elem->next;
		free(elem);
		elem = next;
	}
	free(*s);
	*s = NULL;
}

void init_incident(Node_1 **head, Node_1 **last)
{
	Node_1 *elem = malloc(sizeof(Node_1));
	if (!elem)
		return;
	elem->incident = malloc(sizeof(incident));
	elem->incident->id = 0;
	strcpy(elem->incident->priority, "low");
	elem->incident->description = strdup("test incident");
	strcpy(elem->incident->status, "solved");
	elem->prev = elem;
	elem->next = elem;
	(*head) = elem;
	(*last) = elem;
}

int priority_check(incident *elem)
{
	if (strcmp(elem->priority, "low") == 0)
		return 1;
	if (strcmp(elem->priority, "medium") == 0)
		return 2;
	return 3;
}

void add_incident(Node_1 **head, Node_1 **last, int id_incident, char *priority, char *description, Queue *queue_high, Queue *queue_medium, Queue *queue_low)
{
	Node_1 *elem = malloc(sizeof(Node_1));
	if (!elem)
		return;
	elem->incident = malloc(sizeof(incident));
	if(!elem->incident) {
		free(elem);
		return;
	}
	elem->incident->id = id_incident;
	strcpy(elem->incident->priority, priority);
	elem->incident->description = strdup(description);
	strcpy(elem->incident->status, "queued");

	elem->next = (*head);
	elem->prev = (*last);
	(*last)->next = elem;
	(*last) = (*last)->next;
	(*head)->prev = (*last);
	if(priority_check(elem->incident) == 1)
		enqueue(queue_low, elem->incident);
	else if (priority_check(elem->incident) == 2)
		enqueue(queue_medium, elem->incident);
	else
		enqueue(queue_high, elem->incident);
}

void init_intervention(Node_2 **head, Node_2 **last)
{
	Node_2 *elem = malloc(sizeof(Node_2));
	if (!elem)
		return;
	elem->intervention = NULL;
	elem->prev = elem;
	elem->next = elem;
	(*head) = elem;
	(*last) = elem;
}

void add_intervention(Node_2 **head, Node_2 **last, incident *p, unit *u, Stack *prev_interventions)
{
	Node_2 *elem = malloc(sizeof(Node_2));
	if (!elem)
		return;
	elem->intervention = malloc(sizeof(intervention));
	if(!elem->intervention)
		return;
	elem->intervention->incident = p;
	elem->intervention->unit = u;
	elem->next = (*head);
	elem->prev = (*last);
	(*last)->next = elem;
	(*last) = (*last)->next;
	(*head)->prev = (*last);
	push(prev_interventions, elem->intervention);
}

void check_units_availability(Queue *q, FILE *fout)
{
	fprintf(fout, "Number of available units: %d\n", q->len);
}

void dispatch(Queue *q, Queue *queue_high, Queue *queue_medium, Queue *queue_low, Node_2 **head, Node_2 **last, Stack *prev, FILE *fout)
{
	if (q->len != 0) {
		if (queue_high->len != 0) {
			incident *p = (incident *)queue_high->head->next->p;
			strcpy(p->status, "intervened");
			dequeue(queue_high);
			unit *u = (unit *)q->head->next->p;
			u->availability = 0;
			dequeue(q);
			add_intervention(head, last, p, u, prev);
		} else if (queue_medium->len != 0) {
			incident *p = (incident *)queue_medium->head->next->p;
			strcpy(p->status, "intervened");
			dequeue(queue_medium);
			unit *u = (unit *)q->head->next->p;
			u->availability = 0;
			dequeue(q);
			add_intervention(head, last, p, u, prev);
		} else if (queue_low->len != 0) {
			incident *p = (incident *)queue_low->head->next->p;
			strcpy(p->status, "intervened");
			dequeue(queue_low);
			unit *u = (unit *)q->head->next->p;
			u->availability = 0;
			dequeue(q);
			add_intervention(head, last, p, u, prev);
		} else
			fprintf(fout, "INVALID OPERATION! ERROR 404\n");
	} else
		fprintf(fout, "INVALID OPERATION! ERROR 404\n");
}

void undo_last_dispatch(Stack *prev, Queue *q, Queue *queue_high, Queue *queue_medium, Queue *queue_low, Node_2 **head, Node_2 **last, FILE *fout)
{
	if(stack_is_empty(prev)) {
		fprintf(fout, "INVALID OPERATION! ERROR 404\n");
		return;
	}
	Node_s_q *elem = prev->top;
	intervention *i= ((intervention *)elem->p);
	while(strcmp(i->incident->status, "solved") == 0) {
		pop(prev);
		if (stack_is_empty(prev)) {
			fprintf(fout, "INVALID OPERATION! ERROR 404\n");
			return;
		}
		elem = prev->top;
		if (!elem || !elem->p) return;
		i = ((intervention *)elem->p);
	}
	strcpy(i->incident->status, "queued");
	if (priority_check(i->incident) == 1)
		enqueue_first(queue_low, i->incident);
	else if (priority_check(i->incident) == 2)
		enqueue_first(queue_medium, i->incident);
		else
			enqueue_first(queue_high, i->incident);
	i->unit->availability = 1;
	enqueue(q, i->unit);
	Node_2 *aux = (*head)->next;
	while(aux != *head) {
		if (aux->intervention == i) {
			aux->prev->next = aux->next;
			aux->next->prev = aux->prev;
			if ((*last) == aux)
			(*last) = aux->prev;
			free(aux->intervention);
			free(aux);
			break;
		}
		aux = aux->next;
	}
	pop(prev);
}

void solved_incident(Node_2 **head, int id_incident, Queue *q, FILE *fout)
{
	Node_2 *elem = (*head)->next;
	Node_2 *aux = (*head);
	while (elem != aux && elem->intervention->incident->id != id_incident)
		elem = elem->next;
	if (elem == aux)
		fprintf(fout, "INVALID OPERATION! ERROR 404\n");
	else if (strcmp(elem->intervention->incident->status, "intervened") == 0) {
		strcpy(elem->intervention->incident->status, "solved");
		elem->intervention->unit->availability = 1;
		enqueue(q, elem->intervention->unit);
	} else
		fprintf(fout, "INVALID OPERATION! ERROR 404\n");
}

void show_unit(int id, int len_unit, unit *units, FILE *fout)
{
	int i, ok = 0;
	for (i = 0; i < len_unit; i++)
		if ((units+i)->id == id) {
			ok = 1;
			break;
		}
	if (ok == 0)
		fprintf(fout, "INVALID OPERATION! ERROR 404\n");
	else {
		fprintf(fout, "Unit %d ", (units + i)->id);
		fprintf(fout, "is type %c and is ", (units + i)->type);
		if(units[i].availability == 0)
			fprintf(fout, "unavailable\n");
		else
			fprintf(fout, "available\n");
	}
}

void show_incident(int id, Node_1 *head, FILE *fout)
{
	Node_1 *elem = head->next;
	while (elem != head && elem->incident->id != id)
		elem = elem->next;
	if (elem == head)
		fprintf(fout, "INVALID OPERATION! ERROR 404\n");
	else {
		fprintf(fout, "Incident %d ", elem->incident->id);
		fprintf(fout, "has %s priority, ", elem->incident->priority);
		fprintf(fout, "the following description: \"%s\" ", elem->incident->description);
		fprintf(fout, "and is %s\n", elem->incident->status);
	}
}

void show_interventions(Node_2 *head, FILE *fout)
{
	if (head->next == head)
		fprintf(fout, "No intervention has been initiated\n");
	else {
		Node_2 *elem = head->next;
		while (elem != head && elem->intervention != NULL)
		{
			fprintf(fout, "Incident %d ", elem->intervention->incident->id);
			fprintf(fout, "was assigned to unit %d, ", elem->intervention->unit->id);
			fprintf(fout, "and has the following status: \"%s\" \n", elem->intervention->incident->status);
			elem = elem->next;
		}
	}
}

void free_incidents(Node_1 *head) {
	if (!head->next) {
		if (head) {
		free(head->incident->description);
		free(head->incident);
		free(head);
		}
		return;
	}
	Node_1 *elem = head->next;
	while (elem != head) {
		Node_1 *next = elem->next;
		if (elem->incident) {
			free(elem->incident->description);
			free(elem->incident);
		}
		free(elem);
		elem = next;
	}
	if (head->incident) {
		free(head->incident->description);
		free(head->incident);
		}
	free(head);
}

void free_interventions(Node_2 *head) {
	if (head->next == head) {
		if (head) {
		free(head->intervention);
		free(head);
		}
		return;
	}
	Node_2 *elem = head->next;
	while (elem != NULL && elem != head) {
		Node_2 *next = elem->next;
		if (elem->intervention)
			free(elem->intervention);
		free(elem);
		elem = next;
	}
	if (head->intervention)
		free(head->intervention);
	free(head);
}

int main()
{
	Queue *queue_high = NULL, *queue_medium = NULL, *queue_low = NULL;
	Queue *queue_available_units = NULL;
	Stack *prev_interventions = NULL;
	Node_1 *incidents = NULL, *last_incident = NULL;
	Node_2 *interventions = NULL, *last_intervention = NULL;
	unit *units;

	int num = 0, i, id, len;
	char type, s[50];
	
	FILE *fin = fopen("tema1.in", "r");
	FILE *fout = fopen("tema1.out", "w");

	init_incident(&incidents,&last_incident);
	init_intervention(&interventions,&last_intervention);
	init_queue(&queue_available_units);
	init_queue(&queue_high);
	init_queue(&queue_medium);
	init_queue(&queue_low);
	init_stack(&prev_interventions);

	if (fin == NULL || fout == NULL)
		return 1;
	if (fscanf(fin, "%d", &num) != 1)
		return 1;

	units = malloc(sizeof(unit)*num);
	len = num;

	for (i = 0; i < num; i++) {
		if (fscanf(fin, "%d", &id) != 1)
		return 1;
		(units[i]).id = id;
		fgetc(fin);
		if (fscanf(fin, "%c", &type) != 1)
		return 1;
		units[i].type = type;
		units[i].availability = 1;
		enqueue(queue_available_units, (units + i));
	}
	num = 0;
	if(fscanf(fin, "%d", &num) != 1)
		return 1;
	for (i = 0; i < num; i++)
	{
		fscanf(fin, "%s", s);
		if (strcmp(s, "ADD_INCIDENT") == 0) {
			fscanf(fin, "%d", &id);
			char p[7], *d = malloc(sizeof(char) * 100);
			fscanf(fin, "%s", p);
			fgetc(fin);
			fgetc(fin);
			fgets(d, 100, fin);
			d[strlen(d)-2] = '\0';
			add_incident(&incidents, &last_incident, id, p, d, queue_high, queue_medium, queue_low);
			free(d);
		}
		if (strcmp(s, "CHECK_UNITS_AVAILABILITY") == 0) {
			check_units_availability(queue_available_units,fout);
		}
		if (strcmp(s, "DISPATCH") == 0) {
			if(!interventions)
				init_intervention(&interventions, &last_intervention);
			dispatch(queue_available_units, queue_high, queue_medium, queue_low, &interventions, &last_intervention, prev_interventions, fout);
		}
		if (strcmp(s, "UNDO_LAST_DISPATCH") == 0) {
			if(!interventions)
				init_intervention(&interventions, &last_intervention);
			undo_last_dispatch(prev_interventions, queue_available_units, queue_high, queue_medium, queue_low, &interventions, &last_intervention, fout);
		}
		if (strcmp(s, "SOLVED_INCIDENT") == 0) {
			fscanf(fin,"%d", &id);
			solved_incident(&interventions, id, queue_available_units, fout);
		}
		if (strcmp(s, "SHOW_UNIT") == 0) {
			fscanf(fin, "%d", &id);
			show_unit(id, len, units, fout);
		}
		if (strcmp(s, "SHOW_INCIDENT") == 0) {
			fscanf(fin, "%d", &id);
			show_incident(id, incidents,fout);
		}
		if (strcmp(s, "SHOW_INTERVENTIONS") == 0) {
			show_interventions(interventions, fout);
		}
	}
	if (queue_available_units)
		queue_free(&queue_available_units);
	if (queue_high)
		queue_free(&queue_high);
	if (queue_medium)
		queue_free(&queue_medium);
	if (queue_low)
		queue_free(&queue_low);
	stack_free(&prev_interventions);
	free_incidents(incidents);
	incidents = NULL;
	free(units);
	units = NULL;
	free_interventions(interventions);
	fclose(fin);
	fclose(fout);
	return 0;
}
