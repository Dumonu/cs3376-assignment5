#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

char** strspl(char *str, char delim, int size);

int  main(int argc, char** argv)
{
    if(argc <= 2)
    {
        //we have 1 or 0 commands. As per the assignment, this should be an error.
        fprintf(stderr, "error: too few arguments\n");
        exit(22);
    }
    if(argc > 6)
    {
        // we have 6 or more commands. As per the assignment, this should be an error
        fprintf(stderr, "error: too many arguments\n");
        exit(7);
    }

    int pipes[4][2];
    pid_t children[5];

    for(int i = 0; i < argc - 2; ++i)
    {
        pipe(pipes[i]);
    //  printf("{%d, %d}\n", pipes[i][0], pipes[i][1]);
    }

    for(int i = 0; i < argc - 1; ++i)
    {
        char **command = strspl(argv[i+1], ' ', 2);
        if((children[i] = fork()) == -1)
        {
            perror("Error forking");
            // kill all children which exist so far.
            for(int j = 0; j < i; ++j)
            {
                kill(children[i], SIGKILL);
            }
            exit(errno);
        }
        if (children[i] == 0)
        {
       //   printf("children[%d] = %d\n",i,children[i]);
       //   fprintf(stderr, "entered children[%d]'s execution\nCommand is:%s %s\n", i, *command, command[1]);
            // if this is not the first command, connect the read end of the last pipe to stdin
            if(i > 0)//{
                dup2(pipes[i-1][0],0);
      //        fprintf(stderr, "connected children[%d]'s stdin to pipe %d\n", i, i-1);}
            // if this is not the last command, connect the write end of the next pipe to stdout
            if(i < argc - 2)//{
                dup2(pipes[i][1], 1);
      //        fprintf(stderr, "connected children[%d]'s stdout to a pipe %d\n", i, i);}
                
            // close all unnecessary file descriptors
        //  for(int j = 0; j < argc - 2; ++i)
        //  {
        //      close(pipes[j][0]);
        //      close(pipes[j][1]);
        //  }

            int err = execvp(*command, command);
            fprintf(stderr, "Execvp returned %d\n", err);
            exit(0);
        }
     // printf("childpid %d: This is stdout.\n", children[i]);
    }
    return 0;
}

char** strspl(char *str, char delim, int size)
{
    char *result = str;
    char *first = str;
    char **ret = new char*[size+1];

    ret[0] = first;
    int i = 1;
    while((result = strchr(result, delim)) != NULL)
    {
        *result = 0;
        ++result;
        ret[i++] = result;
    }
    for(; i <= size; ++i)
    {
        ret[i] = NULL;
    }
    return ret;
}
