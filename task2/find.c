#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

char *merge (char *path, char *dir) {
    int n = strlen(path);
    int m = strlen(dir);
    if (path[n-1] == '/') {
        char *new_path = (char*)malloc((n+m+1)*sizeof(char));
        if (new_path == NULL) {
            perror("malloc");
            exit(1);
        }
        strcpy(new_path, path);
        strcpy(new_path+n, dir);
        new_path[n+m] = '\0';
        return new_path;
    }
    char *new_path = (char*)malloc((n+m+2)*sizeof(char));
    if (new_path == NULL) {
        perror("malloc");
        exit(1);
    }
    strcpy(new_path, path);
    new_path[n] = '/';
    strcpy(new_path+n+1, dir);
    new_path[n+m+1] = '\0';
    return new_path;
}


void find (char *path, char *file, char *user) {
    DIR *dir = opendir(path);
    if (dir == NULL) {
      perror("opendir");
      return;
    }
    struct dirent *entry;
    struct passwd *userinfo, *pwd;
    struct stat buff;
    pwd = getpwnam(user);
    if (pwd == NULL) {
      perror("getpwnam");
      return;
    }
    uid_t id = pwd->pw_uid;
    while ((entry = readdir(dir)) != NULL) {
        char *new_path = merge(path, entry->d_name);
        if (stat(new_path, &buff) == -1) {
          perror("stat");
          free(new_path);
          continue;
        }
        userinfo = getpwuid(buff.st_uid);
        if (userinfo == NULL) {
          perror("getpwuid");
          free(new_path);
          continue;
        }
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) {
          free(new_path);
          continue;
        }
        if (entry->d_type == DT_DIR) {
          find(new_path, file, user);
        }
        if (!strcmp(file, entry->d_name) && userinfo->pw_uid == id) {
          printf("%s\n", new_path);
        }
        free(new_path);
    }
    closedir(dir);
}

int main (int argc, char ** argv) {
  while (1) {
    if (argc == 4) {
      find(argv[1], argv[2], argv[3]);
      break;
    }
    else {
      fputs("Too few arguments.\n", stderr);
    }
  }
  return 0;
}
