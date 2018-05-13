#include "include.h"

char * prognum;

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
  prognum = 0x20000100;


  registerrpc(prognum , VERSNUM, 1, inscription, (xdrproc_t) xdr_int, (xdrproc_t) xdr_int);
  svc_run();
  return 0;
}
