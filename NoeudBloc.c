#include "include.h"

char * prognum;

short inscription(short b) {
  printf("Inscription acceptée par le serveur\n");
  return 1;

}

int main (int argc, char *argv[]) {
  if (argc != 2)
  {
    printf("Usage : %s <prognum>\n",argv[0]) ;
    exit(1) ;
  }
  prognum = argv[1];


  register(prognum, VERSNUM, 1, inscription, (xdrproc_t) xdr_short, (xdrproc_t) xdr_short);
  svc_run();
  return 0;
}
