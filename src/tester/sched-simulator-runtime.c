/* This program is free software; you can redistribute it and/or modify it
 * under the terms of the license (GNU LGPL) which comes with this package. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include "simgrid/msg.h" /* Yeah! If you want to use msg, you need to include msg/msg.h */
#include "xbt/sysdep.h"  /* calloc, printf */

#include "polynomials.h"

/* Create a log channel to have nice outputs. */
#include "xbt/log.h"
#include "xbt/asserts.h"
XBT_LOG_NEW_DEFAULT_CATEGORY(msg_test,
                             "Messages specific for this msg example");

void backFill(double *runtimes, int *cores, int *submit, int *orig_pos, int policy, int queue_num_tasks, int num_tasks_disp);
void sortTasksQueue(double *runtimes, int *cores, int *submit, double * runtimes_norm, double* cores_norm, double * submit_norm, int *orig_pos, int policy, int queue_num_tasks, int num_tasks_disp);
const char *getfield(char *line, int num);
void readModelFile(void);
int master(int argc, char *argv[]);
int taskManager(int argc, char *argv[]);
msg_error_t test_all(const char *platform_file,
                     const char *application_file);

#define FINALIZE ((void *)221297) /* a magic number to tell people to stop working */
#define P_min 1.0
#define Q_min 1.0
#define R_min 611.0
#define P_max 162192.0
#define Q_max 256.0
#define R_max 453219.0
#define MAX_TASKS 1024
#define WORKERS_PER_NODE 1
#define MAX_TASK_TYPES 5
#define TERA 1e12
#define MEGA 1e6
#define TAO 10
#define QUEUE_NUM_TASKS 32
#define NUM_TASKS_STATE 16
#define EPSILON 1e-20

#define FCFS 0
#define SJF 1
#define WFP3 2
#define UNICEF 3
#define EASY 4
#define F4 6
#define F3 7
#define F2 8
#define F1 9
#define SAF 10
#define LIN 11
#define QDR 12
#define CUB 13
#define QUA 14
#define QUI 15
#define SEX 16
#define NEW_3_1 17
#define MEM1 18
#define MEM2 19
#define NEW_3_2 20
#define NEW_3_3 21
#define NEW_3_4 22
#define NEW_3_5 23
#define SER_1 24
#define NEW_3_6 25
#define NEW_3_7 26
#define NEW_3_8 27
#define NEW_3_9 28
#define NEW_3_10 29
//CORRELATION ANALYSIS
#define S3_V1_D3 31
#define S3_V1_D4 32
#define S3_V2_D1 33
#define S3_V2_D2 34
#define S3_V2_D3 35
#define S3_V2_D4 36
#define S3_V3_D1 37
#define S3_V3_D2 38
#define S3_V3_D3 39
#define S3_V3_D4 40
#define S3_V4_D1 41
#define S3_V4_D2 42
#define S3_V4_D3 43
#define S3_V4_D4 44
#define S3_V5_D1 45
#define S3_V5_D2 46
#define S3_V5_D3 47
#define S3_V5_D4 48
#define S3_V6_D1 49
#define S3_V6_D2 50
#define S3_V6_D3 51
#define S3_V7_D1 52
#define S3_V7_D2 53
#define S3_V7_D3 54
#define S3_V7_D4 55
#define S3_V8_D1 56
#define S3_V8_D2 57
#define S3_V8_D3 58
#define S3_V8_D4 59
#define S3_V9_D1 60
#define S3_V9_D2 61
#define S3_V9_D3 62
#define S3_V9_D4 63
#define S3_V10_D1 64
#define S3_V10_D2 65
#define S3_V10_D3 66
#define S3_V10_D4 67

#define S4_V7_D1 68
#define S4_V4_D3 69
#define S4_V9_D2 70
#define S4_V3_D1 71
#define S4_V9_D4 72
#define MEM3 73
#define MEM4 74
#define SER_1_1 75
#define SER_1_2 76
#define SER_1_3 77
#define SER_2_1 78
#define SER_2_2 79
#define SER_2_3 80
#define SER_3_1 81
#define SER_3_2 82
#define SER_3_3 83
#define SER_4_1 84
#define SER_4_2 85
#define SER_4_3 86
#define SER_5_1 87
#define SER_5_2 88
#define SER_5_3 89
#define SER_6_1 90
#define SER_6_2 91
#define SER_6_3 92
#define SER_7_1 93
#define SER_7_2 94
#define SER_7_3 95
#define SER_8_1 96
#define SER_8_2 97
#define SER_8_3 98
#define SER_9_1 99
#define SER_9_2 100
#define SER_9_3 101
#define SER_10_1 102
#define SER_10_2 103
#define SER_10_3 104
#define Q3P 105
#define Q2P 106

int number_of_tasks = 0;

struct task_t
{
    int numNodes;
    double startTime;
    double endTime;
    double submitTime;
    double *task_comp_size;
    double *task_comm_size;
    msg_host_t *task_workers;
    int *task_allocation;
};

// long task_comp_sizes[MAX_TASK_TYPES];
// long task_comm_sizes[MAX_TASK_TYPES];
// int num_tasks[10];
// int tasks_created[10];
// int seed;
int VERBOSE = 0;
int STATE = 0;

double *all_runtimes;
int *all_submit;
int *all_cores;

int *orig_task_positions;

// double** tasks_comp_sizes;
// double** tasks_comm_sizes;
// msg_host_t** tasks_workers;
// int** tasks_allocation;

double *slowdown;

struct task_t *task_queue = NULL;
msg_process_t p_master;

int chosen_policy = FCFS;
int *busy_workers;
int num_managers;
int number_of_nodes;

double *sched_task_placement;
// int** sched_task_types;
// int* tasks_per_worker;
// int number_of_tasks = QUEUE_NUM_TASKS + NUM_TASKS_STATE;
double t0 = 0.0f;
double findMax(double array[], int size) {
    double max = array[0];
    for (int i = 1; i < size; i++) {
        if (array[i] > max) {
            max = array[i];
        }
    }
    return max;
}
double findMin(double array[], int size) {
    double min = array[0];
    for (int i = 1; i < size; i++) {
        if (array[i] < min) {
            min = array[i];
        }
    }
    return min;
}
double* convertIntToDouble(int* tableau, int taille) {
    double* tableauDouble = (double*)malloc(taille * sizeof(double));
    if (tableauDouble == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < taille; i++) {
        tableauDouble[i] = (double)tableau[i];
    }
    return tableauDouble;
}
double computeMean(double* tableau, int taille) {
    double somme = 0.0;
    for (int i = 0; i < taille; i++) {
        somme += tableau[i];
    }
    //XBT_INFO("tab = %f",tableau[0]);
    return somme / ((double)taille);
}

// Fonction pour calculer l'écart-type d'un tableau
double computeSigma(double* tableau, int taille, double moyenne) {
    float somme = 0.0;
    for (int i = 0; i < taille; i++) {
        somme += pow(tableau[i] - moyenne, 2);
    }
    return sqrt(somme / ((double)taille));
}

// Fonction pour normaliser le tableau
double* minMaxNorm(double* tableau, int taille,double min, double max) {
    
    
   //XBT_INFO("Mean = %f",moyenne);
    //XBT_INFO("STD = %f",ecartType);
    
    double* tableauNormalise = (double*)malloc(taille * sizeof(double));
    if (tableauNormalise == NULL) {
        // Si l'allocation de mémoire échoue
        fprintf(stderr, "Erreur d'allocation de mémoire\n");
        exit(EXIT_FAILURE);
    }
    //double x_min= findMin(tableau,taille);
    //double x_max= findMax(tableau,taille);
    for (int i = 0; i < taille; i++) {
        tableauNormalise[i]= (tableau[i]-min)/(max-min);
 
        //XBT_INFO("STD = %f",tableauNormalise[i]);
    }

    return tableauNormalise;
}
void backFill(double *runtimes, int *cores, int *submit, int *orig_pos, int policy, int queue_num_tasks, int num_tasks_disp)
{
    int i, j;
    int curr_time = MSG_get_clock();
    int num_arrived_tasks = 0;
    for (i = 0; i < queue_num_tasks; i++)
    {
        if (submit[i] <= curr_time)
        {
            num_arrived_tasks++;
        }
        else
        {
            break;
        }
    }
    // printf("%d ", num_arrived_tasks);
    if (num_arrived_tasks == 1)
        return;

    // double remaining_time[num_tasks_disp];
    double *remaining_time = (double *)calloc(num_tasks_disp, sizeof(double));
    for (i = 0; i < num_tasks_disp; i++)
    {
        if (task_queue[i].endTime == 0.0f)
        {
            float task_elapsed_time = curr_time - task_queue[i].startTime;
            remaining_time[i] = all_runtimes[i] - task_elapsed_time;
        }
        else
        {
            remaining_time[i] = -1.0;
        }
    }
    int available_nodes = 0;
    for (j = 0; j < number_of_nodes; j++)
    {
        if (busy_workers[j] == 0)
        {
            available_nodes++;
        }
    }
    int shadow_time = 0;
    int available_nodes_future = 0;
    int extra_nodes = 0;
    int min_remaining = INT_MAX;
    int min_remaining_task = 0;
    if (available_nodes < cores[0])
    {
        for (i = 0; i < num_tasks_disp; i++)
        {
            min_remaining = INT_MAX;
            for (j = 0; j < num_tasks_disp; j++)
            {
                if (remaining_time[j] != -1.0 && remaining_time[j] < min_remaining)
                {
                    min_remaining = remaining_time[j];
                    min_remaining_task = j;
                }
            }
            remaining_time[min_remaining_task] = INT_MAX;
            available_nodes_future += all_cores[min_remaining_task];
            if (available_nodes + available_nodes_future >= cores[0])
            {
                shadow_time = curr_time + min_remaining;
                extra_nodes = (available_nodes + available_nodes_future) - cores[0];
                break;
            }
        }
    }
    for (i = 1; i < num_arrived_tasks; i++)
    {
        if ((cores[i] <= available_nodes && (curr_time + runtimes[i]) <= shadow_time) || (cores[i] <= (available_nodes < extra_nodes ? available_nodes : extra_nodes)))
        {
            if (VERBOSE)
                XBT_INFO("\"Task_%d\" [r=%.1f,c=%d, s=%d] Backfilled. Shadow Time=%d, Extra Nodes=%d.", orig_pos[i], runtimes[i], cores[i], submit[i], shadow_time, extra_nodes);
            double r_buffer = runtimes[i];
            int c_buffer = cores[i];
            int s_buffer = submit[i];
            int p_buffer = orig_pos[i];
            for (j = i; j > 0; j--)
            {
                runtimes[j] = runtimes[j - 1];
                cores[j] = cores[j - 1];
                submit[j] = submit[j - 1];
                orig_pos[j] = orig_pos[j - 1];
            }
            runtimes[0] = r_buffer;
            cores[0] = c_buffer;
            submit[0] = s_buffer;
            orig_pos[0] = p_buffer;
            break;
        }
    }
}

void sortTasksQueue(double *runtimes, int *cores, int *submit,double * runtimes_norm, double* cores_norm, double * submit_norm, int *orig_pos, int policy, int queue_num_tasks, int num_tasks_disp)
{
    const char* filename = "out.txt";
    
    int fd = open(filename, O_WRONLY);
    FILE* output_file = fopen(filename, "w+");
    int i, j;
    int curr_time = MSG_get_clock();
    int num_arrived_tasks = 0;
    for (i = 0; i < queue_num_tasks; i++)
    {
        if (submit[i] <= curr_time)
        {
            num_arrived_tasks++;
        }
        else
        {
            break;
        }
    }
    // printf("%d ", num_arrived_tasks);
    if (num_arrived_tasks == 1)
        return;
    if (policy == EASY)
    {
        // double remaining_time[num_tasks_disp];
        double *remaining_time = (double *)calloc(num_tasks_disp, sizeof(double));
        for (i = 0; i < num_tasks_disp; i++)
        {
            if (task_queue[i].endTime == 0.0f)
            {
                float task_elapsed_time = curr_time - task_queue[i].startTime;
                remaining_time[i] = all_runtimes[i] - task_elapsed_time;
            }
            else
            {
                remaining_time[i] = -1.0;
            }
        }
        int available_nodes = 0;
        for (j = 0; j < number_of_nodes; j++)
        {
            if (busy_workers[j] == 0)
            {
                available_nodes++;
            }
        }
        int shadow_time = 0;
        int available_nodes_future = 0;
        int extra_nodes = 0;
        int min_remaining = INT_MAX;
        int min_remaining_task = 0;
        if (available_nodes < cores[0])
        {
            for (i = 0; i < num_tasks_disp; i++)
            {
                min_remaining = INT_MAX;
                for (j = 0; j < num_tasks_disp; j++)
                {
                    if (remaining_time[j] != -1.0 && remaining_time[j] < min_remaining)
                    {
                        min_remaining = remaining_time[j];
                        min_remaining_task = j;
                    }
                }
                remaining_time[min_remaining_task] = INT_MAX;
                available_nodes_future += all_cores[min_remaining_task];
                if (available_nodes + available_nodes_future >= cores[0])
                {
                    shadow_time = curr_time + min_remaining;
                    extra_nodes = (available_nodes + available_nodes_future) - cores[0];
                    break;
                }
            }
        }
        for (i = 1; i < num_arrived_tasks; i++)
        {
            if ((cores[i] <= available_nodes && (curr_time + runtimes[i]) <= shadow_time) || (cores[i] <= (available_nodes < extra_nodes ? available_nodes : extra_nodes)))
            {
                if (VERBOSE)
                    XBT_INFO("\"Task_%d\" [r=%.1f,c=%d, s=%d] Backfilled. Shadow Time=%d, Extra Nodes=%d.", orig_pos[i], runtimes[i], cores[i], submit[i], shadow_time, extra_nodes);
                double r_buffer = runtimes[i];
                int c_buffer = cores[i];
                int s_buffer = submit[i];
                int p_buffer = orig_pos[i];
                for (j = i; j > 0; j--)
                {
                    runtimes[j] = runtimes[j - 1];
                    cores[j] = cores[j - 1];
                    submit[j] = submit[j - 1];
                    orig_pos[j] = orig_pos[j - 1];
                }
                runtimes[0] = r_buffer;
                cores[0] = c_buffer;
                submit[0] = s_buffer;
                orig_pos[0] = p_buffer;
                break;
            }
        }
        free(remaining_time);
        return;
    }
    if (policy == FCFS)
    {
        // backFill(runtimes, cores, submit, orig_pos, policy, queue_num_tasks,  num_tasks_disp);
        return;
    }
    if (policy == SJF)
    {
        double r_buffer;
        int c_buffer;
        int s_buffer;
        int p_buffer;
        for (i = 0; i < num_arrived_tasks; i++)
        {
            for (j = 0; j < num_arrived_tasks; j++)
            {
                if (runtimes[i] < runtimes[j])
                {
                    r_buffer = runtimes[i];
                    c_buffer = cores[i];
                    s_buffer = submit[i];
                    p_buffer = orig_pos[i];
                    runtimes[i] = runtimes[j];
                    cores[i] = cores[j];
                    submit[i] = submit[j];
                    runtimes_norm[i]=runtimes_norm[j];
                    cores_norm[i]=cores_norm[j];
                    submit_norm[i]=submit_norm[j];
                    orig_pos[i] = orig_pos[j];
                    runtimes[j] = r_buffer;
                    cores[j] = c_buffer;
                    submit[j] = s_buffer;
                    orig_pos[j] = p_buffer;
                }
            }
        }
        // backFill(runtimes, cores, submit, orig_pos, policy, queue_num_tasks,  num_tasks_disp);
        return;
    }
    double *h_values = (double *)calloc(num_arrived_tasks, sizeof(double));
    double *r_temp = (double *)calloc(num_arrived_tasks, sizeof(double));
    int *c_temp = (int *)calloc(num_arrived_tasks, sizeof(int));
    int *s_temp = (int *)calloc(num_arrived_tasks, sizeof(int));
    double *rn_temp = (double *)calloc(num_arrived_tasks, sizeof(double));
    double *cn_temp = (double *)calloc(num_arrived_tasks, sizeof(double));
    double *sn_temp = (double *)calloc(num_arrived_tasks, sizeof(double));
    int *p_temp = (int *)calloc(num_arrived_tasks, sizeof(int));
    int max_arrive = 0;
    double p_mean = 0;
    double r_mean = 0;
    double q_mean = 0;
    double p_mean_4 = 0;
    double r_mean_4 = 0;
    double q_mean_4 = 0;
    
    for (i = 0; i < num_arrived_tasks; i++)
    {
        if (submit[i] > max_arrive)
        {
            max_arrive = submit[i];
        }
    }
    if(i==0){
            r_mean_4=submit[i];
            p_mean_4=runtimes[i];
            q_mean_4=cores[i];
        }else{
            r_mean_4= ((i*r_mean_4)/(i+1))+(submit[i]/(i+1));
            p_mean_4= ((i*p_mean_4)/(i+1))+(runtimes[i]/(i+1));
            q_mean_4= ((i*q_mean_4)/(i+1))+(cores[i]/(i+1));
        }

    // printf("%d\n", max_arrive);
    int task_age = 0;
    for (i = 0; i < num_arrived_tasks; i++)
    {
        task_age = curr_time - submit[i];
        switch (policy)
        {
        case 30:
            h_values[i] = ((float)task_age / (float)runtimes[i]) * cores[i];
            break;
        case WFP3:
            h_values[i] = pow((float)task_age / (float)runtimes[i], 3) * cores[i];
            break;
        case UNICEF:
            h_values[i] = (task_age + EPSILON) / (log2((double)cores[i] + EPSILON) * runtimes[i]);
            break;
        case MEM1:
            if(i==0){
                h_values[i] = submit[i];
            }else{
                h_values[i] = r_mean + (runtimes[i]-p_mean)+(cores[i]-q_mean);
            }
            break;
        case MEM2:
            if(i==0){
                h_values[i] = submit[i];
            }else{
                h_values[i] = r_mean + ((runtimes[i]*cores[i])-(p_mean*q_mean));
            }
            break;
        case MEM3:
            if(i==0){
                h_values[i] = submit[i];
            }else{
                h_values[i] = submit[i] + ((runtimes[i]*cores[i])-(p_mean*q_mean));
            }
            break;
        case MEM4:
            
            h_values[i] = r_mean_4 + ((runtimes[i]*cores[i])-(p_mean_4*q_mean_4));
           
            break;
        case F4:
            h_values[i] = (0.0056500287 * runtimes[i]) * (0.0000024814 * sqrt(cores[i])) + (0.0074444355 * log10(submit[i])); // 256nodes
            break;
        case F3:
            h_values[i] = (-0.2188093701 * runtimes[i]) * (-0.0000000049 * cores[i]) + (0.0073580361 * log10(submit[i])); // 256nodes
            break;
        case F2:
            h_values[i] = (0.0000342717 * sqrt(runtimes[i])) * (0.0076562110 * cores[i]) + (0.0067364626 * log10(submit[i])); // 256nodes
            break;
        case F1:
            h_values[i] = (-0.0155183403 * log10(runtimes[i])) * (-0.0005149209 * cores[i]) + (0.0069596182 * log10(submit[i])); // 256nodes
            break;
        case SAF:
            h_values[i] = runtimes[i] * cores[i];
            break;
        case LIN:
            h_values[i] = linear( runtimes_norm[i], cores_norm[i], submit_norm[i]);
            break;
        case QDR:
            h_values[i] = quadratic(runtimes[i], cores[i], submit[i]);
            break;
        case CUB:
            h_values[i] = cubic(runtimes[i], cores[i], submit[i]);
            break;
        case QUA:
            h_values[i] = quartic(runtimes[i], cores[i], submit[i]);
            break;
        case QUI:
            h_values[i] = quintic(runtimes[i], cores[i], submit[i]);
            break;
        case SEX:
            h_values[i] = sextic(runtimes[i], cores[i], submit[i]);
            break;
        case NEW_3_1:
            h_values[i] = new_3_1(runtimes[i], cores[i], submit[i]);
            break;
        case NEW_3_2:
            h_values[i] = new_3_2(runtimes[i], cores[i], submit[i]);
            break;
        case NEW_3_3:
            h_values[i] = new_3_3(runtimes[i], cores[i], submit[i]);
            break;
        case NEW_3_4:
            h_values[i] = new_3_4(runtimes[i], cores[i], submit[i]);
            break;
        case NEW_3_5:
            h_values[i] = new_3_5(runtimes[i], cores[i], submit[i]);
            break;
        case NEW_3_6:
            h_values[i] = new_3_6(runtimes[i], cores[i], submit[i]);
            break;
        case NEW_3_7:
            h_values[i] = new_3_7(runtimes[i], cores[i], submit[i]);
            break;
        case NEW_3_8:
            h_values[i] = new_3_8(runtimes[i], cores[i], submit[i]);
            break;
        case NEW_3_9:
            h_values[i] = new_3_9(runtimes[i], cores[i], submit[i]);
            break;
        case NEW_3_10:
            h_values[i] = new_3_10(runtimes[i], cores[i], submit[i]);
            break;
        case Q3P:
            h_values[i] = pow(cores_norm[i],3)*runtimes_norm[i];
            break;
        case Q2P:
            h_values[i] = pow(cores_norm[i],2)*runtimes_norm[i];
            break;
        
        // CORRELATION ANALYSIS
        case S3_V1_D3:
            h_values[i] = s3_vif_1_3( runtimes_norm[i], cores_norm[i], submit_norm[i]);
            break;
        case S3_V1_D4:
            h_values[i] = s3_vif_1_4( runtimes_norm[i], cores_norm[i], submit_norm[i]);
            break;
        case S3_V2_D1:
            h_values[i] = s3_vif_2_1( runtimes_norm[i], cores_norm[i], submit_norm[i]);
            break;
        case S3_V2_D2:
            h_values[i] = s3_vif_2_2( runtimes_norm[i], cores_norm[i], submit_norm[i]);
            break;
        case S3_V2_D3:
            h_values[i] = s3_vif_2_3( runtimes_norm[i], cores_norm[i], submit_norm[i]);
            break;
        case S3_V2_D4:
            h_values[i] = s3_vif_2_4( runtimes_norm[i], cores_norm[i], submit_norm[i]);
            break;
        case S3_V3_D1:
            h_values[i] = s3_vif_3_1( runtimes_norm[i], cores_norm[i], submit_norm[i]);
            break;
        case S3_V3_D2:
            h_values[i] = s3_vif_3_2( runtimes_norm[i], cores_norm[i], submit_norm[i]);
            break;
        case S3_V3_D3:
            h_values[i] = s3_vif_3_3( runtimes_norm[i], cores_norm[i], submit_norm[i]);
            break;
        case S3_V3_D4:
            h_values[i] = s3_vif_3_4( runtimes_norm[i], cores_norm[i], submit_norm[i]);
            break;
        case S3_V4_D1:
            h_values[i] = s3_vif_4_1( runtimes_norm[i], cores_norm[i], submit_norm[i]);
            break;
        case S3_V4_D2:
            h_values[i] = s3_vif_4_2( runtimes_norm[i], cores_norm[i], submit_norm[i]);
            break;
        case S3_V4_D3:
            h_values[i] = s3_vif_4_3( runtimes_norm[i], cores_norm[i], submit_norm[i]);
            break;
        case S3_V4_D4:
            h_values[i] = s3_vif_4_4( runtimes_norm[i], cores_norm[i], submit_norm[i]);
            break;
        case S3_V5_D1:
            h_values[i] = s3_vif_5_1( runtimes_norm[i], cores_norm[i], submit_norm[i]);
            break;
        case S3_V5_D2:
            h_values[i] = s3_vif_5_2( runtimes_norm[i], cores_norm[i], submit_norm[i]);
            break;
        case S3_V5_D3:
            h_values[i] = s3_vif_5_3( runtimes_norm[i], cores_norm[i], submit_norm[i]);
            break;
        case S3_V5_D4:
            h_values[i] = s3_vif_5_4( runtimes_norm[i], cores_norm[i], submit_norm[i]);
            break;
        case S3_V6_D1:
            h_values[i] = s3_vif_6_1( runtimes_norm[i], cores_norm[i], submit_norm[i]);
            break;
        case S3_V6_D2:
            h_values[i] = s3_vif_6_2( runtimes_norm[i], cores_norm[i], submit_norm[i]);
            break;
        case S3_V6_D3:
            h_values[i] = s3_vif_6_3( runtimes_norm[i], cores_norm[i], submit_norm[i]);
            break;
        case S3_V7_D1:
            h_values[i] = s3_vif_7_1( runtimes_norm[i], cores_norm[i], submit_norm[i]);
            break;
        case S3_V7_D2:
            h_values[i] = s3_vif_7_2( runtimes_norm[i], cores_norm[i], submit_norm[i]);
            break;
        case S3_V7_D3:
            h_values[i] = s3_vif_7_3( runtimes_norm[i], cores_norm[i], submit_norm[i]);
            break;
        case S3_V7_D4:
            h_values[i] = s3_vif_7_4( runtimes_norm[i], cores_norm[i], submit_norm[i]);
            break;
        case S3_V8_D1:
            h_values[i] = s3_vif_8_1( runtimes_norm[i], cores_norm[i], submit_norm[i]);
            break;
        case S3_V8_D2:
            h_values[i] = s3_vif_8_2( runtimes_norm[i], cores_norm[i], submit_norm[i]);
            break;
        case S3_V8_D3:
            h_values[i] = s3_vif_8_3( runtimes_norm[i], cores_norm[i], submit_norm[i]);
            break;
        case S3_V8_D4:
            h_values[i] = s3_vif_8_4( runtimes_norm[i], cores_norm[i], submit_norm[i]);
            break;
        case S3_V9_D1:
            h_values[i] = s3_vif_9_1( runtimes_norm[i], cores_norm[i], submit_norm[i]);
            break;
        case S3_V9_D2:
            h_values[i] = s3_vif_9_2( runtimes_norm[i], cores_norm[i], submit_norm[i]);
            break;
        case S3_V9_D3:
            h_values[i] = s3_vif_9_3( runtimes_norm[i], cores_norm[i], submit_norm[i]);
            break;
        case S3_V9_D4:
            h_values[i] = s3_vif_9_4( runtimes_norm[i], cores_norm[i], submit_norm[i]);
            break;
        case S3_V10_D1:
            h_values[i] = s3_vif_10_1( runtimes_norm[i], cores_norm[i], submit_norm[i]);
            break;
         case S3_V10_D2:
            h_values[i] = s3_vif_10_2( runtimes_norm[i], cores_norm[i], submit_norm[i]);
            break;
         case S3_V10_D3:
            h_values[i] = s3_vif_10_3( runtimes_norm[i], cores_norm[i], submit_norm[i]);
            break;
        case S3_V10_D4:
            h_values[i] = s3_vif_10_4( runtimes_norm[i], cores_norm[i], submit_norm[i]);
            break;

        case S4_V7_D1:
            h_values[i] = s4_vif_7_1(runtimes[i], cores[i], submit[i]);
            break;
        case S4_V4_D3:
            h_values[i] = s4_vif_4_3(runtimes[i], cores[i], submit[i]);
            break;
        case S4_V9_D2:
            h_values[i] = s4_vif_9_2(runtimes[i], cores[i], submit[i]);
            break;
        case S4_V3_D1:
            h_values[i] = s4_vif_3_1(runtimes[i], cores[i], submit[i]);
            break;
        case S4_V9_D4:
            h_values[i] = s4_vif_9_4(runtimes[i], cores[i], submit[i]);
            break;
        case SER_1_1:
            h_values[i] = ser_1_1(runtimes_norm[i], cores_norm[i], submit_norm[i],p_mean,q_mean,r_mean);
            break;
        case SER_1_2:
            h_values[i] = ser_1_2(runtimes_norm[i], cores_norm[i], submit_norm[i],p_mean,q_mean,r_mean);
            break;
        case SER_1_3:
            h_values[i] = ser_1_3(runtimes_norm[i], cores_norm[i], submit_norm[i],p_mean,q_mean,r_mean);
            break;
        case SER_2_1:
            h_values[i] = ser_2_1(runtimes_norm[i], cores_norm[i], submit_norm[i],p_mean,q_mean,r_mean);
            break;
        case SER_2_2:
            h_values[i] = ser_2_2(runtimes_norm[i], cores_norm[i], submit_norm[i],p_mean,q_mean,r_mean);
            break;
        case SER_2_3:
            h_values[i] = ser_2_3(runtimes_norm[i], cores_norm[i], submit_norm[i],p_mean,q_mean,r_mean);
            break;
        case SER_3_1:
            h_values[i] = ser_3_1(runtimes_norm[i], cores_norm[i], submit_norm[i],p_mean,q_mean,r_mean);
            break;
        case SER_3_2:
            h_values[i] = ser_3_2(runtimes_norm[i], cores_norm[i], submit_norm[i],p_mean,q_mean,r_mean);
            break;
        case SER_3_3:
            h_values[i] = ser_3_3(runtimes_norm[i], cores_norm[i], submit_norm[i],p_mean,q_mean,r_mean);
            break;
        case SER_4_1:
            h_values[i] = ser_4_1(runtimes_norm[i], cores_norm[i], submit_norm[i],p_mean,q_mean,r_mean);
            break;
        case SER_4_2:
            h_values[i] = ser_4_2(runtimes_norm[i], cores_norm[i], submit_norm[i],p_mean,q_mean,r_mean);
            break;
        case SER_4_3:
            h_values[i] = ser_4_3(runtimes_norm[i], cores_norm[i], submit_norm[i],p_mean,q_mean,r_mean);
            break;
        case SER_5_1:
            h_values[i] = ser_5_1(runtimes_norm[i], cores_norm[i], submit_norm[i],p_mean,q_mean,r_mean);
            break;
        case SER_5_2:
            h_values[i] = ser_5_2(runtimes_norm[i], cores_norm[i], submit_norm[i],p_mean,q_mean,r_mean);
            break;
        case SER_5_3:
            h_values[i] = ser_5_3(runtimes_norm[i], cores_norm[i], submit_norm[i],p_mean,q_mean,r_mean);
            break;
        case SER_6_1:
            h_values[i] = ser_6_1(runtimes_norm[i], cores_norm[i], submit_norm[i],p_mean,q_mean,r_mean);
            break;
        case SER_6_2:
            h_values[i] = ser_6_2(runtimes_norm[i], cores_norm[i], submit_norm[i],p_mean,q_mean,r_mean);
            break;
        case SER_6_3:
            h_values[i] = ser_6_3(runtimes_norm[i], cores_norm[i], submit_norm[i],p_mean,q_mean,r_mean);
            break;
        case SER_7_1:
            h_values[i] = ser_7_1(runtimes_norm[i], cores_norm[i], submit_norm[i],p_mean,q_mean,r_mean);
            break;
        case SER_7_2:
            h_values[i] = ser_7_2(runtimes_norm[i], cores_norm[i], submit_norm[i],p_mean,q_mean,r_mean);
            break;
        case SER_7_3:
            h_values[i] = ser_7_3(runtimes_norm[i], cores_norm[i], submit_norm[i],p_mean,q_mean,r_mean);
            break;
        case SER_8_1:
            h_values[i] = ser_8_1(runtimes_norm[i], cores_norm[i], submit_norm[i],p_mean,q_mean,r_mean);
            break;
        case SER_8_2:
            h_values[i] = ser_8_2(runtimes_norm[i], cores_norm[i], submit_norm[i],p_mean,q_mean,r_mean);
            break;
        case SER_8_3:
            h_values[i] = ser_8_3(runtimes_norm[i], cores_norm[i], submit_norm[i],p_mean,q_mean,r_mean);
            break;
        case SER_9_1:
            h_values[i] = ser_9_1(runtimes_norm[i], cores_norm[i], submit_norm[i],p_mean,q_mean,r_mean);
            break;
        case SER_9_2:
            h_values[i] = ser_9_2(runtimes_norm[i], cores_norm[i], submit_norm[i],p_mean,q_mean,r_mean);
            break;
        case SER_9_3:
            h_values[i] = ser_9_3(runtimes_norm[i], cores_norm[i], submit_norm[i],p_mean,q_mean,r_mean);
            break;
        case SER_10_1:
            h_values[i] = ser_10_1(runtimes_norm[i], cores_norm[i], submit_norm[i],p_mean,q_mean,r_mean);
            break;
        case SER_10_2:
            h_values[i] = ser_10_2(runtimes_norm[i], cores_norm[i], submit_norm[i],p_mean,q_mean,r_mean);
            break;
        case SER_10_3:
            h_values[i] = ser_10_3(runtimes_norm[i], cores_norm[i], submit_norm[i],p_mean,q_mean,r_mean);
            break;
        }
        if(i==0){
            r_mean=submit_norm[i];
            p_mean=runtimes_norm[i];
            q_mean=cores_norm[i];
        }else{
            r_mean= ((i*r_mean)/(i+1))+(submit_norm[i]/(i+1));
            p_mean= ((i*p_mean)/(i+1))+(runtimes_norm[i]/(i+1));
            q_mean= ((i*q_mean)/(i+1))+(cores_norm[i]/(i+1));
        }
        if (VERBOSE)
            XBT_INFO("Score for \"Task_%d\" [r=%f,c=%f,s=%f]=%.7f", orig_pos[i], runtimes_norm[i], cores_norm[i], submit_norm[i], h_values[i]);
    }
    if (policy == WFP3 || policy == UNICEF)
    {
        double max_val = 0.0;
        int max_index;
        for (i = 0; i < num_arrived_tasks; i++)
        {
            max_val = -1e20;
            for (j = 0; j < num_arrived_tasks; j++)
            {
                if (h_values[j] > max_val)
                {
                    max_val = h_values[j];
                    max_index = j;
                }
            }
            r_temp[i] = runtimes[max_index];
            c_temp[i] = cores[max_index];
            s_temp[i] = submit[max_index];
            p_temp[i] = orig_pos[max_index];
            h_values[max_index] = -1e20;
        }
    }
    else if (policy >= F4)
    {
        double min_val = 1e20;
        int min_index;
        for (i = 0; i < num_arrived_tasks; i++)
        {
            min_val = 1e20;
            min_index = 0;
            for (j = 0; j < num_arrived_tasks; j++)
            {
                    /*char text[50];
                    sprintf(text, "%f\n", h_values[j]);
                    //fwrite("coucou",1,strlen(text),output_file);
                    write(fd, text, strlen(text));*/
                    
                if (h_values[j] < min_val)
                {
                    

                    min_val = h_values[j];
                    min_index = j;
                }
            }
            r_temp[i] = runtimes[min_index];
            c_temp[i] = cores[min_index];
            s_temp[i] = submit[min_index];
            rn_temp[i] = runtimes_norm[min_index];
            cn_temp[i] = cores_norm[min_index];
            sn_temp[i] = submit_norm[min_index];
            p_temp[i] = orig_pos[min_index];
            h_values[min_index] = 1e20;
        }
    }
    for (i = 0; i < num_arrived_tasks; i++)
    {
        runtimes[i] = r_temp[i];
        cores[i] = c_temp[i];
        submit[i] = s_temp[i];
        runtimes_norm[i] = rn_temp[i];
        cores_norm[i] = cn_temp[i];
        submit_norm[i] = sn_temp[i];
        orig_pos[i] = p_temp[i];
    }
    // backFill(runtimes, cores, submit, orig_pos, policy, queue_num_tasks,  num_tasks_disp);
    free(r_temp);
    free(c_temp);
    free(s_temp);
    free(rn_temp);
    free(cn_temp);
    free(sn_temp);
    free(p_temp);
    free(h_values);
    
}

const char *getfield(char *line, int num)
{
    const char *tok;
    for (tok = strtok(line, ",");
         tok && *tok;
         tok = strtok(NULL, ",\n"))
    {
        if (!--num)
            return tok;
    }
    return NULL;
}

void readModelFile(void)
{

    all_runtimes = (double *)malloc((number_of_tasks) * sizeof(double));
    all_submit = (int *)malloc((number_of_tasks) * sizeof(int));
    all_cores = (int *)malloc((number_of_tasks) * sizeof(int));
    int task_count = 0;

    // FILE* stream = fopen("workload-test.csv.old", "r");
    FILE *stream = fopen("initial-simulation-submit.csv", "r");

    char line[1024];
    while (fgets(line, 1024, stream))
    {
        char *tmp = strdup(line);
        all_runtimes[task_count] = atof(getfield(tmp, 1));
        ;
        free(tmp);
        tmp = strdup(line);
        all_cores[task_count] = atoi(getfield(tmp, 2));
        ;
        free(tmp);
        tmp = strdup(line);
        all_submit[task_count] = atoi(getfield(tmp, 3));
        free(tmp);
        // NOTE strtok clobbers tmp
        task_count++;
    }
}

/** Emitter function  */
int master(int argc, char *argv[])
{
    int workers_count = 0;
    number_of_nodes = 0;
    msg_host_t *workers = NULL;
    msg_host_t task_manager = NULL;
    msg_task_t *todo = NULL;
    // double task_comp_size = 0;
    // double task_comm_size = 0;

    int i;

    int res = sscanf(argv[1], "%d", &workers_count);
    xbt_assert(res, "Invalid argument %s\n", argv[1]);
    res = sscanf(argv[2], "%d", &number_of_nodes);
    xbt_assert(res, "Invalid argument %s\n", argv[2]);
    // res = sscanf(argv[3], "%lg", &task_comm_size);
    // xbt_assert(res, "Invalid argument %s\n", argv[3]);

    // workers_count = argc - 4;

    readModelFile();
    orig_task_positions = (int *)malloc((number_of_tasks) * sizeof(int));
    int c = 0;
    for (i = 0; i < number_of_tasks; i++)
    {
        orig_task_positions[c++] = i;
    }

    /*  criacao das matrizes de saída */
    // sched_task_placement = (double*) calloc((MAX_TASKS),sizeof(double));
    // sched_task_types = (int**) malloc(workers_count*sizeof(int*));

    // srand(seed);

    p_master = MSG_process_self();
    { /* Process organisation (workers) */
        // int i;
        char sprintf_buffer[64];
        workers = xbt_new0(msg_host_t, workers_count);

        for (i = 0; i < workers_count; i++)
        {
            sprintf(sprintf_buffer, "node-%d", (i + WORKERS_PER_NODE) / WORKERS_PER_NODE);
            workers[i] = MSG_get_host_by_name(sprintf_buffer);
            xbt_assert(workers[i] != NULL, "Unknown host %s. Stopping Now! ",
                       sprintf_buffer);
        }
    }

    { /* Process organisation (managers) */
        // num_managers = QUEUE_NUM_TASKS + NUM_TASKS_STATE;
        task_manager = MSG_get_host_by_name("node-0");
        xbt_assert(task_manager != NULL, "Unknown host %s. Stopping Now! ",
                   "node-0");
    }

    if (VERBOSE)
        XBT_INFO("Got %d cores and %d tasks to process", number_of_nodes, number_of_tasks);

    { /*  Task creation */
        int j, k;

        todo = xbt_new0(msg_task_t, number_of_tasks);

        busy_workers = (int *)calloc(number_of_nodes, sizeof(int));
        task_queue = (struct task_t *)malloc(number_of_tasks * sizeof(struct task_t));
        // tasks_comp_sizes = (double**) malloc(number_of_tasks * sizeof(double*));
        // tasks_comm_sizes = (double**) malloc(number_of_tasks * sizeof(double*));
        // tasks_allocation = (int**) malloc(number_of_tasks * sizeof(int*));
        // tasks_workers = xbt_new0(msg_host_t**, number_of_tasks);
        //XBT_INFO("P :");
        //XBT_INFO("cores : %d",all_cores[0]);
        // XBT_INFO("P :");
        double * runtimes_norm = minMaxNorm(all_runtimes,number_of_tasks,P_min,P_max);
        //XBT_INFO("Q :");
        double * cores_norm = minMaxNorm(convertIntToDouble(all_cores,number_of_tasks),number_of_tasks,Q_min,Q_max);
        //XBT_INFO("R :");
        double * submit_norm = minMaxNorm(convertIntToDouble(all_submit,number_of_tasks),number_of_tasks,R_min,R_max);
        for (i = 0; i < number_of_tasks; i++)
        {

            int available_nodes;
            do
            {
                if (i >= NUM_TASKS_STATE)
                {
                    sortTasksQueue(&all_runtimes[i], &all_cores[i], &all_submit[i],&runtimes_norm[i],&cores_norm[i],&submit_norm[i], &orig_task_positions[i], chosen_policy, number_of_tasks - i >= QUEUE_NUM_TASKS ? QUEUE_NUM_TASKS : number_of_tasks - i, i);
                }

                while (MSG_get_clock() < all_submit[i])
                { // task has not arrived yet
                    MSG_process_sleep(all_submit[i] - MSG_get_clock());
                }
                available_nodes = 0;
                for (j = 0; j < number_of_nodes; j++)
                {
                    if (busy_workers[j] == 0)
                    {
                        available_nodes++;
                        if (available_nodes == all_cores[i])
                        {
                            break;
                        }
                    }
                }
                // if(VERBOSE)
                //   XBT_INFO("Available nodes=%d. Nodes needed=%d", available_nodes, all_cores[i]);
                if (available_nodes < all_cores[i])
                {
                    if (VERBOSE)
                        XBT_INFO("Insuficient workers for \"Task_%d\" [r=%.1f,c=%d,s=%d] (%d available workers. need %d). Waiting.", orig_task_positions[i], all_runtimes[i], all_cores[i], all_submit[i], available_nodes, all_cores[i]);
                    // MSG_process_sleep(1.0f);
                    MSG_process_suspend(p_master);
                }
            } while (available_nodes < all_cores[i]);

            task_queue[i].numNodes = all_cores[i];
            task_queue[i].startTime = 0.0f;
            task_queue[i].endTime = 0.0f;
            task_queue[i].submitTime = all_submit[i];
            // task_queue[i].task_comp_size = (double*) malloc(all_cores[i] * sizeof(double));
            // task_queue[i].task_comm_size = (double*) malloc(all_cores[i] * all_cores[i] * sizeof(double));
            task_queue[i].task_allocation = (int *)malloc((all_cores[i]) * sizeof(int));
            // task_queue[i].task_workers = (msg_host_t*) calloc(all_cores[i], sizeof(msg_host_t));

            // tasks_comp_sizes[i] = (double*) malloc(all_cores[i] * sizeof(double));
            // tasks_comm_sizes[i] = (double*) malloc(all_cores[i] * all_cores[i] * sizeof(double));
            // tasks_allocation[i] = (int*) malloc((all_cores[i] + 1) * sizeof(int));
            // tasks_allocation[i][0] = all_cores[i];
            // tasks_workers[i] = xbt_new0(msg_host_t*, all_cores[i]);

            int count = 0;
            for (j = 0; j < number_of_nodes; j++)
            {
                if (busy_workers[j] == 0)
                {
                    // task_queue[i].task_workers[count] = workers[j];
                    task_queue[i].task_allocation[count] = j;
                    busy_workers[j] = 1;
                    count++;
                }
                if (count >= all_cores[i])
                {
                    break;
                }
            }

            msg_host_t self = MSG_host_self();
            double speed = MSG_host_get_speed(self);

            double comp_size = all_runtimes[i] * speed;
            double comm_size = 1000.0f; // 0.001f * MEGA;

            /*
            for (j = 0; j < all_cores[i]; j++) {
                task_queue[i].task_comp_size[j] = comp_size;
            }

            for (j = 0; j < all_cores[i]; j++)
                for (k = j + 1; k < all_cores[i]; k++)
                  task_queue[i].task_comm_size[j * all_cores[i] + k] = comm_size;
            */

            char sprintf_buffer[64];
            if (i < NUM_TASKS_STATE)
            {
                sprintf(sprintf_buffer, "Task_%d", i);
            }
            else
            {
                sprintf(sprintf_buffer, "Task_%d", orig_task_positions[i]);
            }
            todo[i] = MSG_task_create(sprintf_buffer, comp_size, comm_size, &task_queue[i]);

            if (VERBOSE)
                XBT_INFO("Dispatching \"%s\" [r=%.1f,c=%d,s=%d]", todo[i]->name, all_runtimes[i], all_cores[i], all_submit[i]);

            MSG_task_send(todo[i], MSG_host_get_name(workers[i]));

            // if(VERBOSE)
            // XBT_INFO("Sent");

            if (i == NUM_TASKS_STATE - 1)
            {
                t0 = MSG_get_clock();

                if (STATE)
                {
                    float *elapsed_times = (float *)calloc(number_of_nodes, sizeof(float));
                    for (j = 0; j <= i; j++)
                    {
                        if (task_queue[j].endTime == 0.0f)
                        {
                            float task_elapsed_time = MSG_get_clock() - task_queue[j].startTime;
                            if (j == i)
                            {
                                task_elapsed_time = 0.01f;
                            }
                            // printf("%d - %f\n", j, task_elapsed_time);
                            for (k = 0; k < all_cores[j]; k++)
                            {
                                // printf("%d\n", task_queue[j].task_allocation[k]);
                                elapsed_times[(task_queue[j].task_allocation[k])] = task_elapsed_time;
                            }
                        }
                    }
                    for (j = 0; j < number_of_nodes; j++)
                    {
                        if (j < number_of_nodes - 1)
                            printf("%f,", elapsed_times[j]);
                        else
                            printf("%f\n", elapsed_times[j]);
                    }
                    break;
                }
            }
        }

        if (STATE)
        {
            if (VERBOSE)
                XBT_INFO("All tasks have been dispatched. Let's tell everybody the computation is over.");
            for (i = NUM_TASKS_STATE; i < num_managers; i++)
            {
                msg_task_t finalize = MSG_task_create("finalize", 0, 0, FINALIZE);
                MSG_task_send(finalize, MSG_host_get_name(workers[i]));
            }
        }

        if (VERBOSE)
            XBT_INFO("Goodbye now!");
        free(workers);
        free(todo);
        return 0;
    }

} /* end_of_master */

/** Receiver function  */
int taskManager(int argc, char *argv[])
{
    msg_task_t task = NULL;
    struct task_t *_task = NULL;
    int i;
    int res;
    // while (1) {
    res = MSG_task_receive(&(task), MSG_host_get_name(MSG_host_self()));
    xbt_assert(res == MSG_OK, "MSG_task_receive failed");
    _task = (struct task_t *)MSG_task_get_data(task);

    if (VERBOSE)
        XBT_INFO("Received \"%s\"", MSG_task_get_name(task));

    if (!strcmp(MSG_task_get_name(task), "finalize"))
    {
        MSG_task_destroy(task);
        return 0;
    }

    if (VERBOSE)
        XBT_INFO("Processing \"%s\"", MSG_task_get_name(task));
    /*
    XBT_INFO("Executing on hosts: ");
    for(i=0;i<_task->numNodes;i++){
      XBT_INFO("\"%s\"", MSG_host_get_name(_task->task_workers[i]));
    }
    */
    _task->startTime = MSG_get_clock();
    MSG_task_execute(task);
    _task->endTime = MSG_get_clock();
    if (VERBOSE)
        XBT_INFO("\"%s\" done", MSG_task_get_name(task));
    int *allocation = _task->task_allocation;
    int n = _task->numNodes;
    // int i;
    for (i = 0; i < n; i++)
    {
        busy_workers[allocation[i]] = 0;
    }
    MSG_task_destroy(task);
    task = NULL;
    MSG_process_resume(p_master);
    //}
    // if(VERBOSE)
    // XBT_INFO("I'm done. See you!");
    return 0;
} /* end_of_worker */

int taskMonitor(int argc, char *argv[])
{
    int i;
    for (i = 0; i < number_of_tasks; i++)
    {
        while (MSG_get_clock() < all_submit[i])
        { // task has not arrived yet
            MSG_process_sleep(all_submit[i] - MSG_get_clock());
        }
        if (VERBOSE)
            XBT_INFO("\"Task_%d\" [r=%.1f,c=%d,s=%d] arrived. Waking up master.", i, all_runtimes[i], all_cores[i], all_submit[i]);
        MSG_process_resume(p_master);
    }
    return 0;
}

/** Test function */
msg_error_t test_all(const char *platform_file,
                     const char *application_file)
{
    
    const char* filename = "out.txt";
    
    int fd = open(filename, O_WRONLY);
    FILE* output_file = fopen(filename, "w+");
    msg_error_t res = MSG_OK;
    int i;

    { /*  Simulation setting */
        MSG_config("host/model", "default");
        // MSG_set_channel_number(1);df3["r"]=df3["r"]*10
        MSG_create_environment(platform_file);
    }
    { /*   Application deployment */
        // int i;

        MSG_function_register("master", master);
        MSG_function_register("taskManager", taskManager);
        MSG_function_register("taskMonitor", taskMonitor);

        MSG_launch_application(application_file);

        char sprintf_buffer[64];

        for (i = 0; i < num_managers; i++)
        {
            sprintf(sprintf_buffer, "node-%d", i + 1);
            MSG_process_create("taskManager", taskManager, NULL, MSG_get_host_by_name(sprintf_buffer));
        }

        sprintf(sprintf_buffer, "node-%d", number_of_tasks + 1);
        MSG_process_create("taskMonitor", taskMonitor, NULL, MSG_get_host_by_name(sprintf_buffer));
    }
    res = MSG_main();

    double sumSlowdown = 0.0f;
    slowdown = (double *)calloc(number_of_tasks - NUM_TASKS_STATE, sizeof(double));
    int _count = 0;
    for (i = NUM_TASKS_STATE; i < number_of_tasks; i++)
    {
        double waitTime = task_queue[i].startTime - task_queue[i].submitTime;
        double runTime = task_queue[i].endTime - task_queue[i].startTime;
        double quocient = runTime >= TAO ? runTime : TAO;
        double slow = (waitTime + runTime) / quocient;
        slowdown[_count] = slow >= 1.0f ? slow : 1.0f;
        /*char text[50];
        sprintf(text, "%f\n", runTime);
        //fwrite("coucou",1,strlen(text),output_file);
        write(fd, text, slowdown[_count]);*/
        sumSlowdown += slowdown[_count];
        // printf("%.2f\n", waitTime);
        // printf("[%.2f %.2f %.2f %d] ", waitTime, slowdown[_count], runTime, task_queue[i].numNodes);
        if (VERBOSE)
            XBT_INFO("Execution Stats for \"Task_%d\" [r=%.1f,c=%d, s=%d]: Wait Time=%.2f, Slowdown=%.2f, Simulated Runtime=%.2f.", orig_task_positions[i], all_runtimes[i], all_cores[i], all_submit[i], waitTime, slowdown[_count], runTime);
        _count++;
    }

    double AVGSlowdown = sumSlowdown / (number_of_tasks - NUM_TASKS_STATE);

    if (VERBOSE)
    {
        XBT_INFO("Average bounded slowdown: %f", AVGSlowdown);
        XBT_INFO("Simulation time %g", MSG_get_clock());
    }
    else if (!STATE)
    {
        printf("%f\n", AVGSlowdown);
    }
    // int i,j;

    /*
    for(i=0;i<MAX_TASKS; i++){
    printf("%f,", sched_task_placement[i]);
    }
    printf("%g\n", MSG_get_clock());
    */

    return res;
} /* end_of_test_all */

/** Main function */
int main(int argc, char *argv[])
{
    msg_error_t res = MSG_OK;

    int i;

    MSG_init(&argc, argv);
    if (argc < 3)
    {
        printf("Usage: %s platform_file deployment_file [-verbose]\n", argv[0]);
        printf("example: %s msg_platform.xml msg_deployment.xml -verbose\n", argv[0]);
        exit(1);
    }
    // seed = atoi(argv[3]);
    if (argc >= 4)
    {
        for (i = 3; i < argc; i++)
        {
            if (strcmp(argv[i], "-verbose") == 0)
            {
                VERBOSE = 1;
            }
            if (strcmp(argv[i], "-state") == 0)
            {
                STATE = 1;
            }
            if (strcmp(argv[i], "-spt") == 0)
            {
                chosen_policy = SJF;
            }
            if (strcmp(argv[i], "-wfp3") == 0)
            {
                chosen_policy = WFP3;
            }
            if (strcmp(argv[i], "-unicef") == 0)
            {
                chosen_policy = UNICEF;
            }
            if (strcmp(argv[i], "-mem1") == 0)
            {
                chosen_policy = MEM1;
            }
            if (strcmp(argv[i], "-mem2") == 0)
            {
                chosen_policy = MEM2;
            }
            if (strcmp(argv[i], "-mem3") == 0)
            {
                chosen_policy = MEM3;
            }
            if (strcmp(argv[i], "-mem4") == 0)
            {
                chosen_policy = MEM4;
            }
            if (strcmp(argv[i], "-easy") == 0)
            {
                chosen_policy = EASY;
            }
            if (strcmp(argv[i], "-f4") == 0)
            {
                chosen_policy = F4;
            }
            if (strcmp(argv[i], "-f3") == 0)
            {
                chosen_policy = F3;
            }
            if (strcmp(argv[i], "-f2") == 0)
            {
                chosen_policy = F2;
            }
            if (strcmp(argv[i], "-f1") == 0)
            {
                chosen_policy = F1;
            }
            if (strcmp(argv[i], "-saf") == 0)
            {
                chosen_policy = SAF;
            }
            if (strcmp(argv[i], "-lin") == 0)
            {
                chosen_policy = LIN;
            }
            if (strcmp(argv[i], "-qdr") == 0)
            {
                chosen_policy = QDR;
            }
            if (strcmp(argv[i], "-cub") == 0)
            {
                chosen_policy = CUB;
            }
            if (strcmp(argv[i], "-qua") == 0)
            {
                chosen_policy = QUA;
            }
            if (strcmp(argv[i], "-qui") == 0)
            {
                chosen_policy = QUI;
            }
            if (strcmp(argv[i], "-sex") == 0)
            {
                chosen_policy = SEX;
            }
            if (strcmp(argv[i], "-new_3_1") == 0)
            {
                chosen_policy = NEW_3_1;
            }
            if (strcmp(argv[i], "-new_3_2") == 0)
            {
                chosen_policy = NEW_3_2;
            }
            if (strcmp(argv[i], "-new_3_3") == 0)
            {
                chosen_policy = NEW_3_3;
            }
            if (strcmp(argv[i], "-new_3_4") == 0)
            {
                chosen_policy = NEW_3_4;
            }
            if (strcmp(argv[i], "-new_3_5") == 0)
            {
                chosen_policy = NEW_3_5;
            }
            if (strcmp(argv[i], "-new_3_6") == 0)
            {
                chosen_policy = NEW_3_6;
            }
            if (strcmp(argv[i], "-new_3_7") == 0)
            {
                chosen_policy = NEW_3_7;
            }
            if (strcmp(argv[i], "-new_3_8") == 0)
            {
                chosen_policy = NEW_3_8;
            }
            if (strcmp(argv[i], "-new_3_9") == 0)
            {
                chosen_policy = NEW_3_9;
            }
            if (strcmp(argv[i], "-new_3_10") == 0)
            {
                chosen_policy = NEW_3_10;
            }
           
            // CORRELATION ANALYSIS
            if (strcmp(argv[i], "-s3_v1_d3") == 0)
            {
                chosen_policy = S3_V1_D3;
            }
            if (strcmp(argv[i], "-s3_v1_d4") == 0)
            {
                chosen_policy = S3_V1_D4;
            }
            if (strcmp(argv[i], "-s3_v2_d1") == 0)
            {
                chosen_policy = S3_V2_D1;
            }
            if (strcmp(argv[i], "-s3_v2_d2") == 0)
            {
                chosen_policy = S3_V2_D2;
            }
            if (strcmp(argv[i], "-s3_v2_d3") == 0)
            {
                chosen_policy = S3_V2_D3;
            }
            if (strcmp(argv[i], "-s3_v2_d4") == 0)
            {
                chosen_policy = S3_V2_D4;
            }
            if (strcmp(argv[i], "-s3_v3_d1") == 0)
            {
                chosen_policy = S3_V3_D1;
            }
            if (strcmp(argv[i], "-s3_v3_d2") == 0)
            {
                chosen_policy = S3_V3_D2;
            }
            if (strcmp(argv[i], "-s3_v3_d3") == 0)
            {
                chosen_policy = S3_V3_D3;
            }
            if (strcmp(argv[i], "-s3_v3_d4") == 0)
            {
                chosen_policy = S3_V3_D4;
            }
            if (strcmp(argv[i], "-s3_v4_d1") == 0)
            {
                chosen_policy = S3_V4_D1;
            }
            if (strcmp(argv[i], "-s3_v4_d2") == 0)
            {
                chosen_policy = S3_V4_D2;
            }
            if (strcmp(argv[i], "-s3_v4_d3") == 0)
            {
                chosen_policy = S3_V4_D3;
            }
            if (strcmp(argv[i], "-s3_v4_d4") == 0)
            {
                chosen_policy = S3_V4_D4;
            }
            if (strcmp(argv[i], "-s3_v5_d1") == 0)
            {
                chosen_policy = S3_V5_D1;
            }
            if (strcmp(argv[i], "-s3_v5_d2") == 0)
            {
                chosen_policy = S3_V5_D2;
            }
            if (strcmp(argv[i], "-s3_v5_d3") == 0)
            {
                chosen_policy = S3_V5_D3;
            }
            if (strcmp(argv[i], "-s3_v5_d4") == 0)
            {
                chosen_policy = S3_V5_D4;
            }
            if (strcmp(argv[i], "-s3_v6_d1") == 0)
            {
                chosen_policy = S3_V6_D1;
            }
            if (strcmp(argv[i], "-s3_v6_d2") == 0)
            {
                chosen_policy = S3_V6_D2;
            }
            if (strcmp(argv[i], "-s3_v6_d3") == 0)
            {
                chosen_policy = S3_V6_D3;
            }
            if (strcmp(argv[i], "-s3_v7_d1") == 0)
            {
                chosen_policy = S3_V7_D1;
            }
            if (strcmp(argv[i], "-s3_v7_d2") == 0)
            {
                chosen_policy = S3_V7_D2;
            }
            if (strcmp(argv[i], "-s3_v7_d3") == 0)
            {
                chosen_policy = S3_V7_D3;
            }
            if (strcmp(argv[i], "-s3_v7_d4") == 0)
            {
                chosen_policy = S3_V7_D4;
            }
            if (strcmp(argv[i], "-s3_v8_d1") == 0)
            {
                chosen_policy = S3_V8_D1;
            }
            if (strcmp(argv[i], "-s3_v8_d2") == 0)
            {
                chosen_policy = S3_V8_D2;
            }
            if (strcmp(argv[i], "-s3_v8_d3") == 0)
            {
                chosen_policy = S3_V8_D3;
            }
            if (strcmp(argv[i], "-s3_v8_d4") == 0)
            {
                chosen_policy = S3_V8_D4;
            }
            if (strcmp(argv[i], "-s3_v9_d1") == 0)
            {
                chosen_policy = S3_V9_D1;
            }
            if (strcmp(argv[i], "-s3_v9_d2") == 0)
            {
                chosen_policy = S3_V9_D2;
            }
            if (strcmp(argv[i], "-s3_v9_d3") == 0)
            {
                chosen_policy = S3_V9_D3;
            }
            if (strcmp(argv[i], "-s3_v9_d4") == 0)
            {
                chosen_policy = S3_V9_D4;
            }
            if (strcmp(argv[i], "-s3_v10_d1") == 0)
            {
                chosen_policy = S3_V10_D1;
            }
            if (strcmp(argv[i], "-s3_v10_d2") == 0)
            {
                chosen_policy = S3_V10_D2;
            }
            if (strcmp(argv[i], "-s3_v10_d3") == 0)
            {
                chosen_policy = S3_V10_D3;
            }
            if (strcmp(argv[i], "-s3_v10_d4") == 0)
            {
                chosen_policy = S3_V10_D4;
            }


            if (strcmp(argv[i], "-s4_v7_d1") == 0)
            {
                chosen_policy = S4_V7_D1;
            }
            if (strcmp(argv[i], "-s4_v4_d3") == 0)
            {
                chosen_policy = S4_V4_D3;
            }
            if (strcmp(argv[i], "-s4_v9_d2") == 0)
            {
                chosen_policy = S4_V9_D2;
            }
            if (strcmp(argv[i], "-s4_v3_d1") == 0)
            {
                chosen_policy = S4_V3_D1;
            }
            if (strcmp(argv[i], "-s4_v9_d4") == 0)
            {
                chosen_policy = S4_V9_D4;
            }

            /// SERIAL REGRESSION
            if (strcmp(argv[i], "-ser_1_1") == 0)
            {
                chosen_policy = SER_1_1;
            }
            if (strcmp(argv[i], "-ser_1_2") == 0)
            {
                chosen_policy = SER_1_2;
            }
            if (strcmp(argv[i], "-ser_1_3") == 0)
            {
                chosen_policy = SER_1_3;
            }
            if (strcmp(argv[i], "-ser_2_1") == 0)
            {
                chosen_policy = SER_2_1;
            }
            if (strcmp(argv[i], "-ser_2_2") == 0)
            {
                chosen_policy = SER_2_2;
            }
            if (strcmp(argv[i], "-ser_2_3") == 0)
            {
                chosen_policy = SER_1_3;
            }
            if (strcmp(argv[i], "-ser_3_1") == 0)
            {
                chosen_policy = SER_3_1;
            }
            if (strcmp(argv[i], "-ser_3_2") == 0)
            {
                chosen_policy = SER_3_2;
            }
            if (strcmp(argv[i], "-ser_3_3") == 0)
            {
                chosen_policy = SER_3_3;
            }
            if (strcmp(argv[i], "-ser_4_1") == 0)
            {
                chosen_policy = SER_4_1;
            }
            if (strcmp(argv[i], "-ser_4_2") == 0)
            {
                chosen_policy = SER_4_2;
            }
            if (strcmp(argv[i], "-ser_4_3") == 0)
            {
                chosen_policy = SER_1_3;
            }
            if (strcmp(argv[i], "-ser_5_1") == 0)
            {
                chosen_policy = SER_5_1;
            }
            if (strcmp(argv[i], "-ser_5_2") == 0)
            {
                chosen_policy = SER_5_2;
            }
            if (strcmp(argv[i], "-ser_5_3") == 0)
            {
                chosen_policy = SER_5_3;
            }
            if (strcmp(argv[i], "-ser_6_1") == 0)
            {
                chosen_policy = SER_6_1;
            }
            if (strcmp(argv[i], "-ser_6_2") == 0)
            {
                chosen_policy = SER_6_2;
            }
            if (strcmp(argv[i], "-ser_6_3") == 0)
            {
                chosen_policy = SER_6_3;
            }
            if (strcmp(argv[i], "-ser_7_1") == 0)
            {
                chosen_policy = SER_7_1;
            }
            if (strcmp(argv[i], "-ser_7_2") == 0)
            {
                chosen_policy = SER_7_2;
            }
            if (strcmp(argv[i], "-ser_7_3") == 0)
            {
                chosen_policy = SER_7_3;
            }
            if (strcmp(argv[i], "-ser_8_1") == 0)
            {
                chosen_policy = SER_8_1;
            }
            if (strcmp(argv[i], "-ser_8_2") == 0)
            {
                chosen_policy = SER_8_2;
            }
            if (strcmp(argv[i], "-ser_8_3") == 0)
            {
                chosen_policy = SER_8_3;
            }
            if (strcmp(argv[i], "-ser_9_1") == 0)
            {
                chosen_policy = SER_9_1;
            }
            if (strcmp(argv[i], "-ser_9_2") == 0)
            {
                chosen_policy = SER_9_2;
            }
            if (strcmp(argv[i], "-ser_9_3") == 0)
            {
                chosen_policy = SER_9_3;
            }
            if (strcmp(argv[i], "-ser_10_1") == 0)
            {
                chosen_policy = SER_10_1;
            }
            if (strcmp(argv[i], "-ser_10_2") == 0)
            {
                chosen_policy = SER_10_2;
            }
            if (strcmp(argv[i], "-ser_10_3") == 0)
            {
                chosen_policy = SER_10_3;
            }
            if (strcmp(argv[i], "-q3p") == 0)
            {
                chosen_policy = Q3P;
            }
             if (strcmp(argv[i], "-q2p") == 0)
            {
                chosen_policy = Q2P;
            }



            if (strcmp(argv[i], "-nt") == 0)
            {
                number_of_tasks = atoi(argv[i + 1]);
                num_managers = number_of_tasks;
            }
        }
    }
    if (number_of_tasks == 0)
    {
        printf("Invalid number_of_tasks parameter. Please set -nt parameter in runtime.\n");
        exit(1);
    }
    res = test_all(argv[1], argv[2]);

    if (res == MSG_OK)
        return 0;
    else
        return 1;
} /* end_of_main */
