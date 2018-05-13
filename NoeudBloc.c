#include "include.h"

u_long prognum;
bloc chainbloc[100] ;
operation attente[50];
noeudb voisins[20];


/*
 * Operations serveur
 */
short inscription(int id) {
  static int res = 1;
  printf("Inscription acceptée par le serveur\n");
  operation o = {"Inscription", id , 0 , 0};
  ajoutoperationdansattente(o);
  //printf("%s\n",attente[0].nom);
  for(int i = 0; i<20; i++) {
    if(voisins[i].pn !=0) {
      noeudb nbcourant = voisins[i];
      int retour;
      enum clnt_stat stat ;
      stat = callrpc(nbcourant.addr,nbcourant.pn, VERSNUM, 4, (xdrproc_t) xdr_operation, (char *)&o , (xdrproc_t) xdr_int , (char *)&res );

      if(stat != RPC_SUCCESS) {
        fprintf(stderr, "Echec de l'appel distant\n") ;
        clnt_perrno(stat) ;
        fprintf(stderr, "\n") ;
        return 1 ;
      }
    }
  }
  return &res;
}

float demandepts(int id) {
  float res = 0;
  for(int i=0; i<100; i++) {
    if(strcmp(chainbloc[i].hash,"")!=0) {
      for(int j = 0; j<4; j++) {
        operation o = chainbloc[i].operations[j];
        if(strcmp(o.nom, "Don")==0) {
          if(o.noeud1 == id) {
            res =res - o.quantite;
          }
          else if(o.noeud2 == id) {
            res =res + o.quantite;
          }
        }
      }
    }
  }
  return res;
}

void recevoirbloc(bloc b) {

}

void recevoiroperation(operation o) {

}

/*
 * Operations auxiliaires
 */

void ajoutoperationdansattente(operation o) {
  int i=0;
  while(i<50 && strcmp(attente[i].nom, "")!=0) {
    i++;
  }
  if(i!=50) {
    attente[i]=o;
  }
}

/*
 * creation blocs
 */

/*
 * main
 */

int main (int argc, char *argv[]) {
  if (argc < 2 || argc % 2 == 1)
  {
    printf("Usage : %s <prognum> [<hostvoisin> <prognumvoisin> *]\n",argv[0]) ;
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
  for(int i=0; i<50;i++) {
    attente[i]=operationvide;
  }
  for(int i=0;i<20;i++) {
    noeudb n = {"",0};
    voisins[i]=n;
  }

  if(argc > 2) {
    int nbvoisin = 0;
    for(int i = 2; i<argc; i=i+2) {
      noeudb n = {argv[i] , strtoul(argv[i+1], &entrop,16)};
      voisins[nbvoisin] = n ;
    }
  }


  registerrpc(prognum , VERSNUM, 1, inscription, (xdrproc_t) xdr_int, (xdrproc_t) xdr_int);
  registerrpc(prognum , VERSNUM, 2, demandepts, (xdrproc_t) xdr_int, (xdrproc_t) xdr_float);
  registerrpc(prognum , VERSNUM, 3, recevoiroperation, (xdrproc_t) xdr_operation, (xdrproc_t) xdr_int);
  svc_run();
  return 0;
}
