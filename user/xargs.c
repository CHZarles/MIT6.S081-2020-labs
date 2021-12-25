#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

#define STDIN 0
#define MAXLINE 1024

//find . b | xargs grep hello
//shell调用带管道的命令时, 会在把管道后的程序的标准输入(fd = 0)指向
//管道的read端
//xrags grep hello
//对应 argc = 3 , argv[0] = xrags , argv[1] = grep, argv[2] = hello , 
int main(int argc , char* argv[]){
    if( argc > MAXARG ){
        fprintf(2 , "err: too much error\n");
        exit(1);
    }

    //处理标准输入，作为附加参数
    char* exec_name = argv[1];
    char* params[MAXLINE];


    for(int i = 1 ; i < argc ; ++ i)
        params[i - 1] = argv[i];

    //记录新加参数在params的位置
    int append_pos = argc - 1;
    int end_pos = argc;

    //append_param是要加入 params 的参数
    char append_param[MAXLINE];
    int idx = 0;
    
    while( read(STDIN , (append_param + idx), 1 ) == 1 ) {
        //截取到一个完整的参数
        if(append_param[idx] == ' ' || append_param[idx] == '\n'){
            //放置结束符
            append_param[idx] = '\0';
            //拷贝一下参数
            char *append = malloc(sizeof(append_param));
            memcpy(append, append_param, sizeof(append_param));
            params[append_pos] = append;
            params[end_pos] = 0;
            //运行程序
            if(fork( ) == 0){
                exec( exec_name , params);
            }else
                wait(0);

            idx = 0;
        }
        else
         ++ idx;
    }

    exit(0);

}
