#include <pthread.h>

#define PID_MIN  	300
#define PID_MAX 	500

/* mutex lock for accessing pid_map */
pthread_mutex_t mutex;

/* allocate pid map */
int pid_map[PID_MAX+1];

int last;	// last pid in use

// Function declarations
int allocate_map(void);		// allocates and initializes pid map
int allocate_pid(void);		// allocates pid from pid map
void release_pid(int pid);	// releases a given pid 
