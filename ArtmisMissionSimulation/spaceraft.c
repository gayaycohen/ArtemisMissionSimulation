#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include "spaceraft.h"

 
void initspacecraft(spacecraft* s, int Crewcount, double Fuel, double Oxygen, double Battery)
{
	strcpy(s->name, "Orion");
	s->crewcount = Crewcount;
	s->fuel = Fuel;
	s->oxygen = Oxygen;
	s->battery = Battery;
	s->communicationactive = 1;
	s->missionstatus = MISSION_READY;
	strcpy(s->currentphase, "Ready");
}


void printspacecraftstatus(spacecraft s)
{
	printf("\n============================== SPACECRAFT STATUS ============================\n");
	printf("Name:                %s\n", s.name);
	printf("Crew count:          %d\n", s.crewcount);
	printf("fuel:                %.2f\n", s.fuel);
	printf("Oxygen:              %.2f\n", s.oxygen);
	printf("Battery:             %.2f%%\n", s.battery);
	printf("Communication active:%s\n", getCommunicationStatusName(s.communicationactive));
	printf("Mission status:      %s\n", getMissionStatusName(s.missionstatus));
	printf("Current phase:       %s\n", s.currentphase);
	printf("=============================================================================\n");
}


void updatespacecraftphase(spacecraft* s, char phasename[])
{
	strncpy(s->currentphase, phasename, MAX_NAME - 1);
	s->currentphase[MAX_NAME - 1] = '\0';
}


void applyResourceeUsage(spacecraft* s, double fuelUsed, double oxygenUsed, double batteryUsed)
{
	s->fuel = s->fuel - fuelUsed;
	s->oxygen = s->oxygen - oxygenUsed;
	s->battery = s->battery - batteryUsed;
	if (s->fuel < 0)
	{
		s->fuel = 0;
	}
	if (s->oxygen < 0)
	{
		s->oxygen = 0;
	}
	if (s->battery < 0)
	{
		s->battery = 0;
	}
}


const char* getMissionStatusName(int status)
{
	switch (status)
	{
	case MISSION_READY:
		return "READY";
	case MISSION_IN_PROGRESS:
		return "IN PROGRESS";
	case MISSION_SUCCESS:
		return "SUCCESS";
	case MISSION_FAILED:
		return "FAILED";
	default:
		return "UNKNOWN";
	}
}

const char* getCommunicationStatusName(int communicationActive)
{
	if (communicationActive == 1) {
		return "ACTIVE";
	}
		return "BLACKOUT";
}