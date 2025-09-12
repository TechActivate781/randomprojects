#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>

int main(){
    int i = 0;
    printf("Welcome to the stopwatch. Press any key to begin.\n");
    printf("Once it has begun, press ctrl + c to stop the stopwatch\n");
  
    getch();

    printf("The stopwatch has now begun\n");

    while (1) {


        Sleep(1000);
        i += 1;
        printf("%d Second (s) have elapsed since the timer begun\n", i);




    }


    
}
