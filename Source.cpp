#include <algorithm>    // std::sort
#include <charconv>
#include <chrono>
#include <iostream>
#include <string>
#include <math.h>
#include <ncurses.h>
#include <utility>
#include <vector>
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
const char boundry = ' ';

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
            fPlayerA -= (0.3f) * fElapsedTime;
        }
        if (getch() == (int)'d'){
            fPlayerA += (0.3f)* fElapsedTime;
        }

        if(getch() == (int)'w'){
            fPlayerX += sinf(fPlayerA) * 7.3f * fElapsedTime;
            fPlayerY += cosf(fPlayerA) * 7.3f * fElapsedTime;

            if (((int) fPlayerY*nMapWidth + (int)fPlayerX) && map[(int)fPlayerY*nMapWidth + (int)fPlayerX] == '#'){
                fPlayerX -= sinf(fPlayerA) * 7.3f * fElapsedTime;
                fPlayerY -= cosf(fPlayerA) * 7.3f * fElapsedTime;
            } 
        }

        if(getch() == (int)'s'){
            fPlayerX -= sinf(fPlayerA) * 7.3f * fElapsedTime;
            fPlayerY -= cosf(fPlayerA) * 7.3f * fElapsedTime;
            if (((int) fPlayerY*nMapWidth + (int)fPlayerX) &&map[(int)fPlayerY*nMapWidth + (int)fPlayerX] == '#'){
                fPlayerX += sinf(fPlayerA) * 7.3f * fElapsedTime;
                fPlayerY += cosf(fPlayerA) * 7.3f * fElapsedTime;
            } 
        }


        for (int x = 0; x < nScreenWidth; x++){
            //For each column, calc the projected ray angle into world space
            float fRayAngle = (fPlayerA-fFOV/2.0f) + ((float)x/(float)nScreenWidth * fFOV);
            
            float fDistanceToWall = 0.0f;
            bool bHitWall = false;
            bool bBoundry =false;

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

                        vector<pair<float,float>> p; //distance, dot 

                        for (int tx = 0; tx <2;tx++){
                            for( int ty= 0; ty< 2; ty++){
                                float vy = (float)nTestY + ty - fPlayerY;

                                float vx = (float)nTestX + tx - fPlayerX;
                                float d = sqrt(vx*vx + vy*vy);
                                float dot = (fEyeX * vx/d) + (fEyeY*vy/d);
                                p.push_back(make_pair(d,dot));
                            }
                        }
                        sort(p.begin(),p.end(),[](const pair<float,float> &left ,const pair<float,float> &right){ return left.first < right.first;});

                        float fBound = 0.01;
                        if(acos(p.at(0).second) < fBound) bBoundry = true;
                        if(acos(p.at(1).second) < fBound) bBoundry = true;
                        if(acos(p.at(2).second) < fBound) bBoundry = true;
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
            else nShade = "`";

        
            if (bBoundry) nShade =&boundry; 

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
        move(0,0);
        clrtoeol();
        mvwprintw(screen, 0, 0, "X=%3.2f, Y=%3.2f, A=%3.2f, FPS=%3.2f ", fPlayerX,fPlayerY, fPlayerA, 1.0f/fElapsedTime);
    }


    int c = getch();
    endwin();

    return 0;
}
