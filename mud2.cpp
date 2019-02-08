// mud2.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include "mud.h"

const int MSG_DELAY = 600;
const int SCREEN_ROW = 17;
const int SCREEN_COL = 52;
const int BATTLE_CONTINUE = 0;
const int BATTLE_WIN = 1;
const int BATTLE_LOSE = -1;
const int PC_HP_MAX = 999;
const int PC_ATK_MAX = 999;
const int MSG_ROOT_BATTLE = 7;
const int MSG_ROOT_TOWN = 13;
const int MSG_ROOT_MAX = 14;

int LAST_MSG_ROW = MSG_ROOT_BATTLE;
int Battle_Result = BATTLE_CONTINUE;
bool isVisible = true;



void CLEAR_MSG(bool isTown)
{
	if (isTown == false)
		for (int i = MSG_ROOT_BATTLE; i <= MSG_ROOT_MAX; i++)
		{
			CUR_MOVE(2, i);
			printf("                                                ");
		}
	else
		for (int i = MSG_ROOT_TOWN; i <= MSG_ROOT_MAX; i++)
		{
			CUR_MOVE(2, i);
			printf("                                                ");
		}
}

void SHOW_CUR(bool isVisible)
{
	HANDLE console_cur;
	CONSOLE_CURSOR_INFO cursor;
	console_cur = GetStdHandle(STD_OUTPUT_HANDLE);

	cursor.bVisible = isVisible;
	cursor.dwSize = 3;

	SetConsoleCursorInfo(console_cur, &cursor);
}

void showTitle()
{
	system("cls");
	fseek(stdin, 0, SEEK_END);

	FILE *fpTitle = NULL;
	char Buffer[512] = { "0" };

	//앞부분 보이기
	fopen_s(&fpTitle, ".\\dat\\Title.dat", "r");

	if (fpTitle == NULL)
		return;

	while (fgets(Buffer, sizeof(Buffer), fpTitle))
	{
		printf("%s", Buffer);
		memset(Buffer, 0, sizeof(Buffer));
	}
	putchar('\n');


	printf("\t\t\t\tPress Any Key");
	int tmp = _getch();
	fseek(stdin, 0, SEEK_END);
	fclose(fpTitle);
}

void showPrologue()
{

	FILE *fpPrologue = NULL;
	char Buffer[512] = { "0" };

	//앞부분 보이기
	fopen_s(&fpPrologue, ".\\dat\\Prologue.dat", "r");

	if (fpPrologue == NULL)
		return;

	system("cls");
	SHOW_CUR(false);
	while (fgets(Buffer, sizeof(Buffer), fpPrologue))
	{
		static int cnt = 0;
		static int tmp = 0;

		printf("%s", Buffer);
		memset(Buffer, 0, sizeof(Buffer));

		Sleep(MSG_DELAY);
		cnt++;

		if (cnt > SCREEN_ROW - 2)
		{
			printf("\n\t\t\t\tPress Any Key");
			tmp = _getch();
			if (tmp)
			{
				//system("cls");
				fflush(stdin);
				cnt = 0;
			}
		}
	}
	putchar('\n');

	printf("\t\t\t\tPress Any Key");
	int tmp = _getch();
	fseek(stdin, 0, SEEK_END);

	fclose(fpPrologue);
}

CHARACTER* SetPlayer()
{
	CHARACTER *p = (CHARACTER*)malloc(sizeof(CHARACTER));
	FILE *fpStat = NULL;

	fopen_s(&fpStat, ".\\dat\\mud.sav", "r");

	if (fpStat == NULL)
		fopen_s(&fpStat, ".\\dat\\status.pc", "r");

	if (fpStat == NULL)
		printf("파일을 읽을 수 없습니다");

	fscanf_s(fpStat, "%s", p->Title, sizeof(p->Title));
	fscanf_s(fpStat, "%d %d %d %d %d %d %d %d %d %d %d %d",
		&p->isTown, &p->Atk, &p->Def, &p->Hp, &p->Hp_MAX, &p->Fame, &p->Exp, &p->Cur_pos_x, &p->Cur_pos_y, &p->Next_pos_x, &p->Shape, &p->COLOR);

	fclose(fpStat);

	return p;
}

CHARACTER* SetMob()
{
	CHARACTER *p = (CHARACTER*)malloc(sizeof(CHARACTER));
	FILE *fpStat_m = NULL;

	//앞부분 보이기
	fopen_s(&fpStat_m, ".\\dat\\status.mob", "r");

	if (fpStat_m == NULL)
		printf(".\\dat\\Battle.map 파일을 읽을 수 없습니다");

	fscanf_s(fpStat_m, "%s", p->Title, sizeof(p->Title));
	fscanf_s(fpStat_m, "%d %d %d %d %d %d %d %d %d %d %d %d",
		&p->isTown, &p->Atk, &p->Def, &p->Hp, &p->Hp_MAX, &p->Fame, &p->Exp, &p->Cur_pos_x, &p->Cur_pos_y, &p->Next_pos_x, &p->Shape, &p->COLOR);

	fclose(fpStat_m);

	return p;
}

void ShowMap(char* Map)
{
	system("cls");
	fseek(stdin, 0, SEEK_END);

	FILE *fpMap = NULL;
	char Buffer[512] = { "0" };

	//맵을 읽어온다.
	fopen_s(&fpMap, Map, "r");

	if (fpMap == NULL)
		return;

	SetTxtColor(DEFAULT_COLOR);
	while (fgets(Buffer, sizeof(Buffer), fpMap))
	{
		printf("%s", Buffer);
		memset(Buffer, 0, sizeof(Buffer));
	}
	putchar('\n');

	fclose(fpMap);

}

void drawInfo(CHARACTER PC, CHARACTER MOB, bool isTown)
{

	if (isTown == false)
	{
		//이름 표시
		CUR_MOVE(24 - strlen(PC.Title), 1);
		SetTxtColor(LIGHTWHITE);
		printf("%s", PC.Title);
		SetTxtColor(WHITE);
		CUR_MOVE(28, 1);
		printf("%s", MOB.Title);

		//HP 그리기
		CUR_MOVE(4, 3);
		SetTxtColor(LIGHTRED);
		for (int i = 0; i < 10; i++)
			printf("%c", 219);

		CUR_MOVE(28, 3);
		SetTxtColor(LIGHTRED);
		for (int i = 0; i < 10; i++)
			printf("%c", 219);
	}
	else
	{
		//캐릭터 정보
		CUR_MOVE(10, 7);
		printf("%s", PC.Title);
		CUR_MOVE(10, 8);
		printf("%d", PC.Atk);
		CUR_MOVE(10, 9);
		printf("%d", PC.Def);
		CUR_MOVE(10, 10);
		printf("%3d / %3d", PC.Hp, PC.Hp_MAX);
		CUR_MOVE(10, 11);
		printf("%d", PC.Fame);
		CUR_MOVE(10, 12);
		printf("%d", PC.Exp);
	}


	/*
	printf("%s\n공 %d\n방 %d\nHP %d\n명성 %d\n현재X %d\n현재 Y%d\n다음 X%d\n표시 %c\n",
	PC.Title, PC.Atk, PC.Def, PC.Hp,
	PC.Fame, PC.Cur_pos_x, PC.Cur_pos_y,
	PC.Next_pos_x, PC.Shape);


	printf("%s\n공 %d\n방 %d\nHP %d\n명성 %d\n현재X %d\n현재 Y%d\n다음 X%d\n표시 %c\n",
	MOB.Title, MOB.Atk, MOB.Def, MOB.Hp,
	MOB.Fame, MOB.Cur_pos_x, MOB.Cur_pos_y,
	MOB.Next_pos_x, MOB.Shape);
	*/
}

void ATTACK(CHARACTER* PC, CHARACTER* MOB, bool isAttack_PC, bool isAttack_MOB, int* ROWS)
{

	CLEAR_MSG(false);
	//PC 공격 처리	
	if (isAttack_PC)
	{
		CUR_MOVE(2, *ROWS);
		SetTxtColor(PC->COLOR);
		printf("%s", PC->Title);
		SetTxtColor(DEFAULT_COLOR);
		printf("'s Attack!");
		LAST_MSG_ROW++;
		Sleep(150);
		CUR_MOVE(2, *ROWS);
		SetTxtColor(MOB->COLOR);
		printf("%s", MOB->Title);
		SetTxtColor(DEFAULT_COLOR);
		printf("got ");
		SetTxtColor(DMG_COLOR);
		printf("%d ", PC->Atk - MOB->Def);
		SetTxtColor(DEFAULT_COLOR);
		printf("Damage!");
		LAST_MSG_ROW++;

		MOB->Hp -= PC->Atk - MOB->Def;

		Sleep(150);
		if (MOB->Hp <= 0)
		{
			CUR_MOVE(2, *ROWS);
			SetTxtColor(DEFAULT_COLOR);
			printf("You Killed "); 
			SetTxtColor(MOB->COLOR);
			printf("%s!", MOB->Title);
			LAST_MSG_ROW++;

			Sleep(250);
			CUR_MOVE(2, *ROWS);
			printf("                                                ");
			CUR_MOVE(2, *ROWS);
			SetTxtColor(YELLOW);
			printf("%d", MOB->Fame);
			SetTxtColor(DEFAULT_COLOR);
			printf("Fame Gained.");
			PC->Fame += MOB->Fame;
			LAST_MSG_ROW++;

			Sleep(250);
			CUR_MOVE(2, *ROWS);
			printf("                                                ");
			CUR_MOVE(2, *ROWS);
			SetTxtColor(SKYBLUE);
			printf("%d", MOB->Exp);
			PC->Exp += MOB->Exp;
			SetTxtColor(DEFAULT_COLOR);
			printf("Xp Gained.");
			Sleep(250);
			LAST_MSG_ROW++;

			CUR_MOVE(2, *ROWS);
			printf("                                                ");
			CUR_MOVE(2, *ROWS);
			SetTxtColor(PC->COLOR);
			printf("You : *Pew* What a day");
			Sleep(MSG_DELAY);
			LAST_MSG_ROW++;

			CUR_MOVE(2, *ROWS);
			printf("       Time to go home.");
			LAST_MSG_ROW++;
			Sleep(MSG_DELAY);
			SetTxtColor(DEFAULT_COLOR);
			Battle_Result = BATTLE_WIN;
		}
	}

	//MOB 공격 처리
	if (Battle_Result == BATTLE_CONTINUE && isAttack_MOB)
	{
		CUR_MOVE(2, *ROWS);
		SetTxtColor(MOB->COLOR);
		printf("%s", MOB->Title);
		SetTxtColor(DEFAULT_COLOR);
		printf("'s Attack!");
		LAST_MSG_ROW++;
		Sleep(150);
		CUR_MOVE(2, *ROWS);
		SetTxtColor(PC->COLOR);
		printf("%s", PC->Title);
		SetTxtColor(DEFAULT_COLOR);
		printf("got");
		SetTxtColor(DMG_COLOR);
		printf("%d", MOB->Atk - PC->Def);
		SetTxtColor(DEFAULT_COLOR);
		printf(" Damage!");
		LAST_MSG_ROW++;
		PC->Hp -= MOB->Atk - PC->Def;

		Sleep(150);
		if (PC->Hp <= 0)
		{
			CUR_MOVE(2, *ROWS);
			SetTxtColor(RED);
			printf("YOU DIED!");
			Battle_Result = BATTLE_LOSE;
		}
	}
	LAST_MSG_ROW = MSG_ROOT_BATTLE;
}

void MOVE_CHAR(CHARACTER* PC)
{
	CUR_MOVE(PC->Cur_pos_x, PC->Cur_pos_y);
	SetTxtColor(0);
	CUR_MOVE(PC->Cur_pos_x, PC->Cur_pos_y);
	printf(" ");
	PC->Cur_pos_x = PC->Next_pos_x;
	CUR_MOVE(PC->Cur_pos_x, PC->Cur_pos_y);
	SetTxtColor(PC->COLOR);
	printf("%c", PC->Shape);
	SetTxtColor(DEFAULT_COLOR);
	CUR_MOVE(PC->Cur_pos_x, PC->Cur_pos_y);

}

void MOVE_CHAR(CHARACTER* PC, CHARACTER *MOB)
{
	//PC가 살아있으면 움직인다.
	if (PC->Hp >= 1)
	{
		CUR_MOVE(PC->Cur_pos_x, PC->Cur_pos_y);
		SetTxtColor(0);
		CUR_MOVE(PC->Cur_pos_x, PC->Cur_pos_y);
		printf(" ");
		PC->Cur_pos_x = PC->Next_pos_x;
		CUR_MOVE(PC->Cur_pos_x, PC->Cur_pos_y);
		SetTxtColor(PC->COLOR);
		printf("%c", PC->Shape);
		SetTxtColor(DEFAULT_COLOR);
	}
	else
	{
		CUR_MOVE(PC->Cur_pos_x, PC->Cur_pos_y);
		SetTxtColor(0);
		CUR_MOVE(PC->Cur_pos_x, PC->Cur_pos_y);
		printf(" ");
		PC->Cur_pos_x = PC->Next_pos_x;
		CUR_MOVE(PC->Cur_pos_x, PC->Cur_pos_y);
		SetTxtColor(PC->COLOR);
		printf("*");	//시체
		SetTxtColor(DEFAULT_COLOR);
	}

	//몹이 살아있으면 움직인다.
	if (MOB->Hp >= 0)
	{
		CUR_MOVE(MOB->Cur_pos_x, PC->Cur_pos_y);
		SetTxtColor(0);
		CUR_MOVE(MOB->Cur_pos_x, PC->Cur_pos_y);
		printf(" ");
		MOB->Cur_pos_x = MOB->Next_pos_x;
		CUR_MOVE(MOB->Cur_pos_x, PC->Cur_pos_y);
		SetTxtColor(MOB->COLOR);
		printf("%c", MOB->Shape);
		SetTxtColor(DEFAULT_COLOR);
	}
	else
	{
		CUR_MOVE(MOB->Cur_pos_x, PC->Cur_pos_y);
		SetTxtColor(0);
		CUR_MOVE(MOB->Cur_pos_x, PC->Cur_pos_y);
		printf(" ");
		MOB->Cur_pos_x = MOB->Next_pos_x;
		CUR_MOVE(MOB->Cur_pos_x, PC->Cur_pos_y);
		SetTxtColor(MOB->COLOR);
		printf("*");	//시체
		SetTxtColor(DEFAULT_COLOR);
	}

	CUR_MOVE(PC->Cur_pos_x, PC->Cur_pos_y);
}

void Shop_Menu(CHARACTER *PC, CHARACTER *MOB, int idx, bool *isTown)
{
	int cost_HP = PC->Hp_MAX;
	int cost_ATK = PC->Atk;
	char select = 0;

	switch (idx)
	{
	case 0:	//교회
		if (PC->Hp == PC->Hp_MAX)
		{
			CLEAR_MSG(true);
			CUR_MOVE(2, 13);
			SetTxtColor(LIGHTWHITE);
			printf("Priest : May the God Bless you.");
		}
		else
		{
			CLEAR_MSG(true);
			CUR_MOVE(2, 13);
			SetTxtColor(LIGHTWHITE);
			printf("Priest : God, I beg your Mercy on this pitty...");

			SHOW_CUR(false);
			for (int i = PC->Hp; i <= PC->Hp_MAX; i++)
			{
				CUR_MOVE(10, 10);
				if (i % 2 == 1)
					SetTxtColor(GREEN);
				else
					SetTxtColor(LIGHTGREEN);

				if (i == PC->Hp_MAX)
					SetTxtColor(DEFAULT_COLOR);

				PC->Hp = i;
				printf("%3d", i);

				//HP가 채워지면서 점점 느려지게
				static double spd = 0;
				spd = (double)((double)PC->Hp * 100 / (double)PC->Hp_MAX / 20);
				spd < 100 ? spd = 50 : spd = spd;
				Sleep(spd);
			}

			CUR_MOVE(2, 14);
			SetTxtColor(DEFAULT_COLOR);
			printf("You've Fully Healed");
			
			SHOW_CUR(true);

		}
		break;

	case 1:	//체육관
		if (PC->Hp_MAX < PC_HP_MAX)
		{
			CLEAR_MSG(true);
			CUR_MOVE(2, 13);
			SetTxtColor(LIGHTPURPLE);
			printf("Trainer(%c) : Wow! Come to get Shaped, Boy%c?", 11, 11);
			Sleep(150);
			CUR_MOVE(2, 14);
			printf("Max HP +1 For %d XP (Y / N)", PC->Hp_MAX);

			select = _getch();

			switch (select)
			{
			case 'y':
			case 'Y':
				if (PC->Exp < cost_HP)
				{
					CLEAR_MSG(true);
					CUR_MOVE(2, 13);
					SetTxtColor(LIGHTPURPLE);
					printf("Trainer(%c) : You need more Exprience, Boy%c", 11, 11);

					for (int i = 0; i < 3; i++)
					{
						CUR_MOVE(10, 12);
						if (i % 2 == 0)
							SetTxtColor(LIGHTRED);
						else
							SetTxtColor(RED);

						if (i == 2)
							SetTxtColor(DEFAULT_COLOR);

						printf("%d", PC->Exp);
						Sleep(250);
					}

				}
				else
				{
					CLEAR_MSG(true);
					SetTxtColor(LIGHTPURPLE);
					CUR_MOVE(2, 13);
					printf("Trainer(%c) : You're just My type, Boy%c", 11, 3);
					CUR_MOVE(10, 12);
					SHOW_CUR(false);
					for (int i = 0; i < cost_HP; i++)
					{
						if (PC->Exp % 2 == 1)
							SetTxtColor(RED);
						else
							SetTxtColor(LIGHTRED);

						if (i <= 1)
							SetTxtColor(DEFAULT_COLOR);

						CUR_MOVE(10, 12);
						static double spd = 0;
						spd = ((double)cost_HP / (double)PC->Exp);
						spd < 200 ? spd = 150 : spd = spd;
						Sleep(spd);
						printf("       ");
						CUR_MOVE(10, 12);
						printf("%d", PC->Exp - i);
					}
					SetTxtColor(DEFAULT_COLOR);
					PC->Exp -= cost_HP;
					CUR_MOVE(10, 12);
					printf("       ");
					CUR_MOVE(10, 12);
					printf("%d", PC->Exp);
					PC->Hp_MAX++;
					PC->Hp++;

					for (int i = 0; i <= 5; i++)
					{
						CUR_MOVE(16, 10);
						if (i % 2 == 1)
							SetTxtColor(GREEN);
						else
							SetTxtColor(LIGHTGREEN);

						if (i == 5)
							SetTxtColor(DEFAULT_COLOR);

						printf("%3d", PC->Hp_MAX);

						//HP가 채워지면서 점점 느려지게
						static double spd = 0;
						spd = ((double)i * 100 / (double)cost_HP);
						spd < 150 ? spd = 150 : spd = spd;
						Sleep(spd);
					}
					SHOW_CUR(true);

				}
				break;

			case 'n':
			case 'N':
			{
				CLEAR_MSG(true);
				CUR_MOVE(2, 13);
				SetTxtColor(LIGHTPURPLE);
				printf("트레이너(♂) : 그것 참 아쉽네...Boy♂ (츄릅) ");
			}
			break;
			}
		}
		else
		{
			CLEAR_MSG(true);
			CUR_MOVE(2, 13);
			SetTxtColor(LIGHTPURPLE);
			printf("트레이너(♂) : 우흥♡ 좋은 근육이야♡ 완벽해♥");
			SetTxtColor(DEFAULT_COLOR);
		}
		break;
	case 2:	//대장간
	{
		CLEAR_MSG(true);
		CUR_MOVE(2, 13);
		SetTxtColor(BLUE);
		printf("대장장이 : 준비중 일세! 나중에 다시 오게나!");
		SetTxtColor(DEFAULT_COLOR);
	}
	break;

	case 3:	//의뢰소	
	{
		CLEAR_MSG(true);
		CUR_MOVE(2, 13);
		SetTxtColor(YELLOW);
		printf("접수원 : 지금은 쥐떼 퇴치 의뢰밖에 없네요.");
		CUR_MOVE(2, 14);
		printf("         그럼, 잘 부탁드려요.");
		SetTxtColor(DEFAULT_COLOR);

		//몬스터 선택 (미구현)

		//대상 정보 세팅			
		MOB = SetMob();

		//대상 정보 세팅
		CUR_MOVE(28, 7);
		printf("TARGET");
		CUR_MOVE(35, 8);
		printf("%s", MOB->Title);
		CUR_MOVE(28, 9);
		printf("A T K :");
		CUR_MOVE(35, 9);
		printf("%d", MOB->Atk);
		CUR_MOVE(28, 10);
		printf("D E F :");
		CUR_MOVE(35, 10);
		printf("%d", MOB->Def);
		CUR_MOVE(28, 11);
		printf("H   P :");
		CUR_MOVE(35, 11);
		printf("%d", MOB->Hp_MAX);
	}
	break;
	}
}

void Update_Info(CHARACTER* PC, CHARACTER* MOB)
{

	//HP 계산
	double MOB_cur = MOB->Hp;
	double MOB_max = MOB->Hp_MAX;

	int PC_HP_ON = (double)((double)PC->Hp / (double)PC->Hp_MAX * 100 / 10);
	int MOB_HP_ON = (double)((double)MOB->Hp / (double)MOB->Hp_MAX * 100 / 10);

	if (PC_HP_ON % 10 > 0)
		PC_HP_ON++;

	if (PC_HP_ON == 0 && PC->Hp >= 1)
		PC_HP_ON = 1;

	if (MOB_HP_ON % 10 > 0)
		MOB_HP_ON++;

	if (MOB_HP_ON == 0 && MOB->Hp >= 1)
		MOB_HP_ON = 1;



	//디버깅용
#ifdef DEBUG

	CUR_MOVE(1, 17);
	printf("                                                      ");
	CUR_MOVE(1, 17);
	printf("PC remain : %d, %d칸\nMOB remain: %d, %d칸", PC->Hp, PC_HP_ON, MOB->Hp, MOB_HP_ON);

#endif DEBUG	


	//HP 그리기
	CUR_MOVE(4, 3);

	SetTxtColor(RED);
	if (PC->Hp <= 0)
		for (int pc_loss = 0; pc_loss < 10; pc_loss++)
			printf("%c", 176);
	else
		for (int pc_loss = 0; pc_loss < 10 - PC_HP_ON; pc_loss++)
			printf("%c", 176);

	SetTxtColor(LIGHTRED);
	for (int pc_cur = 0; pc_cur < PC_HP_ON; pc_cur++)
		printf("%c", 219);


	CUR_MOVE(28, 3);

	SetTxtColor(LIGHTRED);
	for (int mob_cur = 0; mob_cur < MOB_HP_ON; mob_cur++)
		printf("%c", 219);

	SetTxtColor(RED);
	if (MOB->Hp <= 0)
		for (int mob_loss = 0; mob_loss < 10; mob_loss++)
			printf("%c", 176);
	else
		for (int mob_loss = 0; mob_loss < 10 - MOB_HP_ON; mob_loss++)
			printf("%c", 176);

	CUR_MOVE(PC->Cur_pos_x, PC->Cur_pos_y);
}

void GAME_OVER()
{
	system("cls");
	int tmp;
	CUR_MOVE(15, 8);
	SetTxtColor(LIGHTRED);
	printf("Y O U   D I E D !!");
	Sleep(300);
	CUR_MOVE(15, 12);
	printf("Quitting Game\n");
	tmp = _getch();
	fseek(stdin, 0, SEEK_END);

	_fcloseall();

}

int main()
{
	bool isAttack_PC = 0;
	bool isAttack_MOB = 0;
	bool isTown = 1;

	// 화면 크기를 세팅 (<windows.h> 필수)
	system("mode con cols=54 lines=17");
	// 타이틀바 제목 변경
	system("title 외길인생 v.0.8");
	SetTxtColor(WHITE);

	showTitle();

	FILE *fpSave = NULL;
	char mapname[64] = { "0" };

	//세이브 파일이 있는지 검사
	fopen_s(&fpSave, ".\\dat\\mud.sav", "r");

	if (fpSave == NULL)
	{
		showPrologue();
	}

	CHARACTER *PC;
	CHARACTER *MOB;

	PC = (CHARACTER*)malloc(sizeof(CHARACTER));
	MOB = (CHARACTER*)malloc(sizeof(CHARACTER));

	PC->Atk = -999;

	//캐릭터 세팅
	PC = SetPlayer();
	MOB = SetMob();

	if (PC->Atk == -999)
	{
		system("cls");
		printf("파일이 잘못되었습니다. 게임을 종료합니다.");
		return 0;
	}

	while (1)
	{
		if (isTown)
		{
			strcpy_s(mapname, sizeof(mapname), ".\\dat\\Town.map");
			ShowMap(mapname);
			drawInfo(*PC, *MOB, true);


			//플레이어 기본 위치 세팅

			MOVE_CHAR(&*PC);

			while (isTown)
			{

				Sleep(100);
				CUR_MOVE(PC->Cur_pos_x, PC->Cur_pos_y);

				static int input = 0;

				fflush(stdin);

				input = _getch();

				if (input == 224 || input == 0)
				{

					input = _getch();
					switch (input)
					{
					case 72:	//상.
						if (PC->Cur_pos_x >= 9 && PC->Cur_pos_x <= 12)	//교회
							Shop_Menu(&*PC, &*MOB, 0, &isTown);
						else if (PC->Cur_pos_x >= 17 && PC->Cur_pos_x <= 22)	//체육관
							Shop_Menu(&*PC, &*MOB, 1, &isTown);
						else if (PC->Cur_pos_x >= 27 && PC->Cur_pos_x <= 32)	//대장간
							Shop_Menu(&*PC, &*MOB, 2, &isTown);
						else if (PC->Cur_pos_x >= 37 && PC->Cur_pos_x <= 44)	//의뢰소
							Shop_Menu(&*PC, &*MOB, 3, &isTown);
						break;
					case 75:	//좌
						if (PC->Next_pos_x > 3)
						{
							PC->Next_pos_x -= 1;
						}
						else
							PC->Next_pos_x = 3;
#ifdef DEBUG

						CUR_MOVE(8, 17);
						printf("x = %d(next : %d), y = %d, isTown %d", PC->Cur_pos_x, PC->Next_pos_x, PC->Cur_pos_y, isTown);
#endif DEBUG

						CLEAR_MSG(true);
						MOVE_CHAR(&*PC);
						CUR_MOVE(PC->Cur_pos_x, PC->Cur_pos_y);
						break;
					case 77:	//우
						if (PC->Next_pos_x == (SCREEN_COL - 4))
						{
							isTown = false;
							PC->Cur_pos_x = 5;
							PC->Next_pos_x = 5;
							Battle_Result = BATTLE_CONTINUE;
#ifdef DEBUG

							CUR_MOVE(8, 17);
							printf("x = %d(next : %d), y = %d, isTown %d", PC->Cur_pos_x, PC->Next_pos_x, PC->Cur_pos_y, isTown);
#endif DEBUG
							break;
						}
						else if (PC->Next_pos_x < (SCREEN_COL - 3))
						{
							PC->Next_pos_x += 1;
							CLEAR_MSG(true);
#ifdef DEBUG
							CUR_MOVE(8, 17);
							printf("x = %d(next : %d), y = %d, isTown %d", PC->Cur_pos_x, PC->Next_pos_x, PC->Cur_pos_y, isTown);
#endif DEBUG
							MOVE_CHAR(&*PC);
							CUR_MOVE(PC->Cur_pos_x, PC->Cur_pos_y);
						}
						else
						{
							PC->Next_pos_x = SCREEN_COL - 3;
#ifdef DEBUG
							CUR_MOVE(8, 17);
							printf("x = %d(next : %d), y = %d, isTown %d", PC->Cur_pos_x, PC->Next_pos_x, PC->Cur_pos_y, isTown);
#endif DEBUG
							MOVE_CHAR(&*PC);
							CUR_MOVE(PC->Cur_pos_x, PC->Cur_pos_y);
						}
						break;
					default:
						break;
					}
				}
				else
				{
					switch (input)
					{
					case 'S':	//저장하고 종료
						break;
					case 'Q':	//그냥 종료
						break;
					}
				}

			}
		}
		else if (Battle_Result == BATTLE_CONTINUE)
		{
			isTown = false;
			//맵 이름 세팅
			strcpy_s(mapname, sizeof(mapname), ".\\dat\\Battle.map");

			//몬스터 세팅
			MOB = SetMob();

			//기본 화면 그리기;
			system("cls");
			ShowMap(mapname);
			drawInfo(*PC, *MOB, false);
			CLEAR_MSG(false);
			MOVE_CHAR(&*PC, &*MOB);
			Update_Info(&*PC, &*MOB);
			while (Battle_Result == BATTLE_CONTINUE)
			{
				CUR_MOVE(PC->Cur_pos_x, PC->Cur_pos_y);
				//루프 초기화
				isAttack_PC = 0;
				isAttack_MOB = 0;

				Sleep(100);


				static int input = 0;

				fflush(stdin);

				input = _getch();

				if (input == 224 || input == 0)
				{

					input = _getch();
					switch (input)
					{
					case 72:	//상.
						break;
					case 75:	//좌
						if (PC->Next_pos_x > 1)
						{
							PC->Next_pos_x -= 1;
						}
						else
							PC->Next_pos_x = 1;
						break;
					case 77:	//우
						if (PC->Next_pos_x < SCREEN_COL - 2)
						{
							PC->Next_pos_x += 1;
							if (PC->Next_pos_x == MOB->Cur_pos_x)
							{
								isAttack_PC = 1;
								PC->Next_pos_x = PC->Cur_pos_x;
							}
						}
						else
							PC->Next_pos_x = SCREEN_COL - 2;
						break;
					default:
						break;
					}
				}
				else
				{
					switch (input)
					{
					case 'S':	//저장하고 종료
						break;
					case 'Q':	//그냥 종료
						break;
					case 'R':	//현재 화면 다시 그리기
						break;
					}
				}


				if (Battle_Result == BATTLE_WIN)
				{
					isTown = true;
					break;
				}

				if (Battle_Result == BATTLE_LOSE)
				{
					GAME_OVER();
				}

				MOB->Next_pos_x--;

				if (MOB->Cur_pos_x - 1 == PC->Next_pos_x)
				{
					isAttack_MOB = 1;
					MOB->Next_pos_x = MOB->Cur_pos_x;
				}

				CLEAR_MSG(&isTown);
				ATTACK(&*PC, &*MOB, isAttack_PC, isAttack_MOB, &LAST_MSG_ROW);
				MOVE_CHAR(&*PC, &*MOB);
				Update_Info(&*PC, &*MOB);
				//CUR_MOVE(4, 16);
			}

		}
		else if (Battle_Result == BATTLE_LOSE)
		{
			GAME_OVER();
			break;
		}
		else if (Battle_Result == BATTLE_WIN)
		{
			isTown = true;
			PC->Cur_pos_x = 3;
			PC->Next_pos_x = 3;

		}
	}
	//엔딩();

	//종료 처리
	free(PC);
	free(MOB);

	return 0;

}



