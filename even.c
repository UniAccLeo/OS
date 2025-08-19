#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void handleSignal(int sig) {
  if(sig == SIGINT){
    printf("Yeah!");
  }
  if(sig == SIGHUP) {
    printf("Ouch!");
  }
}

int main(int argc, char *argv[]) {
    signal(SIGINT, handleSignal);
    signal(SIGHUP, handleSignal);
    
    int n = atoi(argv[1]);
    for(int i = 0; i< n ; i++){
      printf("%d", i*2);
      fflush(stdout);
      sleep(5);
    }

    return 0;
}
