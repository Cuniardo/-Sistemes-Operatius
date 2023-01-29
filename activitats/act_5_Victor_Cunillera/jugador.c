#include "utils.h"

void signalHandler(int sig);

int main(int argc, char *argv[]) {

    if(argc != 2) {
        exit(1);
    }

    char *code = argv[1];

    while(1) {
        char key[PASS_LENGTH];
        generatePassword(key);

        if (strncmp(key, code, PASS_LENGTH) == 0) {
            kill(getppid(), SIGUSR2);
        }

        //sleep 50ms
        usleep(50 * 1000);

    }
    
    return 0;
}