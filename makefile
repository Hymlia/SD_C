#----------------------------------------------
all : NoeudBloc NoeudParticipant
#----------------------------------------------
.c.o :
	gcc -Wall -c $<
#----------------------------------------------
client : NoeudParticipant.o xdr_operation.o
	gcc -o NoeudParticipant xdr_operation.o NoeudParticipant.o -lrpcsvc -lnsl -lpthread
server : NoeudBloc.o xdr_operation.o
	gcc -o NoeudBloc xdr_operation.o NoeudBloc.o -lrpcsvc -lnsl -lpthread

#----------------------------------------------
clean :
	/bin/rm -f *.o NoeudBloc NoeudParticipant
