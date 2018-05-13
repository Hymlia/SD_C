#include "include.h"

u_long prognum;
bloc chainbloc[100] ;
operation attente[50];

short inscription(int id) {
  static int res = 1;
  printf("Inscription acceptée par le serveur\n");
  operation o = {"Inscription", id , 0 , 0};
  ajoutoperationdansattente(o);
  printf("%s\n",attente[0].nom);
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

void ajoutoperationdansattente(operation o) {
  int i=0;
  while(i<50 && strcmp(attente[i].nom, "")!=0) {
    i++;
  }
  if(i!=50) {
    attente[i]=o;
  }
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
  for(int i=0; i<50;i++) {
    attente[i]=operationvide;
  }


  registerrpc(prognum , VERSNUM, 1, inscription, (xdrproc_t) xdr_int, (xdrproc_t) xdr_int);
  registerrpc(prognum , VERSNUM, 1, demandepts, (xdrproc_t) xdr_int, (xdrproc_t) xdr_float);
  svc_run();
  return 0;
}
