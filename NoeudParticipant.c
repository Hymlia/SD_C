#include "include.h"

char * host;
char * prognum;

int main (int argc, char *argv[])
{
  enum clnt_stat stat ;
  char *host ;
  int procnum ;

  if (argc != 3)
  {
    printf("Usage : %s <machine_serveur> <prognum> \n",argv[0]) ;
    exit(1) ;
  }
  host = argv[1] ;
  prognum = 0x20000100;

  int arg = 1;
  int res;

  stat = callrpc(host,prognum, VERSNUM, 1, (xdrproc_t) xdr_int, (char *)&arg , (xdrproc_t) xdr_int , (char *)&res );

  if(stat != RPC_SUCCESS) {
    fprintf(stderr, "Echec de l'appel distant\n") ;
    clnt_perrno(stat) ;
    fprintf(stderr, "\n") ;
    return 1 ;
  }

  if(res==1) {
    printf("Le noeud est bien inscrit\n");
  }
  else {
    printf("Le noeud n'est pas inscrit\n");
    return 1;
  }

  

  return 0;


}
