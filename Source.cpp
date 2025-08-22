
#include <iostream>
#include <string>
#include <math.h>
#include <ncurses.h>
using namespace std;


int nScreenWidth = 120;
int nScreenHeight = 40;

float fPlayerX = 8.0f;
float fPlayerY = 8.0f;
float fPlayerA = 0.0f;

int nMapHeight = 16;
int nMapWidth = 16;

float fFOV = 3.1459/4.0;

float fDepth = 16.0f;

int main(){

    wstring map;

    map += L"################";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"################";


    //Game loop 

    initscr();
    refresh();
    WINDOW* screen = newwin(nScreenHeight, nScreenWidth, 0, 0);

    while(1){

        
        //handle keyboard inputs 
        if (getch() == 97){
            fPlayerA -= (0.1f);
        }

        if (getch() == 100){
            fPlayerA += (0.1f);
        }


        for (int x = 0; x < nScreenWidth; x++){
            //For each column, calc the projected ray angle into world space
            float fRayAngle = (fPlayerA-fFOV/2.0f) + ((float)x/(float)nScreenWidth * fFOV);
            
            float fDistanceToWall = 0.0f;
            bool bHitWall = false;

            float fEyeX = sinf(fRayAngle); //unit vector for ray in player space
            float fEyeY = cosf(fRayAngle);

            while(!bHitWall && fDistanceToWall < fDepth){
            
                fDistanceToWall += 0.1f;
                
                int nTestX = (int)(fPlayerX + fEyeX * fDistanceToWall);
                int nTestY = (int)(fPlayerY + fEyeY * fDistanceToWall);


                // test to see if out of depth
                if( nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight){
                    bHitWall = true; //set to max depth
                    fDistanceToWall = fDepth;
                }
                else {
                    if(map[nTestY*nMapWidth+nTestY] == '#'){
                        bHitWall = true;
                    }

                }
            }
            //Calculate the distance from the ceiling and floor, potter algor
            int nCeiling = (float)(nScreenHeight/2.0) - nScreenHeight/ ((float)fDistanceToWall);
            int nFloor = nScreenHeight - nCeiling;

            for (int y = 0; y< nScreenHeight; y++){

                if (y <nCeiling ){
                    move(y,x);
                    wprintw(screen," ");
                    wrefresh(screen);
                }
                else if (y > nCeiling && y <= nFloor){
                    mvwprintw(screen,y,x,"#");
                    wrefresh(screen);
                }
                else{
                    mvwprintw(screen,y,x," ");
                    wrefresh(screen);
                    
                }

            }

        }
    }


    int c = getch();
    endwin();

    return 0;
}
