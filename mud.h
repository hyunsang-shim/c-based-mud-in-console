#pragma once
#include <Windows.h>

#define BLACK 0
#define BLUE 1
#define GREEN 2
#define SKYBLUE 3
#define RED 4
#define DMG_COLOR 4
#define PURPLE 5 
#define YELLOW 6
#define WHITE 7
#define GRAY 8
#define LIGHTBLUE 9
#define LIGHTGREEN 10
#define LIGHTSKYBLUE 11
#define LIGHTRED 12
#define LIGHTPURPLE 13
#define LIGHTYELLOW 14
#define LIGHTWHITE 15
#define DEFAULT_COLOR 7

void SetTxtColor(int color_number)

{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_number);

}

void CUR_MOVE(int x, int y)
{
	COORD Pos;
	Pos.X = x;
	Pos.Y = y;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}


