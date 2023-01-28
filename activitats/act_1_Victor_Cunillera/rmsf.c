#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>

int
main (int argc, char *argv[])
{
  char current_dir[200];
  struct passwd *pw = getpwuid (getuid ());
  const char *userHome = pw->pw_dir;
  char trashdir[] = "/.trash/";

  getcwd (current_dir, sizeof (current_dir));
  if(chdir(userHome) == 0)
  strncat(userHome, trashdir, sizeof(trashdir));

  if (mkdir (userHome, 0777) == 0)
    printf ("directori .trash creat.\n");

  for (int i = 1; i < argc; i++)
    {
      char arg[100];
      char aux_arg[200];
      strncpy(arg, current_dir, sizeof(current_dir));
      strncat(arg, argv[i], sizeof(argv[i]));
      strncpy(aux_arg, userHome, sizeof(userHome));
      strncat(aux_arg, argv[i], sizeof(argv[i]));
      rename (arg, aux_arg);

    }
  return 0;

}
