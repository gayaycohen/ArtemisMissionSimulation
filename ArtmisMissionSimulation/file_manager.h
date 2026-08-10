#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "mission.h"

#define CONFIG_FILE "config.txt"
#define LOG_FILE "mission_log.txt"
#define REPORT_FILE "mission_report.txt"
int loadMissionConfig(missionconfig* config);        
void printMissionConfig(missionconfig config);      
double** allocateResourceMatrix(int rows, int cols);  
int loadResourceMatrix(double** matrix, int rows, int cols);   
void printResourceMatrix(double** matrix, int rows, int cols); 
void freeResourceMatrix(double** matrix, int rows);            
void clearMissionLog();                                           
void writeEventToLog(spacecraft s, event currentEvent);       
void saveMissionReport(spacecraft s, missionstats stats);    
#endif 
