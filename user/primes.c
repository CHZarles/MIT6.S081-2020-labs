#include "kernel/types.h"
/* 模仿echo.c 的头文件 ; */
#include "kernel/stat.h"
#include "user/user.h"

#define  READPID 0
#define  WRITEPID 1

void prime_sieve(int readPip){

    //读第一个数字 , 并打出来
    int val;
    read(readPip , &val , sizeof(int));
    printf("prime %d\n" , val );
   
   //不断接收
    int nxt;
   //新建一个管道，用来传数据给小弟进程
    int p[2];
    pipe(p);
    
    int flag = 0; //标记有没有数据
    while(read(readPip , &nxt , sizeof(int)))
       if( nxt % val ) //可能是另一个素数
        {
            write(p[WRITEPID] ,  &nxt , sizeof(int));
            flag = 1;
        }   

    if(!flag) exit(0);
     //开一样小弟进程
    close(p[WRITEPID]); //没用的关了
    if( fork() == 0 ){
        prime_sieve(p[READPID]);
        close(p[READPID]); //没用的关了
        }else
            wait(0);  // 等小弟处理

    close(p[READPID]); //没用的关了
    exit(0);
}

int main(){

   int p[2];
   pipe(p); 

   //初始化第一个管道
   for(int i = 2 ; i <= 35 ; ++ i)
      write(p[WRITEPID] , &i , sizeof(int));
    //关了写端口
    close(p[WRITEPID]);
    prime_sieve(p[READPID]);
    close(p[READPID]);
    wait(0);
    //调用
    exit(0);

}