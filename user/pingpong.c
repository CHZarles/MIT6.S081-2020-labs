#include "kernel/types.h"
/* 模仿echo.c 的头文件 ; */
#include "kernel/stat.h"
#include "user/user.h"


int main(int argc , char* argv[]){

    //创建管道数组
    // arry[0]  read
    // arry[1]  write
    int son2fa[2] , fa2son[2];
    char buf = 'c';
    if( pipe(fa2son) != 0 ||  pipe(son2fa) != 0 )
    {
        printf("error \n");
        exit(1);
    }
    
    //每个方向一个
    if( fork() == 0){
        close(son2fa[0]); //读端口不需要 关掉
        close(fa2son[1]); //写端口不需要 关掉
        // 先读
        read(fa2son[0] , &buf ,1); 
        printf("%d: received ping\n" , getpid());
        //向父进程对应的读管道发一个字节
        write(son2fa[1] , "c" , 1);
        close(son2fa[1]);
    }else{
        close(fa2son[0]); //读端口不需要 关掉
        close(son2fa[1]); //写端口不需要 关掉
        //先写
        write(fa2son[1] , "c" , 1);
        close(fa2son[1]);   
        read(son2fa[0] , &buf ,1); //读父进程的
        printf("%d: received pong\n" , getpid());
    }
    exit(0);
}