#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(){

pid_t pid;

if((pid = fork())>0)
sleep(5);
else
exit(0);

return 0;
}
