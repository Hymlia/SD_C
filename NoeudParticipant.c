#include <pthread.h>
#include "include.h"


char * host;
u_long prognum;
float mespoints =0;
int id;

void * demandemespoints(void *arg) {
  while(1) {
    sleep(5);
    float res;
    enum clnt_stat stat ;
    stat = callrpc(host,prognum, VERSNUM, 2, (xdrproc_t) xdr_int, (char *)&id , (xdrproc_t) xdr_float , (char *)&res );

    if(stat != RPC_SUCCESS) {
      fprintf(stderr, "Echec de l'appel distant\n") ;
      clnt_perrno(stat) ;
      fprintf(stderr, "\n") ;
      return 1 ;
    }
    mespoints = res;
    printf("%i a %f points\n", id ,mespoints);
  }
}

int main (int argc, char *argv[])
{
  enum clnt_stat stat ;


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

  sleep(1);
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
    printf("Le noeud %i n'est pas inscrit\n", id);
    return 1;
  }

  pthread_t thr ;
  if(pthread_create(&thr, NULL, demandemespoints, NULL) == -1) {
    perror("pthread_create");
    return EXIT_FAILURE;
  }
  if (pthread_join(thr, NULL)) {
perror("pthread_join");
return EXIT_FAILURE;
  }

  return 0;


}
