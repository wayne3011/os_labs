#include "helpers.h"
#include "list.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define MAX_INPUT_SIZE 100

int processCount = 0;
List *processList = NULL;

void sigintHandler(int signo) {
  if (signo == SIGINT) {
    deleteList(processList);
    exit(0);
  }
}

void getArguments(char *command, char ***argv) {
  int argc = strchrc(command, ' ');
  *argv = (char **)calloc(argc + 1, sizeof(char *));
  if (*argv == NULL) {
    perror("Allocation error");
    return;
  }
  (*argv)[0] = strtok(command, " ");
  char *token;
  int i = 1;
  while ((token = strtok(NULL, " ")) != NULL) {
    (*argv)[i] = token;
    i++;
  }
  (*argv)[i] = NULL;
}

void executeCommand(char *command, char **argv) {
  pid_t pid = fork();

  if (pid == 0) {
    execvp(command, argv);
    perror("Error executing command");
    exit(EXIT_FAILURE);
  } else if (pid > 0) {
    append(processList, pid, processCount);
    printf("Process number %d started.\n", processCount);
    processCount++;
  } else {
    perror("Error forking");
  }
}

int main() {
  signal(SIGINT, sigintHandler);

  char input[MAX_INPUT_SIZE];
  processList = createList();
  while (1) {
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';

    if (strstr(input, "exit") != NULL) {
      int processNumber;
      sscanf(input, "exit %d", &processNumber);
      int pid = deleteByNumber(processList, processNumber);
      if (pid >= 0) {
        printf("Process %d terminated\n", pid);
      } else
        printf("Invalid process number.\n");

    } else {
      char **argv;
      getArguments(input, &argv);
      executeCommand(input, argv);
      free(argv);
    }
  }
}
