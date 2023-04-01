#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

void cd(char **args)
{
    if (args[1] == NULL) {
        // if no argument is provided, change directory to home directory
        if (chdir(getenv("HOME")) != 0) {
            perror("chdir");
        }
    } else {
        if (chdir(args[1]) != 0) {
            perror("chdir");
        }
    }
}

void excmd(char **args)
{
    if (args[1] == NULL) {
        exit(0);
    } else {
        int excode=atoi(args[1]);
        exit(excode);
    }
}


void exec(char** args)
{
    if (args[0] == NULL) {
        return;
    }
    if (strcmp(args[0], "cd") == 0) {
        cd(args);
        return;
    }
    else if (strcmp(args[0], "exit") == 0) {
        excmd(args);
        return;
    }
    else if (strcmp(args[0], "clear") == 0) {
        printf("\033[2J");
        printf("\033[H");
        fflush(stdout);
        return;
    }
    int pipefd[2];
    pid_t pid;
    pipe(pipefd);
    pid = fork();
    if (pid == 0) {
        // redirect stdout to the pipe
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);

        execvp(args[0], args);
        perror("vega");
        exit(1);
    } else if (pid < 0) {
        perror("fork");
    } else {
        close(pipefd[1]);
        char buf[BUFSIZ];
        ssize_t nbytes;
        size_t len = 0;
        while ((nbytes = read(pipefd[0], buf + len, BUFSIZ - len)) > 0) {
            len += nbytes;
            if (len >= BUFSIZ) {
                break;
            }
        }
        buf[len] = '\0';
        close(pipefd[0]);
        waitpid(pid, NULL, 0);

        // check if stdout is a terminal and add a newline if necessary
        struct stat st;
        fstat(STDOUT_FILENO, &st);
        if (S_ISCHR(st.st_mode) && isatty(STDOUT_FILENO)) {
            int last_char = '\0';
            if (len > 0) {
                last_char = buf[len - 1];
            }
            if (last_char != '\n') {
                write(STDOUT_FILENO, "\n", 1);
                len++; // increment length to include added newline character
            }
        }

        // write the output of the child process to the console
        write(STDOUT_FILENO, buf, len);
    }
}