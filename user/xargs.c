#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"

int main(int argc, char *argv[]){
    if (argc < 2){
        fprintf(2, "Usage: xargs command [args...]\n");
        exit(1);
    }
    char buf[128];
    char x;
    char* args[MAXARG];

    int cnt = 1;
    // copy the arguments after xargs
    for (; cnt < argc; cnt++){
        args[cnt - 1] = argv[cnt];
    }

    int buflen = 0;
    while (read(0, &x, 1) > 0){ // read each character from stdin
        if (x == '\n'){
            buf[buflen] = '\0';
            args[cnt - 1] = buf;
            args[cnt] = 0;

            if (fork() == 0){
                exec(args[0], args);
            }
            else {
                wait(0);
                buflen = 0;
            }
        }
        else {
            buf[buflen++] = x;
        }   
    }
    exit(0);
}