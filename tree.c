#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "fs.h"

#define MAX_LEVEL 500
// int tag = 0;
int level = 1;
int nfile = 0, ndir = 0;
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

int isNumber(char *s)
{
    int i;
    for(i = 0; i < strlen(s); i++)
	if(s[i] < 48 || s[i] > 57) return 0;
    return 1;
}

void tree(char *path, int depth, int indent, int flag)
{
    if (depth == level)
        return;

    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0)
    {
        printf(2, "tree: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0)
    {
        printf(2, "tree: cannot stat %s\n", path);
        close(fd);
        return;
    }
    int k, id;
    if (!flag && st.type == 2) {
	k = 0;
	char pjg[100];
	id = indent * 4;
	while(k <= id) pjg[k++] = ' ';
	pjg[k++] = '|'; pjg[k++] = '-'; pjg[k++] = '-'; pjg[k] = ' ';
	printf(1, "%s%s\n", pjg, fmtname(path));
	nfile++;
    }
    else if (st.type == 1 && strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
    {
	printf(1, "tree: path too long\n");
    }
    else if(st.type == 1)
    {
	strcpy(buf, path);
	p = buf + strlen(buf);
	*p++ = '/';
	while (read(fd, &de, sizeof(de)) == sizeof(de))
	{
	    if (de.inum == 0) continue;
 	    memmove(p, de.name, DIRSIZ);
	    if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) continue;
	    p[DIRSIZ] = 0;
	    if (stat(buf, &st) < 0)
	    {
	        printf(1, "tree: cannot stat %s\n", buf);
	        continue;
	    }
	    else if (st.type == 1) //directory
	    {
	        int kk; kk = 0;
 	        char pjg2[100];
	        id = indent * 4;
	        while(kk <= id) pjg2[kk++] = ' ';
	        pjg2[kk++] = '|'; pjg2[kk++] = '-'; pjg2[kk++] = '-'; pjg2[kk] = ' ';
	        printf(1, "%s%s\n", pjg2, fmtname(buf));
	        ndir++;
	    }
	    else if (st.type == 2 && !flag) //file
	    {
		k = 0;
		char pjg[100];
		id = indent * 4;
		while(k <= id) pjg[k++] = ' ';
		pjg[k++] = '|'; pjg[k++] = '-'; pjg[k++] = '-'; pjg[k] = ' ';
		printf(1, "%s%s\n", pjg, fmtname(buf));
		nfile++;
	    }
	    if (st.type == 1) tree(buf, depth + 1, indent + 1, flag);
	}
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    if (argc >= 5) {
	printf(1, "tree: invalid argument\n");
	exit();
    }
    if (argc < 2)
    {
        tree(".", MAX_LEVEL, 0, 0);
    }
    else if (strcmp(argv[1],"-L") == 0 )
    {
	if(!isNumber(argv[2])) {
	    printf(1, "tree: invalid argument\n");
	    exit();
	}
        level = atoi(argv[2]);
        tree(".", 0, 0, 0);
    }
    else if (strcmp(argv[1],"-d") == 0 )
    {
        tree(".", MAX_LEVEL, 0, 1);
    }
    else if (argc == 2)
    {
        tree(argv[1], MAX_LEVEL, 0, 1);
    }
    printf(1, "\n%d directories, %d files\n", ndir, nfile);
    exit();
}
