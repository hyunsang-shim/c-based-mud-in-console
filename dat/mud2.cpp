// mud2.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include "console_game.h"

bool isSaveFileLoaded = false;
bool showPrologue = false;
FILE *fpSaveFile = 0;

// methods
bool Initialize();
void Setup_GameWindow();
void LoadSaveFile();
bool initPlayerData(OUT PC_CHARACTER *statPlayer);

void main()
{
	// initialize game
	Initialize();

}


bool Initialize()
{
	// set game console window
	Setup_GameWindow();

	// check save file
	// if save file exists load it and delete save file and set isSaveFileLoaded on


	// if none, set showPrologue flag on 


}

void Setup_GameWindow()
{

}

void LoadSaveFile()
{

	fopen_s(&fpSaveFile, "./mud.sav", "r");

	if (fpSaveFile == NULL)
	{
		isSaveFileLoaded = false;
		showPrologue = true;
		bool InitPlayerData();

	}
	else
	{
		isSaveFileLoaded = true;
		showPrologue = false;
	}
}

bool initPlayerData(OUT PC_CHARACTER *p)
{
	p = (PC_CHARACTER*)malloc(sizeof(PC_CHARACTER));
	FILE *fpStatus = 0;

	fopen_s(&fpStatus, "status.pc", "r");

	if (fpStatus == 0)
		printf("Cannot find target file!");


	


	fclose(fpStatus);

	return p;
}

void SaveFile(IN PC_CHARACTER* player, IN MOB_CHARACTER* mob, IN TOWN* town)
{
	FILE* SaveData = 0;

	fopen_s(&SaveData, "1waylife.sav", "wb");

	fprintf(SaveData, "%s %s %d %d %d %d %s %s %d %d %d %d %d %d %d %d",
		player->name, player->title, player->Atk, player->Def, player->HP,
		player->HP_MAX, player->fame, player->exp, player->shape, player->color,
		player->curPOSX, player->curPOSY, player->nextPOSX, player->nextPOSY);

}