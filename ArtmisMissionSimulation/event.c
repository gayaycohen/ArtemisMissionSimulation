#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "event.h"
void initEventList(eventlist* list)
{
	list->head = NULL;
	list->size = 0;
}

event* createEvent(double time, int type, char description[])
{
	event* newEvent = (event*)malloc(sizeof(event));
	if (newEvent == NULL)
	{
		printf("Memory allocation failed for event.\n");
		return NULL;
	}
	newEvent->time = time;
	newEvent->type = type;
	strncpy(newEvent->description, description, 99);
	newEvent->description[99] = '\0';
	newEvent->next = NULL;
	return newEvent;
}

void insertEventSorted(eventlist* list, event* newEvent)
{
	event* current;
	if (newEvent == NULL)
	{
		return;
	}
	if (list->head==NULL || newEvent->time < list->head->time)
	{
		newEvent->next = list->head;
		list->head = newEvent;
	}
	else {
		current = list->head;
		while (current->next != NULL && current->next->time <= newEvent->time)
		{
			current = current->next;
		}
		newEvent->next = current->next;
		current->next = newEvent;
	}
	list->size++;
}

event* popNextEvent(eventlist* list)
{
	event* firstEvent;
	if (list->head == NULL)
	{
		return NULL;
	}
	firstEvent = list->head;
	list->head = list->head->next;
	firstEvent->next = NULL;
	list->size--;
	return firstEvent;
}


void printEventList(eventlist list)
{
	event* current = list.head;
	printf("\n============================== EVENT LIST ===================================\n");
	if (current == NULL)
	{
		printf("Event list is empty.\n");
	}
	while (current != NULL)
	{
		printf("Time:%-10.2f | Type:%-25s |Description:%-28s\n", current->time, getEventTypeName(current->type), current->description);
		current = current->next;
	}
	printf("Total events : %d\n", list.size);
	printf("=============================================================================\n");
}

void freeEventList(eventlist* list)
{
	event* current = list->head;
	event* temp;
	while (current != NULL)
	{
		temp = current;
		current = current->next;
		free(temp);
	}
	list->head = NULL;
	list->size = 0;
}


const char* getEventTypeName(int type)
{
	switch (type)
	{
	case EVENT_LAUNCH:
			return "Launch";
	case EVENT_EARTH_ORBIT:
		return "Earth Orbit";
	case EVENT_SYSTEM_CHECK:
		return "System Check";
	case EVENT_TLI_BURN:
		return "TLI Burn";
	case EVENT_LUNAR_FLYBY:
		return "Lunar Flyby";
	case EVENT_COMM_BLACKOUT:
		return "Communication Blackout";
	case EVENT_RETURN_TRAJECTORY:
		return "Return Trajectory";
	case EVENT_REENTRY:
		return "Reentry";
	case EVENT_SPLASHDOWN:
		return "Splashdown";
	case EVENT_SYSTEM_WARNING:
		return "System Warning";
	default:
		return "Unknown Event";
	}
}