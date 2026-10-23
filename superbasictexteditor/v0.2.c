#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    printf("Welcome! Type a filename that is under 20 characters and does not have spaces - spaces will be cut off: ");

    char Name[50] = {0};
    char option;
    char Path[53] = {0};
    char LastFourLettersOfName[5] = {0}; // this is just to check if the user had .txt already in the file path
    const char *FileExtension = ".txt";

    while(1){
        fgets(Name, 50, stdin);
        Name[(strlen(Name) - 1)] = '\0';

        int temp = 0;

        for(int i = 0; i < 4; i++){
            temp = strlen(Name) - 4 + i;
            LastFourLettersOfName[i] = Name[temp];
        }

        if(strcmp(LastFourLettersOfName, FileExtension) != 0){
           sprintf(Path, "%s.txt", Name);
        }

        if(strcmp(LastFourLettersOfName, ".txt") == 0){
           strcpy(Path, Name);
        }

        FILE *pFile = fopen(Path, "r"); // opening it w/ read to see if it exists.

        if(pFile == NULL){
            // file doesn't exist, great
            option = 'o';
            break;
        }

        if(pFile != NULL){
            while(option != 'a' && option != 'p' && option != 'o'){
                printf("This file already exists. Would you like to overrwrite (o), append to the file (a), or try a new file path (p): ");
                scanf(" %c", &option);

                if(option == 'a' || option == 'o' || option == 'p'){
                    break;
                }
            }

            if(option == 'p'){
                printf("Write the path again: ");
                option = 's'; // just temp
                getchar();
                continue;
            }
        }

        fclose(pFile);
        break;
    }

    printf("To go to a new line, simply write enter.\n");
    printf("To save a file, please write \"savefile\" on a new line.\n");

    char Buffer[1000] = {0};
    FILE *pFile;
    char SaveFile[10] = {0};
    int SizeOfFile;
    const char *CorrectSaveFile = "savefile";
    char BufferForReadingFile[1024] = {0};

    if(option == 'o'){
        pFile = fopen(Path, "w");
        if(pFile == NULL){
            printf("The file did not open correctly. Exiting\n");
            return 1;
        }
        printf("Start writing below: \n\n");
    }

    if(option == 'a'){
        pFile = fopen(Path, "a");
        if(pFile == NULL){
            printf("The file did not open correctly. Exiting\n");
            return 1;
        }
        fclose(pFile); // just testing opening first.
        pFile = fopen(Path, "r");
        if(pFile == NULL){
            printf("Error in opening file.\n");
            return 1;
        }

        printf("Start writing below: \n\n");

        while(fgets(BufferForReadingFile, 1000, pFile) != NULL){
            printf("%s", BufferForReadingFile);
        }

        fclose(pFile);
        pFile = fopen(Path, "a");
        if(pFile == NULL){
            printf("The file didn't open correctly. Exiting\n");
            return 1;
        }
    }

    getchar();

    while(1){
        fgets(Buffer, 1000, stdin);
        for(int i = 0; i < 9; i++){
            SizeOfFile = strlen(Buffer);
            SaveFile[i] = Buffer[SizeOfFile - (9 - i)];
        }
        SaveFile[strlen(SaveFile) - 1] = '\0';

        if(strcmp(SaveFile, CorrectSaveFile) == 0){
            printf("\n\nSaving file and exiting.\n");
            break;
        }
        fprintf(pFile, "%s", Buffer);
    }

    fclose(pFile);
    
    
}
