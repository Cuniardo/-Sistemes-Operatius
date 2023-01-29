#include "utils.h"

 void signalHandler (int sig, siginfo_t * siginfo, void *context);

 
int players = 0;

char *alarmTime;

char *unlock;

 
pid_t alarmPid;

pid_t * childs;

 
time_t alarmStart;

time_t alarmEnd;

 
struct sigaction siga;

 
int
main (int argc, char *argv[])
{
  
 
if (argc != 4)
    {
      
printf ("Ús: %s <TEMPS_DE_JOC> <N_JUGADORS> <PARAULA>\n",
	       argv[0]);
      
exit (1);
    
}
  
 
alarmTime = argv[1];
  
players = atoi (argv[2]);
  
unlock = argv[3];
  
 
siga.sa_sigaction = *signalHandler;
  
siga.sa_flags |= SA_SIGINFO;
  
 
    
    if (sigaction (SIGUSR1, &siga, NULL) != 0)
    {
      
perror ("senyal");
      
exit (1);
    
}
  
 
if (sigaction (SIGUSR2, &siga, NULL) != 0)
    {
      
perror ("senyal");
      
exit (1);
    
}
  
 
printf ("Comença l'escape room [%d] (%s segons)\n", getpid (),
	     alarmTime);
  
printf ("--- Han entrat %d jugadors a la sala\n", players);
  
printf ("--- La paraula misteriosa C)s: %s\n", unlock);
  
 
childs = (pid_t *) malloc ((unsigned) players * sizeof (pid_t));
  
 
for (int i = 0; i < players; i++)
    {
      
switch (childs[i] = fork ())
	{
	
case -1:
	  
perror ("fork");
	  
exit (EXIT_FAILURE);
	
case 0:
	  
printf (BLUE "EscapeRoom[%d] ha entrat el jugador[%d]\n" RESET,
		   getppid (), getpid ());
	  
char *args[] = { "./player", unlock, NULL };
	  
execv (args[0], args);
	  
exit (EXIT_SUCCESS);
	
default:
	  
break;
	
}
    
}
  
 
usleep (100 * 1000);
  
 
switch (alarmPid = fork ())
    {
    
case -1:
      
perror ("Error");
      
exit (-1);
    
case 0:;
      
char *args[] = { "./alarm", alarmTime, NULL };
      
execv (args[0], args);
      
perror ("Error");
      
exit (-1);
    
default:
      
	
	alarmStart = time (NULL);
      
localtime (&alarmStart);
      
break;
    
}
  
 
pause ();
  
 
return 0;

}


 
void
signalHandler (int sig, siginfo_t * siginfo, void *context)
{
  
pid_t sender_pid = siginfo->si_pid;
  
 
if (sig == SIGUSR1)
    {				// ALARM
      printf (RED "************DING DONG\n" RESET);
      
for (int i = 0; i < players; i++)
	{
	  
printf (RED
		   "El jugador[%d] esta atrapat i queda elminat[%d]\n"
		   RESET, childs[i], getpid ());
	
} 
printf ("--- Heu perdut la partida. Esperem que tornis aviat!\n");
    
}
  else if (sig == SIGUSR2)
    {				// PLAYER FOUND KEY
      printf (RESET "El jugador[%d] ha trobat la paraula misteriosa %s!\n",
	      sender_pid, unlock);
      
for (int i = 0; i < players; i++)
	{
	  
printf (GREEN
		   "El jugador[%d] ha escapat i ha finalitzat l'escape room[%d]\n"
		   RESET, childs[i], getpid ());
	
} 
alarmEnd = time (NULL);
      
localtime (&alarmEnd);
      
printf ("--- Heu guanyat la partida en %d segons!\n",
	       (int) difftime (alarmEnd, alarmStart));
    
}
  
 
for (int i = 0; i < players; i++)
    {
      
kill (childs[i], SIGKILL);
    
} 
kill (alarmPid, SIGKILL);
  
exit (0);

}
