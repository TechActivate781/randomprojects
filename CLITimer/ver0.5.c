#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){

    printf("how much time\n");
    int i;
    scanf("%d", &i);
   

  printf("The timer of %d second (s) starts now\n", i);

// max value is 2 million

  if (i > 1999999)
  {
    printf("the max value is <2000000");
  }

// no negative numbers

  if (i < 1)
  {
    printf("no negative numbers are allowed");
  }
  
  
// actual timer bit below

do
{
    sleep(1);
    i -= 1;
    printf("there are %d second (s) remaining\n", i);
    if (i == 0)
    {
        printf("time up");
    }
    
} while (i > 0); // only does it when int i is above 0



return 0;

}
