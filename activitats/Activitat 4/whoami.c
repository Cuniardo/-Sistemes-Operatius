#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int
main ()
{
  char username[256];
  char line[256];
  FILE *passwd, *userfile;


  getlogin_r (username, sizeof (username));


  passwd = fopen ("/etc/passwd", "r");
  if (passwd == NULL)
    {
      perror ("Error opening /etc/passwd");
      return 1;
    }


  userfile = fopen ("user.txt", "a");
  if (userfile == NULL)
    {
      perror ("Error opening user.txt");
      return 1;
    }


  while (fgets (line, sizeof (line), passwd))
    {
      // Check if the line contains the current user's username
      if (strstr (line, username))
	{
	  // If it does, write it to the user.txt file
	  fputs (line, userfile);
	}
    }


  fclose (passwd);
  fclose (userfile);

  return 0;
}