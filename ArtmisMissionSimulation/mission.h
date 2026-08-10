#ifndef MISSION_H
#define MISSION_H

#include "spaceraft.h"
#include "event.h"

#define NUM_RESOURCES 3   
#define MAX_PHASES 9     

#define RESOURCE_FUEL 0         
#define RESOURCE_OXYGEN 1        
#define RESOURCE_BATTERY 2    

typedef struct missionconfig {
	double missionduration;     
	int crewcount;              
	double initialfuel;        
	double initialoxygen;       
	double initialbattery;     
	int risklevel;             
	int numphases;             
	int numresources;          
}missionconfig;

typedef struct missionstats {
	int totalevents;          
	int warnings;             
	int blackouts;             
	int criticalevents;       
	double minfuel;          
	double minoxygen;         
	double minbattery;      
}missionstats;

void initMissionStats(missionstats* stats, spacecraft s);                      
void scheduleMissionEvent(eventlist* list);                                       
int getResourceRowByEventType(int eventType);                               
void handleMissionEvent(spacecraft* s, missionstats* stats, event* currentEvent, double** resourceMatrix); 
void runBasicMissionSimulation(spacecraft* s, eventlist* list, missionstats* stats, double** resourceMatrix); 
void printMissionStats(missionstats stats);                                             
int checkMissionFailure(spacecraft* s, missionstats* stats);                            

#endif 

