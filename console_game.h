#pragma once
#include <Windows.h>

enum colors {
	BLACK=0,
	BLUE,
	GREEN,
	SKYBLUE,
	RED,
	DMG_COLOR = 4,
	PURPLE,
	YELLOW,
	WHITE,
	DEFAULT_COLOR= 7,
	GRAY,
	LIGHTBLUE,
	LIGHTGREEN,
	LIGHTSKYBLUE,
	LIGHTRED,
	LIGHTPURPLE,
	LIGHTYELLOW,
	LIGHTWHITE,	
};

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


typedef struct struct_player{
	char name[32];
	char title[32];
	int Atk;
	int Def;
	int HP;
	int HP_MAX; 
	char name[32];
	char title[32];
	int fame;
	int exp;
	int shape;
	int color;
	int curPOSX;
	int curPOSY;
	int nextPOSX;
	int nextPOSY;
} PC_CHARACTER;

typedef struct struct_monster {
	char name[32];
	char title[32];
	int Atk;
	int Def;
	int HP;
	int HP_MAX;
	int fame;
	int exp;
	int shape;
	int color;
	int curPOSX;
	int curPOSY;
	int nextPOSX;
	int nextPOSY;
} MOB_CHARACTER;