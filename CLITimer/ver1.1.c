#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

/*
 * Simple Timer Program
 *
 * Copyright (C) 2024, TechActivate 781
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 */

#define ERROR_VALUE_TOO_HIGH 2
#define ERROR_TOO_LOW_NUMBER 3

int main() {

    printf("This program is licensed under the GNU GPL. For more info, check the LICENSE file in the github or visit https://www.gnu.org/licenses/gpl-3.0.html.\n");
    printf("Enter the time for the timer below\n");
    int i;
    scanf("%d", &i);


    // max value is 2 million

    if (i > 1999999)
    {
        printf("The program max value is 1,999,999. Exiting now.");
        exit(ERROR_VALUE_TOO_HIGH);
    }

    // no negative numbers

    if (i < 1)
    {
        printf("Negative numbers are not allowed. Exiting now.");
        exit(ERROR_TOO_LOW_NUMBER);
    }

    printf("The timer of %d second (s) starts now\n", i);


    if (i == 1999999)
    {
        printf("Are you trying to test the max value :)?\n"); 
    }

    // actual timer bit below

    do
    {
        Sleep(1000);
        i -= 1;
        printf("There are %d second (s) remaining\n", i);
        if (i == 0)
        {
            printf("The timer has now finished\n\a");
        }

    } while (i > 0); // only does the above bit when int i is above 0

    Beep(750, 300);
    Beep(750, 300);
    Beep(750, 300);

    system("pause");

    return 0;

}

