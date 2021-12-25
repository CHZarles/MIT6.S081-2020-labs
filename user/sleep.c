// user/sleep.c 

/* sleep 和 atoi 的声明都在里面 
 *真正的实现(我估计)是做成动态链接库了*/
#include "kernel/types.h"
/* 模仿echo.c 的头文件 ; 不加会编译不了*/
#include "kernel/stat.h"
#include "user/user.h"

int 
main(int argc , char* argv[]){

  if(argc <= 1){
    printf("sleep: need parameter error\n");
    exit(0);
  }
  
   sleep(atoi(argv[1]));
   exit(0);
}