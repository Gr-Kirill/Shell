#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <wait.h>

typedef struct start_struct
{
    char word[50][255];
    char command[50][3];
    //int priority[50];
}s_s;

int i_main; //индекс word
pid_t pid_son;
s_s structura; //структура

void build(char *);
void execution();
int f_and_or(int,int);
int f_file(int,int);
int f_pipe(int,int);
int exec(int,int,int);

int main()
{
    char start_str[255];

    while(1)
    {
        //int queue = 0; //приоритет выполнения
        //int max_queue = 0;

        for(int j = 0; j < 50; j++)
        {
            memset(structura.word[j], 0, sizeof(char)*255);
            memset(structura.command[j], 0, sizeof(char)*3);
            //memset(structura.priority, 0, sizeof(int)*50);
        }

        if(!fgets(start_str, sizeof(start_str), stdin)) exit(0);
        int len_start = strlen(start_str);
        if (start_str[len_start - 1] == '\n') start_str[len_start - 1] = 0;
        if (!(strcmp(start_str, "exit"))) exit(0);

        build(start_str);

        execution();
    }
}

void build(char *start_str)
{
   	i_main = 0;
  	int i_cmd = 0; //индекс command
    int i = 0;    //индекс всей строки
	while (start_str[i] != 0)
    {            
        if ((start_str[i] == '&') && (start_str[i+1] == '&'))
        {
            structura.command[i_main][0] = start_str[i++]; //присвоется &
            structura.command[i_main][1] = start_str[i++]; //присвоется &
            i_main++;
            i_cmd = 0;
        }
        else if ((start_str[i] == '|') && (start_str[i+1] == '|'))
        {
            structura.command[i_main][0] = start_str[i++]; //присвоется |
            structura.command[i_main][1] = start_str[i++]; //присвоется |
            i_main++;
            i_cmd = 0;
        }
        else if ((start_str[i] == '>') && (start_str[i+1] == '>'))
        {
            structura.command[i_main][0] = start_str[i++]; //присвоется >
            structura.command[i_main][1] = start_str[i++]; //присвоется >
            i_main++;
            i_cmd = 0;
        }
        else
        {
            switch (start_str[i])
            {
            case '(':
                //queue++;
                //structura.priority[i_main] = queue;
                //if (queue > max_queue) max_queue = queue;
                i++;
                break;
            case ')':
                //structura.priority[i_main] = queue;
                //queue--;
                i++;
                break;
            case '|':
                structura.command[i_main][0] = start_str[i];
                i_main++;
                i_cmd = 0;
                i++;
                break;
            case '>':
                structura.command[i_main][0] = start_str[i];
                i_main++;
                i_cmd = 0;
                i++;
                break;  
            case '<':
                structura.command[i_main][0] = start_str[i];
                i_main++;
                i_cmd = 0;
                i++;
                break;
            case ';':
                structura.command[i_main][0] = start_str[i];
                i_main++;
                i_cmd = 0;
                i++;
                break;    
            case '&':
                structura.command[i_main][0] = start_str[i];
                i_main++;
                i_cmd = 0;
                i++;
                break;
            default:
                /*
                cmd1 > file
                "cmd1 ", ">"
                " file", ""
                */
                  
                structura.word[i_main][i_cmd++] = start_str[i++]; 
                break;
            }
        }
    }
  	if (structura.command[i_main][0] == '\0')
    {
      	structura.command[i_main][0] = ';';
    }
}

int f_and_or(int start, int end)
{
    for (int i = start; i < end; i++)
    {
        if(!strcmp(structura.command[i], "&&"))
        {
            if(f_file(start, i)) //команды слева выполнились
            {
                for(int j = i+1; j < end; j++)
                {
                    printf("^%s # %s\n", structura.word[j], structura.command[j]);
                }
                f_and_or(i+1, end); //выполняем команды справа
            }
        }
        else if (!strcmp(structura.command[i], "||")) // команды слева не выполнились
        {
            if (!(f_file(start, i)))
            {
                f_and_or(i+1, end);
            }
        }
    }
    return f_file(start, end);//Если не встретили && и ||
}


int f_file(int start, int end)
{
    int f;
    //разве тут нам не надо прев? или не может дважды встретиться запись в файл? 
    for(int i = start; i <= end; i++)
    {
        if(!strcmp(structura.command[i], ">>"))
        {
            f = open(structura.word[i+1], O_RDWR | O_APPEND | O_CREAT, 0666); //+1 тк файл в след элем структуры 
            dup2(f, 1);
            return f_file(start, i);
        }
        else if(!strcmp(structura.command[i], ">"))
        {
            f = open(structura.word[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
            dup2(f, 1);
            return f_file(start, i+1);
        }
        else if(!strcmp(structura.command[i], "<"))
        {
            f = open(structura.word[i+1], O_RDONLY);
            dup2(f, 0);
            return f_file(start, i);
        }
    }
    return f_pipe(start, end); //на случай если нет >>/>/<
}

int f_pipe(int start, int end)
{
    int fd[2]; int prev[2]; //два канала
    prev[0] = 0; prev[1] = 1;
    int pid; int status;
    for (int i = start; i <= end; i++)
    {
         	if(i == end)
            {
                pid = exec(i, prev[0], prev[1]);
                waitpid(pid, &status, 0);
                while (wait(0) != -1); // завершить сыновей
                if (WIFEXITED(status) && (!WEXITSTATUS(status)))
                    return 1; // команда выполнилась
                else return 0;
            }
            else if (!(strcmp(structura.command[i], "|")))
            {
                pipe(fd);
                exec(i, prev[0], fd[1]);
                prev[0] = fd[0];
            }
    }
    return 1;
}

int exec(int i, int from, int to)
{
    dup2(from, 0); dup2(to, 1);
    pid_t pid;
    if (!(pid = fork()))
    {         //разделение на пробелы и execvp
        char *arr[16];
        int wcount = 0;
        char *token = strtok(structura.word[i], " ");
        while(token != NULL)
        {
            arr[wcount] = token;
            wcount++;
            token = strtok(NULL, " ");
        }  
        arr[wcount] = NULL;
        execvp(arr[0], arr);
      	printf("command %s not found\n", arr[0]);
        exit(1);
    }
    else
    {
        return pid;
    }
}

void execution()
{
    int i_execution = 0;
    int start = i_execution;
    int end; 
    for (i_execution = 0; i_execution <= i_main; i_execution++)
    {
        if (!strcmp(structura.command[i_execution], "&"))
        {
            end = i_execution;
            pid_t pid_fon;
            if (!(pid_fon = fork())) //сын - фон
            {
                setpgid(0, 0);
                signal(SIGINT, SIG_IGN);
                int f_null = open("/dev/null", O_RDONLY);
                dup2(f_null, 0);
                if (f_and_or(start, end)){exit(0);}
                else {exit(1);}
            }
            else
            {
                start = end + 1;
            }
        }
        else if (!strcmp(structura.command[i_execution], ";"))
        {
            end = i_execution;
            f_and_or(start, end);
            start = end + 1;
        }
    }
}

