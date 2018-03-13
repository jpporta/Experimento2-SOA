#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <signal.h>

int loop = 1;
void INThandler(int sig){
  signal(sig, SIG_IGN);
  loop = 0;
}

int main(){
  long long int dentroCirculo = 0;
  long long int numTentativas = 0;

signal(SIGINT, INThandler);

  while(loop){
    double xpos = (rand()%2000000 - 1000000)/1000000.0;
    double ypos = (rand()%2000000 - 1000000)/1000000.0;
    if(hypot(xpos,ypos) <= 1) dentroCirculo++;
    numTentativas++;
  }
  printf("PI eh igual a %lf\n", (4*dentroCirculo) / (float)numTentativas);
  return 0;
}
