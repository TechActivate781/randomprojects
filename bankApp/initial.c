#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
//3:53:46
// messing with prototyping

void CheckBalance(float balance);
float DepositMoney(float deposit, float balance);
float WithdrawMoney(float amount, float balance);
float GenerateMoney();

int main(){
    srand(time(NULL));
    system("cls");
    float balance = GenerateMoney();
    int option = 0;
    do{
        printf("Welcome\nType 1 to check balance\nType 2 to deposit money\nType 3 to withdraw money\nType 4 to exit\n");
        scanf("%d", &option);

        if(option == 1){
            CheckBalance(balance);
            break;
        }

        else if(option == 2){
            printf("How much money would you like to deposit?\n");
            int depositAmount;
            scanf("%d", &depositAmount);
            balance = DepositMoney(depositAmount, balance);
            printf("Your new balance is $%f\n", balance);
            break;
        }

        else if(option == 3){
            printf("How much money would you like to withdraw?\n");
            int withdrawAmount;
            scanf("%d", &withdrawAmount);
            if(withdrawAmount > balance){
                printf("You are trying to withdraw more money than you have\nCurrent Balance:$%f", balance);
                break;
            }
            balance = WithdrawMoney(withdrawAmount, balance);
            printf("Your new balance is $%f\n", balance);
            break;
        }

        else if(option == 4){
            printf("Exiting\n");
            break;
        }

        else{
            printf("No valid user option detected; exiting\n");
            break;
        }

    }while(option >= 1 && option <= 4);

    printf("Thank you for using our banking app\n");
        system("pause");
        return 0;
}

void CheckBalance(float balance){
    printf("Your balance is $%f\n", balance);
}

float DepositMoney(float deposit, float balance){
    int newMoney = deposit + balance;
    return newMoney;
}

float WithdrawMoney(float amount, float balance){
    int newMoney = balance - amount;
    return newMoney;
}

float GenerateMoney(){
    int max = 10000;
    int min = 100;
    return (rand() % (max - min + 1)) + min;
}
