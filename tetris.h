#ifndef TETRISS_H_INCLUDED
#define TETRISS_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <stdbool.h>

//��Ϸ����λ�ý�������
#define COL_BEGIN 2
#define COL_END 14
#define ROW_BEGIN 4
#define ROW_END 26

static const unsigned int TetrisTable[7][4] =
{
	{ 0x0072, 0x0262, 0x0270, 0x0232 },  // T��
	{ 0x00F0, 0x2222, 0x00F0, 0x2222 },  // I��
	{ 0x0223, 0x0074, 0x0622, 0x0170 },  // L��
	{ 0x0226, 0x0470, 0x0322, 0x0071 },  // J��
	{ 0x0063, 0x0264, 0x0063, 0x0264 },  // Z��
	{ 0x006C, 0x0462, 0x006C, 0x0462 },  // S��
	{ 0x0660, 0x0660, 0x0660, 0x0660 }   // O��
};

//��ʼ״̬����Ϸ��
static const unsigned int gs_TetrisPool[28] =
{
	0xC003,0xC003,0xC003,0xC003,0xC003,0xC003,
	0xC003,0xC003,0xC003,0xC003,0xC003,0xC003,
	0xC003,0xC003,0xC003,0xC003,0xC003,0xC003,
	0xC003,0xC003,0xC003,0xC003,0xC003,0xC003,
	0xC003,0xC003,0xFFFF,0xFFFF
};
//����洢��Ϸ������ݵĽṹ��
typedef struct TetrisManager {
	unsigned int pool[28];
	//��Ϸ��һ��28��
	int x;
	int y;
	//��ǰ���������
	int type[3];
	//��ǰ����һ��������һ���������״
	int orientation[3];
	//��ǰ����һ��������һ���������ת״̬
	unsigned score;
	unsigned erasedCount[4];
	unsigned erasedTotal;
	unsigned tetrisCount[7];
	unsigned tetrisTotal;
	bool dead;
	int leve;
	unsigned pow;

}Manager;
//����洢�������ݵĽṹ��
typedef struct TetrisControl {
	bool pause;//��ͣ
	bool clockwise;//��ת����˳ʱ��ΪTrue
	int direction;//�ƶ�����0���ƣ�1 ����
	int color[28][16];//��Ϸ��ÿ�����ɫ
}Control;
HANDLE Output;//����һ�����
void  initGame(Manager* manager, Control* control);
//��ʼ����Ϸ
void startGame(Manager* manager, Control* control);
//��ʼ��Ϸ
void printPrompting();
void gotoxyWithFullwidth(short x, short y);
//���ù��λ��
void printPoolBorder();
//��ӡ��Ϸ�ر߽�
void printScore(Manager* manager);
//��ӡ���ķ���
void printsquare();
//��ӡ���Ͻǵķ�����
void printNextTetris(Manager* manager);
//��ӡ��һ��������һ������
void printTetrisPool(const Manager* manager, const Control* control);
//ˢ����Ϸ��

void initTetris(Manager* manager);
//��ʼ����һ������
void insertTetris(Manager* manger);
//���뷽��
bool checkCollision(Manager* manager);
//��ײ���

void  setPoolColor(const Manager* manager, Control* control);
//������Ϸ���з�������λ�õ���ɫ
void printCurrentTetris(const Manager* manager, const Control* control);
//��ʾ��ǰ����

void removeTetris(Manager* manager);
//�Ƴ���ǰ����
void moveDownTetris(Manager* manager, Control* control);
//��������
bool checkErasing(Manager* manager, Control* control);
void moveTetris(Manager* manager, Control* control);
void rotateTetris(Manager* manager, Control* control);
void keydownControl(Manager* manager, Control* control, int key);
void printgame(Manager* manager, Control* control);
void rungame(Manager* manager, Control* control);

#endif // TETRISS_H_INCLUDED
