//马迪轩 李卓远 杨昌昇  大一下期末作业
#undef UNICODE
#pragma comment(lib, "winmm")

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>
#include<graphics.h>
#include<easyx.h>
#include<conio.h>
#include<process.h>
#include <mmsystem.h>

struct single {
	char name[50];//单品名
	int time;//制作时间
	int progress;//制作进度
	int max;//最大存量
	int storage;//当前存量
};
typedef struct single SINGLE;

struct combo {
	char name[50];//套餐名
	int sID[5];//套餐内单品的id
};
typedef struct combo COMBO;

struct order {
	char name[50];//订单内容，即单品或套餐名
	int singleState[5];//单品完成情况，若为-1为不存在或已完成，若为单品ID则为未完成
	int orderTime[3];//下单时间
	int state;//订单状态  0未接单 1未完成 2已完成 3拒绝接单 
	int oID;//订单编号
};
typedef struct order ORDER;

struct pArgument {//线程函数参数
	int* mouseMessageLoc;
	int singleNumber;
	int comboNumber;
};


unsigned _stdcall mouseCheck(void * pagm) {
	int o = 0, p = 0 ,l = 0;
	pArgument* pagms =(pArgument*)pagm;
	ExMessage msg;
	int singleI, comboI;
	while (1) {
		singleI = 0, comboI = 0;
		peekmessage(&msg, EM_MOUSE);
		if (msg.message == WM_LBUTTONDOWN && *(pagms->mouseMessageLoc) == 0)
		{
			for (o = 0; o < 4; o++) {
				for (p = 0; p < 5; p++) {
					if (singleI < pagms->singleNumber) {
						if (msg.x > 400 + 160 * p && msg.x < 400 + 160 * (p + 1) && msg.y > 50 + 100 * o && msg.y < 50 + 100 * (o + 1)&&*(pagms->mouseMessageLoc)==0) {
							*(pagms->mouseMessageLoc) = 100+singleI;
						}
					}
					singleI++;
				}
			}
			for (o = 0; o < 3; o++) {
				for (p = 0; p < 5; p++) {
					if (comboI < pagms->comboNumber&& *(pagms->mouseMessageLoc) == 0)
					{
						if (msg.x > 400 + 160 * p && msg.x < 400 + 160 * (p + 1) && msg.y > 500 + 100 * o && msg.y < 500 + 100 * (o + 1)) {
							*(pagms->mouseMessageLoc) = 200+comboI;
						}
					}
					comboI++;
				}
			}
		}
		Sleep(10);
	}
}

// 写订单文字
void drawButton(int x, int y, int width, int height, char * text)
{
	// 计算文本位置
	int textX = x + (width / 2) - (textwidth(text) / 2);
	int textY = y + (height / 2) - (textheight(text) / 2);

	// 绘制按钮文本
	outtextxy(textX, textY, text);
}
//画时钟
void drawClock(int hour, int minute, int second){
	char timeStr[10];
	sprintf(timeStr, "%02d:%02d:%02d", hour, minute, second);
	clearrectangle(10,10,390,140);
	// 清除上个时间
	settextstyle(96, 37, "Elephant");
	drawButton(10,10,380,130,timeStr); // 绘制时间
	settextstyle(20, 7, "微软雅黑");
}

//写单品数字
void drawSingle(int x, int y, int width, int height, int singleStorage) {
	char text[10];
	sprintf(text, "%d", singleStorage);
	clearrectangle(x, y, x+width, y+height);//清除上个数字
	drawButton(x, y, width, height, text);
}

//主函数
int main() {
	int i = 0, j = 0, k = 0;


	//分格  绘制


	initgraph(1600, 800);//建立画布


	setbkcolor(RGB(230, 230, 219));//background
	cleardevice();//clearout
	setlinestyle(PS_SOLID, 5);//drawline style
	setfillcolor(BLACK);
	setlinecolor(BLACK);
	rectangle(0, 0, 400, 150);
	rectangle(0, 150, 400, 200);
	rectangle(0, 200, 400, 800);
	rectangle(400, 0, 1200, 50);
	
	// 加载图片
	IMAGE img;
	loadimage(&img, "1.jpg");  // 替换为图片路径和文件名
	// 显示图片
	putimage(1199, 0, &img);  // 在窗口左上角显示图片

	//快进
	setlinestyle(PS_SOLID, 2);//drawline style
	rectangle(320, 160, 390, 190);
	setlinestyle(PS_SOLID, 5);
	ExMessage accelerate;

	rectangle(0, 500, 400, 501);
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 5; j++) {
			rectangle(400 + 160 * j, 50 + 100 * i, 400 + 160 * (j + 1), 50 + 100 * (i + 1));
		}
	}
	rectangle(400, 450, 1200, 500);
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 5; j++) {
			rectangle(400 + 160 * j, 500 + 100 * i, 400 + 160 * (j + 1), 500 + 100 * (i + 1));
		}
	}

	//第负一步，建立输出文件

	FILE* fOutPut;
	fOutPut = fopen("C:\\Users\\D7\\iCloudDrive\\visual\\McWeTheBest\\output.dic", "w");

	//第一步，读取文件，建立菜单
	int singleNumber, comboNumber;//单品数量，套餐数量
	FILE* fPtr;
	fPtr = fopen("dict.dic", "r");
	fscanf(fPtr, "%d", &singleNumber);
	fscanf(fPtr, "%d", &comboNumber);
	SINGLE* singleMenu = (SINGLE*)malloc(sizeof(SINGLE) * singleNumber);
	COMBO* comboMenu = (COMBO*)malloc(sizeof(COMBO) * comboNumber);
	for (i = 0; i < singleNumber; i++) {//建立单品菜单
		fscanf(fPtr, "%s", singleMenu[i].name);
	}
	for (i = 0; i < singleNumber; i++) {//单品制作时长，单品制作进度
		fscanf(fPtr, "%d", &singleMenu[i].time);
		singleMenu[i].progress = -1;
	}
	for (i = 0; i < singleNumber; i++) {//单品最大存量，单品目前存量
		fscanf(fPtr, "%d", &singleMenu[i].max);
		singleMenu[i].storage = 0;
	}
	int stop, restart, onHand = 0;//手头最大订单数，重启订单数,当前手头订单
	fscanf(fPtr, "%d %d", &stop, &restart);
	for (i = 0; i < comboNumber; i++) {//把COMBO类型变量的所有单品位置都设为-1
		for (j = 0; j < 5; j++) {
			comboMenu[i].sID[j] = -1;
		}
	}
	char tempo[50];
	i = -1;
	int teller = 0, tellerID;
	while (i < comboNumber) {//在套餐菜单内对应单品ID
		fscanf(fPtr, "%s", tempo);
		for (j = 0; j < singleNumber; j++) {
			if (strcmp(tempo, singleMenu[j].name) == 0) {
				teller = 1;
				tellerID = j;
			}
		}
		if (teller == 0) {
			i++;
			strcpy(comboMenu[i].name, tempo);
			k = 0;
		}
		else {
			comboMenu[i].sID[k] = tellerID;
			k++;
			if (i == comboNumber - 1 && k == 5) {
				i++;
			}
		}
		teller = 0;
	}
	fclose(fPtr);
	i--;
	for (j = 4; j > 1; j--) {
		if (comboMenu[i].sID[j] == comboMenu[i].sID[j - 1]) {
			comboMenu[i].sID[j] = -1;
		}
	}
	ORDER* orders;//建立订单数组
	orders = (ORDER*)malloc(sizeof(ORDER) * 54000);
	int orderFlag = -1;
	for (i = 0; i < 54000; i++) {
		orders[i].state = 0;
	}


	//开启界面，写上菜单
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	settextstyle(20, 8, "黑体");
	int singleI = 0, comboI = 0;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 5; j++) {
			if (singleI < singleNumber) {
				drawButton(400 + 160 * j, 50 + 100 * i, 160, 50, singleMenu[singleI].name);
			}
			singleI++;
		}
	}
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 5; j++) {
			if (comboI < comboNumber) {
				drawButton(400 + 160 * j, 500 + 100 * i, 160, 50, comboMenu[comboI].name);
			}
			comboI++;
		}
	}
	settextstyle(20, 0, "黑体");
	char singleM[40] = "单品菜单(点击下单)";
	char comboM[40] = "套餐菜单(点击下单)";
	char welcome[30] = "欢迎来到麦当劳！";
	char success[30] = "下单成功！";
	char fail[30] = "下单失败！";
	char complete[30] = "订单完成";
	char completeID[30] = "订单号";
	drawButton(400, 0, 800, 50, singleM);
	drawButton(400, 450, 800, 50, comboM);
	drawButton(0, 150, 400, 50, welcome);

	//第四步，时钟，制作单品，接单，处理订单
	//参数
	int time[3] = { 6,59,59 };
	int capability = 1;//是否能接单
	int doBreak = 0;//每秒接单时，如果该订单下单时间大于当前时间，就=1，停止接单循环
	int flag = 0;
	int mouseMessage = 0;


	struct pArgument {//线程函数参数
		int* mouseMessageLoc;
		int singleNumber;
		int comboNumber;
	};

	pArgument pArgument;
	pArgument.mouseMessageLoc = &mouseMessage;
	pArgument.singleNumber = singleNumber;
	pArgument.comboNumber = comboNumber;

	//开启线程
	HANDLE hThread;
	unsigned ThreadID = 1;
	hThread = (HANDLE)_beginthreadex(NULL, 0, mouseCheck, &pArgument, 0, NULL);
	
	//循环外 用于快进时间相关变量
	int k0 = 0;
	int tt = 1000;//系统中一秒等于现实中多少毫秒
	const char* time0= "X 1";//显示的时间快进倍数

	//点击p开始工作
	settextcolor(BLACK);
	settextstyle(26, 0, "微软雅黑");
	outtextxy(325, 165, "未启动");
	while (1)
	{
		if (_getch() == 'p')
		{
			clearrectangle(322, 162, 388, 188);
			//outtextxy(325, 165, "未启动");
			break;
		}

	}
	//循环开始
	while (time[0] < 24) {
		time[2]++;//时钟
		if (time[2] == 60) {
			time[2] = 0;
			time[1]++;
		}
		if (time[1] == 60) {
			time[1] = 0;
			time[0]++;
		}
		for (i = 0; i < singleNumber; i++) {//制作单品
			if (singleMenu[i].storage < singleMenu[i].max) {
				singleMenu[i].progress++;
			}
			if (singleMenu[i].progress == singleMenu[i].time) {
				singleMenu[i].progress = 0;
				singleMenu[i].storage++;
			}
		}
		//绘制时钟
		drawClock(time[0], time[1], time[2]);
		settextcolor(BLACK);
		settextstyle(26, 0, "微软雅黑");
		outtextxy(325, 165, time0);

		/*peekmessage(&accelerate, EM_MOUSE); //加速,没做好
		if (accelerate.message == WM_LBUTTONDOWN && accelerate.x > 350 && accelerate.x < 390 && accelerate.y > 160 && accelerate.y < 190) {
			tt = 100;
			settextcolor(BLACK);
			settextstyle(30, 0, "微软雅黑");
			//             setbkmode(TRANSPARENT);
			outtextxy(0, 0, time0);
		}
		else {
			Sleep(tt);
		}*/

		if (_kbhit()) 
		{ // 检查是否有键盘输入
			char ch = _getch(); // 获取按下的键值
			if (ch == '1') { // 检查是否是
				k0 = 60*(1000/tt);
			}
			if (ch == '2') { // 检查是否是
				k0 = 120 * (1000 / tt);
			}
			if (ch == '3') { // 检查是否是
				k0 = 180 * (1000 / tt);
			}
			if (ch == '4') { // 检查是否是
				k0 = 240 * (1000 / tt);
			}
			if (ch == '5') { // 检查是否是
				k0 = 300 * (1000 / tt);
			}
			if (ch == '6') { // 检查是否是
				k0 = 360 * (1000 / tt);
			}
			if (ch == '7') { // 检查是否是
				k0 = 420 * (1000 / tt);
			}
			if (ch == '8') { // 检查是否是
				k0 = 480 * (1000 / tt);
			}
			if (ch == ' ')//切换倍速模式
			{
				if (tt == 1000)
				{
					tt = 100;

					//             setbkmode(TRANSPARENT);
					time0 = "X 10";
					clearrectangle(322, 162, 388, 188);
					outtextxy(325, 165, time0);
				}
				else if (tt == 100)
				{
					tt = 10;

					//             setbkmode(TRANSPARENT);
					time0 = "X 100";
					clearrectangle(322, 162, 388, 188);
					outtextxy(325, 165, time0);
				}
				else if (tt == 10)
				{
					tt = 1000;

					//             setbkmode(TRANSPARENT);
					time0 = "X 1";
					clearrectangle(322, 162, 388, 188);
					outtextxy(325, 165, time0);
				}



				
			}
			if (ch == 'b')//切换倍速模式
			{
				tt = 1000;

				//             setbkmode(TRANSPARENT);
				time0 = "X 1";
				clearrectangle(322, 162, 388, 188);
				outtextxy(325, 165, time0);
			}
			if (ch == 'p')//暂停键
			{
				//time0 = "暂停";
				clearrectangle(322, 162, 388, 188);
				outtextxy(325, 165, "暂停");
				while (1)
				{
					if (_getch() == 'p')
					{
						clearrectangle(322, 162, 388, 188);
						outtextxy(325, 165, time0);
						break;
					}

				}
		

				
			}
		}
		else
		{
			if (k0 == 0)
			{
				Sleep(tt);
			}
			else
			{
				k0--;
			}

		}
		
		
		//写单品制作进度
		singleI = 0;
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 5; j++) {
				if (singleI < singleNumber) {
					drawSingle(405 + 160 * j, 105 + 100 * i, 150, 40, singleMenu[singleI].storage);
				}
				singleI++;
			}
		}

		//读取鼠标点击，下单, 恢复鼠标状态
		if (mouseMessage != 0)
		{
			orderFlag++;
			orders[orderFlag].oID = orderFlag;
			if (mouseMessage < 200) {
				strcpy(orders[orderFlag].name,singleMenu[mouseMessage - 100].name);
				orders[orderFlag].singleState[0] = mouseMessage - 100;
				orders[orderFlag].singleState[1] = -1; 
				orders[orderFlag].singleState[2] = -1;
				orders[orderFlag].singleState[3] = -1;
				orders[orderFlag].singleState[4] = -1;
			}
			else {
				strcpy(orders[orderFlag].name, comboMenu[mouseMessage - 200].name);
				for (i = 0; i < 5; i++) {
					orders[orderFlag].singleState[i] = comboMenu[mouseMessage - 200].sID[i];
				}
			}

			if (capability == 1) {
				orders[orderFlag].state = 1;
				if (mouseMessage < 200) {
					clearrectangle(10,210,390,330);
					settextstyle(30, 0, "微软雅黑");
					drawButton(10, 270, 380, 60, success);
					settextstyle(40, 0, "微软雅黑");
					drawButton(10, 210, 380, 60, singleMenu[mouseMessage-100].name);
				}
				else {
					clearrectangle(10, 210, 390, 330);
					settextstyle(30, 0, "微软雅黑");
					drawButton(10, 270, 380, 60, success);
					settextstyle(40, 0, "微软雅黑");
					drawButton(10, 210, 380, 60, comboMenu[mouseMessage-200].name);
				}
				onHand++;
			}
			if (capability == 0) {
				orders[orderFlag].state = 3;
				if (mouseMessage < 200) {
					clearrectangle(10, 210, 390, 330);
					settextstyle(30, 0, "微软雅黑");
					drawButton(10, 270, 380, 60, fail);
					settextstyle(40, 0, "微软雅黑");
					drawButton(10, 210, 380, 60, singleMenu[mouseMessage - 100].name);
				}
				else {
					clearrectangle(10, 210, 390, 330);
					settextstyle(30, 0, "微软雅黑");
					drawButton(10, 270, 380, 60, fail);
					settextstyle(40, 0, "微软雅黑");
					drawButton(10, 210, 380, 60, comboMenu[mouseMessage - 200].name);
				}
			}

			mouseMessage = 0;
		}


		for (i = flag; i <= orderFlag; i++) {//处理订单
			if (orders[i].state == 1) {//分配单品
				for (j = 0; j <= 4; j++) {
					if (orders[i].singleState[j] != -1) {
						if (singleMenu[orders[i].singleState[j]].storage > 0) {
							singleMenu[orders[i].singleState[j]].storage--;
							orders[i].singleState[j] = -1;
						}
					}
				}//完成订单
				if (orders[i].singleState[0] == -1 && orders[i].singleState[1] == -1 && orders[i].singleState[2] == -1 && orders[i].singleState[3] == -1 && orders[i].singleState[4] == -1) {
					orders[i].state = 2;
					orders[i].orderTime[0] = time[0];
					orders[i].orderTime[1] = time[1];
					orders[i].orderTime[2] = time[2];
					onHand--;

					clearrectangle(10, 550, 390, 790);
					settextstyle(40, 0, "微软雅黑");
					drawButton(10, 550, 380, 60, complete);
					settextstyle(30, 0, "微软雅黑");
					drawButton(10, 610, 380, 60, completeID);
					drawSingle(10, 670, 380, 60, i + 1);
					drawButton(10, 730, 380, 60, orders[i].name);
					PlaySound("finish.wav", NULL, SND_FILENAME | SND_ASYNC);
				}
			}
		}
		while (flag <= orderFlag && doBreak == 0) {//flag向后移动找位置，输出路程上的已完成和拒绝接单订单
			if (orders[flag].state == 0 || orders[flag].state == 1) {
				doBreak = 1;
			}
			else {
				if (orders[flag].state == 2) {
					fprintf(fOutPut, "%02d:%02d:%02d\n", orders[flag].orderTime[0], orders[flag].orderTime[1], orders[flag].orderTime[2]);
				}
				if (orders[flag].state == 3) {
					fprintf(fOutPut, "Fail\n");
				}
				flag++;
			}
		}
		doBreak = 0;


		//更新capability
		if (capability == 1) {
			if (onHand > stop) {
				capability = 0;
			}
		}
		else {
			if (onHand < restart) {
				capability = 1;
			}
		}
	}



	//收尾
	fclose(fOutPut);
	WaitForSingleObject(hThread, 100);
	return 0;
}

