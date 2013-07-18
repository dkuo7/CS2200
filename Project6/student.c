/*
 * student.c
 * Multithreaded OS Simulation for CS 2200, Project 6
 *
 * This file contains the CPU scheduler for the simulation.  
 */

#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "os-sim.h"

#ifdef DEBUG
# define DEBUG_PRINT(x) printf x
#else
# define DEBUG_PRINT(x) do {} while (0)
#endif

void logp(pcb_t *pcb, char *message);
void print_help();

/*
 * current[] is an array of pointers to the currently running processes.
 * There is one array element corresponding to each CPU in the simulation.
 *
 * current[] should be updated by schedule() each time a process is scheduled
 * on a CPU.  Since the current[] array is accessed by multiple threads, you
 * will need to use a mutex to protect it.  current_mutex has been provided
 * for your use.
 */
static pcb_t **current;
static pthread_mutex_t current_mutex;

/* head of ready queue */
static pcb_t *head;
static pthread_mutex_t ready_mutex;

/* condition variable for idle function */
static pthread_cond_t not_idle;

/* round-robin and static priority info */
static int is_round_robin;
static int time_slice;
static int is_static_prior;
static int cpu_count;

/* add a process block to the ready queue linked list */
static void add_to_ready(pcb_t *pcb) {
    pcb_t *curr;
    pthread_mutex_lock(&ready_mutex);
    curr = head;
    if(curr==NULL) {
        head = pcb;
    }
    else {
        while(curr->next != NULL) {
           curr = curr->next; 
        }
        curr->next = pcb;
    }
    pcb->next = NULL;
    pthread_cond_broadcast(&not_idle);
    pthread_mutex_unlock(&ready_mutex);
}

/* pop a process from the head of the ready queue */
static pcb_t* pop_from_ready() {
    pcb_t *popped;
    pthread_mutex_lock(&ready_mutex);
    popped = head;
    if(popped != NULL) {
        head = popped->next;
    }
    pthread_mutex_unlock(&ready_mutex);
    return popped;
}

/* pop highest priority process from the ready queue */
static pcb_t* pop_high_priority() {
    pcb_t *popped,*curr,*prev;
    int highest = 0;
    pthread_mutex_lock(&ready_mutex);
    if(head == NULL) {
        popped = NULL;
    }
    else {
        curr = head;
        while(curr != NULL) {
            if(curr->static_priority > highest) {
                highest = curr->static_priority;
            }
            curr = curr->next;
        }
        curr = head;
        prev = head;
        while(curr != NULL) {
            if(curr->static_priority == highest) {
                popped = curr;
                if(curr==head) {
                    head = curr->next;
                }
                else {
                    prev->next = curr->next;
                }
                break;
            }
            prev = curr;
            curr = curr->next;
        }
    }
    pthread_mutex_unlock(&ready_mutex);
    return popped;
}


/*
 * schedule() is your CPU scheduler.  It should perform the following tasks:
 *
 *   1. Select and remove a runnable process from your ready queue which 
 *	you will have to implement with a linked list or something of the sort.
 *
 *   2. Set the process state to RUNNING
 *
 *   3. Call context_switch(), to tell the simulator which process to execute
 *      next on the CPU.  If no process is runnable, call context_switch()
 *      with a pointer to NULL to select the idle process.
 *	The current array (see above) is how you access the currently running process indexed by the cpu id. 
 *	See above for full description.
 *	context_switch() is prototyped in os-sim.h. Look there for more information 
 *	about it and its parameters.
 */
static void schedule(unsigned int cpu_id)
{
    pcb_t *pcb;
    DEBUG_PRINT(("Scheduling something for %d\n",cpu_id));
    if(is_static_prior) {
        pcb = pop_high_priority();
    }
    else {
        pcb = pop_from_ready();
    }
    logp(pcb,"Getting scheduled");
    if(pcb != NULL) {
        pcb->state = PROCESS_RUNNING;
    }
    pthread_mutex_lock(&current_mutex);
    current[cpu_id] = pcb;
    pthread_mutex_unlock(&current_mutex); 
    context_switch(cpu_id,pcb,time_slice);
}


/*
 * idle() is your idle process.  It is called by the simulator when the idle
 * process is scheduled.
 *
 * This function should block until a process is added to your ready queue.
 * It should then call schedule() to select the process to run on the CPU.
 */
extern void idle(unsigned int cpu_id)
{
    pthread_mutex_lock(&ready_mutex);
    while(head==NULL) {
        pthread_cond_wait(&not_idle,&ready_mutex);
    }
    pthread_mutex_unlock(&ready_mutex);
    schedule(cpu_id);
}


/*
 * preempt() is the handler called by the simulator when a process is
 * preempted due to its timeslice expiring.
 *
 * This function should place the currently running process back in the
 * ready queue, and call schedule() to select a new runnable process.
 */
extern void preempt(unsigned int cpu_id)
{
    pcb_t* pcb;
    pthread_mutex_lock(&current_mutex);
    pcb = current[cpu_id];
    pcb->state = PROCESS_READY;
    pthread_mutex_unlock(&current_mutex);
    add_to_ready(pcb);
    schedule(cpu_id);
}


/*
 * yield() is the handler called by the simulator when a process yields the
 * CPU to perform an I/O request.
 *
 * It should mark the process as WAITING, then call schedule() to select
 * a new process for the CPU.
 */
extern void yield(unsigned int cpu_id)
{
   pcb_t *pcb;
   pthread_mutex_lock(&current_mutex); 
   pcb = current[cpu_id];
   pcb->state = PROCESS_WAITING;
   pthread_mutex_unlock(&current_mutex);
   schedule(cpu_id);
}


/*
 * terminate() is the handler called by the simulator when a process completes.
 * It should mark the process as terminated, then call schedule() to select
 * a new process for the CPU.
 */
extern void terminate(unsigned int cpu_id)
{
    pcb_t *pcb;
    pthread_mutex_lock(&current_mutex); 
    pcb = current[cpu_id];
    pcb->state = PROCESS_TERMINATED;
    pthread_mutex_unlock(&current_mutex);
    schedule(cpu_id);
}


/*
 * wake_up() is the handler called by the simulator when a process's I/O
 * request completes.  It should perform the following tasks:
 *
 *   1. Mark the process as READY, and insert it into the ready queue.
 *
 *   2. If the scheduling algorithm is static priority, wake_up() may need
 *      to preempt the CPU with the lowest priority process to allow it to
 *      execute the process which just woke up.  However, if any CPU is
 *      currently running idle, or all of the CPUs are running processes
 *      with a higher priority than the one which just woke up, wake_up()
 *      should not preempt any CPUs.
 *	To preempt a process, use force_preempt(). Look in os-sim.h for 
 * 	its prototype and the parameters it takes in.
 */
extern void wake_up(pcb_t *process)
{
    int i,lowest,low_id;
    logp(process,"WAKING UP");
    process->state = PROCESS_READY;
    add_to_ready(process);
    if(is_static_prior) {
        pthread_mutex_lock(&current_mutex);
        low_id = -1;
        lowest = 10;
        for(i=0; i<cpu_count; i++) {
                if(current[i] == NULL) {
                    low_id = -1;
                    break;
                }
                if(current[i]->static_priority < lowest) {
                    lowest = current[i]->static_priority;
                    low_id = i;
                }
        }
        pthread_mutex_unlock(&current_mutex);
        if(low_id != -1 && lowest < process->static_priority) {
            force_preempt(low_id); 
        }
    }
}

void logp(pcb_t *pcb, char *message) {
    DEBUG_PRINT(("PID: %d NAME: %s\t\t%s\n",pcb->pid,pcb->name,message));
}



/*
 * main() simply parses command line arguments, then calls start_simulator().
 * You will need to modify it to support the -r and -p command-line parameters.
 */
int main(int argc, char *argv[])
{
    int i;
    is_round_robin = 0;
    is_static_prior = 0;
    time_slice = -1;

    /* Parse command-line arguments */

    if(argc < 2 || argc > 5) {
        print_help();
        exit(1);
    }

    cpu_count = atoi(argv[1]);
    if(cpu_count == 0) {
        print_help();
        exit(1);
    }

    for(i=0; i<argc; i++) {
        if(strcmp(argv[i],"-r")==0) {
            is_round_robin = 1;
            if(i+1>=argc) {
                print_help();
                exit(1);
            }
            else {
                time_slice = atoi(argv[i+1]);
            }
        }
        if(strcmp(argv[i],"-p")==0) {
            is_static_prior = 1;
        }
    }

    /* Allocate the current[] array and its mutex */
    current = malloc(sizeof(pcb_t*) * cpu_count);
    assert(current != NULL);
    pthread_mutex_init(&current_mutex, NULL);

    /* Setup up the ready queue */
    head = NULL;
    pthread_mutex_init(&ready_mutex, NULL);

    /* Setup the idle variable */
    pthread_cond_init(&not_idle, NULL);

    /* Start the simulator in the library */
    start_simulator(cpu_count);

    return 0;
}
    
void print_help() {
    fprintf(stderr, "CS 2200 Project 4 -- Multithreaded OS Simulator\n"
        "Usage: ./os-sim <# CPUs> [ -r <time slice> | -p ]\n"
        "    Default : FIFO Scheduler\n"
        "         -r : Round-Robin Scheduler\n"
        "         -p : Static Priority Scheduler\n\n");
}


