#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <string.h>

// 按照固定area进行大核小核数目分配
// 不抢占

const int PeriodNumber = 3;
long long int system_time = 0;
bool debug = 0;

const int BigArea = 18;
const int LittleArea = 1;
const int Policy_number = 3;
const int TaskType_number = 8;
const int LittleCore_number_MAX = 20;

int LittleCore_Opt[Policy_number][TaskType_number] = {
    { 2, 3, 4, 4, 3, 1, 4, 1 },  //PMC
    { 2, 2, 5, 5, 2, 4, 2, 5 },  //AS
    { 3, 1, 3, 2, 3, 2, 2, 3 },  //SS
};
const int WCET[TaskType_number] = { 9, 25, 59, 47, 32, 820, 64, 33 };
double WCET_Ratio[Policy_number][TaskType_number][LittleCore_number_MAX + 1] = {
    {
        { 1,    1.083, 1.038, 1.013, 1, 1,            1, 1, 1, 1, 1,                       1, 1, 1, 1, 1,    1, 1, 1, 1, 1 },
        { 1,    1.111, 1.08, 1.012, 1, 1,             1, 1, 1, 1, 1,                       1, 1, 1, 1, 1,    1, 1, 1, 1, 1 },
        { 1,    1.257, 1.127, 1.063, 1.031, 1.016,    1, 1, 1, 1, 1,                       1, 1, 1, 1, 1,    1, 1, 1, 1, 1 },
        { 1,    1.103, 1.066, 1.042, 1.027, 1.017,    1.011, 1, 1, 1, 1,                   1, 1, 1, 1, 1,    1, 1, 1, 1, 1 },

        { 1,    1.056, 1.037, 1.024, 1.016, 1.01,     1, 1, 1, 1, 1,                       1, 1, 1, 1, 1,    1, 1, 1, 1, 1 },
        { 1,    1, 1, 1, 1, 1,                        1, 1, 1, 1, 1,                       1, 1, 1, 1, 1,    1, 1, 1, 1, 1 },
        { 1,    1.033, 1.029, 1.026, 1.022, 1.02,     1.017, 1.015, 1.013, 1.012, 1.01,    1, 1, 1, 1, 1,    1, 1, 1, 1, 1 },
        { 1,    1, 1, 1, 1, 1,                        1, 1, 1, 1, 1,                       1, 1, 1, 1, 1,    1, 1, 1, 1, 1 }
    },//PMC
    {
        { 1,    1.083, 1, 1, 1, 1,                    1, 1, 1, 1, 1,                         1, 1, 1, 1, 1,                      1, 1, 1, 1, 1 },
        { 1,    2.308, 1.886, 1.6, 1.406, 1.275,      1.186, 1.256, 1.078, 1.058, 1.039,     1.026, 1.018, 1.012, 1, 1,          1, 1, 1, 1, 1 },
        { 1,    3.248, 2.662, 2.229, 1.909, 1.672,    1.497, 1.367, 1.272, 1.201, 1.149,     1.11, 1.081, 1.06, 1.044, 1.033,    1.024, 1.018, 1.013, 1.01, 1 },
        { 1,    1.799, 1.466, 1.272, 1.159, 1.093,    1.054, 1.032, 1.018, 1.011, 1,         1, 1, 1, 1, 1,                      1, 1, 1, 1, 1 },

        { 1,    1.027, 1, 1, 1, 1,                    1, 1, 1, 1, 1,                         1, 1, 1, 1, 1,                      1, 1, 1, 1, 1 },
        { 1,    1.147, 1.109, 1.081, 1.06, 1.045,     1.033, 1.025, 1.018, 1.014, 1.01,      1, 1, 1, 1, 1,                      1, 1, 1, 1, 1 },
        { 1,    1.019, 1, 1, 1, 1,                    1, 1, 1, 1, 1,                         1, 1, 1, 1, 1,                      1, 1, 1, 1, 1 },
        { 1,    1.367, 1.264, 1.19, 1.137, 1.098,     1.07, 1.05, 1.037, 1.026, 1.019,       1.014, 1.01, 1, 1, 1,               1, 1, 1, 1, 1 }
    },//AS
    {
        { 1,    1.299, 1.058, 1, 1, 1,                1, 1, 1, 1, 1,           1, 1, 1, 1, 1,    1, 1, 1, 1, 1 },
        { 1,    1, 1, 1, 1, 1,                        1, 1, 1, 1, 1,           1, 1, 1, 1, 1,    1, 1, 1, 1, 1 },
        { 1,    1.105, 1.059, 1.033, 1.019, 1.011,    1, 1, 1, 1, 1,           1, 1, 1, 1, 1,    1, 1, 1, 1, 1 },
        { 1,    1.012, 1, 1, 1, 1,                    1, 1, 1, 1, 1,           1, 1, 1, 1, 1,    1, 1, 1, 1, 1 },

        { 1,    1.2, 1.127, 1.08, 1.051, 1.032,       1.02, 1.013, 1, 1, 1,    1, 1, 1, 1, 1,    1, 1, 1, 1, 1 },
        { 1,    1.02, 1.016, 1.013, 1.01, 1,          1, 1, 1, 1, 1,           1, 1, 1, 1, 1,    1, 1, 1, 1, 1 },
        { 1,    1.232, 1, 1, 1, 1,                    1, 1, 1, 1, 1,           1, 1, 1, 1, 1,    1, 1, 1, 1, 1 },
        { 1,    1.486, 1.17, 1.059, 1.021, 1,         1, 1, 1, 1, 1,           1, 1, 1, 1, 1,    1, 1, 1, 1, 1 }
    } //SS
};

typedef enum
{
    no,
    generated,
    ready,
    waiting,
    working,
    completed,
} State;

typedef struct task
{
    int id = 0;

    // 基本参数
    double utilisation = 0;
    int wcet;
    long long int period = 0;
    long long int deadline = 0;

    // 运行参数
    double workload;
    State state = no;
    struct task* next = NULL;

    // 专用参数
    int policy;
    int task_type;
    int littleCore_opt;
} Task;

typedef struct
{
    Task* current_task;
} BigCore;


typedef struct
{
    // 系统配置
    long long int maxtime;

    // 硬件配置
    int area;
    int total_BigCore_number;  //依据可利用面积或功耗决定
    int total_LittleCore_number;
	int idle_LittleCore_number;
    BigCore* cores;

    // 软件配置
    int total_task_number;  //任务根据不同的安全情况，具有不同的资源需求，并在实际情况下产生不同的WCET
    int task_number[Policy_number][TaskType_number];
    Task* tasks;

    // 任务生成配置
    double rate;
    double total_rate;    

    // 运行配置
    bool error;
    int error_type;
	int error_policy;
    long long int error_time;
    Task* error_task;
} System;


long long int random_int(long long int min, long long int max);
double random_double();

void TaskNum(int* task_num, int num);  //随机分配任务数

void EnqueueTask(Task* queue, Task* task, System* system);  // 入队出队
Task* DequeueTask(Task* queue, Task* task, System* system);

// Debug
void LogTasks(int n, Task* tasks, const char* fileid);

// CSV输出
bool FileHeader(const char* fileid, const char* header);
void Write2CSV(System* system, const char* fileid);


void Generate(System* system); // 任务生成
void Execute(System* system); // 任务执行
/* 主函数 */
void Set(System* system, int area, int total_BigCore_number, int total_LittleCore_number, int total_task_number, double core_rate); // 设置系统参数
void Initialization(System* system); // 系统初始化
int SystemMaxTime(System* system);
void Run(System* system);


int main()
{
    srand(time(NULL));
    System system;

	int Area[7] = { 40, 80, 150, 300, 600, 1200, 2400 };
	int TaskRatio[5] = { 2, 5, 10, 15, 20 };

    for (int i = 0; i < 7; i++)
    {
        int area = Area[i];
        int total_BigCore_number = pow(2, (i + 1));
        int total_LittleCore_number = (area - total_BigCore_number * BigArea) / LittleArea;

        for (int k = 0; k < 5; k++)
        {
            int task_number = TaskRatio[k] * total_BigCore_number;

            for (double l = 0.4; l <= 0.8; l += 0.1)
            {
                Set(&system, area, total_BigCore_number, total_LittleCore_number, task_number, l);
                Run(&system);
                fflush(stdout);
            }
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

void TaskNum(int* task_num, int num)
{
    int count = num;

    for (int i = 0; i < Policy_number; i++) {
        for (int j = 0; j < TaskType_number; j++) {
            task_num[i * TaskType_number + j] = 0;
        }
    }

    while (count) {
        task_num[random_int(0, Policy_number - 1) * TaskType_number + random_int(0, TaskType_number - 1)]++;
        count--;
    }
}

void EnqueueTask(Task* queue, Task* task, System* system)
{
    task->deadline = system_time + task->period;
    task->workload = task->wcet;
    task->state = ready;

    if (!queue->next) {  // 队列为空
        queue->next = task;
    }
    else {  // 队列不为空,抢占后续
        Task* current = queue;
        while (current->next) {
            if (task->deadline < current->next->deadline) {
                task->next = current->next;
                break;
            }
            else {
                current = current->next;
            }
        }
        current->next = task;
    }
}
Task* DequeueTask(Task* queue, Task* task, System* system)
{
    Task* current = queue;
	bool found = false;
    
    while (current->next) {
        if (current->next == task) {
            current->next = task->next;
            task->next = NULL;
			found = true;
            break;
        }
        else {
            current = current->next;
        }
    }

    if (found) {
        task->state = working;
        system->idle_LittleCore_number -= task->littleCore_opt;
        return task;
    }
    else {
        printf("Dequeue Failure.\n");
        exit(1);
    }
    
}

void LogTasks(int n, Task* tasks, const char* fileid)
{
    FILE* file;
    fopen_s(&file, fileid, "a");
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

    // 同时在控制台输出提示信息
    printf("任务信息已保存到日志文件: %s\n", fileid);
}

bool FileHeader(const char* fileid, const char* header)
{
    FILE* file;
    fopen_s(&file, fileid, "r");
    if (file == NULL)
        return false;

    char line[256];
    bool header_exists = false;
    if (fgets(line, sizeof(line), file))
    {
        if (strncmp(line, header, strlen(header)) == 0)
            header_exists = true;
    }
    fclose(file);
    return header_exists;
}

void Write2CSV(System* system, const char* fileid)
{
    const char* header = "Area, BigCore Number, LittleCore Number, Task Number, Per Rate, Schedulability, ErrorType\n";
    bool header_exists = FileHeader(fileid, header);

    FILE* file;
    fopen_s(&file, fileid, "a");
    if (file == NULL)
    {
        printf("Error: Could not open file %s for writing.\n", fileid);
        return;
    }

    // 仅在文件中没有表头时写入表头
    if (!header_exists)
    {
        fprintf(file, "%s", header);
    }

    // 写入系统信息
    fprintf(file, "%d,%d,%d,%d,%.2f,%d,%d\n",
        system->area,
        system->total_BigCore_number,
        system->total_LittleCore_number,
        system->total_task_number,
        system->rate,
        system->error ? 0 : 1,
        system->error_type);
    fclose(file);
    printf("System summary appended to %s\n", fileid);
}

void Generate(System* system)
{
    double total_utilisation = system->total_rate;
    int total_task_number = system->total_task_number;
    Task* tasks = system->tasks;

    // UUniFast
    for (int i = 0; i < total_task_number - 1; i++) {
        double utilisation_remaining = total_utilisation * pow(random_double(), 1.0 / (total_task_number - 1 - i));
        tasks[i].utilisation = total_utilisation - utilisation_remaining;
        total_utilisation = utilisation_remaining;
    }
    tasks[total_task_number - 1].utilisation = total_utilisation;

    // 参数初始化
    int count = 0;
    for (int i = 0; i < Policy_number; i++)
    {
        for (int j = 0; j < TaskType_number; j++)
        {
            int k = system->task_number[i][j];
            while (k) {
                tasks[count].id = count + 1;

                // 基本参数
                tasks[count].wcet = WCET[j];
                tasks[count].period = (long long int)WCET[j] / tasks[count].utilisation;
                tasks[count].deadline = tasks[count].period;

                // 运行参数
                tasks[count].workload = 0.0;
                tasks[count].state = generated;
                tasks[count].next = NULL;

                // 专用参数
                tasks[count].policy = i;
                tasks[count].task_type = j;
                tasks[count].littleCore_opt = LittleCore_Opt[i][j];

                k--;
                count++;
            }
        }
    }

    //LogTasks(system->total_task_number,system->tasks,"Generated Tasks.log");
}
void Execute(System* system)
{
    Task queue;
    (&queue)->next = NULL;

    for (; system_time < system->maxtime; system_time++)
    {
        // 错误检查和任务释放
        if (system->error) { return; }
        for (int i = 0; i < system->total_task_number; i++) {
            Task* task = &system->tasks[i];

            // 错误检查：任务超过截止期且未完成
            if (task->workload > 0 && system_time >= task->deadline && task->state != completed)
            {
                system->error = true;
                system->error_type = 1; // 未完成错误
                system->error_time = system_time;
                system->error_task = task;
                return;
            }

            // 任务释放：根据周期释放任务
            if (system_time % task->period == 0)
            {
                if (task->state == generated || task->state == completed)
                {
                    EnqueueTask(&queue, task, system);
                }
            }
        }

        // 任务分配及调度
        Task* task_pointer = (&queue)->next;        
        while (task_pointer) {
			bool update = false;

            // 如果空闲小核不足，检测下一个任务
            if(task_pointer->littleCore_opt <= system->idle_LittleCore_number)
            {
                for (int c = 0; c < system->total_BigCore_number; c++) {
                    BigCore* core = &system->cores[c];
                    Task* ctask = core->current_task;

                    if (!ctask) {  // 没有正在执行的任务，选择该大核					
                        core->current_task = DequeueTask(&queue, task_pointer, system);
                        update = true;
                        break;
                    }
                }
			}
            

            if (update) {
                task_pointer = (&queue)->next;
            }
            else {
				task_pointer = task_pointer->next;
            }            
        }

        // 任务执行
        for (int c = 0; c < system->total_BigCore_number; c++) {
            BigCore* core = &system->cores[c];
            Task* ctask = core->current_task;

            if (ctask) {
                double work = 1 / WCET_Ratio[ctask->policy][ctask->task_type][ctask->littleCore_opt];
                ctask->workload -= work;

                if (ctask->workload <= 0) //任务完成
                {
                    ctask->workload = 0;
                    ctask->state = completed;
                    system->idle_LittleCore_number += ctask->littleCore_opt;
                    core->current_task = NULL;
                }
            }
        }
    }
}

void Set(System* system, int area, int total_BigCore_number, int total_LittleCore_number, int total_task_number, double core_rate)
{
    // 硬件配置
    system->area = area;
    system->total_BigCore_number = total_BigCore_number;
    system->total_LittleCore_number = total_LittleCore_number;
	system->idle_LittleCore_number = total_LittleCore_number;

    // 软件配置
    int task_number[Policy_number * TaskType_number];
    TaskNum(task_number, total_task_number);
    system->total_task_number = 0;
    for (int i = 0; i < Policy_number; i++) {
        for (int j = 0; j < TaskType_number; j++) {
            system->task_number[i][j] = task_number[i * TaskType_number + j];
            system->total_task_number += task_number[i * TaskType_number + j];
        }
    }

    // 任务生成配置
    system->rate = core_rate;
    system->total_rate = core_rate * system->total_BigCore_number;    

    // 任务运行配置
    system->error = false;
    system->error_type = 0;
    system->error_policy = 0;
    system->error_time = 0;
    system->error_task = NULL;
}
void Initialization(System* system)
{
    // 初始化大核
    system->cores = (BigCore*)malloc(system->total_BigCore_number * sizeof(BigCore));
    if (system->cores == NULL)
    {
        printf("Failed to allocate memory for cores.\n");
        exit(1);
    }
    for (int i = 0; i < system->total_BigCore_number; i++)
    {
        system->cores[i].current_task = NULL;
    }

    // 初始化任务
    system->tasks = (Task*)malloc(system->total_task_number * sizeof(Task));
    if (system->tasks == NULL)
    {
        printf("Failed to allocate memory for tasks.\n");
        exit(1);
    }

    Generate(system);  //任务生成
    SystemMaxTime(system);  //系统时间设置
}
int SystemMaxTime(System* system)
{
    long long int period_max = 0;

    for (int i = 0; i < system->total_task_number; i++)
    {
        if (system->tasks[i].period > period_max)
        {
            period_max = system->tasks[i].period;
        }
    }

    system->maxtime = (long long int)(period_max * PeriodNumber);
    return 0;
}
void Run(System* system)
{
    Initialization(system);
    Execute(system);

    Write2CSV(system, "Gang.csv");

    free(system->tasks);
    free(system->cores);
}
