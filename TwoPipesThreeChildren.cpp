// file: TwoPipesThreeChildren.cpp
// author: M. Amine Belkoura
// edited by: Jacob A. Cherry
// date: 04/06/2017
// purpose: CS3376
// description:
// this program executes "ls -ltr | grep 3376 | wc -l", by dividing the three commands
// among the 3 child processes

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

int main(int argc, char **argv){
    int status;
    pid_t childpid_1;
    pid_t childpid_2;
    pid_t childpid_3;

    char *ls_args[] = {"ls", "-ltr", NULL};
    char *grep_args[] = {"grep", "3376", NULL};
    char *wc_args[] = {"wc", "-l", NULL};

    // create one pipe to send the output of "ls" process to "grep" process
    int pipes_1[2];
    pipe(pipes_1);

    // create a pipe to send the output of "grep" to "wc"
    int pipes_2[2];
    pipe(pipes_2);

    // fork the first child (to execute cat)
    if((childpid_1 = fork()) == -1){
        perror("Error creating a child process");
        exit(1);
    }
    if (childpid_1 == 0) {
        // replace ls's stdout with write part of 1st pipe

        dup2(pipes_1[1], 1);
        // close all pipes (very important!); end we're using was safely copied

        close(pipes_1[0]);
        close(pipes_1[1]);
        close(pipes_2[0]);
        close(pipes_2[1]);

        execvp(*ls_args, ls_args);
        exit(0);
    }
    else {
        if((childpid_2 = fork()) == -1){
            perror("Error creating second child process");
            // if the error occurs here, we already have a child. OH NO! have to kill it
            // or it'll be an orphan!
            kill(childpid_1, SIGKILL);
            exit(1);
        }
        if (childpid_2 == 0) {
            
        
            // replace grep's stdin with read end of 1st pipe and stdout with write end of 2nd pipe
            dup2(pipes_1[0], 0);
            dup2(pipes_2[1], 1);

            close(pipes_1[0]);
            close(pipes_1[1]);
            close(pipes_2[0]);
            close(pipes_2[1]);

            execvp(*grep_args, grep_args);
            exit(0);
        }
        else {
            if((childpid_3 = fork()) == -1){
                perror("Error creating the third child process");
                // if the error occurs here, we already have TWO children. We have to kill them
                // both!
                kill(childpid_1, SIGKILL);
                kill(childpid_2, SIGKILL);
                exit(1);
            }
            if (childpid_3 == 0) {
                
            // replace wc's stdin with read end of 2nd pipe
                dup2(pipes_2[0], 0);

                
                close(pipes_1[0]);
                close(pipes_1[1]);
                close(pipes_2[0]);
                close(pipes_2[1]);

                execvp(*wc_args, wc_args);
                exit(0);
            }
            else { /* lay in the sofa :) */ }
        }
    }
    return(0);

}
