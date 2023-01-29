#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sys/wait.h"
#include "helpers.h" //read_line(); split_line()
#include <string.h> // strstr()
#include "log.h" //log_debug(), log_error()
#include <getopt.h>

//Defineixo els colors
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define RED   "\x1B[31m"
#define MAG   "\x1B[35m"
#define BLU   "\x1B[34m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

//Defineixo les constants
#define OPTSTR "v"
#define NOT_ALLOWED "><&"


int launch(char *args);
int help(void);

int 
main(int argc, char* argv[]){
    int opt;
    log_set_quiet(true);
    while ((opt = getopt(argc, argv, OPTSTR)) != EOF)
    switch(opt) {
        case 'v':
        log_set_quiet(false);
        break;
    }

    char* not_allowed[] = {"<",">","&"};

    char *line = NULL;
    char **cmds = NULL;
    int status = 0;

    
    do {
        printf(BLU "ossh" RED "> " RESET);
        read_line(&line);
        line[strcspn(line,"\n")] = '\0';

        int n_cmds = split_line(line,";",&cmds);
        log_debug("Nº de comandes separades per ';':[%d]", n_cmds);
    
        for (int i=0; i<n_cmds; i++){

        log_debug("Revisant [%d/%d]: %s", i+1,n_cmds, cmds[i]);

        char * current_cmd = strdup(cmds[i]);
        if ( strncmp(current_cmd,"sortir",4) ==0 ){
            status=-1;
        } 
        else if(strncmp(current_cmd,"ajuda",4)==0){
            help();
        }
        else
        {
            if ( contains_invalid_chars(cmds[i],not_allowed,sizeof(not_allowed)/sizeof(char*)) == 0 ){
                log_debug(":) Arrencant [%d/%d]: %s", i+1,n_cmds, current_cmd);
                launch(cmds[i]);
            }else{
                fprintf(stderr,":( Impossible d'arrencar [%d/%d]: %s: Operadors no autoritzats [%s]\n",i+1,n_cmds,cmds[i], NOT_ALLOWED);
            }
        }
        log_debug(":) Llimpiant memoria de current_cmd");
        free(current_cmd);
        }

        log_debug(":) llimpiant memoria de cmds");
        free(cmds);

        log_debug(":) Llimpiant memoria de line");
        free(line);

  } while (status==0);
  log_debug(":( parant la shell de ossh...");
	return EXIT_SUCCESS;
}


int help()
{
  printf("####  GTIDIC - UDL - OSSH ########################################\n");
  printf("#                                                                #\n");
  printf("# Escriu el nom i els arguments del programa i prem enter        #\n");
  printf("# Utilitza la comanda man per informacio de altresprogrames      #\n");
  printf("#                                                                #\n");
  printf("##################################################################\n");
  return 1;
}

int launch(char *cmd)
{
    
  char **child_proc = NULL;
  int n_childs = split_line(cmd,"|",&child_proc);
  log_debug("Nº de nens: %d", n_childs);

  int n_pipes = n_childs -1;
  log_debug("Nº de pipes: %d", n_pipes);

  int **pipes = NULL;
  if (n_pipes > 0){
    pipes = malloc( (size_t) n_pipes * sizeof(int *));
    for(int p=0; p<n_pipes; p++){
      pipes[p]=malloc(2);
      pipe(pipes[p]);
    }
  }

  pid_t* pids = malloc( (size_t) n_childs * sizeof(pid_t)); 
  for(int child=0; child<n_childs; child++){
    pids[child]=fork();
    if (pids[child] == 0) 
    {  // Child  
      char **args = NULL;
      log_debug("Nen %d esta executant %s", child,child_proc[child] );  fflush(stdout);
      int n_args = split_line(child_proc[child]," ",&args);
      log_debug("Nº d'arguments: %d", n_args); fflush(stdout);

      if (n_pipes > 0 && child%2==0){
          dup2(pipes[child][1],STDOUT_FILENO);
          close(pipes[child][0]);
          close(pipes[child][1]);
      }

      if (n_pipes > 0 && child%2!=0){
          dup2(pipes[child-1][0],STDIN_FILENO);
          close(pipes[child-1][0]);
          close(pipes[child-1][1]);
      }

      if (n_args > 0 && args[0]!=NULL){
        if (execvp(args[0], args) == -1) { 
            perror("arrencar::execvp"); 
            exit(-1);
        }
      }

    } else if (pids[child] < 0) {
      perror("arrencar::forking");
      return -1;
    }
  }
  
 /*PARENT ONLY*/
  for(int p=0; p<n_pipes; p++){
    close(pipes[p][0]);
    close(pipes[p][1]);
    log_debug("Pare tancat -> pipes[%d][0] and pipes[%d][1]", p,p); fflush(stdout);
  }

  int st = 0;
  for(int child=0; child<n_childs; child++){
     waitpid(pids[child], &st, WUNTRACED);
     log_debug(":) Nen %d acabat amb l'estat %d.", child, WEXITSTATUS(st));
  }
  
  free(child_proc);
  free(pids);

  if (n_pipes > 0){
    free(pipes);
  }

  log_debug(":) Tot arrencat.");

  return 0;
}