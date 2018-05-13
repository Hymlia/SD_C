#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#include <rpc/types.h>
#include <rpc/xdr.h>
#include <rpc/rpc.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define VERSNUM 1
#define PROCNUM 1

struct operation {
  char * nom;
  int noeud1;
  int noeud2;
  float quantite;
};
typedef struct operation operation;

struct bloc {
  operation operations [4] ;
  char * hash;
  char * previoushash;
};
typedef struct bloc bloc;

#endif
