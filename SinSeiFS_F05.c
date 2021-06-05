#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/wait.h>
#define MKDIR_STATUS 1
#define MKNOD_STATUS 2
#define RMDIR_STATUS 3
#define REMOVE_STATUS 4
#define READDR_STATUS 5
#define RENAME_STATUS 6
#define TRUNCATE_STATUS 7
#define WRITE_STATUS 8
#define READ_STATUS 9
#define OPEN_STATUS 10

static const char *dirpath = "/home/juned/Documents";
char name[10] = "name_";
char encv2[10] = "enc2_";
static int lastCommand = 0;
void encodeatbash(char *enc, int length)
{
    char new[length + 1];
    if (strcmp(enc, ".") == 0 || strcmp(enc, "..") == 0)
        return;
    for (int i = 0; i <= strlen(enc); i++)
    {
        unsigned char c = enc[i];
        if (c < 'A' || (c > 'Z' && c < 'a') || c > 'z')
        {
            new[i] = c;
            continue;
        }
        int flag = 0;
        if (c <= 90)
        {
            c = c + 32;
            flag = 1;
        }
        int off = c - 'a';
        new[i] = 'z' - off;
        if (flag)
            new[i] = new[i] - 32;
    }
    for (int i = 0; i <= strlen(enc); i++)
    {
        enc[i] = new[i];
    }
}

void decodeatbash(char *enc, int length)
{
    if (strlen(enc) < 4)
        return;
    char name[5] = "AtoZ_";
    int counter;
    for (counter = 0; counter <= 4; counter++)
    {
        if (name[counter] != enc[counter])
            return;
    }
    return;
}

void encription1(char *enc)
{
    encodeatbash(enc, strlen(enc));
}

void decription1(char *enc)
{
    decodeatbash(enc, strlen(enc));
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{

    printf("DEBUGGING %s\n", path);
    char *enc1Ptr = strstr(path, name);
    if (enc1Ptr != NULL)
        decription1(enc1Ptr);
    char *enc2Ptr = strstr(path, encv2);

    printf("\n\nDEBUG readdir\n\n");

    char fpath[1000];
    if (strcmp(path, "/") == 0)
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else
        sprintf(fpath, "%s%s", dirpath, path);

    int res = 0;
    DIR *dp;
    struct dirent *de;
    (void)offset;
    (void)fi;
    dp = opendir(fpath);
    if (dp == NULL)
        return -errno;

    while ((de = readdir(dp)) != NULL)
    {
        struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;
        if (enc2Ptr != NULL)
        {
            if (de->d_type == DT_REG)
            {
                if (strcmp(de->d_name + (strlen(de->d_name) - 4), ".000") == 0)
                {
                    de->d_name[strlen(de->d_name) - 4] = '\0';
                    res = (filler(buf, de->d_name, &st, 0));
                }
            }
            else
            {
                res = (filler(buf, de->d_name, &st, 0));
            }
        }
        else
        {
            if (enc1Ptr != NULL)
                encription1(de->d_name);
            res = (filler(buf, de->d_name, &st, 0));
        }
        if (res != 0)
            break;
    }
    closedir(dp);
    lastCommand = READDR_STATUS;
    return 0;
}
static struct fuse_operations xmp_oper = {
    .readdir = xmp_readdir};

int main(int argc, char *argv[])
{
    umask(0);
    return fuse_main(argc, argv, &xmp_oper, NULL);
}