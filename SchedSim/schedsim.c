// C program for implementation of Simulation 
#include<stdio.h> 
#include<limits.h>
#include<stdlib.h>
#include "process.h"
#include "util.h"
#include <stdbool.h>

// reference/ help from:Benjamin Hoesli


// Function to find the waiting time for all  
// processes
void findWaitingTimeRR(ProcessType plist[], int n,int quantum) 
{ 
  /*
     1. Create an array *rem_bt[]* to keep track of remaining burst time of processes. This array is initially a copy of *plist[].bt* (all processes burst times)
     2. Store waiting times of processes in plist[].wt. Initialize this array as 0.
     3. Initialize time : t = 0
     4. Keep traversing the all processes while all processes are not done. Do following for i'th process if it is not done yet.
        - If rem_bt[i] > quantum
          (i)  t = t + quantum
          (ii) bt_rem[i] -= quantum;
        - Else // Last cycle for this process
          (i)  t = t + bt_rem[i];
          (ii) plist[i].wt = t - plist[i].bt
          (iii) bt_rem[i] = 0; // This process is over
       
   */
  int bt_rem[n];
  for (int i = 0; i < n; i++){
    bt_rem[i] = plist[i].bt;
  }
  int t = 0; 
  int count = n; 
  int i = 0; 
  while(count != 0){
    if (i==n){
      i = 0; 
    }
    if(bt_rem[i] == 0){
      i++; 
      continue; 
    }
    else if(bt_rem[i]==0){
      i++;
      continue;
    }
    else if(bt_rem[i] > quantum){
      t = t + quantum; 
      bt_rem[i] -= quantum; 
      i++;
    }
    else{
      t+= bt_rem[i];
      plist[i].wt = t - plist[i].bt;
      bt_rem[i] = 0; 
      i++; 
      count +=1; 
    }
  }
} 

// Function to find the waiting time for all  
// processes 
void findWaitingTimeSJF(ProcessType plist[], int n)
{ 
      /*
       * 1 Traverse until all process gets completely executed.
         - Find process with minimum remaining time at every single time lap.
         - Reduce its time by 1.
         - Check if its remaining time becomes 0 
         - Increment the counter of process completion.
         - Completion time of *current process = current_time +1;*
         - Calculate waiting time for each completed process. *wt[i]= Completion time - arrival_time-burst_time*
         - Increment time lap by one.
 
     */

  int bt_rem[n];
  for(int i = 0; i < n; i++){
    bt_rem[i]=plist[i].bt;
  }
  int t = 0; 
  int count = n; 
  while(count >0){
    int curr = 0; 
    int low = 100; 
    bool proceed = false; 
    for(int i = 0; i < n; i++){
      if(bt_rem[i] < low && bt_rem[i]!=0 && plist[i].art<=t){
        low=bt_rem[i];
        curr = i; 
        proceed = true; 
      }
    }
    if (proceed){
      if(bt_rem[curr] == 1){
        plist[curr].wt=t-plist[curr].art-plist[curr].bt+1; 
        plist[curr].tat=t-plist[curr].art+1; 
      }
      bt_rem[curr] -=1; 
      proceed=false; 
      t++; 
    }
    else{
      t++;
    }
  }
} 

// Function to find the waiting time for all  
// processes 
void findWaitingTime(ProcessType plist[], int n)
{ 
    // waiting time for first process is 0, or the arrival time if not 
    plist[0].wt = 0 +  plist[0].art; 
  
    // calculating waiting time 
    for (int  i = 1; i < n ; i++ ) 
        plist[i].wt =  plist[i-1].bt + plist[i-1].wt; 
} 
  
// Function to calculate turn around time 
void findTurnAroundTime( ProcessType plist[], int n)
{ 
    // calculating turnaround time by adding bt[i] + wt[i] 
    for (int  i = 0; i < n ; i++) 
        plist[i].tat = plist[i].bt + plist[i].wt; 
} 
  
// Function to sort the Process acc. to priority
int my_comparer(const void *this, const void *that)
{ 
  
  int int_this = *((int*) this); 
  int int_that = *((int*) that); 

  if(int_this == int_that){
    return 0; 
  }
  else if(int_this > int_that){
    return -1; 
  }
  else{
    return 1; 
  }

} 

//Function to calculate average time 
void findavgTimeFCFS( ProcessType plist[], int n) 
{ 
    //Function to find waiting time of all processes 
    findWaitingTime(plist, n); 
  
    //Function to find turn around time for all processes 
    findTurnAroundTime(plist, n); 
  
    //Display processes along with all details 
    printf("\n*********\nFCFS\n");
}

//Function to calculate average time 
void findavgTimeSJF( ProcessType plist[], int n) 
{ 
    //Function to find waiting time of all processes 
    findWaitingTimeSJF(plist, n); 
  
    //Function to find turn around time for all processes 
    findTurnAroundTime(plist, n); 
  
    //Display processes along with all details 
    printf("\n*********\nSJF\n");
}

//Function to calculate average time 
void findavgTimeRR( ProcessType plist[], int n, int quantum) 
{ 
    //Function to find waiting time of all processes 
    findWaitingTimeRR(plist, n, quantum); 
  
    //Function to find turn around time for all processes 
    findTurnAroundTime(plist, n); 
  
    //Display processes along with all details 
    printf("\n*********\nRR Quantum = %d\n", quantum);
}

//Function to calculate average time 
void findavgTimePriority( ProcessType plist[], int n) 
{ 
  
   /*
    * 1- Sort the processes (i.e. plist[]), burst time and priority according to the priority.
    * 2- Now simply apply FCFS algorithm.
    */
    int prio[n]; 
    for(int i = 0; i < n; i++){
      prio[i]=plist[i].pri; 
    }

    qsort(prio,n,sizeof(int), my_comparer);

    int pids[n];
    ProcessType plist2[n]; 
    bool move = true; 

    for(int i = 0; i<n; i++){
      move = true; 
      for(int el=0; el<n; el++){
        if(prio[i]==plist[el].pri && move == true && pids[plist[el].pid-1]!=1){
          move = false; 
          plist2[i]=plist[el];
          pids[plist[el].pid-1]=1; 
        }
      }
    }

    for(int i = 0; i < n; i++){
      plist[i] = plist2[i];
    }
  
    //Display processes along with all details 
    printf("\n*********\nPriority\n");
}

void printMetrics(ProcessType plist[], int n)
{
    int total_wt = 0, total_tat = 0; 
    float awt, att;
    
    printf("\tProcesses\tBurst time\tWaiting time\tTurn around time\n"); 
  
    // Calculate total waiting time and total turn  
    // around time 
    for (int  i=0; i<n; i++) 
    { 
        total_wt = total_wt + plist[i].wt; 
        total_tat = total_tat + plist[i].tat; 
        printf("\t%d\t\t%d\t\t%d\t\t%d\n", plist[i].pid, plist[i].bt, plist[i].wt, plist[i].tat); 
    } 
  
    awt = ((float)total_wt / (float)n);
    att = ((float)total_tat / (float)n);
    
    printf("\nAverage waiting time = %.2f", awt); 
    printf("\nAverage turn around time = %.2f\n", att); 
} 

ProcessType * initProc(char *filename, int *n) 
{
  	FILE *input_file = fopen(filename, "r");
	  if (!input_file) {
		    fprintf(stderr, "Error: Invalid filepath\n");
		    fflush(stdout);
		    exit(0);
	  }

    ProcessType *plist = parse_file(input_file, n);
  
    fclose(input_file);
  
    return plist;
}
  
// Driver code 
int main(int argc, char *argv[]) 
{ 
    int n; 
    int quantum = 2;

    ProcessType *proc_list;
  
    if (argc < 2) {
		   fprintf(stderr, "Usage: ./schedsim <input-file-path>\n");
		   fflush(stdout);
		   return 1;
	   }
    
  // FCFS
    n = 0;
    proc_list = initProc(argv[1], &n);
  
    findavgTimeFCFS(proc_list, n);
    
    printMetrics(proc_list, n);
  
  // SJF
    n = 0;
    proc_list = initProc(argv[1], &n);
   
    findavgTimeSJF(proc_list, n); 
   
    printMetrics(proc_list, n);
  
  // Priority
    n = 0; 
    proc_list = initProc(argv[1], &n);
    
    findavgTimePriority(proc_list, n); 
    
    printMetrics(proc_list, n);
    
  // RR
    n = 0;
    proc_list = initProc(argv[1], &n);
    
    findavgTimeRR(proc_list, n, quantum); 
    
    printMetrics(proc_list, n);
    
    return 0; 
} 