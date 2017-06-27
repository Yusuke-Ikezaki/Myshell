#include "command.h"

#define SIZE 64

void ls(int nargs, char *args[]){
  DIR *directory;
  FILE *fp;
  struct dirent *ent;

  if(nargs == 1){
    directory = opendir(".");
    while((ent = readdir(directory)) != NULL)
      printf("%s\n", ent -> d_name);
    closedir(directory);
  } else{
    struct stat st;
    mode_t filetype;
    stat(args[1], &st);
    filetype = st.st_mode & S_IFMT;
    if(filetype == S_IFDIR){
      directory = opendir(args[1]);
      while((ent = readdir(directory)) != NULL)
        printf("%s\n", ent -> d_name);
      closedir(directory);
    } else if(filetype == S_IFREG){
      puts(args[1]);
      fp = fopen(args[1], "r");
      printf("デバイスID                       : %d\n", st.st_dev);
      printf("inode番号                        : %d\n", st.st_ino);
      printf("アクセス保護                     : %o\n", st.st_mode);
      printf("ハードリンクの数                 : %d\n", st.st_nlink);
      printf("所有者のユーザID                 : %d\n", st.st_uid);
      printf("所有者のグループID               : %d\n", st.st_gid);
      printf("デバイスID（特殊ファイルの場合） : %d\n", st.st_rdev);
      printf("容量（バイト単位）               : %d\n", st.st_size);
      printf("ファイルシステムのブロックサイズ : %d\n", st.st_blksize);
      printf("割り当てられたブロック数         : %d\n", st.st_blocks);
      printf("最終アクセス時刻                 : %s", ctime(&(st.st_atime)));
      printf("最終修正時刻                     : %s", ctime(&(st.st_mtime)));
      printf("最終状態変更時刻                 : %s", ctime(&(st.st_ctime)));
      fclose(fp);
    } else{
      puts("No such  file or directory");
    }
  }
}
void cd(int nargs, char *args[]){
  // カレントディレクトリ変更  
  chdir(args[1]);
}
void mv(int nargs, char *args[]){
  rename(args[1], args[2]);
}
void cp(int nargs, char *args[]){
  FILE *fpin,*fpout;
  char buf[SIZE * 8];

  fpin = fopen(args[1], "r");
  if(fpin == NULL){
    perror(args[1]);
    exit(EXIT_FAILURE);
  }

  fpout = fopen(args[2], "w");
  if(fpout == NULL){
    perror(args[2]);
    exit(EXIT_FAILURE);
  }

  while(fgets(buf, SIZE * 8, fpin) != NULL){
    fputs(buf, fpout);
  }

  fclose(fpin);
  fclose(fpout);
}
void rm(int nargs, char *args[]){
  int i;
  for(i = 1; i < nargs; i++)
    remove(args[i]);
}
void ln(int nargs, char *args[]){
  if(strcmp(args[1], "-s") == 0) symlink(args[2], args[3]);
  else link(args[1], args[2]);
}
void echo(int nargs, char *args[]){
  /* 引数をそのまま出力 */
  int i;
  for(i = 1; i < nargs; i++){
    printf("%s ", args[i]);
  }
  printf("\n");
}
void pwd(int nargs){
  char pathname[SIZE * 8];
  getcwd(pathname, SIZE * 8);
  fprintf(stdout, "%s\n", pathname);
}
void cat(int nargs, char *args[]){
  FILE *fp;
  char buf[SIZE * 8];

  fp = fopen(args[1], "r");
  if(fp == NULL){
    perror(args[1]);
    exit(EXIT_FAILURE);
  }

  while(fgets(buf, SIZE * 8, fp) != NULL){
    fputs(buf, stdout);
  }

  fclose(fp);
}
void head(int nargs, char *args[]){
  FILE *fp;
  char c;

  fp = fopen(args[1], "r");
  if(fp == NULL){
    perror(args[1]);
    exit(EXIT_FAILURE);
  }

  int cnt = 0;
  while((c = fgetc(fp)) != EOF && cnt < 10){
    if(c == '\n') cnt++;
    fputc(c, stdout);
  }

  fclose(fp);
}
void unknown(char *arg){
  /* 未知コマンドの対応 */
  printf("unknown command: %s\n", arg);
}
