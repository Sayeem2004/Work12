#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

int print_size(long long sz) {
    // Initializations
    int i = 0;
    char *pre[7] = {" B", "KB", "MB", "GB", "TB", "PB", "EB"};

    // Finding Proper Prefix
    while (sz >= 1000) {
        sz /= 1000;
        i++;
    }

    // Printing
    printf("%3d%s ", sz, pre[i]);
}

void get_file_info(struct dirent *curr, long long *sz) {
    // Getting Info
    struct stat info;
    int err = stat(curr->d_name, &info);
    if (err == -1) {
        printf("Error Found While Getting Info Of %s, More Info Below\n", curr->d_name);
        printf("%s\n", strerror(errno));
        return;
    }

    // Printing File Type And Permissions
    printf("%s", (curr->d_type == DT_DIR ? "d" : "-"));
    printf("%s", (info.st_mode & S_IRUSR ? "r" : "-"));
    printf("%s", (info.st_mode & S_IWUSR ? "w" : "-"));
    printf("%s", (info.st_mode & S_IXUSR ? "x" : "-"));
    printf("%s", (info.st_mode & S_IRGRP ? "r" : "-"));
    printf("%s", (info.st_mode & S_IWGRP ? "w" : "-"));
    printf("%s", (info.st_mode & S_IXGRP ? "x" : "-"));
    printf("%s", (info.st_mode & S_IROTH ? "r" : "-"));
    printf("%s", (info.st_mode & S_IWOTH ? "w" : "-"));
    printf("%s", (info.st_mode & S_IXOTH ? "x" : "-"));
    printf(" ");

    // Printing File Count
    printf("%3d ", info.st_nlink);

    // Printing Owner
    struct passwd *own = getpwuid(info.st_uid);
    printf("%s  ", own->pw_name);

    // Printing Group
    struct group *grp = getgrgid(info.st_gid);
    printf("%s  ", grp->gr_name);

    // Printing And Adding Size
    print_size(info.st_size);
    *sz += (curr->d_type == DT_REG ? info.st_size : 0);

    // Printing Last Modified
    const time_t *time = &info.st_mtime;
    char *str = ctime(time);
    str+=4;
    char *temp = str;
    temp+=12;
    *temp = '\0';
    printf("%s ", str);

    // Printing File Name
    printf("%s", curr->d_name);
    printf("\n");
}

int main() {
    // Opening Current Working Directory
    DIR *cwd = opendir(".");
    if (!cwd) {
        printf("Error Found While Opening Current Working Directory, More Info Below\n");
        printf("%s\n", strerror(errno));
        return 0;
    }

    // Initializations
    long long sz = 0;
    struct dirent *curr = readdir(cwd);

    // Getting File Info
    while (curr) {
        get_file_info(curr, &sz);
        curr = readdir(cwd);
    }
    closedir(cwd);

    // Printing Total Size
    printf("Total Size Of Regular Files: ");
    print_size(sz);
    printf("\n");
}
