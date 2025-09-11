#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>
#include <errno.h>
#include <versionhelpers.h>
#include <time.h>

int main(){
    system("cls");
    srand(time(NULL));
    int intbalance = rand() % 100000 - 1000 + 1;
    float balance = intbalance;

    int posneg[100] = {};
    float trans[100] = {};
    int counttrans = 0;

    float num;
    int choice = 0;
    do{
        printf("Welcome! What would you like to do?\n");
        printf("Type 1 to deposit money\nType 2 to withdraw money\nType 3 to see your balance\nType 4 to view your transactions\nType 5 to exit\n");
        scanf("%d", &choice);
        int choicetoexit;
        switch(choice){
            case 1:
            system("cls"); // stopping it from being confusing
            printf("How much would you like to deposit?\n");
            scanf("%f", &num);
            if(num < 0){
                printf("You have to enter an amount above 0 to deposit\n");
            }
            posneg[counttrans] = 1;
            balance = balance + num;
            trans[counttrans] = num;
            num = 0;
            counttrans++;
            printf("Your new balance is %.2f\n", balance);
            printf("Would you like to exit? Type 1 for yes, any other number for no\n");
            scanf("%d", &choicetoexit);
            if(choicetoexit == 1){
                choice = 100;
            } 
            break;

            case 2:
            system("cls");
            printf("How much would you like to withdraw?\n");
            scanf("%f", &num);
            if(num < 0){
                printf("You have to enter an amount above 0 to withdraw\n");
            }
            if(num > balance){
                printf("You can't withdraw more than your balance. Your balance is %.2f\n", balance);
                break;
            }
            posneg[counttrans] = 0;
            balance = balance - num;
            trans[counttrans] = num;
            num = 0;
            counttrans++;
            printf("Your new balance is %.2f\n", balance);
            printf("Would you like to exit? Type 1 for yes, any other number for no\n");
            scanf("%d", &choicetoexit);
            if(choicetoexit == 1){
                choice = 100;
            } 
            break;

            case 3:
            system("cls");
            printf("Your balance is $%.2f\n", balance);
            printf("Would you like to exit? Type 1 for yes, any other number for no\n");
            scanf("%d", &choicetoexit);
            if(choicetoexit == 1){
                choice = 100;
            } 
            break;

            case 4:
            system("cls");
            for(int i = 0; i < counttrans; i++){
                printf("Your balance was $%d at the start. It is now $%.2f\n", intbalance, balance);
                if(posneg[i] == 1){
                    printf("Transaction tpye: Deposit\n");
                }

                else{
                    printf("Transaction type: Withdrawl\n");
                }

                printf("Amount: $%.2f\n", trans[i]);
            }
            printf("Would you like to exit? Type 1 for yes, any other number for no\n");
            scanf("%d", &choicetoexit);
            if(choicetoexit == 1){
                choice = 100;
            } 
            break;

            case 5:
            choice = 100;
            break;
        }
    } while(choice != 100);

    printf("Thanks for using the program\n");
    return 0;    

}   
