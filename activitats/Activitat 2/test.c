#include <stdio.h>
#include <time.h>
#include <unistd.h>

int gettimes(struct timespec *real, struct timespec *user, struct timespec *sys);

int main(int argc, char *argv[]) {
    struct timespec real, user, sys;
    int i;

    if (gettimes(&real, &user, &sys) == -1) {
        perror("Error en gettimes");
        return -1;
    }

    printf("Antes de hacer nada:\n");
    printf("Real: %ld sec %ld nsec\n", real.tv_sec, real.tv_nsec);
    printf("User: %ld sec %ld nsec\n", user.tv_sec, user.tv_nsec);
    printf("Sys: %ld sec %ld nsec\n", sys.tv_sec, sys.tv_nsec);

    for (i = 0; i < 5000000; i++) { // fem algo que consumeixi temps
        
        getppid();
    }

    if (gettimes(&real, &user, &sys) == -1) {
        perror("Error en gettimes");
        return -1;
    }

    printf("Despues de hacer algo:\n");
    printf("Real: %ld sec %ld nsec\n", real.tv_sec, real.tv_nsec);
    printf("User: %ld sec %ld nsec\n", user.tv_sec, user.tv_nsec);
    printf("Sys: %ld sec %ld nsec\n", sys.tv_sec, sys.tv_nsec);

    return 0;
}
