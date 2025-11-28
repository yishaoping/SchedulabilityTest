#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <string.h>

const int PeriodNumber = 3;
long long int system_time = 0;
bool debug = 0;

const int BigArea = 18;
const int LittleArea = 1;
const int Policy_number = 3;
const int TaskType_number = 8;
const int LittleCore_number_MAX = 20;

int LittleCore_Max[Policy_number][TaskType_number] = {
    { 4, 4, 6, 7, 6, 1, 11, 1 },  //PMC
    { 2, 14, 20, 10, 2, 11, 2, 13 },  //AS
    { 3, 1, 6, 2, 8, 5, 2, 5 },  //SS
};
const int WCET[TaskType_number] = { 9, 25, 59, 47, 32, 820, 64, 33 };
double WCET_Ratio[Policy_number][TaskType_number][LittleCore_number_MAX+1] = {
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
    int wcet = 0;
    long long int period = 0;
    long long int deadline = 0;

    // 运行参数
    double workload = 0;
    State state = no;
    struct task* next = NULL;

    // 专用参数
    int policy = -1;
    int task_type = -1;
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
    int total_BigCore_number;  //依据可利用面积或功耗决定，该方案会具有更多的大核
    int little_for_big;
    BigCore* cores;

    // 软件配置
    int total_task_number;  //任务根据不同的安全情况，具有不同的资源需求，并在实际情况下产生不同的WCET
    int task_number[Policy_number][TaskType_number];
    Task* tasks;

    // 生成配置
    double rate;
    double total_rate;

    // 运行配置
    bool error;
    int error_type;
    long long int error_time;
    Task* error_task;
} System;


long long int random_int(long long int min, long long int max);
double random_double();

void TaskNum(int* task_num, int num);

void EnqueueTask(Task* queue, Task* task);
Task* DequeueTask(Task* queue, Task* task);

// CSV输出
bool FileHeader(const char* fileid, const char* header);
void Write2CSV(System* system, const char* fileid);

void Generate(System* system);
void Execute(System* system);
/* 主函数 */
void Set(System* system, int area, int total_BigCore_number, int little_for_big, int total_task_number, double core_rate);
void Initialization(System* system);
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

        for (int j = 2; j <= 6; j++)
        {
            int total_BigCore_number = area / (BigArea + j * LittleArea);

            if (!total_BigCore_number) { break; }

            for (int k = 0; k < 5; k++)
            {
                int task_number = TaskRatio[k] * pow(2, (i + 1));

                for (double l = 0.4; l <= 0.8; l+=0.1)
                {
                    Set(&system, area, total_BigCore_number, j, task_number, l);
                    Run(&system);
                    fflush(stdout);
                }
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

/* 任务运行所需函数 */
// 入队出队
void EnqueueTask(Task* queue, Task* task)
{
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
Task* DequeueTask(Task* queue, Task* task) {
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
        return task;
    }
    else {
        printf("Dequeue Failure.\n");
        exit(1);
    }
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

    // 参数生成
    int count = 0;
    for (int i = 0; i < Policy_number; i++)
    {
        for (int j = 0; j < TaskType_number; j++)
        {
            int k = system->task_number[i][j];
            while (k) {
                tasks[count].id = count + 1;

                // 基本参数
                tasks[count].wcet = WCET[j] * WCET_Ratio[i][j][system->little_for_big];
                tasks[count].period = (long long int)WCET[j] / tasks[count].utilisation;
                tasks[count].deadline = tasks[count].period;

                // 运行参数
                tasks[count].workload = 0.0;
                tasks[count].state = generated;
                tasks[count].next = NULL;

                // 专用参数
                tasks[count].policy = i;
                tasks[count].task_type = j;

                k--;
                count++;
            }
        }
    }
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
                system->error_type = 1;
                system->error_time = system_time;
                system->error_task = task;
                return;
            }

            if (system_time % task->period == 0)
            {
                if (task->state == generated || task->state == completed)
                {
                    task->deadline = system_time + task->period;
                    task->workload = task->wcet;
                    task->state = ready;
                    EnqueueTask(&queue, task);
                }
            }
        }

        // 任务分配及调度
        Task* task_pointer = (&queue)->next;
        while (task_pointer) {
            BigCore* candidate_core = NULL;
            BigCore* prep_core = NULL;
            int record = -1;            
            bool idle = 0;
            bool preempt = 0;
            bool update = 0;

            for (int c = 0; c < system->total_BigCore_number; c++) {
                BigCore* core = &system->cores[c];
                Task* ctask = core->current_task;

                if (!ctask) {  // 没有正在执行的任务
                    candidate_core = core;
                    record = c;
                    idle = 1;
                    break;
                }
                else {
                    if (task_pointer->deadline < ctask->deadline) {
                        if (!prep_core) {  //还未选择核心，选择之
                            prep_core = core;
                            record = c;
                        }
                        else {  //已选择核心，选择ddl最大者更换之                            
                            if (prep_core->current_task->deadline < ctask->deadline) {
                                prep_core = core;
                                record = c;
                            }
                        }
                        preempt = 1;
                    }
                }
            }
            if (!idle && preempt) { candidate_core = prep_core; }

            // 上核
            if (candidate_core) {
                update = 1;
                Task* mid_task_q = DequeueTask(&queue, task_pointer);
                Task* mid_task_c = candidate_core->current_task;

                mid_task_q->state = working;
                candidate_core->current_task = mid_task_q;

                if (mid_task_c) {  //若核上有任务，则进行交换
                    mid_task_c->state = waiting;
                    EnqueueTask(&queue, mid_task_c);
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
                ctask->workload--;

                if (ctask->workload <= 0)  //任务完成
                {
                    ctask->workload = 0;
                    ctask->state = completed;
                    core->current_task = NULL;
                }
            }
        }
    }
}

void Set(System* system, int area, int total_BigCore_number, int little_for_big, int total_task_number, double core_rate)
{
    system_time = 0;
    // 硬件配置
    system->area = area;
    system->total_BigCore_number = total_BigCore_number;
    system->little_for_big = little_for_big;

    // 任务配置
    int task_number[Policy_number * TaskType_number];
    TaskNum(task_number, total_task_number);
    system->total_task_number = 0;
    for (int i = 0; i < Policy_number; i++) {
        for (int j = 0; j < TaskType_number; j++) {
            system->task_number[i][j] = task_number[i * TaskType_number + j];
            system->total_task_number += task_number[i * TaskType_number + j];
        }
    }

    // 生成配置
    system->rate = core_rate;
    system->total_rate = core_rate * system->total_BigCore_number;

    // 运行配置
    system->error = false;
    system->error_type = 0;
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

    Write2CSV(system, "Fixed.csv");

    free(system->tasks);
    free(system->cores);
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
        system->little_for_big,
        system->total_task_number,
        system->rate,
        system->error ? 0 : 1,
        system->error_type);
    fclose(file);
    printf("System summary appended to %s\n", fileid);
}
