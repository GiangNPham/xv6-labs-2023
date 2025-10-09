#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char* basename(char *path) {
    char *p = path + strlen(path);
    while (p > path && *(p - 1) != '/') p--;
    return p; 
}

void ls(char* path, char* filename){
    char buf[512], *p;
    int fd;
    struct dirent de; // to read directory entries (if it is a folder)
    struct stat st; // to decide if a file is a folder or a file

    if ((fd = open(path, O_RDONLY)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type)
    {
    case T_DEVICE:
    case T_FILE:
        // if it is a file, check if the name matches
        if (strcmp(basename(path), filename) == 0){
            printf("%s\n", path);
        }
        break;
    case T_DIR:
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';

        while (read(fd, &de, sizeof(de)) == sizeof(de)){
            if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) continue;


            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;

            ls(buf, filename);
        }
        break;   
    }
    close(fd);
}

int main(int argc, char* argv[]){
    ls(argv[1], argv[2]);
    exit(0);
}