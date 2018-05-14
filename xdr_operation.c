#include "include.h"

bool_t xdr_operation(XDR * xdrs , operation * o) {
  return (xdr_string(xdrs, &o->nom , 15) &&
          xdr_int(xdrs , &o->noeud1) &&
          xdr_int(xdrs ,&o->noeud2) &&
          xdr_float(xdrs , &o->quantite) &&
          xdr_int(xdrs, &o->time) &&
          xdr_u_long(xdrs, &o->envoyeur)
        );
}
