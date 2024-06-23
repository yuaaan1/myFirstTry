#include<stdio.h>
#include<easyx.h>
int main() 
{
	initgraph(1200, 800);


	setbkcolor(WHITE);//background
	cleardevice();//clearout


	setlinestyle(PS_SOLID, 5);//drawline style
	setfillcolor(BLACK);
	setlinecolor(BLACK);
	circle(50, 50, 100);
	fillcircle(200, 200, 100);
	solidcircle(350, 350, 100);
	rectangle(800, 20, 1180, 780);


	settextcolor(BLACK);
	//settextstyle(50,0,"楷体")；//高度，自适应宽度，楷体
	setbkmode(TRANSPARENT);//文字背景透明
	//outtextxy(50, 400, L"Welcome to McDonald! ");//L means direct string

	getchar();
	return 0;
}