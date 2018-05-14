#include "include.h"

bool_t xdr_bloc(XDR * xdrs , bloc * o) {
  return (xdr_string(xdrs, &o->hash , 200) &&
          xdr_string( xdrs , &o->previoushash , 200) &&
          xdr_operation(xdrs, &o->operations[0]) &&
          xdr_operation(xdrs , &o->operations[1]) &&
          xdr_operation(xdrs, &o->operations[2]) &&
          xdr_operation(xdrs , &o->operations[3])
        );
}
