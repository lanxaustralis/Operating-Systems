#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){

if(argc<4){
printf("Invalid input numnber: there must be at least 3 inputs.");
exit (0);}

char *program_name,*message;
int N;

program_name = argv[1];
N = atoi(argv[2]);

char tmp[1000];
strcpy(tmp,argv[3]);
for(int i=4;i<argc;i++){
strcat(tmp," ");
strcat(tmp,argv[i]);


}
message = tmp;

if(N<2){
perror("The children number must be at least 2.");
exit(0);
}

char buffer[20];

pid_t pid;
printf("Parent: Playing Chinese whisper with %d processes.\n",N);
for(int i=0;i<N;i++){
pid = fork();
if(pid==0){
snprintf(buffer,sizeof(buffer),"%d",i+1);
char *args[5] = {program_name,message,buffer,argv[2],NULL}; 
//printf("Process ID: %d, Parent ID: %d\n",getpid(),getppid());
execvp(args[0],args);
perror("Execvp did not work correctly"); 

}else if (pid>0){
wait(NULL);
}else{
perror("Process id is not valid\n");
exit(1);
}}





return 0;
}
