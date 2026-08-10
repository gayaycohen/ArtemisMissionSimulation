#include <stdio.h>
#include"menu.h"
#include"spaceraft.h"
#include"event.h"
#include"mission.h"
#include"file_manager.h"


void printMainMenu()
{
    printf("\n======================= ARTEMIS II MISSION SIMULATION =======================\n");
    printf("[1] Load mission configuration\n");
    printf("[2] Show mission configuration\n");
    printf("[3] Show resource matrix\n");
    printf("[4] Show scheduled events\n");
    printf("[5] Run simulation\n");
    printf("[6] Show mission statistics\n");
    printf("[7] Save mission report\n");
    printf("[0] Exit\n");
    printf("Enter your choice: ");
}


void runProgram()
{
    missionconfig config;   
    spacecraft orion;        
    eventlist missionEvents; 
    missionstats stats;      
    double** resourceMatrix = NULL;  
    int choice;                      
    int configLoaded = 0;           
    int simulationRun = 0;           
    do {
        printMainMenu();
        if (scanf_s(" %d", &choice) != 1)
        {
            printf("Invalid input,please enter a number.\n");
            while (getchar() != '\n');
            choice = -1;
            continue;
        }
        switch (choice)
        {
        case 1:
        {
            int oldNumPhases = configLoaded ? config.numphases : 0;
            if (loadMissionConfig(&config) == 0) printf("Failed to load mission configuration.\n");
            else {
                printf("\nMission configuration loaded seccessfully.\n");
                configLoaded = 1;
                simulationRun = 0;
                if (resourceMatrix != NULL) {
                    freeResourceMatrix(resourceMatrix, oldNumPhases);
                    resourceMatrix = NULL;
                }
                resourceMatrix = allocateResourceMatrix(config.numphases, config.numresources);
                if (resourceMatrix == NULL)
                {
                    printf("\nFailed to allocate resource matrix.\n");
                    configLoaded = 0;
                }
                else if (loadResourceMatrix(resourceMatrix, config.numphases, config.numresources) == 0)
                {
                    printf("\nFailed to load resource matrix.\n");
                    freeResourceMatrix(resourceMatrix, config.numphases);
                    resourceMatrix = NULL;
                    configLoaded = 0;
                }
                else {
                    printf("\nResource matriix loaded successfully.\n");
                }
            }
            break;
        }
        case 2:
            if (configLoaded == 0)printf("\nPlease load mission configuration first.\n");
            else printMissionConfig(config);
            break;
        case 3:
            if (configLoaded == 0 || resourceMatrix == NULL)printf("\nPlease load mission configuration first.\n");
            else printResourceMatrix(resourceMatrix, config.numphases, config.numresources);
            break;
        case 4:
            if(configLoaded==0)printf("\nPlease load mission configuration first.\n");
            else {
                initEventList(&missionEvents);
                scheduleMissionEvent(&missionEvents);
                printEventList(missionEvents);
                freeEventList(&missionEvents);
            }
            break;
        case 5:
            if (configLoaded == 0 || resourceMatrix == NULL)printf("\nPlease load mission configuration first.\n");
            else {
                initspacecraft(&orion, config.crewcount, config.initialfuel, config.initialoxygen, config.initialbattery);
                initEventList(&missionEvents);
                initMissionStats(&stats, orion);
                scheduleMissionEvent(&missionEvents);
                clearMissionLog();
                printf("\nInitial spacecraft status:\n");
                printspacecraftstatus(orion);
                runBasicMissionSimulation(&orion, &missionEvents, &stats, resourceMatrix);
                simulationRun = 1;
            }
            break;
        case 6:
            if (simulationRun == 0)printf("\nPlease run the simulation first.\n");
            else printMissionStats(stats);
            break;
        case 7:
            if (simulationRun == 0)printf("\nPlease run the simulation first.\n");
            else saveMissionReport(orion, stats);
            break;
        case 0:
            printf("\nExiting program...\n");
            break;
        default:
            printf("\nInvalid choice.Please try again.\n");
            break;
        }

    } while (choice != 0);
    if (resourceMatrix != NULL)freeResourceMatrix(resourceMatrix, config.numphases);
}