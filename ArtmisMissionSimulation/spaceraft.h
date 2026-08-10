
#ifndef SPACERAFT_H
#define SPACERAFT_H

#define MAX_NAME 50
#define MISSION_READY 0           
#define MISSION_IN_PROGRESS 1     
#define MISSION_SUCCESS 2       
#define MISSION_FAILED 3          

typedef struct spacecraft {
	char name[MAX_NAME];        
	int crewcount;              
	double fuel;                 
	double oxygen;               
	double battery;              
	int communicationactive;     
	int missionstatus;           
	char currentphase[MAX_NAME];  
}spacecraft;

void initspacecraft(spacecraft* s, int crewcount, double fuel, double oxygen, double battery);    
void printspacecraftstatus(spacecraft s);                                                          
void updatespacecraftphase(spacecraft* s, char phasename[]);                                      
void applyResourceeUsage(spacecraft* s, double fuelUsed, double oxygenUsed, double batteryUsed);   
const char* getMissionStatusName(int status);                                                      
const char* getCommunicationStatusName(int communicationActive);                                 
#endif