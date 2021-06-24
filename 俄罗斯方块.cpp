#include "tetris.h"
#define gotoxyInPool(x, y) gotoxyWithFullwidth(x + 9, y - 3)
#include<windows.h>
#include<mmsystem.h>
#pragma comment(lib, "WINMM.LIB")
long topscore;
FILE* fp;
int good;
int main()
{
	good = 0;
	Manager manager;
	Control control;
	//initGame(&manager, &control);
	char ch;
	system("color 0f");
	srand(time(NULL));
	initGame(&manager, &control);
	if ((fp = fopen("data", "r+")) == NULL)
	{
		fp = fopen("data", "w+");
		fprintf(fp, "0");
	}
	fscanf(fp, "%ld", &topscore);
	do
	{
		if (manager.dead)
		{
			while (1)
			{
				SetConsoleTextAttribute(Output, 0xF0);
				gotoxyInPool(6, 13);
				printf("游戏结束");
				gotoxyInPool(4, 14);
				printf("按Y重来,按N退出");
				ch = getch();
				if (ch == 'n' || ch == 'N')return 0;
				if (ch == 'y' || ch == 'Y')break;
			}
			system("cls");
			system("color 0f");
			rewind(fp);
			fscanf(fp, "%ld", &topscore);
			manager.dead = false;
			initGame(&manager, &control);
		}
		printgame(&manager, &control);
		rungame(&manager, &control);
	} while (1);

	return 0;
}
void rungame(Manager* manager, Control* control)
{
	char ch;
	int now = 0, last = 0;
	long a = clock();
	static long b = 0;
	static char co = 6;
	static int l = 0, o = 0;
	while (!manager->dead)
	{
		a = l = now = clock();
		if (now - last > 1000 / (manager->leve + 1) && !control->pause)
		{
			PlaySound(TEXT("GameSceneBGM.wav"), 0, SND_FILENAME | SND_LOOP | SND_ASYNC | SND_NOSTOP);
			moveDownTetris(manager, control);
			last = clock();
		}
		while (kbhit())
		{
			ch = getch();
			keydownControl(manager, control, ch);
		}
		if (a - b > 1000)
		{
			b = clock();
			co = (co + 1) % 7 + 1;
			gotoxyWithFullwidth(25, 23);
			SetConsoleTextAttribute(Output, co);
			printf("□by : 卧龙浪士");
			gotoxyWithFullwidth(25, 20);
			SetConsoleTextAttribute(Output, 0xf);
			printf("当前游戏时长: %ldS", clock() / 1000);
		}
		if (l - o > 100)
		{
			o = clock();
			printPoolBorder();
		}

	}
}
void printgame(Manager* manager, Control* control)
{
	printScore(manager);//打印左侧的得分信息
	printPrompting();//打印右下角的提示信息
	printPoolBorder();//打印游戏池边界
	printsquare();           //打印方括号
	printNextTetris(manager);//打印下一个,下下一个方块
	setPoolColor(manager, control);//设置游戏池中当前方块所在位置的颜色，存入color数组中
	printTetrisPool(manager, control);//刷新游戏池
}
void  initGame(Manager* manager, Control* control)
{
	Output = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode;
	GetConsoleMode(Output, &mode);
	mode &= ~ENABLE_QUICK_EDIT_MODE;  //移除快速编辑模式
	mode &= ~ENABLE_INSERT_MODE;      //移除插入模式
	mode &= ~ENABLE_MOUSE_INPUT;
	SetConsoleMode(Output, mode);

	Output = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo = { 1,false };
	SetConsoleCursorInfo(Output, &cursorInfo);
	//光标隐藏
	//SetConsoleTitle("俄罗斯方块");

	CONSOLE_CURSOR_INFO cci;
	SMALL_RECT rc;
	rc.Left = 0;
	rc.Top = 0;
	rc.Right = 75;
	rc.Bottom = 24;
	COORD size = { rc.Right , rc.Bottom };
	SetConsoleScreenBufferSize(Output, size);
	SetConsoleWindowInfo(Output, TRUE, &rc);
	GetConsoleCursorInfo(Output, &cci);    //获取光标信息
	cci.dwSize = 1;    //设置光标大小
	cci.bVisible = 0; //设置光标不可见
	SetConsoleCursorInfo(Output, &cci);    //设置(应用)光标信息
	
	system("title 俄罗斯方块");
	//设置游戏窗口标题
	startGame(manager, control);
}
void startGame(Manager* manager, Control* control)
{
	memset(manager, 0, sizeof(Manager));
	memcpy(manager->pool, gs_TetrisPool, sizeof(unsigned int[28]));
	//游戏池初始化
	manager->type[1] = rand() % 7;
	manager->orientation[1] = rand() % 4;
	//初始化下一个方块的信息
	manager->type[2] = rand() % 7;
	manager->orientation[2] = rand() % 4;
	//初始化下下一个方块的信息
	memset(control, 0, sizeof(Control));
	//初始化控制信息，全部置0
	initTetris(manager);//初始给出一个方块
	manager->pow = 5;
}


void gotoxyWithFullwidth(short x, short y)
{//设置光标的位置
	static COORD cd;
	cd.X = x * 2;
	cd.Y = y;
	SetConsoleCursorPosition(Output, cd);
}
void printPrompting()
{
	int x = 26; int y = 10;
	SetConsoleTextAttribute(Output, 0xb);
	gotoxyWithFullwidth(x, y++);
	printf("■控制：");
	gotoxyWithFullwidth(x, y++);
	printf("□向左移动：← A 4");
	gotoxyWithFullwidth(x, y++);
	printf("□向右移动：→ D 6");
	gotoxyWithFullwidth(x, y++);
	printf("□向下移动：↓ S 2");
	gotoxyWithFullwidth(x, y++);
	printf("□顺时针转：↑ W 8");
	gotoxyWithFullwidth(x, y++);
	printf("□逆时针转：0");
	gotoxyWithFullwidth(x, y++);
	printf("□变形：R");
	gotoxyWithFullwidth(x, y++);
	printf("□直接落地：空格");
	gotoxyWithFullwidth(x, y++);
	printf("□暂停游戏：回车");
	
	gotoxyWithFullwidth(25, 23);
	SetConsoleTextAttribute(Output, 0xb);
	printf("□by : DD");
	gotoxyWithFullwidth(25, 20);
	SetConsoleTextAttribute(Output, 0xf);
	printf("当前游戏时长: %ldS", clock() / 1000);
}

void printPoolBorder()
{
	int y;
	int co;
	static int now = 0, last = -1000;
	static int goodtime;
	co = 0xf0;
	now = clock();
	if (good)
	{
		goodtime = good;
		good = 0;
		last = clock();
	}
	if (now - last < 1000 * goodtime)co += (rand() % 6 + 1) | 8;
	SetConsoleTextAttribute(Output, co);
	//设置文字颜色，前景色黑色，背景色高亮白
	for (y = 4; y < 26; y++)//前4行不显示，后2行为底边界
	{
		gotoxyWithFullwidth(10, y - 3);//打印位置10,1   10,2  ...
		printf("%2s", "☆");
		gotoxyWithFullwidth(23, y - 3);//打印右边界23,1   23,2  ...
		printf("%2s", "★");
	}
	gotoxyWithFullwidth(10, y - 3);//打印最后一行（底边界）
	printf("☆☆☆☆☆☆☆★★★★★★★");
}


void printScore(Manager* manager)
{
	int x = 2;
	int y = 6;
	int i;
	char ty[8] = "ITLJZSO";
	SetConsoleTextAttribute(Output, 0xE);
	gotoxyWithFullwidth(x, 1);
	printf("得分:%u", manager->score);
	gotoxyWithFullwidth(x, 2);
	printf("当前等级:%u", manager->leve + 1);
	gotoxyWithFullwidth(x, 3);
	printf("变形次数:%u", manager->pow);
	gotoxyWithFullwidth(x, 4);
	printf("历史最高分:%u", topscore);

	gotoxyWithFullwidth(2, 2);
	gotoxyWithFullwidth(x - 1, y++);
	printf("■消行总数:%u", manager->erasedTotal);
	for (i = 1, y++; i < 5; i++)
	{
		gotoxyWithFullwidth(x, y++);
		printf("□消%u:%u", i, manager->erasedCount[i - 1]);
	}
	y += 2;
	gotoxyWithFullwidth(x - 1, y++);
	printf("■方块总数:%u", manager->tetrisTotal);
	for (i = 0; i < 7; i++)
	{
		gotoxyWithFullwidth(x, y++);
		printf("□形状%c:%u", ty[i], manager->tetrisCount[i]);
	}
}
void printsquare()
{
	int x, y, i, a;
	x = 26;
	y = 1;
	a = 5;
	gotoxyWithFullwidth(x, y++);
	SetConsoleTextAttribute(Output, 0xc);
	printf("%s", "┏━━━━━━━━━┳━━━━━━━━━┓\n");
	for (i = 0; i < 4; i++)
	{
		gotoxyWithFullwidth(x, y);
		printf("┃");
		x += a;
		gotoxyWithFullwidth(x, y);
		printf("┃");
		x += a;
		gotoxyWithFullwidth(x, y);
		printf("┃");
		x -= 2 * a;
		y++;
	}
	gotoxyWithFullwidth(x, y++);
	printf("%s", "┗━━━━━━━━━┻━━━━━━━━━┛\n"); 
}

void printNextTetris(Manager* manager)
{
	int x = 27;
	int y = 2;
	int i;
	int tetris;
	tetris = TetrisTable[manager->type[1]][manager->orientation[1]];///////////////
	SetConsoleTextAttribute(Output, manager->type[1] | 8);///////////////////////////////////
	for (i = 0; i < 16; i++)
	{
		gotoxyWithFullwidth((i & 3) + x, (i >> 2) + y);
		((tetris >> i) & 1) ? printf("■") : printf("  ");
	}
	x = 32;
	y = 2;
	tetris = TetrisTable[manager->type[2]][manager->orientation[2]];
	SetConsoleTextAttribute(Output, 8);
	for (i = 0; i < 16; i++)
	{
		gotoxyWithFullwidth((i & 3) + x, (i >> 2) + y);
		((tetris >> i) & 1) ? printf("■") : printf("  ");
	}
}
// =============================================================================
// 显示游戏池
void printTetrisPool(const Manager* manager, const Control* control) {
	int x, y;
	for (y = ROW_BEGIN; y < ROW_END; ++y)  // 不显示顶部4行和底部2行
	{
		gotoxyInPool(2, y);
		// 定点到游戏池中的方格
		for (x = COL_BEGIN; x < COL_END; ++x)  // 不显示左右边界
		{
			if ((manager->pool[y] >> x) & 1)  // 游戏池该方格有方块
			{
				// 用相应颜色，显示一个实心方块
				SetConsoleTextAttribute(Output, control->color[y][x]);
				//SetConsoleTextAttribute(Output, 0xf);
				printf("■");
			}
			else  // 没有方块，显示空白
			{
				SetConsoleTextAttribute(Output, 0);
				printf("%2s", "");
			}
		}
	}
}
//初始给出一个方块
void initTetris(Manager* manager)
{
	int i;
	//设置当前方块
	manager->type[0] = manager->type[1];
	manager->orientation[0] = manager->orientation[1];
	//更新下一个方块
	manager->type[1] = manager->type[2];
	manager->orientation[1] = manager->orientation[2];
	//更新下下一个方块
	manager->type[2] = rand() % 7;
	manager->orientation[2] = rand() % 4;
	manager->x = 6;
	//manager->y=0;
	manager->y = 0;/////////////////////////////
	i = 4;
	while (!((TetrisTable[manager->type[0]][manager->orientation[0]] >> (--i * 4)) & 0x000f))manager->y++;
	if (checkCollision(manager))//检查碰撞
	{
		manager->dead = true;//游戏结束
	}
	else
	{
		insertTetris(manager);//插入游戏池
	}
	++manager->tetrisTotal;
	//方块总数更新
	++manager->tetrisCount[manager->type[0]];
	//同类型方块数更新

	printScore(manager);//显示左侧的信息
	printNextTetris(manager);//显示下一个，下下一个方块
}

//插入方块
void insertTetris(Manager* manager)
{
	unsigned int tetris;
	tetris = TetrisTable[manager->type[0]][manager->orientation[0]];
	//取出当前方块的数据

	manager->pool[manager->y + 0] |= (((tetris >> 0x0) & 0x000F) << manager->x);
	manager->pool[manager->y + 1] |= (((tetris >> 0x4) & 0x000F) << manager->x);
	manager->pool[manager->y + 2] |= (((tetris >> 0x8) & 0x000F) << manager->x);
	manager->pool[manager->y + 3] |= (((tetris >> 0xC) & 0x000F) << manager->x);

}


//碰撞检测
bool checkCollision(Manager* manager) {
	int i;
	unsigned crr = TetrisTable[manager->type[0]][manager->orientation[0]];
	unsigned crt = ((manager->pool[manager->y + 0] >> manager->x) & 0x000f) << 0;
	crt |= ((manager->pool[manager->y + 1] >> manager->x) & 0x000f) << 4;
	crt |= ((manager->pool[manager->y + 2] >> manager->x) & 0x000f) << 8;
	crt |= ((manager->pool[manager->y + 3] >> manager->x) & 0x000f) << 12;
	if (crt & crr)return true;
	return false;
}
void  setPoolColor(const Manager* manager, Control* control)
{//设置游戏池中当前方块所在位置的颜色，存入color数组中
	int i, x, y;
	unsigned int tetris = TetrisTable[manager->type[0]][manager->orientation[0]];
	for (i = 0; i < 16; i++)
	{
		y = (i >> 2) + manager->y;
		x = (i & 3) + manager->x;
		if ((tetris >> i) & 1)
		{
			control->color[y][x] = (manager->type[0] | 8);
		}
	}
}
void printCurrentTetris(const Manager* manager, const Control* control)
{
	int x, y;
	y = (manager->y > ROW_BEGIN) ? (manager->y - 1) : ROW_BEGIN;
	//向上扩充一行
	for (; y < ROW_END && y < manager->y + 4; ++y)
	{
		x = (manager->x > COL_BEGIN) ? (manager->x - 1) : COL_BEGIN;
		//向左扩充一列
		for (; x < COL_END && x < manager->x + 5; x++)//向右扩充一列
		{
			gotoxyInPool(x, y);
			if ((manager->pool[y] >> x) & 1)
			{
				SetConsoleTextAttribute(Output, control->color[y][x]);
				printf("■");
			}
			else
			{
				SetConsoleTextAttribute(Output, 0);
				printf("%2s", "");
			}
		}
	}
}
void removeTetris(Manager* manager)
{  //移除当前方块
	unsigned int tetris;
	tetris = TetrisTable[manager->type[0]][manager->orientation[0]];
	manager->pool[manager->y + 0] &= ~(((tetris >> 0x0) & 0x000F) << manager->x);
	manager->pool[manager->y + 1] &= ~(((tetris >> 0x4) & 0x000F) << manager->x);
	manager->pool[manager->y + 2] &= ~(((tetris >> 0x8) & 0x000F) << manager->x);
	manager->pool[manager->y + 3] &= ~(((tetris >> 0xC) & 0x000F) << manager->x);
}

void moveDownTetris(Manager* manager, Control* control)
{
	int y = manager->y;
	//记录当前方块的位置
	removeTetris(manager);
	//擦除当前方块

	manager->y++;
	//移动的下一个位置

	if (checkCollision(manager))//检查碰撞
	{
		manager->y = y;//恢复原始位置
		insertTetris(manager);//重新插入当前方块
		if (checkErasing(manager, control))
		{
			printTetrisPool(manager, control);//刷新游戏池
		}
	}
	else
	{
		insertTetris(manager);//插入方块到下一个位置
		setPoolColor(manager, control);//设置当前方块位置的颜色
		printCurrentTetris(manager, control);//显示当前插入的方块
	}
}

bool checkErasing(Manager* manager, Control* control)
{
	static int breakscore = 0;
	static const unsigned scores[5] = { 0, 10, 30, 90, 150 };  // 消行得分
	static const unsigned getpow[5] = { 0, 1, 3, 9, 15 };
	int count = 0;
	int  y = manager->y + 3;
	do {
		if (y < ROW_END && manager->pool[y] == 0xFFFFU)
		{
			++count;
			memmove(manager->pool + 1, manager->pool, sizeof(unsigned int) * y);
			memmove(control->color[1], control->color[0], sizeof(int[16]) * y);
		}
		else {
			--y;
		}
	} while (y >= manager->y);//&& k < 4);
	manager->erasedTotal += count;  // 消行总数
	manager->score += scores[count];  // 得分
	manager->score++;
	manager->pow += getpow[count];
	manager->leve = manager->score / 100;       ////////分会升一级
	if (breakscore)
	{
		rewind(fp);
		fprintf(fp, "%ld", manager->score);
	}
	else if (topscore < manager->score)
	{
		rewind(fp);
		fprintf(fp, "%ld", manager->score);
		breakscore = 1;
		good = 5;
	}
	printScore(manager);

	if (count > 0) {
		PlaySound(TEXT("4.wav"), 0, SND_FILENAME | SND_ASYNC);
		good += count;
		++manager->erasedCount[count - 1];  // 消行
	}
	initTetris(manager);  // 初始化方块方块
	setPoolColor(manager, control);  // 设置颜色
	return (count > 0);
}
void horzMoveTetris(Manager* manager, Control* control)
{
	int x = manager->x;
	removeTetris(manager);
	manager->x += control->direction;//1表示向右,-1表示向左
	if (checkCollision(manager))
	{
		manager->x = x;
		insertTetris(manager);
	}
	else
	{
		insertTetris(manager);
		setPoolColor(manager, control);
		printCurrentTetris(manager, control);
	}
}
void dropDownTetris(Manager* manager, Control* control)
{
	removeTetris(manager);  // 移走当前方块
	for (; manager->y < ROW_END; ++manager->y)  // 从上往下
	{
		if (checkCollision(manager))  // 检测到碰撞
		{
			break;
		}
	}
	--manager->y;  // 上移一格当然没有碰撞

	insertTetris(manager);  // 放入当前方块
	setPoolColor(manager, control);  // 设置颜色
	checkErasing(manager, control);  // 检测消行
	printTetrisPool(manager, control);  // 显示游戏池
}
void rotateTetris(Manager* manager, Control* control)
{
	if (control->clockwise)
	{
		removeTetris(manager);
		manager->orientation[0] = (manager->orientation[0] + 1) & 3;
		if (checkCollision(manager))
		{
			manager->orientation[0] = (manager->orientation[0] + 3) & 3;
			insertTetris(manager);
		}
		else
		{
			insertTetris(manager);
			setPoolColor(manager, control);
			printCurrentTetris(manager, control);
		}
	}
	else
	{
		removeTetris(manager);
		manager->orientation[0] = (manager->orientation[0] + 3) & 3;
		if (checkCollision(manager))
		{
			manager->orientation[0] = (manager->orientation[0] + 1) & 3;
			insertTetris(manager);
		}
		else
		{
			insertTetris(manager);
			setPoolColor(manager, control);
			printCurrentTetris(manager, control);
		}
	}
}
void keydownControl(Manager* manager, Control* control, int key)
{
	int scandint;
	if (key == 13)  // 暂停/解除暂停
	{
		control->pause = !control->pause;
	}

	if (control->pause)  // 暂停状态，不作处理
	{
		return;//起到终止语句的作用
	}

	switch (key) {
	case 'w':
	case 'W':
	case '8':
	case 72:  // 上
		control->clockwise = true;  // 顺时针旋转
		rotateTetris(manager, control);  // 旋转方块
		break;
	case 'a':
	case 'A':
	case '4':
	case 75:  // 左
		control->direction = -1;  // 向左移动
		horzMoveTetris(manager, control);  // 水平移动方块
		break;
	case 'd':
	case 'D':
	case '6':
	case 77:  // 右
		control->direction = 1;  // 向右移动
		horzMoveTetris(manager, control);  // 水平移动方块
		break;
	case 's':
	case 'S':
	case '2':
	case 80:  // 下
		moveDownTetris(manager, control);  // 向下移动方块
		break;
	case ' ':  // 直接落地
		dropDownTetris(manager, control);
		break;
	case '0':  // 反转
		control->clockwise = false;  // 逆时针旋转
		rotateTetris(manager, control);  // 旋转方块
		break;
	case 53:
	case 'r':
		if (!manager->pow)break;//变形
		manager->pow--;
		removeTetris(manager);
		scandint = rand() % 6;
		if (manager->type[0] <= scandint)scandint++;//不重复变形
		manager->type[0] = scandint;
		insertTetris(manager);
		setPoolColor(manager, control);
		printCurrentTetris(manager, control);
		printScore(manager);//奖励次数重绘
		break;
	default:
		break;
	}
}
