#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>


int main(int argc,char *argv[]){


const int interval =100;
int M,N; 


if(argc!=3){
perror("You must enter two distinct numbers\n");
exit(0);}

M=atoi(argv[1]);
N=atoi(argv[2]);


if(M<N){
perror("Number of children must be lower than array length");
exit(0);
}

printf("Parent: N = %d, M = %d\n",N,M);

int arr[M];
srand(time(0));
for (int i=0;i<M;i++) {
arr[i] = rand() % interval + 1;}


// Pipe operations

int maxIndex;
int part_min, part_max; ; // partition min & max 
int fd[2*N];
int parentArr[N];


// calculations for fair distribution
int remaining = M%N;
int baseDist = M/N;


for (int i = 0; i < N; i++) {
    pipe(&fd[2*i]);
}

for(int i=0;i<N;i++){

if(fork()==0){

if(i<remaining){
part_min = (baseDist+1)*i;
part_max = (baseDist+1)*(i+1)-1;
}else {
part_min = i*baseDist+remaining;
part_max = baseDist*(i+1)+remaining -1;
}

for(int j=part_min;j<=part_max;j++){
if(arr[maxIndex]<arr[j]) maxIndex = j;
}

write(fd[2*i + 1], &arr[maxIndex], sizeof(arr[maxIndex]));
close(fd[2*i + 1]);
printf("Sending sublist to process %d\n",i+1);
exit(0);
}else{
wait(NULL);
read(fd[2*i], &parentArr[i], sizeof(parentArr[i])); 
close(fd[0]);


}
}
int global_max=0;
for(int i=0;i<N;i++){
int curr = parentArr[i];
if(global_max<curr)
global_max=curr;
printf("Max value in Process %d is %d\n",i+1,curr);
}

printf("Global Maximum is %d\n",global_max);
return 0;
}
