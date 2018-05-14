#----------------------------------------------
all : NoeudBloc NoeudParticipant
#----------------------------------------------
.c.o :
	gcc -Wall -c $<
#----------------------------------------------
NoeudParticipant : NoeudParticipant.o xdr_operation.o xdr_bloc.o
	gcc -o NoeudParticipant xdr_operation.o xdr_bloc.o NoeudParticipant.o -lrpcsvc -lnsl -lpthread
NoeudBloc : NoeudBloc.o xdr_operation.o xdr_bloc.o
	gcc -o NoeudBloc xdr_operation.o xdr_bloc.o NoeudBloc.o -lrpcsvc -lnsl -lpthread

#----------------------------------------------
clean :
	/bin/rm -f *.o NoeudBloc NoeudParticipant

#all : NoeudBloc NoeudParticipant

#NoeudBloc : NoeudBloc.o xdr_operation.o xdr_bloc.o
#	gcc -o  NoeudBloc xdr_operation.o xdr_bloc.o NoeudBloc.o
#NoeudParticipant :
#	gcc -o NoeudParticipant xdr_operation.o xdr_bloc.o NoeudParticipant.o

#NoeudBloc.o : NoeudBloc.c include.h
#	gcc -c NoeudBloc.o -c NoeudBloc.c -W -Wall -lrpcsvc -lnsl -lpthread

#NoeudParticipant.o : NoeudParticipant.c include.h
#	gcc -c NoeudParticipant.o -c NoeudParticipant.c -W -Wall -lrpcsvc -lnsl -lpthread

#xdr_operation.o : xdr_operation.c include.h
#	gcc -c xdr_operation.o -c xdr_operation.c -W -Wall

#	xdr_bloc.o : xdr_bloc.c include.h
#		gcc -c xdr_bloc.o -c xdr_bloc.c -W -Wall

#gcc -Wall NoeudParticipant.c xdr_operation.c xdr_bloc.c -o NoeudParticipant -lrpcsvc -lnsl -lpthread
#gcc -Wall NoeudBloc.c xdr_operation.c xdr_bloc.c -o NoeudBloc -lrpcsvc -lnsl -lpthread
#clean :
#	rm -rf *.o NoeudBloc NoeudParticipant
