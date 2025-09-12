#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
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
 * Description: This is a simple timer program that counts down from
 * a specified number of seconds.
 */
    printf("how much time\n");
    int i;
    scanf("%d", &i);
   


// max value is 2 million

  if (i > 1999999)
  {
    printf("the max value is <2000000");
    exit(1);
  }

// no negative numbers

  if (i < 1)
  {
    printf("no negative numbers are allowed");
    exit(1);
  }
  
    printf("The timer of %d second (s) starts now\n", i);

// actual timer bit below

do
{
    sleep(1);
    i -= 1;
    printf("there are %d second (s) remaining\n", i);
    if (i == 0)
    {
        printf("time up\a");
    }
    
} while (i > 0); // only does it when int i is above 0



return 0;

}
