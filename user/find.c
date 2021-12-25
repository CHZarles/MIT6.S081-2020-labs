#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

//改了一下,不填充空格
char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  //p initial advance len(path) and back
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
    
  // if path_name = ./file1/a/b/file_c  then p-> "file_c"
  p++; 

  // Return blank-padded name.
  //return file name
  if(strlen(p) >= DIRSIZ)
    return p;
  
  //if len(p)<DIRSIZ, move *p content into buf,
  memmove(buf, p, strlen(p)); 


  buf[strlen(p)] = 0;
  return buf;
}


//递归找
void find(char *path , char *pattern){

    //名字
    char buf[512], *p;
    //文件描述符
    int fd;
    // 目录项
    struct dirent de; 
    struct stat st;

    //打开文件描述符
    if((fd = open(path, 0)) < 0){
        fprintf(2, "ls: cannot open %s\n", path);
        return;
    }
    //检测合法性
    if(fstat(fd, &st) < 0){
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }

    /*
        struct stat {
        int dev;     // 文件系统的磁盘设备
        uint ino;    // Inode编号
        short type;  // 文件类型
        short nlink; // 指向文件的链接数
        uint64 size; // 文件字节数
            };
    */
    switch(st.type){

    // 如果path名是一个文件名
    case T_FILE: 
        //printf("%s\n" , path);
        if( strcmp(pattern , fmtname(path) ) == 0 ) //检测名字对不对
            printf("%s\n", path);
        break;
    case T_DIR:
    //如果path名是一个文件夹
    //遍历文件项目
        if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
            printf("ls: path too long\n");
            break;
        }

        //路径复制到 buf
        strcpy(buf, path);
        p = buf+strlen(buf);
        *p++ = '/';
        while(read(fd, &de, sizeof(de)) == sizeof(de)){
            //空文件夹
            if(de.inum == 0)
                continue;
            if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
                    continue;
            //拼接成完整路径
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            //打开buf对应的文件描述符
            if(stat(buf, &st) < 0){
                printf("ls: cannot stat %s\n", buf);
                continue;
            }
           
            find( buf , pattern );
        }
        break;
    }
    close(fd);


}



int main(int argc , char* argv[]){

    if(argc != 3){
        fprintf( 2 , " usage: find [filepath] [pattern]\n");
        exit(1);
    }
    
    char *path = argv[1];
    char *pattern = argv[2];
    find(path , pattern);
    exit(0);
}