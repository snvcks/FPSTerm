
#include <charconv>
#include <chrono>
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

const char closetWall = (char)61;
const char closerWall = (char)47;
const char closeWall = (char)45;
const char wall= (char)35;

const char ground = 'M';
const char closetGround = 'x';
const char closeGround = '.';
const char farGround = '-';
const char furthestGround = ' ';


int main(){

    wstring map;

    map += L"################";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#....#.........#";
    map += L"#...#..........#";
    map += L"#..............#";
    map += L"#........####..#";
    map += L"#..............#";
    map += L"#...#.....#....#";
    map += L"#...#.....#....#";
    map += L"#...#..........#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"################";

    

    //Game loop 

    initscr();
    noecho();
    refresh();
    WINDOW* screen = newwin(nScreenHeight, nScreenWidth, 0, 0);
    
    int board = (int)'0';

    wborder(screen,board,board,board,board,board,board,board,board);
    auto tp1 = chrono::system_clock::now();
    auto tp2 = chrono::system_clock::now();

    while(1){

        tp2 = chrono::system_clock::now();
        chrono::duration<float>elapsedTime =tp2 -tp1;
        tp1 = tp2;
        float fElapsedTime = elapsedTime.count();

        
        //handle keyboard inputs 
        if (getch() == (int)'a'){
            fPlayerA -= (0.1f) * fElapsedTime;
        }
        if (getch() == (int)'d'){
            fPlayerA += (0.1f)* fElapsedTime;
        }

        if(getch() == (int)'w'){
            fPlayerX += sinf(fPlayerA) * 5.0f * fElapsedTime;
            fPlayerY += cosf(fPlayerA) * 5.0f * fElapsedTime;
        }

        if(getch() == (int)'s'){
            fPlayerX -= sinf(fPlayerA) * 5.0f * fElapsedTime;
            fPlayerY -= cosf(fPlayerA) * 5.0f * fElapsedTime;
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

            const char* nShade = " ";

            if(fDistanceToWall <= fDepth /4.0f) nShade = &wall;
            else if (fDistanceToWall < fDepth/3.0f) nShade = &closetWall;
            else if (fDistanceToWall < fDepth /2.0f) nShade = &closerWall;
            else if (fDistanceToWall < fDepth ) nShade = &closeWall;
            else nShade = " ";



            for (int y = 0; y< nScreenHeight; y++){

                const char* groundShade = " ";
                float b = 1.0f - (((float)y - nScreenHeight/2.0f) / ((float)nScreenHeight/2.0f));
                if( b < 0.25f) groundShade = &ground;
                else if(b <0.5) groundShade =&closetGround;
                else if (b < 0.75) groundShade = &closeGround;
                else if (b <0.9) groundShade = &farGround;
                else groundShade = &furthestGround;
                if (y <nCeiling ){
                    mvwprintw(screen,y,x," ");
                    wrefresh(screen);
                    refresh();
                }
                else if (y > nCeiling && y <= nFloor){
                    mvwprintw(screen,y,x,nShade);
                    refresh();
                    wrefresh(screen);
                }
                else{
                    mvwprintw(screen,y,x,groundShade);
                    wrefresh(screen);
                    refresh();
                    
                }

            }

        }
    }


    int c = getch();
    endwin();

    return 0;
}
