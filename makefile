#----------------------------------------------
all : NoeudBloc NoeudParticipant
#----------------------------------------------
.c.o :
	gcc -Wall -c $<
#----------------------------------------------
client : NoeudParticipant.o
	gcc -o NoeudParticipant  NoeudParticipant.o -lrpcsvc -lnsl
server : NoeudBloc.o
	gcc -o NoeudBloc NoeudBloc.o -lnsl

#----------------------------------------------
clean :
	/bin/rm -f *.o NoeudBloc NoeudParticipant
