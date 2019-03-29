#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>

void *producer( const char *name,char *msg,int size,void *ptr);
void *consumer(const char *name,int size, int check,int child,void *ptr);
char *shuffle(char *msg,int child);


int main(int argc,char *argv[]){

const char *name = "consumer-producer";
const int SIZE = 4096;
char *message;
int child;
int total;
void *ptr;

if(argc!=4){
perror("Invalid input number: there must be 4 inputs.");
exit(0);
}
message=argv[1];
child = atoi(argv[2]);
total = atoi(argv[3]);

if(child!=total){
if(child==1) producer(name,message,SIZE,ptr);
else consumer(name,SIZE,0,child,ptr);
}else consumer(name,SIZE,1,child,ptr);


return 0;
}

void *producer(const char *name,char *msg,int size,void *ptr){
int shm_fd;
//void *ptr;


shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

ftruncate(shm_fd,size);


ptr = mmap(0,size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
if (ptr == MAP_FAILED) {
	printf("Map failed\n");
	exit (-1);
}

sprintf(ptr,"%s",msg);
//ptr += strlen(msg);

printf("Child 1: %s\n",msg);


}

void *consumer(const char *name,int size,int check,int child,void *ptr){
int shm_fd;
//void *ptr;
int i;
shm_fd = shm_open(name, O_RDONLY, 0666);
if (shm_fd == -1) {
	printf("shared memory failed\n");
	exit(-1);
}

ptr = mmap(0,size, PROT_READ, MAP_SHARED, shm_fd, 0);
if (ptr == MAP_FAILED) {
	printf("Map failed\n");
	exit(-1);
}

char *msg = (char*)ptr;

char *tmp = msg;

tmp=shuffle(tmp,child);

msg=tmp;
shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
ptr = mmap(0,size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

sprintf(ptr,"%s",msg);

ptr += strlen(msg);

printf("Child %d: %s\n",child,msg);

if (check) {
	if(shm_unlink(name)==-1){
	printf("Error removing %s\n",name);
	exit(-1);
}}

}

char *shuffle(char *msg,int child){
int length = strlen(msg);

srand(child);
int index1 = rand()%length;
int index2=rand()%length;


while(!isalnum(msg[index1]))
index1 = (index1 + child)%length;
while(!isalnum(msg[index2])){
int pos = (index2 - child)%length;
index2 = pos>0?pos:length+pos-1;
if(index1==index2) index2 = (index2 + child)%length; 
}


char tmp[strlen(msg)+1];
strcpy(tmp,msg);
tmp[index1]=msg[index2];
tmp[index2]=msg[index1];
if(index1>=length||index2>=length){
printf("fdkhldhjglj %d %d\n\n",index1,index2);}

int index=0;
char *result = (char *)malloc(length+1);
while(index <= length)
{
  result[index] = tmp[index];
  index++;
}

msg=result;
return msg;
}
