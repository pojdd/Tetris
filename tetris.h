#ifndef TETRISS_H_INCLUDED
#define TETRISS_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <stdbool.h>

//游戏区域位置进行设置
#define COL_BEGIN 2
#define COL_END 14
#define ROW_BEGIN 4
#define ROW_END 26

static const unsigned int TetrisTable[7][4] =
{
	{ 0x0072, 0x0262, 0x0270, 0x0232 },  // T型
	{ 0x00F0, 0x2222, 0x00F0, 0x2222 },  // I型
	{ 0x0223, 0x0074, 0x0622, 0x0170 },  // L型
	{ 0x0226, 0x0470, 0x0322, 0x0071 },  // J型
	{ 0x0063, 0x0264, 0x0063, 0x0264 },  // Z型
	{ 0x006C, 0x0462, 0x006C, 0x0462 },  // S型
	{ 0x0660, 0x0660, 0x0660, 0x0660 }   // O型
};

//初始状态的游戏池
static const unsigned int gs_TetrisPool[28] =
{
	0xC003,0xC003,0xC003,0xC003,0xC003,0xC003,
	0xC003,0xC003,0xC003,0xC003,0xC003,0xC003,
	0xC003,0xC003,0xC003,0xC003,0xC003,0xC003,
	0xC003,0xC003,0xC003,0xC003,0xC003,0xC003,
	0xC003,0xC003,0xFFFF,0xFFFF
};
//定义存储游戏相关数据的结构体
typedef struct TetrisManager {
	unsigned int pool[28];
	//游戏池一共28行
	int x;
	int y;
	//当前方块的坐标
	int type[3];
	//当前、下一个、下下一个方块的形状
	int orientation[3];
	//当前、下一个、下下一个方块的旋转状态
	unsigned score;
	unsigned erasedCount[4];
	unsigned erasedTotal;
	unsigned tetrisCount[7];
	unsigned tetrisTotal;
	bool dead;
	int leve;
	unsigned pow;

}Manager;
//定义存储控制数据的结构体
typedef struct TetrisControl {
	bool pause;//暂停
	bool clockwise;//旋转方向：顺时针为True
	int direction;//移动方向：0左移，1 右移
	int color[28][16];//游戏池每格的颜色
}Control;
HANDLE Output;//定义一个句柄
void  initGame(Manager* manager, Control* control);
//初始化游戏
void startGame(Manager* manager, Control* control);
//开始游戏
void printPrompting();
void gotoxyWithFullwidth(short x, short y);
//设置光标位置
void printPoolBorder();
//打印游戏池边界
void printScore(Manager* manager);
//打印左侧的分数
void printsquare();
//打印右上角的方括号
void printNextTetris(Manager* manager);
//打印下一个，下下一个方块
void printTetrisPool(const Manager* manager, const Control* control);
//刷新游戏池

void initTetris(Manager* manager);
//初始给出一个方块
void insertTetris(Manager* manger);
//插入方块
bool checkCollision(Manager* manager);
//碰撞检测

void  setPoolColor(const Manager* manager, Control* control);
//设置游戏池中方块所在位置的颜色
void printCurrentTetris(const Manager* manager, const Control* control);
//显示当前方块

void removeTetris(Manager* manager);
//移除当前方块
void moveDownTetris(Manager* manager, Control* control);
//方块下移
bool checkErasing(Manager* manager, Control* control);
void moveTetris(Manager* manager, Control* control);
void rotateTetris(Manager* manager, Control* control);
void keydownControl(Manager* manager, Control* control, int key);
void printgame(Manager* manager, Control* control);
void rungame(Manager* manager, Control* control);

#endif // TETRISS_H_INCLUDED
