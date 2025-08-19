#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void handleSignal(int sig) {
  if (sig == SIGINT) {
    printf("Yeah!\n");
  }
  if (sig == SIGHUP) {
    printf("Ouch!\n");
  }
}

int main(int argc, char *argv[]) {
  signal(SIGINT, handleSignal);
  signal(SIGHUP, handleSignal);

  int n = atoi(argv[1]);
  for (int i = 0; i < n; i++) {
    printf("%d\n", i * 2);
    fflush(stdout);
    sleep(5);
  }

  return 0;
}
