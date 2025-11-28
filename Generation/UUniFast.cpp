#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef struct task
{
    int id = 0;

    // 基本参数
    double utilisation = 0.0;
    int wcet = 0;
    long long int period = 0;
    long long int deadline = 0;
} Task;

long long int random_int(long long int min, long long int max);
double random_double();
void UUniFast(int n, double u, Task* tasks);
void PrintTasks(int n, Task* tasks);
void LogTasks(int n, Task* tasks, const char* filename);

int main() {
    int task_num = 10;
    double utilisation_pre = 0.8;    
    srand((unsigned int)time(NULL));    

    for(int i=100; i<=500; i+=100){
        for(double j=0.2; j<=0.8; j+=0.1){
            Task* tasks = (Task *)malloc(i * sizeof(Task));
            if (tasks == NULL) {
                printf("内存分配失败\n");
                return -1;
            }
            
            UUniFast(i, i*j, tasks);
    
            // PrintTasks(task_num, tasks);
            LogTasks(i, tasks, "UUniFast.log");
            free(tasks);
        }
    }
     
    return 0;
}

long long int random_int(long long int min, long long int max)
{
    return min + rand() % (max - min + 1);
}
double random_double()
{
    double val;
    do {
        val = (double)rand() / RAND_MAX;
    } while (val == 0.0 || val == 1.0);
    return val;
}

void PrintTasks(int n, Task* tasks)
{
    printf("Generated Tasks:\n");
    printf("%-5s %-7s %-7s %-7s\n", "ID", "Util", "Period", "WCET");

    double utilisation_sum = 0.0;

    for (int i = 0; i < n; i++)
    {
        printf("%-5d %-7.3f %-7lld %-10d\n",
            tasks[i].id, tasks[i].utilisation, tasks[i].period, tasks[i].wcet);
        utilisation_sum += tasks[i].utilisation;
    }

    printf("Task Utilisation Sum: %.3f\n", utilisation_sum);
    printf("\n");
}

void LogTasks(int n, Task* tasks, const char* fileid)
{
    FILE* file = fopen(fileid, "a");
    if (file == NULL) {
        printf("Cant Open: %s\n", fileid);
        return;
    }

    // 写入任务表头
    fprintf(file, "Generated Tasks:\n");
    fprintf(file, "%-5s %-12s %-12s %-12s %-12s\n",
        "ID", "Utilisation", "Period", "Deadline", "WCET");
    fprintf(file, "%-5s %-12s %-12s %-12s %-12s\n",
        "---", "-----------", "------", "--------", "----");

    double utilisation_sum = 0.0;
    // 写入每个任务的信息
    for (int i = 0; i < n; i++)
    {
        fprintf(file, "%-5d %-12.6f %-12lld %-12lld %-12d\n",
            tasks[i].id, tasks[i].utilisation,
            tasks[i].period, tasks[i].deadline, tasks[i].wcet);
        utilisation_sum += tasks[i].utilisation;
    }

    // 写入汇总信息
    fprintf(file, "Task Number: %d\n", n);
    fprintf(file, "Total U: %.6f\n", utilisation_sum);
    fprintf(file, "Avg U: %.6f\n", utilisation_sum / n);

    fclose(file);

    // printf("任务信息已保存到日志文件: %s\n", fileid);
}

void UUniFast(int n, double u, Task* tasks) {
    double u_total = u;
    double count = n;
    
    // 遍历前 n-1 个任务
    for (int i = 0; i < n-1; i++) {        
        double r = random_double();  // 生成 [0, 1] 之间的随机数
        double e = 1.0 / (n-1 - i);  // 剩余任务数的倒数
        double u_remaining = u_total * pow(r, e);

        tasks[i].utilisation = u_total - u_remaining;

        u_total = u_remaining;
    }
    
    // 最后一个任务获得剩余的所有利用率
    tasks[n-1].utilisation = u_total;
}
