#include "include.h"

u_long prognum;
bloc chainbloc[100] ;

short inscription(int b) {
  static int res = 1;
  printf("Inscription acceptée par le serveur\n");
  return &res;

}

int main (int argc, char *argv[]) {
  if (argc != 2)
  {
    printf("Usage : %s <prognum>\n",argv[0]) ;
    exit(1) ;
  }
  char * entrop;
  prognum = strtoul(argv[1], &entrop, 16) ;
  printf("prognum %lu\n", prognum);
  fflush(stdout);

  operation operationvide = {"",0,0,0};

  //chainbloc = malloc(sizeof(bloc)*100);
  for(int i=0; i<100; i++) {
    bloc bloccourant = {.operations ={operationvide,operationvide,operationvide,operationvide},.hash="3",.previoushash=""};
    chainbloc[i] = bloccourant;
  }


  registerrpc(prognum , VERSNUM, 1, inscription, (xdrproc_t) xdr_int, (xdrproc_t) xdr_int);
  svc_run();
  return 0;
}
