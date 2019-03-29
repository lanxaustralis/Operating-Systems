#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main (){

pid_t pid;
int level;
int numberOfForks;

printf("Base Process ID: %d, Level: 0\n",getpid());
level = 0;
numberOfForks = 4; // can be changed

for(int i=0;i<numberOfForks;i++){
pid = fork();

if(pid==0){
level++;
printf("Process ID: %d, Parent ID: %d, Level: %d\n",getpid(),getppid(),level);}

else if (pid>0)
wait(NULL);
}

return 0;
}


