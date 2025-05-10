// random stuff for now - haven't had the time to work on thsi for idk how long

#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>
#include <errno.h>

void shift(char pass[10]){
    // later
}

int main(){
    char test[10] = "abcd";
    char alpha[26] = "abcdefghijklmnopqrstuvwxyz";
    char alphawiththree[26] = "defghijklmnopqrstuvwxyzabc";

    char pass[20] = "TheTest";
    strlwr(pass);

    int length = strlen(pass);

   for(int i = 0; i < 26; i++){
    for(int j = 0; j < length; j++){
        if(pass[j] == alpha[i]){
            pass[j] = alphawiththree[i];
            break;
        }
    }
   }

    printf("%s\n", pass);


}   
