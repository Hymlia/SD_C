#include "include.h"

char * host;
u_long prognum;

int main (int argc, char *argv[])
{
  enum clnt_stat stat ;
  char *host ;
  int procnum ;
  int id;

  if (argc != 4)
  {
    printf("Usage : %s <machine_serveur> <prognum> <id>\n",argv[0]) ;
    exit(1) ;
  }
  host = argv[1] ;
  char * entrop;
  prognum = strtoul(argv[2], &entrop, 16);
  id = atoi(argv[3]);

  printf("prognum %lu\n", prognum);
  fflush(stdout);


  int res;

  stat = callrpc(host,prognum, VERSNUM, 1, (xdrproc_t) xdr_int, (char *)&id , (xdrproc_t) xdr_int , (char *)&res );

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
