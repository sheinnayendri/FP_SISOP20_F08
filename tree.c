#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

#define MAX_LEVEL 500
// int tag = 0;
int level = 1;
// int indent = 0;
char *
fmtname(char *path)
{
    static char buf[DIRSIZ + 1];
    char *p;

    // Find first character after last slash.
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    // Return blank-padded name.
    if (strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    memset(buf + strlen(p), ' ', DIRSIZ - strlen(p));
    return buf;
}

void ls(char *path, int depth, int indent)
{
    if (depth == level)
        return;

    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0)
    {
        printf(2, "ls: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0)
    {
        printf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type)
    {
    case T_FILE:
        printf(1, "level %d, %s %d %d %d\n", indent ,fmtname(path), st.type, st.ino, st.size);
        break;

    case T_DIR:
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
        {
            printf(1, "ls: path too long\n");
            break;
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if (de.inum == 0)
                continue;
            memmove(p, de.name, DIRSIZ);
            if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) continue;
            p[DIRSIZ] = 0;
            if (stat(buf, &st) < 0)
            {
                printf(1, "ls: cannot stat %s\n", buf);
                continue;
            }
            else printf(1, "level %d, %s %d %d %d\n", indent, fmtname(buf), st.type, st.ino, st.size);
            
            if (st.type == 1){
                // indent++;
                ls(buf, depth + 1, indent + 1);
            }
        }
        break;
    }
    close(fd);
}

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        ls(".", MAX_LEVEL, 0);
        exit();
    }
    if (strcmp(argv[1],"-L") == 0 )
    {
        level = atoi(argv[2]);
        ls(".", 0, 0);
    }
    
    exit();
}