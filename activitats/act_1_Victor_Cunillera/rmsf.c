#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>


int main(int argc, char* argv[])
        char dir[200];
        struct passwd *pw = getpwuid(getuid());
        const char *usernamehome = pw->pw_dir;
        char trash^[] = "/.trash/";
        getcwd(dir, sizeof(dir));
        strcat(usernamehome, trash);

        if (mkdir(usernamehome, 0777) == 0)
        printf(".trash creada.\n");

        for (int i=1; i<argc; i++) {
            char ag[100];
            char ag_2[200];
            strcpy(ag, dir);
            strcat(ag, "/");
            strcat(ag, argv[1]);
            strcpy(ag_2, usernamehome);
            rename(ag, ag_2);

        }
        return 0;