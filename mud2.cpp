// mud2.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include "mud.h"

#define MSG_DELAY 600
#define SCREEN_ROW 25
#define SCREEN_COL 52
#define BATTLE_CONTINUE 0
#define BATTLE_WIN 1
#define BATTLE_LOSE -1


#define CHARACTER_ROW 4

int INIT_MSG_ROOT = 7;
int LAST_MSG_ROW = 7;
int Battle_Result = BATTLE_CONTINUE;


typedef struct CHARACTER
{
	char Title[32];
	int Atk;
	int Def;
	int Hp;
	int Hp_MAX;
	int Fame;
	int Exp;
	int Cur_pos_x;
	int Cur_pos_y;
	int Next_pos_x;
	int Shape;
	int COLOR;
} CHARACTER;

void showPrologue();

void showTitle();

CHARACTER* SetPlayer();

CHARACTER* SetMob();

void ShowMap(char*);

void drawInfo(CHARACTER, CHARACTER, bool*);

void ATTACK(CHARACTER*, CHARACTER*, bool, bool, int*);

void GetLoot(CHARACTER*, CHARACTER*);

void MOVE_CHAR(CHARACTER*, CHARACTER*);

void Update_Info(CHARACTER*, CHARACTER*, bool);

void CLEAR_MSG(int*);



/*
게임 세팅()
{
	if (세이브 파일 있음?)
		세이브 데이터 로드();
	

	플레이어 세팅;
	적 세팅;
	맵 불러오기;
	->게임 진행()
}

게임 진입()
{
	맵 표시;
	캐릭터 초기 위치 표시;
	기본 UI 표시;
	->게임 진행;
}

게임 진행(전투결과 == 진행중)
{
	입력 대기;
	입력()
	{
		이동()
		{
			이동 목표 좌표 설정;
			if (적 충돌 ? )
			{
				if 예
				{
					현재 위치 유지;
				플레이어 공격 on;				
				}
				else
				{
					if (!왼쪽 끝 || !오른쪽 끝)
					{
						현재 위치 유지;
					}
				}
			}
		}

		몹 이동 처리()
		{
			이동 목표 좌표 설정;
			if (플레이어 충돌 ? )
			{
				if 예
				{
					현재 위치 유지;
				몹 공격 on;
				}
			}
		}

	}

	->전투 결과 처리(return 전투 결과);
}

전투 결과 처리()
{
	if(플레이어 공격 on?)
		플레이어->몹 데미지 처리;

	if (몹 사망?)
	{
		경험치 증가;
		골드 증가;
		명성 증가;		
		return 전투결과 - 승리;
	}

	if(몹 공격 on?)
		몹->플레이어 데미지 처리;

	if (플레이어 사망 ? )
	{
		return 전투결과 - 패배;
	}

	return 전투 결과 - 진행;
}

엔딩()
{
	엔딩 메시지 로드();
	엔딩 메시지 출력();
	게임 종료;
}

*/

int main()
{
	//showPrologue();
	//showTitle();
	
	bool isAttack_PC = 0;
	bool isAttack_MOB = 0;
	bool isTown = 1;

	char mapname[64] = { "0" };

	CHARACTER *PC;
	CHARACTER *MOB;

	PC = (CHARACTER*)malloc(sizeof(CHARACTER));
	MOB = (CHARACTER*)malloc(sizeof(CHARACTER));
	
	//캐릭터 세팅
	PC = SetPlayer();
	
	//게임 진행();
	//메인 루프 안에
	//입력 대기 넣고
	//받은 입력을 현재 모드에 따라 분기 처리
	// 타운일 때 or 전투일 떄
	/*
	while(1)
	if
		타운일 때	
		{
		}
	else
		타운이 아닐떄
	*/
		if (isTown)
		{
			strcpy_s(mapname, sizeof(mapname), ".\\dat\\Town.map");
			ShowMap(mapname);
			CUR_MOVE(2, 12);
			printf("타운입니다. 추가 예정.\n");
		}
		else
		{
			//맵 이름 세팅
			strcpy_s(mapname, sizeof(mapname), ".\\dat\\Battle.map");

			//몬스터 세팅
			MOB = SetMob();

			//기본 화면 그리기;
			ShowMap(mapname);
			drawInfo(*PC, *MOB, &isTown);
			while (Battle_Result == BATTLE_CONTINUE)
			{

				CUR_MOVE(PC->Cur_pos_x, CHARACTER_ROW);
				//루프 초기화
				isAttack_PC = 0;
				isAttack_MOB = 0;

				Sleep(150);


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

				MOB->Next_pos_x--;

				if (MOB->Cur_pos_x - 1 == PC->Next_pos_x)
				{
					isAttack_MOB = 1;
					MOB->Next_pos_x = MOB->Cur_pos_x;
				}

				CLEAR_MSG(&LAST_MSG_ROW);
				ATTACK(&*PC, &*MOB, isAttack_PC, isAttack_MOB, &LAST_MSG_ROW);
				MOVE_CHAR(&*PC, &*MOB);
				Update_Info(&*PC, &*MOB, &isTown);
				CUR_MOVE(4, 16);
			}

		}
	
	//엔딩();

	//종료 처리
	free(PC);
	free(MOB);

	return 0;

}

void showPrologue()
{
	//화면을 지운다.
	//파일을 열고
	//출력한다.
	//파일을 닫는다.
	//입력 대기 (입력이 들어오면 종료)	


	system("cls");

	FILE *fpPrologue = NULL;
	char Buffer[512] = { "0" };

	//앞부분 보이기
	fopen_s(&fpPrologue, ".\\dat\\Prologue.dat", "r");

	if (fpPrologue == NULL)
		return;

	while (fgets(Buffer, sizeof(Buffer), fpPrologue))
	{
		static int cnt = 0;
		static int tmp = 0;

		printf("%s", Buffer);
		memset(Buffer, 0, sizeof(Buffer));

		if (_kbhit())
			break;

		Sleep(MSG_DELAY);
		cnt++;

		if (cnt > SCREEN_ROW - 2)
		{
			printf("\n\t\t\t\t아무 키나 누르세요.");
			tmp = _getch();
			if (tmp)
			{
				system("cls");
				fflush(stdin);
				cnt = 0;
			}
		}
	}
	putchar('\n');

	printf("아무 키나 누르면 시작합니다.");
	int tmp = _getch();
	if (tmp)
	{
		system("cls");
		fflush(stdin);
	}
	fclose(fpPrologue);
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

	int tmp = _getch();
	if (tmp)
	{
		system("cls");
		fflush(stdin);
	}
	fclose(fpTitle);
}

CHARACTER* SetPlayer()
{
	CHARACTER *p = (CHARACTER*)malloc(sizeof(CHARACTER));
	FILE *fpStat = NULL;

	//앞부분 보이기
	fopen_s(&fpStat, ".\\dat\\status.pc", "r");

	//특수문자라서 char형의 2배를 준다.		
	if (fpStat == NULL)
		printf(".\\dat\\Battle.map 파일을 읽을 수 없습니다");

	fscanf_s(fpStat, "%s", p->Title, sizeof(p->Title));
	fscanf_s(fpStat, "%d %d %d %d %d %d %d %d %d %d %d",
		&p->Atk, &p->Def, &p->Hp, &p->Hp_MAX, &p->Fame, &p->Exp, &p->Cur_pos_x, &p->Cur_pos_y, &p->Next_pos_x, &p->Shape, &p->COLOR);

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
	fscanf_s(fpStat_m, "%d %d %d %d %d %d %d %d %d %d %d",
		&p->Atk, &p->Def, &p->Hp, &p->Hp_MAX, &p->Fame, &p->Exp, &p->Cur_pos_x, &p->Cur_pos_y, &p->Next_pos_x, &p->Shape, &p->COLOR);

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


	while (fgets(Buffer, sizeof(Buffer), fpMap))
	{
		printf("%s", Buffer);
		memset(Buffer, 0, sizeof(Buffer));
	}
	putchar('\n');

	fclose(fpMap);

}

void drawInfo(CHARACTER PC, CHARACTER MOB, bool* isTown)
{

	if (isTown)
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
			printf("%s", "■");

		CUR_MOVE(28, 3);
		SetTxtColor(LIGHTRED);
		for (int i = 0; i < 10; i++)
			printf("%s", "■");

		//모습 그리기
		CUR_MOVE(5, 4);
		SetTxtColor(PC.COLOR);
		printf("%c", PC.Shape);
		CUR_MOVE(46, 4);

		SetTxtColor(MOB.COLOR);
		printf("%c", MOB.Shape);
	}
	else
		printf("타운용 기본 정보 그리기\n");


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

	
	//PC 공격 처리	
	if (isAttack_PC)
	{
		CUR_MOVE(2, *ROWS);
		SetTxtColor(PC->COLOR);
		printf("%s", PC->Title);
		SetTxtColor(DEFAULT_COLOR);
		printf("의 공격!");
		LAST_MSG_ROW++;
		Sleep(150);
		CUR_MOVE(2, *ROWS);
		SetTxtColor(MOB->COLOR);
		printf("%s", MOB->Title);
		SetTxtColor(DEFAULT_COLOR);
		printf("에게");
		SetTxtColor(DMG_COLOR);
		printf("%d", PC->Atk - MOB->Def);
		SetTxtColor(DEFAULT_COLOR);
		printf("의 피해!");
		LAST_MSG_ROW++;

		MOB->Hp -= PC->Atk - MOB->Def;

		Sleep(150);
		if (MOB->Hp <= 0)
		{
			CUR_MOVE(2, *ROWS);
			SetTxtColor(MOB->COLOR);
			printf("%s", MOB->Title);
			SetTxtColor(DEFAULT_COLOR);
			printf(" 처치!");
			LAST_MSG_ROW++;
			Sleep(150);
			CUR_MOVE(2, *ROWS);
			SetTxtColor(YELLOW);
			printf("%d", MOB->Fame);
			SetTxtColor(DEFAULT_COLOR);
			printf("명성 획득");
			PC->Fame += MOB->Fame;
			LAST_MSG_ROW++;
			Sleep(150);
			CUR_MOVE(2, *ROWS);
			SetTxtColor(SKYBLUE);
			printf("%d", MOB->Exp);
			PC->Exp += MOB->Exp;
			SetTxtColor(DEFAULT_COLOR);
			printf("경험치 획득");

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
		printf("의 공격!");
		LAST_MSG_ROW++;
		Sleep(150);
		CUR_MOVE(2, *ROWS);
		SetTxtColor(PC->COLOR);
		printf("%s", PC->Title);
		SetTxtColor(DEFAULT_COLOR);
		printf("에게");
		SetTxtColor(DMG_COLOR);
		printf("%d", MOB->Atk - PC->Def);
		SetTxtColor(DEFAULT_COLOR);
		printf("의 피해!");
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
	

	LAST_MSG_ROW = INIT_MSG_ROOT;

	//Update_Info();

}

void CLEAR_MSG(int* Rows)
{

	for (int i = INIT_MSG_ROOT; i <= 13; i++)
	{
		CUR_MOVE(2, i);	
		printf("                                                ");		
	}
}

void GetLoot(CHARACTER* PC, CHARACTER* MOB)
{
}

void MOVE_CHAR(CHARACTER* PC, CHARACTER *MOB)
{
	//PC가 살아있으면 움직인다.
	if (PC->Hp >= 1)
	{
		CUR_MOVE(PC->Cur_pos_x, CHARACTER_ROW);
		SetTxtColor(0);
		CUR_MOVE(PC->Cur_pos_x, CHARACTER_ROW);
		printf(" ");
		PC->Cur_pos_x = PC->Next_pos_x;
		CUR_MOVE(PC->Cur_pos_x, CHARACTER_ROW);
		SetTxtColor(PC->COLOR);
		printf("%c", PC->Shape);
		SetTxtColor(DEFAULT_COLOR);
	}
	else
	{
		CUR_MOVE(PC->Cur_pos_x, CHARACTER_ROW);
		SetTxtColor(0);
		CUR_MOVE(PC->Cur_pos_x, CHARACTER_ROW);
		printf(" ");
		PC->Cur_pos_x = PC->Next_pos_x;
		CUR_MOVE(PC->Cur_pos_x, CHARACTER_ROW);
		SetTxtColor(PC->COLOR);
		printf("*");	//시체
		SetTxtColor(DEFAULT_COLOR);
	}

	//몹이 살아있으면 움직인다.
	if (MOB->Hp >= 0)
	{
		CUR_MOVE(MOB->Cur_pos_x, CHARACTER_ROW);
		SetTxtColor(0);
		CUR_MOVE(MOB->Cur_pos_x, CHARACTER_ROW);
		printf(" ");
		MOB->Cur_pos_x = MOB->Next_pos_x;
		CUR_MOVE(MOB->Cur_pos_x, CHARACTER_ROW);
		SetTxtColor(MOB->COLOR);
		printf("%c", MOB->Shape);
		SetTxtColor(DEFAULT_COLOR);
	}
	else
	{
		CUR_MOVE(MOB->Cur_pos_x, CHARACTER_ROW);
		SetTxtColor(0);
		CUR_MOVE(MOB->Cur_pos_x, CHARACTER_ROW);
		printf(" ");
		MOB->Cur_pos_x = MOB->Next_pos_x;
		CUR_MOVE(MOB->Cur_pos_x, CHARACTER_ROW);
		SetTxtColor(MOB->COLOR);
		printf("*");	//시체
		SetTxtColor(DEFAULT_COLOR);
	}
}

void Update_Info(CHARACTER* PC, CHARACTER* MOB, bool isTown)
{

	//HP 계산
	if (!isTown)
	{
		
	}
	else
	{
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
				printf("□");
		else
			for (int pc_loss = 0; pc_loss < 10 - PC_HP_ON; pc_loss++)
				printf("□");

		SetTxtColor(LIGHTRED);
		for (int pc_cur = 0; pc_cur < PC_HP_ON; pc_cur++)
			printf("■");


		CUR_MOVE(28, 3);

		SetTxtColor(LIGHTRED);
		for (int mob_cur = 0; mob_cur < MOB_HP_ON; mob_cur++)
			printf("■");

		SetTxtColor(RED);
		if (MOB->Hp <= 0)
			for (int mob_loss = 0; mob_loss < 10; mob_loss++)
				printf("□");
		else
			for (int mob_loss = 0; mob_loss < 10 - MOB_HP_ON; mob_loss++)
				printf("□");

		CUR_MOVE(PC->Cur_pos_x, PC->Cur_pos_y);
	}
	
}