#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int *main_arr;
int threads_number = 0;
int arr_size = 0;
int sizeofint = sizeof(int);

void is_sorted()
{
    for (int i = 1; i < arr_size; i++)
    {
        if (main_arr[i] < main_arr[i - 1])
            printf("ERROR!!! %d > %d\n", main_arr[i], main_arr[i - 1]);
    }
}

int read_by_byte(FILE *input)
{
    int array_size = 10;
    char *buf = (char *)malloc(array_size);
    if (buf == NULL)
    {
        printf("malloc error\n");
        return -1;
    }
    char temp;
    int i = 0;
    while (1)
    {
        temp = fgetc(input);
        if (temp == '\n' || temp == '\0')
        {
            buf[i] = 0;
            break;
        }
        buf[i] = temp;
        i++;
        if (i == array_size)
        {
            array_size = array_size * 2;
            char *temp_ptr = NULL;
            temp_ptr = (char *)realloc(buf, array_size);
            if (temp_ptr == NULL)
            {
                printf("realloc error\n");
                return -1;
            }
            buf = temp_ptr;
        }
        i--;
        int j = i;
        int for_return = 0;
        for_return = atoi(buf);
        free(buf);
        return for_return;
    }
}

void fill_arr(FILE *input)
{
    char buf[12];
    char temp = 0;
    int j = 0;
    while (j != arr_size)
    {
        int i = 0;
        while (1)
        {
            temp = fgetc(input);
            if ((temp < 48 || temp > 58) && temp!=45)
            {
                break;
            }
            buf[i] = temp;
            i++;
        }
        main_arr[j] = atoi(buf);
        j++;
    }
}

int read_input()
{
    FILE *input = fopen("input.txt", "r");
    threads_number = read_by_byte(input);
    if (threads_number < 0)
    {
        printf("invalid number of threads\n");
        return -1;
    }
    arr_size = read_by_byte(input);
    if (arr_size < 0)
    {
        printf("invalid size of array\n");
        return -1;
    }
    main_arr = (int *)malloc(4 * arr_size);
    fill_arr(input);
    fclose(input);
    return 0;
}
struct task{
    int offset;
    int size;
    task * next;

};
task * begin = NULL, * end;
int task_size = sizeof(task);

int tasks = 0;
int done_sorts = 0;
int last_size;
pthread_mutex_t to_sort_mutex;

void make_tasks_to_sort(void)
{
    int cntr = arr_size;
    int last_size = arr_size%1000;

    while(cntr > 0)
    {
        cntr = cntr - 1000;
        tasks++;
    }    
}

void to_sort(void)
{
    int size = 0;
    pthread_mutex_lock(&to_sort_mutex);
    int coef = done_sorts;
    if(done_sorts == tasks)
    {
        pthread_mutex_unlock(&to_sort_mutex);
        return;    
    }
    done_sorts++;
    if(done_sorts == tasks)
    {
        size = last_size;
    }
    pthread_mutex_unlock(&to_sort_mutex);
    if(size == 0) size = 1000;

    int * arr = (int*) malloc(sizeof(int) * size);

    free(arr);
}



int main()
{

    if (read_input())
    {
        printf("input error\n");
        return 0;
    }

    for (int i = 0; i < threads_number; i++)
    {
    }
    printf("reading is done\n");
    
    make_tasks_to_sort();


    for (int i = 0; i < threads_number; i++)
    {
    }

    
    FILE *out = fopen("output.txt", "w");
    FILE *t = fopen("time.txt", "w");
    fprintf(out, "%d\n%d\n", threads_number, arr_size);
    printf("counting is done\n");
    for (int i = 0; i < arr_size; i++)
    {
        fprintf(out, "%d ", main_arr[i]);
    }
    fclose(t);
    fclose(out);
    is_sorted();
    return 0;
}