/*********************************************************************
Program : miniShell Version : 1.3
--------------------------------------------------------------------
skeleton code for linix/unix/minix command line interpreter
--------------------------------------------------------------------
File : minishell.c
Compiler/System : gcc/linux
********************************************************************/
#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define NV 20  /* max number of command tokens */
#define NL 100 /* input buffer size */
char line[NL]; /* command input buffer */
/*
shell prompt
*/
void prompt(void) {
  // ## REMOVE THIS 'fprintf' STATEMENT BEFORE SUBMISSION
  // fprintf(stdout, "\n msh> ");
  fflush(stdout);
};

typedef struct {
  pid_t pid;
  char name[100];
} jobInfo;

static int jobNumber = 1; /* tracks the current number to assignn to a job*/
static jobInfo jobs[100]; /* tracks all pids for jobs */

void handle_sigchld(int sig) {
  int status;
  pid_t pid;
  for (int j = 1; j < jobNumber; j++) {
    if (jobs[j].pid > 0) {
      pid = waitpid(jobs[j].pid, &status, WNOHANG);
      if (pid > 0) {
        printf("[%d]+ Done                 %s\n", j, jobs[j].name);
        jobs[j].pid = 0;
      }
    }
  }
}

/* argk - number of arguments */
/* argv - argument vector from command line */
/* envp - environment pointer */

int main(int argk, char *argv[], char *envp[]) {
  int frkRtnVal;         /* value returned by fork sys call */
  char *v[NV];           /* array of pointers to command line tokens
                          */
  char *sep = " \t\n";   /* command line token separators */
  int i;                 /* parse index */
  bool isBackgroundTask; /* whether the task is a background task */
  pid_t pid;             /* tracks process pid*/
  int status;            /* tracks process status*/

  signal(SIGCHLD, handle_sigchld);

  /* prompt for and process one command line at a time */
  while (1) { /* do Forever */
    prompt();
    fgets(line, NL, stdin);
    // fflush(stdin);
    isBackgroundTask = false;
    // This if() required for gradescope
    if (feof(stdin)) { /* non-zero on EOF */
      exit(0);
    }
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\000') {
      continue; /* to prompt */
    }

    char originalLine[100];
    strcpy(originalLine, line);
    v[0] = strtok(line, sep);

    for (i = 1; i < NV; i++) {
      v[i] = strtok(NULL, sep);
      if (v[i] == NULL) {
        break;
      }
      // check for & for background task
      if (strcmp(v[i], "&") == 0) {
        isBackgroundTask = true;
        v[i] = NULL;
      }
    }

    if (strcmp(v[0], "cd") == 0 && i > 1) {
      if (chdir(v[1]) != 0) {
        perror("cd failed");
      }
      continue;
    }
    /* assert i is number of tokens + 1 */
    /* fork a child process to exec the command in v[0] */
    switch (frkRtnVal = fork()) {
      case -1: /* fork returns error to parent process */
      {
        perror("fork");
        break;
      }
      case 0: /* code executed only by child process */
      {
        if (execvp(v[0], v) == -1) {
          perror("execvp");
          exit(EXIT_FAILURE);
        }
        break;
      }
      default: /* code executed only by parent process */
      {
        if (!isBackgroundTask) {  // do not wait if background task
          if (wait(NULL) == -1) perror("wait");
        } else {
          printf("[%d] %d\n", jobNumber, frkRtnVal);
          jobs[jobNumber].pid = frkRtnVal;
          originalLine[strcspn(originalLine, "&")] = '\0';  // remove &
          strcpy(jobs[jobNumber].name, originalLine);  // copy original line
          jobNumber++;
        }
        // REMOVE PRINTF STATEMENT BEFORE SUBMISSION
        // printf("%s done \n", v[0]);
        break;
      }
    } /* switch */
  } /* while */
} /* main */
