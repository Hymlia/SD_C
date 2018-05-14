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
  int time;
  u_long createur;
};
typedef struct operation operation;

struct bloc {
  operation operations [4] ;
  char * hash;
  char * previoushash;
};
typedef struct bloc bloc;

struct noeudb {
  char * addr;
  u_long pn;
};
typedef struct noeudb noeudb;

bool_t xdr_operation(XDR* xdrs , operation * o);
bool_t xdr_bloc(XDR* xdrs , bloc * o);

#endif
