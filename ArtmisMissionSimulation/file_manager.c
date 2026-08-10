#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#include "file_manager.h"


int loadMissionConfig(missionconfig* config)
{
	FILE* file=NULL;
	errno_t result;
	result = fopen_s(&file,CONFIG_FILE, "r");
	if (result != 0 || file==NULL)
	{
		printf("Error: Could not open config file.\n");
		return 0;
	}
	fscanf_s(file, "%lf", &config->missionduration);
	fscanf_s(file, "%d", &config->crewcount);
	fscanf_s(file, "%lf", &config->initialfuel);
	fscanf_s(file, "%lf", &config->initialoxygen);
	fscanf_s(file, "%lf", &config->initialbattery);
	fscanf_s(file, "%d", &config->risklevel);
	fscanf_s(file, "%d", &config->numphases);
	fscanf_s(file, "%d", &config->numresources);
	fclose(file);
	return 1;
}


void printMissionConfig(missionconfig config)
{
	printf("\n=========================== MISSION CONFIGURATION ==========================\n");
	printf("Mission duration:%.2f hours\n", config.missionduration);
	printf("Crew count:      %d\n", config.crewcount);
	printf("Initial fuel:    %.2f \n", config.initialfuel);
	printf("Initial oxygen:  %.2f \n", config.initialoxygen);
	printf("Initial battery: %.2f %%\n", config.initialbattery);
	printf("Risk level:      %d \n", config.risklevel);
	printf("Number of phases:%d\n", config.numphases);
	printf("Number resources:%d \n", config.numresources);
	printf("=============================================================================\n\n");
}


double** allocateResourceMatrix(int rows, int cols)
{
	double** matrix;
	int i;
	matrix = (double**)malloc(rows * sizeof(double*));
	if (matrix == NULL)
	{
		printf("Error: Could not allocate matrix rows.\n");
		return NULL;
	}
	for (i = 0; i < rows; i++)
	{
		matrix[i] = (double*)malloc(cols * sizeof(double));
		if (matrix[i] == NULL)
		{
			printf("Error: Could not allocate matrix columns.\n");
			for (int j = 0; j < i; j++) {
				free(matrix[j]);
			}
			free(matrix);
			return NULL;
		}
	}
	return matrix;
}


int loadResourceMatrix(double** matrix, int rows, int cols)
{
	int i, j;
	double tempMissionDuration;
	int tempCrewCount;
	double tempFuel;
	double tempOxygen;
	double tempBattery;
	int tempRiskLevel;
	int tempNumPhases;
	int tempNumResources;
	FILE* file = NULL;
	errno_t result;
	result = fopen_s(&file, CONFIG_FILE, "r");
	if (result != 0 || file == NULL)
	{
		printf("Error: Could not open config file.\n");
		return 0;
	}
	fscanf_s(file, "%lf", &tempMissionDuration);
	fscanf_s(file, "%d", &tempCrewCount);
	fscanf_s(file, "%lf", &tempFuel);
	fscanf_s(file, "%lf", &tempOxygen);
	fscanf_s(file, "%lf", &tempBattery);
	fscanf_s(file, "%d", &tempRiskLevel);
	fscanf_s(file, "%d", &tempNumPhases);
	fscanf_s(file, "%d", &tempNumResources);
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			fscanf_s(file, "%lf", &matrix[i][j]);
		}
	}
	fclose(file);
	return 1;
}

void printResourceMatrix(double** matrix, int rows, int cols)
{
	int i, j;
	const char* phaseNames[] = { "Launch","Earth Orbit","System Check","TLI Burn","Lunar Flyby","Communication Blackout","Return Trajectory","Reentry","Slashdown" };
	printf("\n============================== RESOURCE MATRIX ==============================\n");
	printf("%-28s %-10s %-10s %-10s\n","mission phases","Fuel","Oxygen","Battery");
	printf("---------------------------------------------------------------------\n");
	for (i = 0; i < rows; i++)
	{
		if (i < 9)printf("%-28s", phaseNames[i]);
		else printf("Phase %-22d:", i + 1);
		for (j = 0; j < cols; j++)
		{
			printf("%-10.2f", matrix[i][j]);
		}
		printf("\n");
	}
	printf("=============================================================================\n");
}


void freeResourceMatrix(double** matrix, int rows)
{
	int i;
	if (matrix == NULL)
	{
		return;
	}
	for (i = 0; i < rows; i++)
	{
		free(matrix[i]);
	}
	free(matrix);
}


void clearMissionLog()
{
	FILE* file = NULL;
	errno_t result;
	result = fopen_s(&file, LOG_FILE, "w");
	if (result != 0 || file == NULL)
	{
		printf("Error: Could not create mission log file.\n");
		return ;
	}
	fprintf(file, "      ============================== MISSION LOG ==============================\n\n");
	fclose(file);
}


void writeEventToLog(spacecraft s, event currentEvent)
{
	FILE* file = NULL;
	errno_t result;
	result = fopen_s(&file, LOG_FILE, "a");
	if (result != 0 || file == NULL)
	{
		printf("Error: Could not write to mission log file.\n");
		return ;
	}
	fprintf(file, "Time:  %.2f hours\n", currentEvent.time);
	fprintf(file, "Event:  %s\n", getEventTypeName(currentEvent.type));
	fprintf(file, "Description:  %s\n", currentEvent.description);
	fprintf(file, "Fuel:  %.2f \n", s.fuel);
	fprintf(file, "Oxygen:  %.2f \n", s.oxygen);
	fprintf(file, "Battery:  %.2f%% \n", s.battery);
	fprintf(file, "Communication:  %s\n", getCommunicationStatusName(s.communicationactive));
	fprintf(file, "Mission Status:  %s\n", getMissionStatusName(s.missionstatus));
	fprintf(file, "Current phase:  %s\n", s.currentphase);
	fprintf(file, "===========================\n");
	fclose(file);
}


void saveMissionReport(spacecraft s, missionstats stats)
{
	FILE* file = NULL;
	errno_t result;
	result = fopen_s(&file, REPORT_FILE, "w");
	if (result != 0 || file == NULL)
	{
		printf("Error: Could not create mission report file.\n");
		return;
	}
	fprintf(file, "========== MISSION REPORT ==========\n\n");
	fprintf(file, "Mission result: %s\n", getMissionStatusName(s.missionstatus));
	fprintf(file, "Final phase: %s\n", s.currentphase);
	fprintf(file, "\nFinal spacecraft resources:\n");
	fprintf(file, "Fuel: %.2f\n", s.fuel);
	fprintf(file, "Oxygen: %.2f\n", s.oxygen);
	fprintf(file, "Battery: %.2f%%\n", s.battery);
	fprintf(file, "Communication: %s\n", getCommunicationStatusName(s.communicationactive));
	fprintf(file, "\nMission statistics:\n");
	fprintf(file, "Total events handled: %d\n", stats.totalevents);
	fprintf(file, "Warnings: %d\n", stats.warnings);
	fprintf(file, "Blackouts: %d\n", stats.blackouts);
	fprintf(file, "Critical events: %d\n", stats.criticalevents);
	fprintf(file, "\nMinimum resources during mission:\n");
	fprintf(file, "Minimum fuel: %.2f\n", stats.minfuel);
	fprintf(file, "Minimum oxygen: %.2f\n", stats.minoxygen);
	fprintf(file, "Minimum battery: %.2f%%\n", stats.minbattery);
	fprintf(file, "\n====================================\n");
	fclose(file);
	printf("\nMission report saved to %s\n", REPORT_FILE);
}