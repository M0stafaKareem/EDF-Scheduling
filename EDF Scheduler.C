#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int task_id;
    int arrival_time;
    int iteration;
    int execution_time;
    int rem_execution_time;
    int deadline;
    int curr_deadline;
} Task;

// Function to compare tasks based on their deadlines
int compare(const void *a, const void *b) {
    return ((Task*)a)->curr_deadline - ((Task*)b)->curr_deadline;
}

// Function to find the greatest common divisor (GCD) of two numbers
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Function to find the least common multiple (LCM) of two numbers
int lcm(int a, int b) {
    return (a * b) / gcd(a, b);
}

// Function to find the LCM of an array of numbers
int find_lcm(int arr[], int n) {
    int result = arr[0];
    for (int i = 1; i < n; i++) {
        result = lcm(result, arr[i]);
    }
    return result;
}


void refresh_Tasks(Task tasks[] , int n , int time_stamp){
    for(int i=0;i<n;i++){
        if(time_stamp % tasks[i].deadline == 0 && tasks[i].rem_execution_time > 0 && time_stamp != 0){
            printf("Task%d Missed deadline at T: %d\n",tasks[i].task_id , time_stamp);
        }
        if( time_stamp % tasks[i].deadline == 0 ){
            tasks[i].rem_execution_time = tasks[i].execution_time ;
            tasks[i].curr_deadline = tasks[i].deadline ;
            tasks[i].iteration = (time_stamp / tasks[i].deadline) + 1 ;
        }
        else{
        tasks[i].curr_deadline = (tasks[i].deadline * tasks[i].iteration) - time_stamp;
        }
    }
    
}

// Function to simulate EDF scheduling
void edf_schedule(Task tasks[], int n, int lcm_of_periods) {
    int current_time = 0 ;
    
    while (current_time < lcm_of_periods) {
        refresh_Tasks(tasks , n , current_time) ;
        qsort(tasks, n, sizeof(Task), compare);
        for (int i = 0; i < n; i++) {
            if(tasks[i].rem_execution_time == 0 || tasks[i].arrival_time > current_time) {
                continue ;
            }
            tasks[i].rem_execution_time--;
            printf( "Time: %d ~ %d executing: Task%d\n", current_time , current_time+1 , tasks[i].task_id );
            break;
        }
            current_time++;
    }
}
    

int main() {
    int n;

    printf("Enter the number of tasks: ");
    scanf("%d", &n);

    int periods[n];
    Task tasks[n] ;

    for (int i = 0; i < n; i++) {
        printf("Enter details for Task %d:\n", i + 1);
        tasks[i].task_id = i + 1;

        printf("Arrival Time: ");
        scanf("%d", &tasks[i].arrival_time);

        printf("Execution Time: ");
        scanf("%d", &tasks[i].execution_time);

        printf("Deadline: ");
        scanf("%d", &tasks[i].deadline);
        tasks[i].deadline += tasks[i].arrival_time ;

        periods[i] = tasks[i].deadline;  // Store the periods for LCM calculation
    }
    int end_time ;
    printf("Choose When to stop testing:\n  For Least common multiple ,Enter: 1\n  For custom end time stamp: 2\n\tChoise: ");
    scanf("%d",&end_time);
    if( end_time == 2 ){
        printf("Enter time to stop at: ");
        scanf("%d", &end_time);

    }
    else{
    end_time = find_lcm(periods, n) + 1 ;
    printf("LCM of periods: %d\n", end_time - 1);
    }

    edf_schedule(tasks, n, end_time );

    return 0;
}
