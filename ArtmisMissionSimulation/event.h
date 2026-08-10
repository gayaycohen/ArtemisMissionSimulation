#ifndef EVENT_H
#define EVENT_H

#define EVENT_LAUNCH 1            
#define EVENT_EARTH_ORBIT 2          
#define EVENT_SYSTEM_CHECK 3        
#define EVENT_TLI_BURN 4            
#define EVENT_LUNAR_FLYBY 5         
#define EVENT_COMM_BLACKOUT 6       
#define EVENT_RETURN_TRAJECTORY 7    
#define EVENT_REENTRY 8             
#define EVENT_SPLASHDOWN 9           
#define EVENT_SYSTEM_WARNING 10      

typedef struct event {
	double time;                  
	int type;                      
	char description[100];          
	struct event* next;          
}event;

typedef struct eventlist {
	event* head;                   
	int size;                       
}eventlist;

void initEventList(eventlist* list);                                   
event* createEvent(double time, int type, char description[]);          
void insertEventSorted(eventlist* list, event* newEvent);               
event* popNextEvent(eventlist* list);                                  
void printEventList(eventlist list);                                    
void freeEventList(eventlist* list);                                   
const char* getEventTypeName(int type);                                
#endif