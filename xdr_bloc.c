#include "include.h"

bool_t xdr_bloc(XDR * xdrs , bloc * o) {
  return (xdr_string(xdrs, o->hash , 200) &&
          xdr_string( xdrs , o->previoushash , 200) &&
          xdr_operation(xdrs, o->operation[0]) &&
          xdr_operation(xdrs , o->operation[1]) &&
          xdr_operation(xdrs, o->operation[2]) &&
          xdr_operation(xdrs , o->operation[3])
        );
}
