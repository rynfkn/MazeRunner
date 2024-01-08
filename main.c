#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <time.h>

#include "ColorText.h"

#define CLEAR printf("\x1b[2J\x1b[H");

char map[20][20];
int markmap[20][20];

//properties for level impossible (auto generate map)
int maxBar = 100;
int maxBom = 100;

void LoadFiles(int level){
    FILE *file;
    if(level == 0){
        file = fopen("level0.txt", "r");
    }
    else if(level == 1){
        file = fopen("level1.txt", "r");
    }
    else if(level == 2){
        file = fopen("level2.txt", "r");
    }
    else if(level == 3){
        file = fopen("level3.txt", "r");
    }

    if (file == NULL){
        perror("\nError opening file");
        return;
    }

    for(int i = 0 ; i < 20 ; i++){
        for(int j = 0 ; j < 20 ; j++){
            fscanf(file, " %c", &map[i][j]);

            if(map[i][j] == '#'){
                markmap[i][j] = 2;
            }
            else if(map[i][j] == '*'){
                map[i][j] = '.';
                markmap[i][j] = -1;
            }
            else if(map[i][j] == '.'){
                markmap[i][j] = 1;
            }
        }
    }
    fclose(file);
}

void printoption(int index, char option[][30]){
    for(int i = 0 ; i < 5 ; i++){
        if(i == index){
            setcolors(fgRED, bgBLACK, caBOLD);
            printf("%s\n", option[i]);
            resetcolors();
        }
        else{
            printf("%s\n", option[i]);
        }
    }
}

bool checkbomb(int posI, int posJ){
    if(markmap[posI][posJ] == -1){
        return false;
    }

    return true;
}

void printmap(bool cek){
    if(cek == true){
        for(int i = 0 ; i < 20 ; i++){
            for(int j = 0 ; j < 20 ; j++){
                if(j != 0){
                    printf(" ");
                }
                if(map[i][j] == '+'){
                    setcolors(fgYELLOW, bgBLACK, caNORMAL);
                    printf("%c", map[i][j]);
                    resetcolors();
                }
                else{
                    printf("%c", map[i][j]);
                }
            }
            printf("\n");
        }
    }
    else{
        for(int i = 0 ; i < 20 ; i++){
            for(int j = 0 ; j < 20 ; j++){
                if(j != 0){
                    printf(" ");
                }

                if(markmap[i][j] == -1){
                    setcolors(fgRED, bgBLACK, caNORMAL);
                    printf("*");
                    resetcolors();
                }
                else{
                    if(map[i][j] == '+'){
                        setcolors(fgYELLOW, bgBLACK, caNORMAL);
                        printf("%c", map[i][j]);
                        resetcolors();
                    }
                    else{
                        printf("%c", map[i][j]);
                    }
                }
            }
            printf("\n");   
        }
    }
}

void generatemap(){
    srand(time(NULL));

    //make the border
    for(int i = 0 ; i < 20 ; i++){
        map[0][i] = '#';
        markmap[0][i] = 2;
        
        map[i][0] = '#';
        markmap[i][0] = 2;

        map[i][19] = '#';
        markmap[i][19] = 2;

        map[19][i] = '#';
        markmap[19][i] = 2;
    }

    //initiaton start pos;
    int row = 18;
    int col = 1;
    
    while((row >=  1 && col <= 18) && (markmap[row][col] == 0)){
        //full right direction
        if(row == 1 && col != 18){
            map[row][col] = '.';
            markmap[row][col++] = 1;
        }
        else if(row != 1 && col == 18){
            map[row][col] = '.';
            markmap[row--][col] = 1;
        }
        else{
            int randpath = rand() % 2;

            switch (randpath)
            {
            case 0:
                //up
                map[row][col] = '.';
                markmap[row--][col] = 1;
                break;
            
            case 1:
                //right
                map[row][col] = '.';
                markmap[row][col++] = 1;
                break;
            }
        }
    }

    map[18][1] = 'S';
    markmap[18][1] = 1;
    map[1][18] = 'F';
    markmap[1][18] = 1;

    //isi pagar
    int countbar = 0;
    while(countbar <= maxBar){
        int indexI = rand() % 18 + 1;
        int indexJ = rand() % 18 + 1;

        if(markmap[indexI][indexJ] == 0){
            map[indexI][indexJ] = '#';
            markmap[indexI][indexJ] = 2;
            countbar++;
        }

    }

    int countbom = 0;
    while(countbom <= maxBom){
        int indexI = rand() % 18 + 1;
        int indexJ = rand() % 18 + 1;

        if(markmap[indexI][indexJ] == 0){
            markmap[indexI][indexJ] = -1;
            countbom++;
        }
    }


    for(int i = 0 ; i < 20 ; i++){
        for(int j = 0 ; j < 20 ; j++){
            if((i == 1 && j == 18) || (i == 18 && j == 1)){
                continue;
            }
            if((markmap[i][j] == 0 || markmap[i][j] == -1)){
                map[i][j] = '.';
            }
        }
    }
}

int main(){
    char option[6][30] = {"LEVEL 0", "LEVEL 1", "LEVEL 2", "LEVEL 3", "LEVEL IMPOSSIBLE"};

    int choice;
    int index = 0;
    while(1){
        CLEAR;
        printoption(index, option);

        while(1){
            choice = _getch();
            if(choice == 72 || choice == 80 || choice == 12 || choice == 27 || choice == 13){
                break;
            }
        }

        switch (choice)
        {
        case 72:
            index--;
            if(index == -1){
                index = 4;
            }
            break;
        case 80:
            index++;
            if(index == 5){
                index = 0;
            }
            break;
        case 13:
            //enter the level based the index
            printf("enter\n");
            // printf("DEBUG KONTOL\n");
            printf("%d\n", index);
            if(index >= 0 && index <= 3){
                LoadFiles(index);
                
                //print the initial map
                for(int i = 0 ; i < 20 ; i++){
                    for(int j = 0 ; j < 20 ; j++){
                        if(j != 0){
                            printf(" ");
                        }
                        printf("%c", map[i][j]);
                    }
                    printf("\n");
                }

                int move;
                int curposI = 18;
                int curposJ = 1;

                bool done = false;
                while(1){

                    if(curposI == 1 && curposJ == 18){
                        CLEAR;
                        printmap(true);
                        printf("\nCONGRATS YOU WON!!!\n");

                        int esc;
                        bool finish = false;
                        while(1){
                            printf("\nPRESS ESC TO BACK\n");

                            while(1){
                                esc = _getch();
                                if(esc == 27){
                                    break;
                                }
                            }

                            if(esc == 27){
                                finish = true;
                                break;
                            }
                        }

                        if(finish){
                            break;
                        }
                    }

                    bool cek;

                    move = _getch();
                    switch (move)
                    {
                    case 72:
                        // printf("UP\n");
                        map[curposI][curposJ] = '+';

                        curposI--;
                        if(markmap[curposI][curposJ] == 2){
                            curposI++;
                            continue;
                        }

                        cek = checkbomb(curposI, curposJ);
                        if(cek == false){
                            CLEAR;
                            printmap(cek);
                            printf("\nGAME OVER\n");

                            int esc;
                            while(1){
                                printf("\nPRESS ESC TO BACK\n");

                                while(1){
                                    esc = _getch();

                                    if(esc == 27){
                                        done = 1;
                                        break;;
                                    }
                                }

                                if(esc == 27){
                                    break;
                                }
                            }
                            break;
                        }

                        map[curposI][curposJ] = '^';
                        CLEAR;
                        printmap(cek);
                        break;
                    
                    case 80:
                        // printf("DOWN\n");
                        map[curposI][curposJ] = '+';

                        curposI++;
                        if(markmap[curposI][curposJ] == 2){
                            curposI--;
                            continue;
                        }
                        
                        cek = checkbomb(curposI, curposJ);
                        if(cek == false){
                            CLEAR;
                            printmap(cek);
                            printf("\nGAME OVER\n");

                            int esc;
                            while(1){
                                printf("\nPRESS ESC TO BACK\n");

                                while(1){
                                    esc = _getch();

                                    if(esc == 27){
                                        done = 1;
                                        break;;
                                    }
                                }

                                if(esc == 27){
                                    break;
                                }
                            }
                            break;
                        }

                        map[curposI][curposJ] = 'v';
                        CLEAR;
                        printmap(cek);
                        break;
                    
                    case 75:
                        // printf("LEFT\n");
                        map[curposI][curposJ] = '+';

                        curposJ--;
                        if(markmap[curposI][curposJ] == 2){
                            curposJ++;
                            continue;
                        }

                        cek = checkbomb(curposI, curposJ);
                        if(cek == false){
                            CLEAR;
                            printmap(cek);
                            printf("\nGAME OVER\n");

                            int esc;
                            while(1){
                                printf("\nPRESS ESC TO BACK\n");

                                while(1){
                                    esc = _getch();

                                    if(esc == 27){
                                        done = 1;
                                        break;;
                                    }
                                }

                                if(esc == 27){
                                    break;
                                }
                            }
                            break;
                        }

                        map[curposI][curposJ] = '<';
                        CLEAR;
                        printmap(cek);
                        break;
                    
                    case 77:
                        // printf("RIGHT\n");
                        map[curposI][curposJ] = '+';

                        curposJ++;
                        if(markmap[curposI][curposJ] == 2){
                            curposJ--;
                            continue;
                        }
                        
                        cek = checkbomb(curposI, curposJ);
                        if(cek == false){
                            CLEAR;
                            printmap(cek);
                            printf("\nGAME OVER\n");
                            
                            int esc;
                            while(1){
                                printf("\nPRESS ESC TO BACK\n");

                                while(1){
                                    esc = _getch();

                                    if(esc == 27){
                                        done = 1;
                                        break;;
                                    }
                                }

                                if(esc == 27){
                                    break;
                                }
                            }
                            break;
                        }
                        
                        map[curposI][curposJ] = '>';
                        CLEAR;
                        printmap(cek);
                        break;
                    
                    case 27:
                        printf("ESC\n");
                        done = true;
                        break;
                    }

                    if(done == true){
                        break;
                    }
                }
            }
            //impossible level
            else{
                //reset the map and the marking array
                memset(map, '.', sizeof(map));
                memset(markmap, 0, sizeof(markmap));
                
                //generate the map
                generatemap();
                CLEAR;
                printmap(false);

                int done = 0;
                int move;

                bool cek;
                int curposI = 18;
                int curposJ = 1;
                while(1){
                    if(curposI == 1 && curposJ == 18){
                        CLEAR;
                        printmap(true);
                        printf("\nCONGRATS YOU WON!!!\n");

                        int esc;
                        while(1){
                        printf("\nPRESS ESC TO BACK\n");

                            while(1){
                                esc = _getch();

                                if(esc == 27){
                                    done = 1;
                                    break;
                                }
                            }

                            if(esc == 27){
                                break;
                            }
                        }
                        
                    }

                    move = _getch();

                    switch (move)
                    {
                    case 72:
                        // printf("UP\n");
                        map[curposI][curposJ] = '+';
                        curposI--;
                        if(markmap[curposI][curposJ] == 2){
                            curposI++;
                            continue;
                        }

                        cek = checkbomb(curposI, curposJ);
                        //if there is a bomb then game over
                        if(cek == false){
                            CLEAR;
                            printmap(cek);
                            printf("\nGAME OVER\n");

                            int esc;
                            while(1){
                                printf("\nPRESS ESC TO BACK\n");

                                while(1){
                                    esc = _getch();

                                    if(esc == 27){
                                        done = 1;
                                        break;
                                    }
                                }

                                if(esc == 27){
                                    break;
                                }
                            }
                        }

                        if(done == 0){
                            map[curposI][curposJ] = '^';
                            CLEAR;
                            printmap(cek);
                        }
                        
                        break;
                    
                    case 80:
                        // printf("DOWN\n");
                        map[curposI][curposJ] = '+';
                        curposI++;
                        if(markmap[curposI][curposJ] == 2){
                            curposI--;
                            continue;
                        }

                        cek = checkbomb(curposI, curposJ);
                        //if there is a bomb then game over
                        if(cek == false){
                            CLEAR;
                            printmap(cek);
                            printf("\nGAME OVER\n");

                            //press esc to exit the level
                            int esc;
                            while(1){
                                printf("\nPRESS ESC TO BACK\n");

                                while(1){
                                    esc = _getch();

                                    if(esc == 27){
                                        done = 1;
                                        break;
                                    }
                                }

                                if(esc == 27){
                                    break;
                                }
                            }
                        }

                        if(done == 0){
                            map[curposI][curposJ] = 'v';
                            CLEAR;
                            printmap(cek);
                        }

                        break;
                    
                    case 75:
                        // printf("LEFT\n");
                        map[curposI][curposJ] = '+';
                        curposJ--;
                        if(markmap[curposI][curposJ] == 2){
                            curposJ++;
                            continue;
                        }

                        cek = checkbomb(curposI, curposJ);
                        //if there is a bomb then game over
                        if(cek == false){
                            CLEAR;
                            printmap(cek);
                            printf("\nGAME OVER\n");

                            //press esc to exit the level
                            int esc;
                            while(1){
                                printf("\nPRESS ESC TO BACK\n");

                                while(1){
                                    esc = _getch();

                                    if(esc == 27){
                                        done = 1;
                                        break;
                                    }
                                }

                                if(esc == 27){
                                    break;
                                }
                            }
                        }

                        if(done == 0){
                            map[curposI][curposJ] = '<';
                            CLEAR;
                            printmap(cek);
                        }
                        
                        break;
                    
                    case 77:
                        // printf("RIGHT\n");
                        map[curposI][curposJ] = '+';
                        curposJ++;
                        if(markmap[curposI][curposJ] == 2){
                            curposJ--;
                            continue;
                        }

                        cek = checkbomb(curposI, curposJ);
                        //if there is a bomb then game over
                        if(cek == false){
                            CLEAR;
                            printmap(cek);
                            printf("\nGAME OVER\n");

                            //press esc to exit the level
                            int esc;
                            while(1){
                                printf("\nPRESS ESC TO BACK\n");

                                while(1){
                                    esc = _getch();

                                    if(esc == 27){
                                        done = 1;
                                        break;
                                    }
                                }

                                if(esc == 27){
                                    break;
                                }
                            }
                        }

                        if(done == 0){
                            map[curposI][curposJ] = '>';
                            CLEAR;
                            printmap(cek);
                        }
                        
                        break;
                    
                    case 27:
                        done = 1;
                        break;
                    }

                    //break from the loop
                    if(done == 1){
                        break;
                    }
                }
            }
            break;

        case 27:
            CLEAR;
            return 0;
            break;
        }
    }
}