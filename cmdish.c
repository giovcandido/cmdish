#include <direct.h>
#include <io.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>

#define MAX_STRING 5000

#define KEY_DOWN 80
#define KEY_UP 72
#define ENTER 13
#define F1 59 //0 --> 59
#define ESC 27
#define ESPERA 2000

void displayMenu();
void returnToMenu();

void readOption(int *option, int upperLimit, int lowerLimit);

char confirm();
void listDirectoryItems();
void changeFileDirectory();
void renameFile();
void deleteFile();
void createDirectory();
void renameDirectory();
void changeSystemDate();
void changeSystemTime();

int checkSlashInPath(char path[]);

void gotoxy(int x, int y);
void setCursorVisibility(int x);
void clearScreen();

int main(){
	int option;
    char fileName[MAX_STRING];

    system("color 02");

    do{
        clearScreen();

        setCursorVisibility(0);

        displayMenu();

        readOption(&option, 6, 13);

        switch(option){
            case 1:
                listDirectoryItems();
                break;
            case 2:
                changeFileDirectory();
                break;
            case 3:
                renameFile();
                break;
            case 4:
                deleteFile();
                break;
            case 5:
                createDirectory();
                break;
            case 6:
                renameDirectory();
                break;
            case 7:
                changeSystemDate();
                break;
            case 8:
                changeSystemTime();
                break;
        }
    }while(option != -1);

	return 0;
}

void displayMenu(){
    gotoxy(1,1); printf("CMDish - Your best prompt choice");

    gotoxy(1,4); printf("Select the desired option:");

    gotoxy(3,6); printf("1- List all files from a directory");
    gotoxy(3,7); printf("2- Move file from one to another directory");
    gotoxy(3,8); printf("3- Rename file");
    gotoxy(3,9); printf("4- Delete file");
    gotoxy(3,10); printf("5- Create directory");
    gotoxy(3,11); printf("6- Rename directory");
    gotoxy(3,12); printf("7- Change system date");
    gotoxy(3,13); printf("8- Change system time");

    gotoxy(1,17); printf("PRESS ESC TO EXIT THE PROGRAM");
}

void returnToMenu(){
    setCursorVisibility(0);

    printf("\nPress any key to return to the menu...\n");
    getch();
}

void readOption(int *option, int upperLimit, int lowerLimit){
	int key, y;

	*option = -2;
	y = upperLimit;

	do{
		gotoxy(1,y); printf("*");

		if(kbhit){
			key = getch();
		}

		if(!key){
			key = getch();
		}

		switch(key){
			case KEY_DOWN:
				gotoxy(1,y); printf(" ");
				y = y < lowerLimit ? y + 1 : upperLimit;
				break;
			case KEY_UP:
				gotoxy(1,y); printf(" ");
				y = y > upperLimit ? y - 1 : lowerLimit;
				break;
			case ENTER:
				*option = y-(upperLimit-1);
				clearScreen();
				break;
            case F1:
                *option = 0;
                clearScreen();
                break;
            case ESC:
                *option = -1;
                clearScreen();
		}
	}while(*option == -2);
}

char confirm(){
    char resp;

    printf("\nWish to countinue (Y/N)?");

    do{
        resp = toupper(getch());
    }while(resp != 'Y' && resp != 'N');

    printf("\n");

    return resp;
}

void listDirectoryItems(){
    DIR *directory;
    struct dirent *item;
    char pathDirectory[MAX_STRING];
    int count, check;

    setCursorVisibility(1);

    printf("Type in the path with the desired directory name: ");
    gets(pathDirectory);

    check = !checkSlashInPath(pathDirectory) && strlen(pathDirectory) == 1;

    if((directory = opendir(pathDirectory)) == NULL || check){
        printf("\nCannot access directory!\n");
        returnToMenu();
        return;
    }

    count = 0;

    while((item = readdir(directory))){
        printf("\nItem %3i: %s", ++count, item->d_name);
    }

    printf("\n");

    closedir(directory);

    returnToMenu();
}

void changeFileDirectory(){
    char path1[MAX_STRING], path2[MAX_STRING], fileName[MAX_STRING];
    int check;

    setCursorVisibility(1);

    printf("Type in the path to the desired file: ");
    gets(path1);

    check = !checkSlashInPath(path1) && strlen(path1) == 1;

    if(_access(path1, 0) == -1 || check){
        printf("\nCannot access path!\n");
        returnToMenu();
        return;
    }

    printf("Type in the filename: ");
    gets(fileName);

    check = strlen(fileName) == 0;

    if(_access(strcat(path1, fileName), 0) == -1 || check){
        printf("\nCannot access the file!\n");
        returnToMenu();
        return;
    }

    printf("Type in the new path for the file: ");
    gets(path2);

    check = !checkSlashInPath(path2) && strlen(path2) == 1;

    if(_access(path2, 0)== -1 || check){
        printf("\nCannot access new path!\n");
        returnToMenu();
        return;
    }

    if(rename(path1, strcat(path2, fileName)) != 0){
        printf("\nCannot move file from old to new directory!\n");
    }
    else{
        printf("\nFile sucessfully moved.\n");
    }

    returnToMenu();
}

void renameFile(){
    char path1[MAX_STRING], path2[MAX_STRING], fileName[MAX_STRING], nomeNovo[MAX_STRING];
    int check;

    setCursorVisibility(1);

    printf("Type in the path of the file to be renamed: ");
    gets(path1);

    check = !checkSlashInPath(path1) && strlen(path1) == 1;

    if(_access(path1, 0) == -1 || check){
        printf("\nCannot access file path!\n");
        returnToMenu();
        return;
    }

    strcpy(path2, path1);

    printf("Type in the filename: ");
    gets(fileName);

    check = strlen(fileName) == 0;

    if(_access(strcat(path1, fileName), 0) == -1 || check){
        printf("\nCannot access the file!\n");
        returnToMenu();
        return;
    }

    printf("Type in the filename: ");
    gets(nomeNovo);

    if(MoveFile(path1, strcat(path2, nomeNovo)) == 0){
        printf("\nCannot rename file!\n");
    }
    else{
        printf("\nFile successfully renamed.\n");
    }

    returnToMenu();
}

void deleteFile(){
    char path1[MAX_STRING], fileName[MAX_STRING];
    int check;

    setCursorVisibility(1);

    printf("Type in the path of the file to be deleted: ");
    gets(path1);

    check = !checkSlashInPath(path1) && strlen(path1) == 1;

    if(chdir(path1) == -1 || check){
        printf("\nCannot access path!\n");
        returnToMenu();
        return;
    }

    printf("Type in the filename: ");
    gets(fileName);

    check = strlen(fileName) == 0;

    if(_access(strcat(path1, fileName), 0) == -1 || check){
        printf("\nCannot access file!\n");
        returnToMenu();
        return;
    }

    if(remove(path1) != 0){
        printf("\nCannot delete file!\n");
    }else{
        printf("\nFile successfully renamed.\n");
    }

    returnToMenu();
}

void createDirectory(){
    int check;
    char pathDirectory[MAX_STRING];

    setCursorVisibility(1);

    printf("Type in the path and the desired directory name: ");
    gets(pathDirectory);

    check = mkdir(pathDirectory);

    if(!check){
        printf("\nDirectory successfully created!\n");
    }
    else{
        printf("\nCannot create directory!\n");
    }

    returnToMenu();
}

void renameDirectory(){
    char path1[MAX_STRING], path2[MAX_STRING], directoryName[MAX_STRING], newName[MAX_STRING];
    int check;

    setCursorVisibility(1);

    printf("Type in the directory path to be renamed: ");
    gets(path1);

    check = !checkSlashInPath(path1) && strlen(path1) == 1;

    if(_access(path1, 0) == -1 || check){
        printf("\nCannot access path!\n");
        returnToMenu();
        return;
    }

    strcpy(path2, path1);

    printf("Type in the directory name: ");
    gets(directoryName);

    check = strlen(directoryName) == 0;

    if(_access(strcat(path1, directoryName), 0) == -1 || check){
        printf("\nCannot access the directory!\n");
        returnToMenu();
        return;
    }

    printf("Type in the new directory name: ");
    gets(newName);

    if(rename(path1, strcat(path2, newName)) != 0){
        printf("\nCannot rename the directory!\n");
    }
    else{
        printf("\nDirectory successfully renamed!\n");
    }

    returnToMenu();
}

void changeSystemDate(){
    SYSTEMTIME lt;
    unsigned int day, month, year;
    int check;

    GetLocalTime(&lt);

    printf("Current date system: %02u/%02u/%u\n", lt.wDay, lt.wMonth, lt.wYear);

    setCursorVisibility(1);

    if(confirm() != 'S'){
        printf("\nCannot change date!\n");
        returnToMenu();
        return;
    }

    do{
        printf("\nType in the new date: ");
        check = scanf("%02u/%02u/%u", &day, &month, &year);
        fflush(stdin);

        if(check != 3){
            printf("Cannot change date!\n\n");
        }

    }while(check != 3);

    lt.wDay = day;
	lt.wMonth = month;
	lt.wYear = year;

    if(!SetLocalTime(&lt)){
        printf("\nCannot change date!\n");

        if(GetLastError() == 0x522){
            printf("Try executing the program as an administrator.\n");
        }
    }
    else{
        printf("\nDate successfully changed!\n");
    }

    returnToMenu();
}

void changeSystemTime(){
    SYSTEMTIME lt;
    unsigned int hr, min, sec;

    GetLocalTime(&lt);

    printf("Current system time: %02u:%02u:%02u\n", lt.wHour, lt.wMinute, lt.wSecond);

    setCursorVisibility(1);

    if(confirm() != 'S'){
        printf("\nCannot change the time!\n");
        returnToMenu();
        return;
    }

    printf("\nType in the new time: ");
    scanf("%02u:%02u:%02u", &hr, &min, &sec);

    lt.wHour = hr;
	lt.wMinute = min;
	lt.wSecond = sec;
    lt.wMilliseconds = 0;

    if(!SetLocalTime(&lt)){
        printf("\nCannot change the time!\n");

        if(GetLastError() == 0x522){
            printf("Try executing the program as an administrator.\n");
        }
    }
    else{
        printf("\nTime successfully changed!\n");
    }

    returnToMenu();
}

int checkSlashInPath(char path[]){
    if(path[strlen(path)-1] != '\\'){
        strcat(path, "\\");
        return 0;
    }

    return -1;
}

void gotoxy(int x, int y){
	COORD c;
    c.X = x;
	c.Y = y;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void setCursorVisibility(int x){
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = x == 0 ? 100 : 10;
	info.bVisible = x == 0 ? FALSE : TRUE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

void clearScreen(){
	system("cls");
}
