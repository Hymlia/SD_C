#include "include.h"

u_long prognum;
bloc chainbloc[100] ;
operation attente[50];
noeudb voisins[20];



/*
 * Operations auxiliaires
 */

//revoie l'indice du dernier bloc occupe dans chainbloc
 int indicedernierbloc() {
   int i=0;
   int trouve=0;
   while(i<100 && trouve==0) {
     if(chainbloc[i].time==0) {
       trouve=1;
     }
     else {
       i++;
     }
   }

     return i-1;
}

//supprime les operations qui sont dans le bloc de la liste d'attente
void supprimeropdubloc(bloc b) {
  operation operationvide = {"",0,0,0};
  for(int i=0; i<4;i++) {
    for(int j=0;j<50; j++) {
      operation o1 = b.operations[i];
      operation o2 = attente[j];
      if(strcmp(o1.nom,o2.nom)==0 && o1.noeud1==o2.noeud1 && o1.noeud2 == o2.noeud2 && o1.quantite == o2.quantite && o1.time == o2.time) {
        attente[i] = operationvide;
      }
    }
  }
}

//revoie 1 si l'operation existe dans la liste d'attente 0 sinon
 int operationdejadansattente(operation o) {
   int res=0;
   for(int i=0; i<50; i++) {
     operation oc = attente[i];
     if(strcmp(oc.nom,o.nom)==0 && o.noeud1==oc.noeud1 && o.noeud2 == oc.noeud2 && o.quantite == oc.quantite && o.time == oc.time) {
       res=1;
     }
   }
   return res;
 }

//renvoie le hash du bloc
 char * hashbloc(bloc b) {
   char * res ="hash";
   return res;
 }

//ajoute l'operation dans la liste d'attente renvoie 1 si c'est reussi 0 sinon
int ajoutoperationdansattente(operation o) {
  int i=0;
  if(operationdejadansattente(o)==0) {
    while(i<50 && strcmp(attente[i].nom, "")!=0) {
      i++;
    }
    if(i!=50) {
      attente[i]=o;
      printf("Operation ajoutée\n");
      return 1;
    }
    else {
      printf("Problème ajout operation\n");
      return -1;
    }
  }
  else {
    printf("Operation deja dans attente\n");
    return -1;
  }
}

//ajoute les operations du bloc a la liste d'attente
void  ajouteropdubloc(bloc b) {
  int i=0;
  while(i<4) {
    operation o = b.operations[i];
    if(operationdejadansattente(o)==0) {
      ajoutoperationdansattente(o);
    }
    i++;
  }
}

//supprime l'operation si elle est dans la liste d'attente
void supprimeropsidejapresente(operation o) {
  for(int i=0;i<50; i++) {
    operation ocourante = attente[i];
    if(strcmp(ocourante.nom,"")!=0) {
      if(strcmp(ocourante.nom, o.nom)==0 && o.noeud1==ocourante.noeud1 && o.noeud2==ocourante.noeud2 && o.quantite==ocourante.quantite && o.time == ocourante.time) {
        attente[i].nom ="";
        attente[i].noeud1=0;
        attente[i].noeud2 =0;
        attente[i].quantite=0;
      }
    }
  }
}

//envoie l'operation a tous les voisins
void envoyeroperation(operation o) {
  for(int i = 0; i<20; i++) {
    if(voisins[i].pn !=0 && voisins[i].pn!=o.envoyeur) {
      noeudb nbcourant = voisins[i];
      int retour;
      enum clnt_stat stat ;
      o.envoyeur = prognum;
      stat = callrpc(nbcourant.addr,nbcourant.pn, VERSNUM, 4, (xdrproc_t) xdr_operation, (char *)&o , (xdrproc_t) xdr_int , (char *)&retour );

      if(stat != RPC_SUCCESS) {
        fprintf(stderr, "Echec de l'appel distant\n") ;
        clnt_perrno(stat) ;
        fprintf(stderr, "\n") ;
      }

      if(retour==1) {
        printf("L'operation a été envoyée au voisin %s : %lu\n", nbcourant.addr, nbcourant.pn);
      }
      else {
        printf("L'operation n'a été pas envoyée au voisin %s : %lu\n", nbcourant.addr, nbcourant.pn);
      }
    }
  }
}

//envoie le bloc aux voisins
void envoyerbloc(bloc b) {
  for(int i = 0; i<20; i++) {
    if(voisins[i].pn !=0 && voisins[i].pn!=b.createur) {
      noeudb nbcourant = voisins[i];
      int retour;
      enum clnt_stat stat ;
      stat = callrpc(nbcourant.addr,nbcourant.pn, VERSNUM, 3, (xdrproc_t) xdr_bloc, (char *)&b , (xdrproc_t) xdr_int , (char *)&retour );

      if(stat != RPC_SUCCESS) {
        fprintf(stderr, "Echec de l'appel distant\n") ;
        clnt_perrno(stat) ;
        fprintf(stderr, "\n") ;
        return 1 ;
      }

      if(retour==1) {
        printf("Le bloc a été envoyée au voisin %s : %lu\n", nbcourant.addr, nbcourant.pn);
      }
      else {
        printf("Le bloc n'a été pas envoyée au voisin %s : %lu\n", nbcourant.addr, nbcourant.pn);
      }
    }
  }
}

//recupere les 4 premiere operations dans la liste d'attente
void recuperer4op(operation * tab) {
//  operation * tab = malloc(4*sizeof(operation));
  int i =0;
  int nbtrouve =0;
  while(i<50 && nbtrouve<4) {
    if(strcmp(attente[i].nom,"")!=0) {
      tab[nbtrouve] = attente[i];
      nbtrouve++;
    }
    i++;
  }

  operation ovide = {" ",0,0,0,0,0};
  while(nbtrouve<4) {
    tab[nbtrouve] = ovide;
    nbtrouve++;
  }

}

/*
 * Operations serveur
 */
int * inscription(int * id) {
  static int resinsc = 1;
  printf("Inscription acceptée par le serveur\n");
  time_t t = time(0);
  operation o = {"Inscription", id , 0 , 0, (int) t, prognum};
  ajoutoperationdansattente(o);
  envoyeroperation(o);
  return &resinsc;
}

float * demandepts(int * id) {
  static float respts = 0;
  for(int i=0; i<100; i++) {
    if(strcmp(chainbloc[i].hash,"")!=0) {
      for(int j = 0; j<4; j++) {
        operation o = chainbloc[i].operations[j];
        if(strcmp(o.nom, "Don")==0) {
          if(o.noeud1 == id) {
            respts =respts - o.quantite;
          }
          else if(o.noeud2 == id) {
            respts =respts + o.quantite;
          }
        }
      }
    }
  }
  time_t t = time(0);
  operation o = {"Demande", id , 0 ,0 , (int) t, prognum};
  ajoutoperationdansattente(o);
  envoyeroperation(o);
  return &respts;
}

int * recevoirbloc(bloc * b) {
  static int resb = 1;
  int ilastb = indicedernierbloc();
  if(ilastb<99) {
    bloc lastb = chainbloc[ilastb];
    char * hashlast = hashbloc(lastb);
    printf("dans ajouter bloc de %lu, hash du dernier bloc de la chaine local : %s , hash previousbloc du nouveau bloc : %s \n",prognum, hashlast, b->previoushash);
    if(strcmp(hashbloc, b->previoushash)==0) {
      chainbloc[ilastb+1] = *b;
      //supprimeropdubloc(*b);
      envoyerbloc(*b);
    }
    else {
      printf("%lu n'accepte pas le nouveau bloc\n", prognum );
      ajouteropdubloc(*b);
    }
  }
  else {
    printf("La blocchain de %lu est pleine\n", prognum);
  }
  return &resb;
}

int * recevoiroperation(operation * o) {
  static int resro ;
  int i = ajoutoperationdansattente(*o);
  envoyeroperation(*o);
  if(i==1) {
    resro=1;
  }
  else {
    resro=-1;
  }

  return &resro;
}


/*
 * creation blocs
 */
 void * creerbloc() {
    while(1) {
      int attente = rand()%(40-20) +20;
      sleep(attente);
      operation * tab = malloc(sizeof(operation)*4);
      recuperer4op(tab);
      operation o1 = tab[0];
      operation o2 = tab[1];
      operation o3 = tab[2];
      operation o4 = tab[3];
      int ilastb = indicedernierbloc();
      if(ilastb<99) {
        bloc lastb = chainbloc[ilastb];
        char * hashancien = hashbloc(lastb);
        time_t ti = time(0);
        bloc b = {.operations={o1,o2,o3,o4},.hash="",.previoushash=hashancien,.time =(int) ti,.createur= prognum};
        char * nhash = hashbloc(b);
        b.hash=nhash;
        chainbloc[ilastb+1]= b;
        supprimeropdubloc(b);
        envoyerbloc(b);

      }
      else {
        printf("La blocchain de %lu est pleine\n", prognum);
      }

    }
 }

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
  fflush(stdout);

  operation operationvide = {"",0,0,0};

//initialisation chainbloc
  for(int i=0; i<100; i++) {
    bloc bloccourant = {.operations ={operationvide,operationvide,operationvide,operationvide},.hash="hash",.previoushash="hash", .time=0, .createur=0};
    chainbloc[i] = bloccourant;
  }
  //initialisation liste d'attente
  for(int i=0; i<50;i++) {
    attente[i]=operationvide;
  }
  //initialisation voisins
  for(int i=0;i<20;i++) {
    noeudb n = {"",0};
    voisins[i]=n;
  }
  if(argc > 2) {
    int nbvoisin = 0;
    for(int i = 2; i<argc; i=i+2) {
      noeudb n = {argv[i] , strtoul(argv[i+1], &entrop,16)};
      voisins[nbvoisin] = n ;
      nbvoisin ++;
    }
  }
  pthread_t thr ;
  if(pthread_create(&thr, NULL, creerbloc, NULL) == -1) {
    perror("pthread_create");
    return EXIT_FAILURE;
  }


  registerrpc(prognum , VERSNUM, 1, inscription, (xdrproc_t) xdr_int, (xdrproc_t) xdr_int);
  registerrpc(prognum , VERSNUM, 2, demandepts, (xdrproc_t) xdr_int, (xdrproc_t) xdr_float);
  registerrpc(prognum , VERSNUM, 3, recevoirbloc, (xdrproc_t) xdr_bloc, (xdrproc_t) xdr_int);
  registerrpc(prognum , VERSNUM, 4, recevoiroperation, (xdrproc_t) xdr_operation, (xdrproc_t) xdr_int);
  printf("%lu a enregistrer ses fonctions\n", prognum);
  svc_run();

  printf("fin serveur \n");
  if (pthread_join(thr, NULL)) {
      perror("pthread_join");
      return EXIT_FAILURE;
  }


  return 0;
}
