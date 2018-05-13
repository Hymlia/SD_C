#include "include.h"

char * prognum;

bool inscription(bool b) {
  printf("Inscription acceptée par le serveur\n");
  return true;

}

int main (int argc, char *argv[]) {
  if (argc != 2)
  {
    printf("Usage : %s <prognum>\n",argv[0]) ;
    exit(1) ;
  }
  prognum = argv[1];


  register(prognum, VERSNUM, 1, inscription, (xdrproc_t) xdr_bool, (xdrproc_t) xdr_bool);
  svc_run();
  return 0;
}
