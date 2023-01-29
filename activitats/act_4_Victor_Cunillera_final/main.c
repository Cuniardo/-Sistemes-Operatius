#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

int
main ()
{
  int fd[2];
  FILE *fileToRead = ("/etc/passwd", "r");
  FILE *fileToWrite = ("user.txt", "a");
  pid_t pid1, pid2;

  char *p1[] = { "whoami", NULL };
  char *p2[] = { "grep", NULL };

  if (pipe (fd) < 0)
    {
      perror ("Error creant el pipe fd[]");
      return EXIT_FAILURE;
    }

  pid1 = fork ();

  if (pid1 == -1)
    {
      perror ("fork 1");
      exit (1);
      return EXIT_FAILURE;

    }
  else if (pid1 == 0)
    {

      close (fd[0]);

      dup2 (fd[1], STDOUT_FILENO);
      close (fd[1]);

      execvp (p1[0], p1);

      return EXIT_FAILURE;
    }

  pid2 = fork ();

  if (pid2 == -1)
    {
      perror ("fork 2");
      exit (1);
      return EXIT_FAILURE;

    }
  else if (pid2 == 0)
    {
      char *paraula;
      dup2 (fileno (fileToRead), STDIN_FILENO);
      dup2 (fileno (fileToWrite), STDOUT_FILENO);
      close (fd[1]);
      strcpy (read (fd[0], paraula, sizeof (paraula)), p2[1]) close (fd[0]);
      execvp (p2[0], p2);
      return EXIT_FAILURE;
    }
  close (fd[0]);
  close (fd[1]);
  waitpid (pid1, 0, 0);
  waitpid (pid2, 0, 0);
  return EXIT_SUCCESS;
}