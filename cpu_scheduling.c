#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct process
{
    int pid, arrival, burst, wait, turn, priority;
    int complete, flag, left, ctime, system;
    int temp, started, quantum;
} process;

int k = 0;
int *pid;
int *time;

typedef struct queue
{
    process *p;
    struct queue *next;
} queue;

process *highestPriority(queue *q)
{
    queue *ptr = q->next;
    process *min = ptr->p;
    process *i;
    while (ptr != NULL)
    {
        i = ptr->p;
        if (min->priority < i->priority)
        {
            min = i;
        }
        ptr = ptr->next;
    }
    return min;
}

void enqueue(queue *q, process *p)
{
    queue *ptr = q;
    while (ptr->next != NULL)
    {
        ptr = ptr->next;
    }
    queue *newPtr = (queue *)malloc(sizeof(queue));
    newPtr->next = NULL;
    newPtr->p = p;
    ptr->next = newPtr;
}

process *dequeue(queue *q)
{
    process *i = highestPriority(q);
    process *p = NULL;
    queue *ptr = q->next;
    queue *prev = NULL;
    while (ptr != NULL)
    {
        if (ptr->p == i)
        {
            p = i;
            if (prev != NULL)
            {
                prev->next = ptr->next;
            }
            else
            {
                q->next = ptr->next;
            }
            free(ptr);
            ptr = NULL;
            return p;
        }
        prev = ptr;
        ptr = ptr->next;
    }
    return p;
}

void sort_arrival_time(process *p, int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        int min = i;
        for (int j = i + 1; j < n; j++)
        {
            if (p[min].arrival > p[j].arrival)
                min = j;
        }
        if (i != min)
        {
            process temp = p[i];
            p[i] = p[min];
            p[min] = temp;
        }
    }
}

void fcfs(process *p, int n)
{
    p[0].wait = 0;
    p[0].turn = p[0].burst;
    int sum = p[0].arrival;
    for (int i = 1; i < n; i++)
    {
        sum += p[i - 1].burst;
        p[i].wait = sum - p[i].arrival;
        if (p[i].wait < 0)
            p[i].wait = 0;
        p[i].turn = p[i].burst + p[i].wait;
    }
}

void priority(process *p, int n)
{
    int previous = p[0].arrival;
    for (int i = 0; i < n; i++)
    {
        int min = p[i].priority;
        int min_index = i;
        int flag = 0;
        for (int j = i + 1; j < n; j++)
        {
            if (p[j].arrival > previous)
                break;
            if (p[j].priority < min)
            {
                min = p[j].priority;
                min_index = j;
                flag = 1;
            }
        }
        if (flag == 1)
        {
            process temp = p[i];
            p[i] = p[min_index];
            p[min_index] = temp;
        }
        p[i].wait = previous - p[i].arrival;
        if (p[i].wait < 0)
            p[i].wait = 0;
        p[i].turn = p[i].burst + p[i].wait;
        previous = p[i].arrival + p[i].turn;
    }
}

void sjfs(process *p, int n)
{
    int previous = p[0].arrival;
    for (int i = 0; i < n; i++)
    {
        int min = p[i].burst;
        int min_index = i;
        int flag = 0;
        for (int j = i + 1; j < n; j++)
        {
            if (p[j].arrival > previous)
                break;
            if (p[j].burst < min)
            {
                min = p[j].burst;
                min_index = j;
                flag = 1;
            }
        }
        if (flag == 1)
        {
            process temp = p[i];
            p[i] = p[min_index];
            p[min_index] = temp;
        }
        p[i].wait = previous - p[i].arrival;
        if (p[i].wait < 0)
            p[i].wait = 0;
        p[i].turn = p[i].burst + p[i].wait;
        previous = p[i].arrival + p[i].turn;
    }
}

void roundrobin2(process *p, int n)
{
    int total_time = 0;
    process *current;
    queue *q = (queue *)malloc(sizeof(queue));
    enqueue(q, &p[0]);
    p[0].started = 1;
    while (1)
    {
        current = dequeue(q);
        if (current->temp >= current->quantum)
        {
            current->temp -= current->quantum;
            total_time += current->quantum;
        }
        else
        {
            total_time += current->temp;
            current->temp = 0;
        }
        pid[k] = current->pid;
        time[k++] = total_time;

        for (int i = 0; i < n; i++)
        {
            if ((p[i].started == 0) && (p[i].arrival <= total_time))
            {
                enqueue(q, &p[i]);
                p[i].started = 1;
            }
        }
        if (current->temp > 0)
        {
            enqueue(q, current);
        }
        else
        {
            current->turn = total_time - current->arrival;
            current->wait = current->turn - current->burst;
        }

        if (q->next == NULL)
        {
            break;
        }
    }
}

void round_robin(process *p, int n, int quantum)
{
    int rem[n];
    for (int i = 0; i < n; i++)
        rem[i] = p[i].burst;
    int previous = p[0].arrival;
    while (1)
    {
        int flag = 0;
        int i;
        int min = 0;
        for (i = 0; i < n; i++)
        {
            if (rem[i] > 0)
            {
                if (p[i].temp <= p[min].temp)
                    min = i;
            }
        }
        if (rem[min] > 0)
        {
            flag = 1;
            if (rem[min] > quantum)
            {
                previous += quantum;
                rem[min] -= quantum;
            }
            else
            {
                previous += rem[min];
                p[min].wait = previous - p[min].burst - p[min].arrival;
                rem[min] = 0;
                p[min].turn = p[min].burst + p[min].wait;
            }
            p[min].temp = previous;
            pid[k] = p[min].pid;
            time[k++] = p[min].temp;
        }
        if (!flag)
            break;
    }
}

void gantt(process *p, int n, int size)
{
    printf("\n\n");
    for (int i = 0; i < (time[size - 1] * 2 + size - 11) / 2; i++)
        printf(" ");
    printf("GANTT CHART\n\n ");
    int i, j;
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < time[i] - time[i - 1]; j++)
            printf("--");
        printf(" ");
    }
    printf("\n|");
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < time[i] - time[i - 1] - 1; j++)
            printf(" ");
        printf("P%d", pid[i]);
        for (j = 0; j < time[i] - time[i - 1] - 1; j++)
            printf(" ");
        printf("|");
    }
    printf("\n ");
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < time[i] - time[i - 1]; j++)
            printf("--");
        printf(" ");
    }
    printf("\n%d", p[0].arrival);
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < time[i] - time[i - 1]; j++)
            printf("  ");
        if (time[i] > 9)
            printf("\b");
        printf("%d", time[i]);
    }
    printf("\n");
}

void gantt_chart(process *p, int n)
{
    printf("\n\n");
    for (int i = 0; i < p[n - 1].turn - 1; i++)
    {
        printf(" ");
    }
    printf("GANTT CHART\n\n ");

    int i, j;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < p[i].burst; j++)
            printf("--");
        printf(" ");
    }
    printf("\n|");
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < p[i].burst - 1; j++)
            printf(" ");
        printf("P%d", p[i].pid);
        for (j = 0; j < p[i].burst - 1; j++)
            printf(" ");
        printf("|");
    }
    printf("\n ");
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < p[i].burst; j++)
            printf("--");
        printf(" ");
    }
    printf("\n%d", p[0].arrival);
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < p[i].burst; j++)
            printf("  ");
        if (p[i].turn + p[i].arrival > 9)
            printf("\b");
        printf("%d", p[i].turn + p[i].arrival);
    }
    printf("\n");
}

void average(process *p, int n)
{
    float sumw = 0, sumt = 0;
    for (int i = 0; i < n; i++)
    {
        sumw += p[i].wait;
        sumt += p[i].turn;
    }
    printf("\n\nAverage Waiting Time = %0.3f", sumw / n);
    printf("\nAverage Turnaround Time = %0.3f\n", sumt / n);
}

int n;

void updateq(process p[], int t, int *front, int *rear, int ready[])
{
    for (int i = 0; i < n; i++)
    {
        if (p[i].arrival <= t && p[i].flag == 0)
        {
            p[i].flag++;
            ready[*rear] = i;
            *rear = *rear + 1;
        }
    }
}

void multilevel_main()
{

    int quan = 2, quans = 5, t = 0, front, rear, ready[20], cnum = 0, current, f, increment;
    front = rear = 0;
    process p[10];

    printf("Enter no. of processess:");
    scanf("%d", &n);

    //Reading data
    for (int i = 0; i < n; i++)
    {
        p[i].pid = i;
        p[i].flag = 0;
        printf("Process %d:\nArrival time:", i);
        scanf("%d", &p[i].arrival);
        printf("Burst time:");
        scanf("%d", &p[i].burst);
        printf("Enter 0 for user process and 1 for system process:");
        scanf("%d", &p[i].system);
        p[i].left = p[i].burst;
        p[i].wait = p[i].complete = 0;
    }

    while (cnum < n)
    {
        f = 0;
        updateq(p, t, &front, &rear, ready);
        if (front == rear)
        {
            t++;
            continue;
        }

        int flag = 0;
        for (int i = front; i < rear; i++)
        {
            if (p[ready[i]].system == 1)
            {
                flag = 1;
                current = ready[i];
                for (int j = i; j < rear - 1; j++)
                    ready[j] = ready[j + 1];
                rear--;
                break;
            }
        }
        if (flag == 0)
        {
            current = ready[front];
            front++;
        }

        if (flag == 0)
        {
            if (p[current].left <= quan)
            {
                p[current].ctime = t + p[current].left;
                increment = p[current].left;
                t += p[current].left;
                p[current].left = 0;
                p[current].complete = 1;
                cnum++;
                f++;
            }
            else
            {
                p[current].left -= quan;
                increment = quan;
                t += quan;
            }
            printf("P%d\t%d-%d\n", current, t - increment, t);
        }
        else
        {
            if (p[current].left <= quans)
            {
                p[current].ctime = t + p[current].left;
                increment = p[current].left;
                t += p[current].left;
                p[current].left = 0;
                p[current].complete = 1;
                cnum++;
                f++;
            }
            else
            {
                p[current].left -= quans;
                increment = quans;
                t += quans;
            }
            printf("P%d\t%d-%d\n", current, t - increment, t);
        }

        for (int i = 0; i < n; i++)
        {
            if (p[i].complete == 0 && i != current)
            {
                p[i].wait += increment;
            }
        }
        updateq(p, t, &front, &rear, ready);

        if (f == 0)
        {
            ready[rear++] = current;
        }
    }

    printf("\n\n                               MULTILEVEL SCHEDULING");
    printf("\n-----------------------------------------------------------------------------------\n");
    printf("Processes   Arrival time   Burst time      S/U   Waiting time    Turnaround time\n");
    printf("-----------------------------------------------------------------------------------\n");
    for (int i = 0; i < n; i++)
    {
        printf("    P%d\t\t%d\t\t%d\t    %d\t\t%d\t\t%d\n", p[i].pid, p[i].arrival,
               p[i].burst, p[i].system, p[i].wait, p[i].ctime - p[i].arrival);
    }
    printf("-----------------------------------------------------------------------------------\n");
    printf("\n");
    float avgw, turn;
    avgw = turn = 0;
    for (int i = 0; i < n; i++)
    {
        avgw += p[i].wait - p[i].arrival;
        turn = turn + (p[i].ctime - p[i].arrival);
    }
    avgw = avgw / n;
    turn = turn / n;
    printf("\nWaiting time:%f\nTurn:%f\n", avgw, turn);
}

int main()
{
    process *p;
    int n, type = 0, quantum, size = 0;
    while (type != 6)
    {
        printf("Select the algorithm to be used : \n");
        printf("1.FCFS \n2.SJF \n3.ROUND ROBIN \n4.PRIORITY \n5.MULTI LEVEL JOB SCHEDULING \n6.EXIT \n\n");
        printf("Enter your choice :");
        scanf("%d", &type);
        printf("\n");
        switch (type)
        {
        case 1:
        {
            printf("Enter the number of processes: ");
            scanf("%d", &n);
            process *p = (process *)malloc(sizeof(process) * n);
            printf("Enter the arrival time and burst time of Processes\n");
            for (int i = 0; i < n; i++)
            {
                printf("\nProcess [%d] : \nArrival time : ", i + 1);
                scanf("%d", &p[i].arrival);
                printf("Burst time : ");
                scanf("%d", &p[i].burst);
                p[i].pid = i + 1;
            }
            sort_arrival_time(p, n);
            fcfs(p, n);
            printf("\n\n              FIRST COME FIRST SERVED SCHEDULING");
            printf("\n------------------------------------------------------------------------\n");
            printf("Processes   Arrival time   Burst time    Waiting time    Turnaround time\n");
            printf("------------------------------------------------------------------------\n");
            for (int i = 0; i < n; i++)
            {
                printf("    P%d\t\t%d\t\t%d\t\t%d\t\t%d\n", p[i].pid, p[i].arrival, p[i].burst,
                       p[i].wait, p[i].turn);
            }
            printf("------------------------------------------------------------------------\n");
            gantt_chart(p, n);
            average(p, n);
            break;
        }
        case 2:
        {
            printf("Enter the number of processes: ");
            scanf("%d", &n);
            process *p = (process *)malloc(sizeof(process) * n);
            printf("Enter the arrival time and burst time of Processes\n");
            for (int i = 0; i < n; i++)
            {
                printf("\nProcess [%d] : \nArrival time : ", i + 1);
                scanf("%d", &p[i].arrival);
                printf("Burst time : ");
                scanf("%d", &p[i].burst);
                p[i].pid = i + 1;
            }
            sort_arrival_time(p, n);
            sjfs(p, n);
            printf("\n\n                   SHORTEST JOB FIRST SCHEDULING");
            printf("\n------------------------------------------------------------------------\n");
            printf("Processes   Arrival time   Burst time    Waiting time    Turnaround time\n");
            printf("------------------------------------------------------------------------\n");
            for (int i = 0; i < n; i++)
            {
                printf("    P%d\t\t%d\t\t%d\t\t%d\t\t%d\n", p[i].pid, p[i].arrival, p[i].burst,
                       p[i].wait, p[i].turn);
            }
            printf("------------------------------------------------------------------------\n");
            gantt_chart(p, n);
            average(p, n);
            break;
        }
        case 3:
        {
            printf("Enter the number of processes: ");
            scanf("%d", &n);
            p = (process *)malloc(sizeof(process) * n);
            printf("Enter the time quantum : ");
            scanf("%d", &quantum);
            printf("Enter the arrival time and burst time of Processes\n");
            for (int i = 0; i < n; i++)
            {
                printf("\nProcess [%d] : \nArrival time : ", i + 1);
                scanf("%d", &p[i].arrival);
                printf("Burst time : ");
                scanf("%d", &p[i].burst);
                p[i].pid = i + 1;
                p[i].temp = p[i].arrival;
                float f = p[i].burst / (float)quantum;
                size += ceil(f);
            }
            pid = (int *)malloc(sizeof(int) * size);
            time = (int *)malloc(sizeof(int) * size);
            sort_arrival_time(p, n);
            round_robin(p, n, quantum);
            printf("\n\n                       ROUND ROBIN SCHEDULING");
            printf("\n------------------------------------------------------------------------\n");
            printf("Processes   Arrival time   Burst time    Waiting time    Turnaround time\n");
            printf("------------------------------------------------------------------------\n");
            for (int i = 0; i < n; i++)
            {
                printf("    P%d\t\t%d\t\t%d\t\t%d\t\t%d\n", p[i].pid, p[i].arrival, p[i].burst,
                       p[i].wait, p[i].turn);
            }
            printf("------------------------------------------------------------------------\n");
            gantt(p, n, size);
            average(p, n);
            break;
        }
        case 4:
        {
            printf("4");
            printf("Enter the number of processes: ");
            scanf("%d", &n);
            process *p = (process *)malloc(sizeof(process) * n);
            printf("Enter the arrival time, burst time and priority of Processes\n");
            for (int i = 0; i < n; i++)
            {
                printf("\nProcess [%d] : \nArrival time : ", i + 1);
                scanf("%d", &p[i].arrival);
                printf("Burst time : ");
                scanf("%d", &p[i].burst);
                printf("Priority : ");
                scanf("%d", &p[i].priority);
                p[i].pid = i + 1;
            }
            sort_arrival_time(p, n);
            priority(p, n);
            printf("\n\n                               PRIORITY SCHEDULING");
            printf("\n-----------------------------------------------------------------------------------\n");
            printf("Processes   Arrival time   Burst time    Priority   Waiting time    Turnaround time\n");
            printf("-----------------------------------------------------------------------------------\n");
            for (int i = 0; i < n; i++)
            {
                printf("    P%d\t\t%d\t\t%d\t    %d\t\t%d\t\t%d\n", p[i].pid, p[i].arrival,
                       p[i].burst, p[i].priority, p[i].wait, p[i].turn);
            }
            printf("-----------------------------------------------------------------------------------\n");
            gantt_chart(p, n);
            average(p, n);
            break;
        }
        case 5:
        {

            multilevel_main();
            break;
        }
        case 6:
        {
        }
        }
    }

    return 0;
}