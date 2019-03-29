#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(){

pid_t pid;

pid = fork();
char *args[3] = {"ps","-f",NULL}; 

int properTermination=1;

if(pid==0){
execvp(args[0],args); 
perror("Execvp did not work correctly"); 
exit(0);
}
else if(pid >0){
wait(NULL);
printf("Child finished execution\n");
exit(0);
}

return 0;
}
