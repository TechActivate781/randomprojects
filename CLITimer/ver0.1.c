#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){

    printf("how much time\n");
    int i;
    scanf("%d", &i);
   

  printf("The timer of %d second (s) starts now\n", i);

do
{
    sleep(1);
    i -= 1;
    printf("there are %d second (s) remaining\n", i);
    if (i == 0)
    {
        printf("time up");
    }
    
} while (i > 0);



return 0;

}
