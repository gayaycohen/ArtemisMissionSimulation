#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mission.h"
#include "file_manager.h"


void initMissionStats(missionstats* stats, spacecraft s)
{
	stats->totalevents = 0;
	stats->warnings = 0;
	stats->blackouts = 0;
	stats->criticalevents = 0;
	stats->minfuel = s.fuel;
	stats->minoxygen = s.oxygen;
	stats->minbattery = s.battery;
}


void scheduleMissionEvent(eventlist* list)
{
	insertEventSorted(list, createEvent(0.00, EVENT_LAUNCH, "Launch from Earth"));
	insertEventSorted(list, createEvent(2.00, EVENT_EARTH_ORBIT, "Entering Earth Orbit"));
	insertEventSorted(list, createEvent(24.00, EVENT_SYSTEM_CHECK, "Main System Check"));
	insertEventSorted(list, createEvent(25.00, EVENT_TLI_BURN, "Trans-Lunar Injection Burn"));
	insertEventSorted(list, createEvent(120.00, EVENT_LUNAR_FLYBY, "Lunar Flyby"));
	insertEventSorted(list, createEvent(121.00, EVENT_COMM_BLACKOUT, "Temporary Communication Blackout"));
	insertEventSorted(list, createEvent(125.00, EVENT_RETURN_TRAJECTORY, "Return Trajectory to Earth"));
	insertEventSorted(list, createEvent(235.00, EVENT_REENTRY, "Reentry to Earth's Atmosphere"));
	insertEventSorted(list, createEvent(240.00, EVENT_SPLASHDOWN, "Splashdown and Mission End"));
}


int getResourceRowByEventType(int eventType)
{
	switch (eventType)
	{
	case EVENT_LAUNCH:
		return 0;

	case EVENT_EARTH_ORBIT:
		return 1;

	case EVENT_SYSTEM_CHECK:
		return 2;

	case EVENT_TLI_BURN:
		return 3;

	case EVENT_LUNAR_FLYBY:
		return 4;

	case EVENT_COMM_BLACKOUT:
		return 5;

	case EVENT_RETURN_TRAJECTORY:
		return 6;

	case EVENT_REENTRY:
		return 7;

	case EVENT_SPLASHDOWN:
		return 8;

	default:
		return -1;
	}
}

void handleMissionEvent(spacecraft* s, missionstats* stats, event* currentEvent,double** resourceMatrix)
{
	int rowindex;
	if (currentEvent == NULL)return;
	rowindex = getResourceRowByEventType(currentEvent->type);
	if (rowindex == -1)
	{
		printf("Error:Unknown event type.\n");
		stats->warnings++;
		return;
	}
	printf("\nHandling Event : %s\n", getEventTypeName(currentEvent->type));
	printf("Time:             %.2f hours\n", currentEvent->time);
	printf("Description:      %s\n", currentEvent->description);
	stats->totalevents++;
	switch (currentEvent->type)
	{
	case EVENT_LAUNCH:
		updatespacecraftphase(s, "Launch");
		s->missionstatus = MISSION_IN_PROGRESS;
		applyResourceeUsage(s, resourceMatrix[rowindex][RESOURCE_FUEL], resourceMatrix[rowindex][RESOURCE_OXYGEN], resourceMatrix[rowindex][RESOURCE_BATTERY]);
		break;
	case EVENT_EARTH_ORBIT:
		updatespacecraftphase(s, "Earth Orbit");
		applyResourceeUsage(s, resourceMatrix[rowindex][RESOURCE_FUEL], resourceMatrix[rowindex][RESOURCE_OXYGEN], resourceMatrix[rowindex][RESOURCE_BATTERY]);
		break;
	case EVENT_SYSTEM_CHECK:
		updatespacecraftphase(s, "System Check");
		applyResourceeUsage(s, resourceMatrix[rowindex][RESOURCE_FUEL], resourceMatrix[rowindex][RESOURCE_OXYGEN], resourceMatrix[rowindex][RESOURCE_BATTERY]);
		break;
	case EVENT_TLI_BURN:
		updatespacecraftphase(s, "TLI Burn");
		applyResourceeUsage(s, resourceMatrix[rowindex][RESOURCE_FUEL], resourceMatrix[rowindex][RESOURCE_OXYGEN], resourceMatrix[rowindex][RESOURCE_BATTERY]);
		break;
	case EVENT_LUNAR_FLYBY:
		updatespacecraftphase(s, "Lunar Flyby");
		applyResourceeUsage(s, resourceMatrix[rowindex][RESOURCE_FUEL], resourceMatrix[rowindex][RESOURCE_OXYGEN], resourceMatrix[rowindex][RESOURCE_BATTERY]);
		break;
	case EVENT_COMM_BLACKOUT:
		updatespacecraftphase(s, "Communication Blackout");
		s->communicationactive = 0;
		stats->blackouts++;
		stats->warnings++;
		applyResourceeUsage(s, resourceMatrix[rowindex][RESOURCE_FUEL], resourceMatrix[rowindex][RESOURCE_OXYGEN], resourceMatrix[rowindex][RESOURCE_BATTERY]);
		break;
	case EVENT_RETURN_TRAJECTORY:
		updatespacecraftphase(s, "Return Trajectory");
		s->communicationactive = 1;
		applyResourceeUsage(s, resourceMatrix[rowindex][RESOURCE_FUEL], resourceMatrix[rowindex][RESOURCE_OXYGEN], resourceMatrix[rowindex][RESOURCE_BATTERY]);
		break;
	case EVENT_REENTRY:
		updatespacecraftphase(s, "Reentry");
		applyResourceeUsage(s, resourceMatrix[rowindex][RESOURCE_FUEL], resourceMatrix[rowindex][RESOURCE_OXYGEN], resourceMatrix[rowindex][RESOURCE_BATTERY]);
		break;
	case EVENT_SPLASHDOWN:
		updatespacecraftphase(s, "Splashdown");
		s->missionstatus = MISSION_SUCCESS;
		applyResourceeUsage(s, resourceMatrix[rowindex][RESOURCE_FUEL], resourceMatrix[rowindex][RESOURCE_OXYGEN], resourceMatrix[rowindex][RESOURCE_BATTERY]);
		break;
	default:
		updatespacecraftphase(s, "Unknown Event");
		stats->warnings++;
		break;
	}
	if (s->fuel < stats->minfuel)stats->minfuel = s->fuel;
	if (s->oxygen < stats->minoxygen)stats->minoxygen = s->oxygen;
	if (s->battery < stats->minbattery)stats->minbattery = s->battery;
	if (s->battery <= 20 || s->oxygen <= 100 || s->fuel <= 100)
	{
		stats->criticalevents++;
		printf("Warning: Critical level detected.\n");
	}
	if (checkMissionFailure(s, stats) == 1)
	{
		printspacecraftstatus(*s);
		writeEventToLog(*s, *currentEvent);
		return;
	}
	printspacecraftstatus(*s);
	writeEventToLog(*s, *currentEvent);
}

void runBasicMissionSimulation(spacecraft* s, eventlist* list, missionstats* stats,double**resourceMatrix)
{
	event* currentEvent;
	printf("\n======================== STARTING MISSION SIMULATION ========================\n");
	while (list->head != NULL&&s->missionstatus!=MISSION_FAILED)
	{
		currentEvent = popNextEvent(list);
		handleMissionEvent(s, stats, currentEvent,resourceMatrix);
		free(currentEvent);
	}
	printf("\n========================= MISSION SIMULATION ENDED ==========================\n");
}


void printMissionStats(missionstats stats)
{
	printf("\n============================== MISSION STATISTICS ===========================\n");
	printf("Total events handled: %d\n", stats.totalevents);
	printf("Warnings:             %d\n", stats.warnings);
	printf("Blackouts:            %d\n", stats.blackouts);
	printf("Critical events:      %d\n", stats.criticalevents);
	printf("Minimum fuel:         %.2f\n", stats.minfuel);
	printf("Minimum oxygen:       %.2f\n", stats.minoxygen);
	printf("Minimum battery:      %.2f%%\n", stats.minbattery);
	printf("=============================================================================\n");
}


int checkMissionFailure(spacecraft* s, missionstats* stats)
{
	if (s->fuel <= 0)
	{
		printf("MISSION FAILURE: Fuel depleted.\n");
		s->missionstatus = MISSION_FAILED;
		stats->criticalevents++;
		stats->warnings++;
		return 1;
	}
	if (s->oxygen <= 0)
	{
		printf("MISSION FAILURE: Oxygen depleted.\n");
		s->missionstatus = MISSION_FAILED;
		stats->criticalevents++;
		stats->warnings++;
		return 1;
	}
	if (s->battery <= 0)
	{
		printf("MISSION FAILURE: Battery depleted.\n");
		s->missionstatus = MISSION_FAILED;
		stats->criticalevents++;
		stats->warnings++;
		return 1;
	}
	return 0;
}