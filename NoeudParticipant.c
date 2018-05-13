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
  prognum = argv[2];

  short arg = 1;
  short res;

  stat = callrpc(host,prognum, VERSNUM, 1, (xdrproc_t) xdr_short , (char *)&arg , (xdrproc_t) xdr_short , (char *)&res );

  if(stat != RPC_SUCESS) {
    fprintf(stderr, "Echec de l'appel distant\n") ;
    clnt_perrno(stat) ;
    fprintf(stderr, "\n") ;
    return 1 ;
  }

  if(res) {
    printf("Le noeud est bien inscrit\n");
  }
  else {
    printf("Le noeud n'est pas inscrit\n");
  }

  return 0;


}
