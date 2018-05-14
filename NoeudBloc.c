#include "include.h"

u_long prognum;
bloc chainbloc[100] ;
operation attente[50];
noeudb voisins[20];



/*
 * Operations auxiliaires
 */

 int indicedernierbloc() {
   int i=0;
   trouve=0;
   while(i<100 && trouve==0) {
     if(strcmp(chainbloc[i],"")==0) {
       trouve=1;
     }
     else {
       i++;
     }
   }

     return i;

}

 int operationdejadansattente(operation o) {
   printf("%lu dans operationdejaenattente \n", prognum);
   int res=0;
   for(int i=0; i<50; i++) {
     operation oc = attente[i];
     if(strcmp(oc.nom,o.nom)==0 && o.noeud1==oc.noeud1 && o.noeud2 == oc.noeud2 && o.quantite == oc.quantite && o.time == oc.time) {
       res=1;
     }
   }
   return res;
 }

 char * hashbloc(bloc b) {
   char * res =b.operations[0].nom[0]+b.operations[1].nom[0]+b.operations[2].nom[0]+b.operations[3].nom[0];
   return res;
 }

int ajoutoperationdansattente(operation o) {
  printf("%lu : dans ajouterop\n" , prognum);
  printf("operation : %s\n", o.nom);
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

void envoyeroperation(operation o) {
  printf("Dans envoyeroperation \n operation : %s\n", o.nom);
  for(int i = 0; i<20; i++) {
    printf("voisin[i].pn = %i\n", voisins[i].pn);
    if(voisins[i].pn !=0 && voisins[i].pn!=o.envoyeur) {
      printf("il y a un voisin a qui envoyer op \n");
      noeudb nbcourant = voisins[i];
      int retour;
      enum clnt_stat stat ;
      o.envoyeur = prognum;
      stat = callrpc(nbcourant.addr,nbcourant.pn, VERSNUM, 4, (xdrproc_t) xdr_operation, (char *)&o , (xdrproc_t) xdr_int , (char *)&retour );

      if(stat != RPC_SUCCESS) {
        fprintf(stderr, "Echec de l'appel distant\n") ;
        clnt_perrno(stat) ;
        fprintf(stderr, "\n") ;
        //return 1 ;
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

void envoyerbloc(bloc b) {
  for(int i = 0; i<20; i++) {
    if(voisins[i].pn !=0) {
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

operation* recuperer4op() {
  operation o1= {" ",0,0,0,0,0};
  operation o2= {" ",0,0,0,0,0};
  operation o3= {" ",0,0,0,0,0};
  operation o4= {" ",0,0,0,0,0};

  int i =0;
  int trouve =0;
  while(i<50 && trouve==0) {
    if(strcmp(attente[i].nom,"")!=0) {
      trouve =1;
      o1 = attente[i];
    }
    i++;
  }
  trouve=0;
  while(i<50 && trouve==0) {
    if(strcmp(attente[i].nom,"")!=0) {
      trouve =1;
      o2 = attente[i];
    }
    i++;
  }
  while(i<50 && trouve==0) {
    if(strcmp(attente[i].nom,"")!=0) {
      trouve =1;
      o3 = attente[i];
    }
    i++;
  }
  trouve=0;
  while(i<50 && trouve==0) {
    if(strcmp(attente[i].nom,"")!=0) {
      trouve =1;
      o4 = attente[i];
    }
    i++;
  }
  operation * tab = malloc(4*sizeof(operation));
  tab[0]=o1;
  tab[1]=o2;
  tab[2]=o3;
  tab[3]=o4;
  return tab;

}

/*
 * Operations serveur
 */
int * inscription(int * id) {
  static int resinsc = 1;
  printf("Inscription acceptée par le serveur\n");
  time_t t = time(0);
  operation o = {"Inscription", id , 0 , 0, (int) t, prognum};
  printf("Temps op %i\n", o.time);
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

void * recevoirbloc(bloc * b) {

}

int * recevoiroperation(operation * o) {
  static int resro ;
  printf("Dans recevoir operation\n");
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
      operation * tab = recuperer4op();
      operation t[] = {tab[0], tab[1], tab[2] ,tab[3]};
      int ilastb = indicedernierbloc();
      if(ilastb<99) {
        bloc lastb = chainbloc[ilastb];
        char * hashancien = hash(lastb);
        bloc b = {"",hashancien,t};
        printf("%lu a cree un bloc\n", prognum);
        chainbloc[ilastb+1]= b;

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
