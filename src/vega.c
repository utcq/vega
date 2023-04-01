#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <execute.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

char prompt[] = "$  ";
int MAX_CMD_LENGTH=500;

void listen();
void fakelisten();

void sig_handler1(int signo)
{
  if (signo == SIGINT) {
    printf("\n");
    fflush(stdout);
    fakelisten();
  } else if (signo == SIGTERM) {
    printf("\n");
    fflush(stdout);
    exit(0);
  }
}

int claction(int count, int key) {
  printf("CLEL");
  const char* cmd = "clear";
  char* args[] = {const_cast<char*>(cmd), NULL};
  exec(args);
  fflush(stdout);
  fakelisten();
  return 0;
}


void fakelisten() {
  char *cmd;
  char* args[MAX_CMD_LENGTH];
  cmd = readline(prompt);
  if (cmd == NULL) {
      printf("\n");
      exit(0);
  }
}

void listen() {
  char *cmd;
  char* args[MAX_CMD_LENGTH];
  cmd = readline(prompt);
  if (cmd == NULL) {
      printf("\n");
      exit(0);
  }
  add_history(cmd);
  int i = 0;
  args[i] = strtok(cmd, " \n");
  while (args[i] != NULL) {
      i++;
      args[i] = strtok(NULL, " \n");
  }
  args[i] = NULL;
  exec(args);
}

int main() 
  {
  char cmd[MAX_CMD_LENGTH];
  char* args[MAX_CMD_LENGTH];
  signal(SIGINT, sig_handler1);
  signal(SIGTERM, sig_handler1);
  rl_bind_keyseq("\\C-l", claction);
  while (1) 
    {
    listen();
    }
  return 0;
  }

