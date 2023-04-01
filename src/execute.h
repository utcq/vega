#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#ifndef EXECUTE_H
#define EXECUTE_H

void cd(char **args);
void excmd(char **args);
void exec(char** args);

#endif
