//����� ��׿Զ ����N  ��һ����ĩ��ҵ
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
	char name[50];//��Ʒ��
	int time;//����ʱ��
	int progress;//��������
	int max;//������
	int storage;//��ǰ����
};
typedef struct single SINGLE;

struct combo {
	char name[50];//�ײ���
	int sID[5];//�ײ��ڵ�Ʒ��id
};
typedef struct combo COMBO;

struct order {
	char name[50];//�������ݣ�����Ʒ���ײ���
	int singleState[5];//��Ʒ����������Ϊ-1Ϊ�����ڻ�����ɣ���Ϊ��ƷID��Ϊδ���
	int orderTime[3];//�µ�ʱ��
	int state;//����״̬  0δ�ӵ� 1δ��� 2����� 3�ܾ��ӵ� 
	int oID;//�������
};
typedef struct order ORDER;

struct pArgument {//�̺߳�������
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

// д��������
void drawButton(int x, int y, int width, int height, char * text)
{
	// �����ı�λ��
	int textX = x + (width / 2) - (textwidth(text) / 2);
	int textY = y + (height / 2) - (textheight(text) / 2);

	// ���ư�ť�ı�
	outtextxy(textX, textY, text);
}
//��ʱ��
void drawClock(int hour, int minute, int second){
	char timeStr[10];
	sprintf(timeStr, "%02d:%02d:%02d", hour, minute, second);
	clearrectangle(10,10,390,140);
	// ����ϸ�ʱ��
	settextstyle(96, 37, "Elephant");
	drawButton(10,10,380,130,timeStr); // ����ʱ��
	settextstyle(20, 7, "΢���ź�");
}

//д��Ʒ����
void drawSingle(int x, int y, int width, int height, int singleStorage) {
	char text[10];
	sprintf(text, "%d", singleStorage);
	clearrectangle(x, y, x+width, y+height);//����ϸ�����
	drawButton(x, y, width, height, text);
}

//������
int main() {
	int i = 0, j = 0, k = 0;


	//�ָ�  ����


	initgraph(1600, 800);//��������


	setbkcolor(RGB(230, 230, 219));//background
	cleardevice();//clearout
	setlinestyle(PS_SOLID, 5);//drawline style
	setfillcolor(BLACK);
	setlinecolor(BLACK);
	rectangle(0, 0, 400, 150);
	rectangle(0, 150, 400, 200);
	rectangle(0, 200, 400, 800);
	rectangle(400, 0, 1200, 50);
	
	// ����ͼƬ
	IMAGE img;
	loadimage(&img, "1.jpg");  // �滻ΪͼƬ·�����ļ���
	// ��ʾͼƬ
	putimage(1199, 0, &img);  // �ڴ������Ͻ���ʾͼƬ

	//���
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

	//�ڸ�һ������������ļ�

	FILE* fOutPut;
	fOutPut = fopen("C:\\Users\\D7\\iCloudDrive\\visual\\McWeTheBest\\output.dic", "w");

	//��һ������ȡ�ļ��������˵�
	int singleNumber, comboNumber;//��Ʒ�������ײ�����
	FILE* fPtr;
	fPtr = fopen("dict.dic", "r");
	fscanf(fPtr, "%d", &singleNumber);
	fscanf(fPtr, "%d", &comboNumber);
	SINGLE* singleMenu = (SINGLE*)malloc(sizeof(SINGLE) * singleNumber);
	COMBO* comboMenu = (COMBO*)malloc(sizeof(COMBO) * comboNumber);
	for (i = 0; i < singleNumber; i++) {//������Ʒ�˵�
		fscanf(fPtr, "%s", singleMenu[i].name);
	}
	for (i = 0; i < singleNumber; i++) {//��Ʒ����ʱ������Ʒ��������
		fscanf(fPtr, "%d", &singleMenu[i].time);
		singleMenu[i].progress = -1;
	}
	for (i = 0; i < singleNumber; i++) {//��Ʒ����������ƷĿǰ����
		fscanf(fPtr, "%d", &singleMenu[i].max);
		singleMenu[i].storage = 0;
	}
	int stop, restart, onHand = 0;//��ͷ��󶩵���������������,��ǰ��ͷ����
	fscanf(fPtr, "%d %d", &stop, &restart);
	for (i = 0; i < comboNumber; i++) {//��COMBO���ͱ��������е�Ʒλ�ö���Ϊ-1
		for (j = 0; j < 5; j++) {
			comboMenu[i].sID[j] = -1;
		}
	}
	char tempo[50];
	i = -1;
	int teller = 0, tellerID;
	while (i < comboNumber) {//���ײͲ˵��ڶ�Ӧ��ƷID
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
	ORDER* orders;//������������
	orders = (ORDER*)malloc(sizeof(ORDER) * 54000);
	int orderFlag = -1;
	for (i = 0; i < 54000; i++) {
		orders[i].state = 0;
	}


	//�������棬д�ϲ˵�
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	settextstyle(20, 8, "����");
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
	settextstyle(20, 0, "����");
	char singleM[40] = "��Ʒ�˵�(����µ�)";
	char comboM[40] = "�ײͲ˵�(����µ�)";
	char welcome[30] = "��ӭ�������ͣ�";
	char success[30] = "�µ��ɹ���";
	char fail[30] = "�µ�ʧ�ܣ�";
	char complete[30] = "�������";
	char completeID[30] = "������";
	drawButton(400, 0, 800, 50, singleM);
	drawButton(400, 450, 800, 50, comboM);
	drawButton(0, 150, 400, 50, welcome);

	//���Ĳ���ʱ�ӣ�������Ʒ���ӵ���������
	//����
	int time[3] = { 6,59,59 };
	int capability = 1;//�Ƿ��ܽӵ�
	int doBreak = 0;//ÿ��ӵ�ʱ������ö����µ�ʱ����ڵ�ǰʱ�䣬��=1��ֹͣ�ӵ�ѭ��
	int flag = 0;
	int mouseMessage = 0;


	struct pArgument {//�̺߳�������
		int* mouseMessageLoc;
		int singleNumber;
		int comboNumber;
	};

	pArgument pArgument;
	pArgument.mouseMessageLoc = &mouseMessage;
	pArgument.singleNumber = singleNumber;
	pArgument.comboNumber = comboNumber;

	//�����߳�
	HANDLE hThread;
	unsigned ThreadID = 1;
	hThread = (HANDLE)_beginthreadex(NULL, 0, mouseCheck, &pArgument, 0, NULL);
	
	//ѭ���� ���ڿ��ʱ����ر���
	int k0 = 0;
	int tt = 1000;//ϵͳ��һ�������ʵ�ж��ٺ���
	const char* time0= "X 1";//��ʾ��ʱ��������

	//���p��ʼ����
	settextcolor(BLACK);
	settextstyle(26, 0, "΢���ź�");
	outtextxy(325, 165, "δ����");
	while (1)
	{
		if (_getch() == 'p')
		{
			clearrectangle(322, 162, 388, 188);
			//outtextxy(325, 165, "δ����");
			break;
		}

	}
	//ѭ����ʼ
	while (time[0] < 24) {
		time[2]++;//ʱ��
		if (time[2] == 60) {
			time[2] = 0;
			time[1]++;
		}
		if (time[1] == 60) {
			time[1] = 0;
			time[0]++;
		}
		for (i = 0; i < singleNumber; i++) {//������Ʒ
			if (singleMenu[i].storage < singleMenu[i].max) {
				singleMenu[i].progress++;
			}
			if (singleMenu[i].progress == singleMenu[i].time) {
				singleMenu[i].progress = 0;
				singleMenu[i].storage++;
			}
		}
		//����ʱ��
		drawClock(time[0], time[1], time[2]);
		settextcolor(BLACK);
		settextstyle(26, 0, "΢���ź�");
		outtextxy(325, 165, time0);

		/*peekmessage(&accelerate, EM_MOUSE); //����,û����
		if (accelerate.message == WM_LBUTTONDOWN && accelerate.x > 350 && accelerate.x < 390 && accelerate.y > 160 && accelerate.y < 190) {
			tt = 100;
			settextcolor(BLACK);
			settextstyle(30, 0, "΢���ź�");
			//             setbkmode(TRANSPARENT);
			outtextxy(0, 0, time0);
		}
		else {
			Sleep(tt);
		}*/

		if (_kbhit()) 
		{ // ����Ƿ��м�������
			char ch = _getch(); // ��ȡ���µļ�ֵ
			if (ch == '1') { // ����Ƿ���
				k0 = 60*(1000/tt);
			}
			if (ch == '2') { // ����Ƿ���
				k0 = 120 * (1000 / tt);
			}
			if (ch == '3') { // ����Ƿ���
				k0 = 180 * (1000 / tt);
			}
			if (ch == '4') { // ����Ƿ���
				k0 = 240 * (1000 / tt);
			}
			if (ch == '5') { // ����Ƿ���
				k0 = 300 * (1000 / tt);
			}
			if (ch == '6') { // ����Ƿ���
				k0 = 360 * (1000 / tt);
			}
			if (ch == '7') { // ����Ƿ���
				k0 = 420 * (1000 / tt);
			}
			if (ch == '8') { // ����Ƿ���
				k0 = 480 * (1000 / tt);
			}
			if (ch == ' ')//�л�����ģʽ
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
			if (ch == 'b')//�л�����ģʽ
			{
				tt = 1000;

				//             setbkmode(TRANSPARENT);
				time0 = "X 1";
				clearrectangle(322, 162, 388, 188);
				outtextxy(325, 165, time0);
			}
			if (ch == 'p')//��ͣ��
			{
				//time0 = "��ͣ";
				clearrectangle(322, 162, 388, 188);
				outtextxy(325, 165, "��ͣ");
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
		
		
		//д��Ʒ��������
		singleI = 0;
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 5; j++) {
				if (singleI < singleNumber) {
					drawSingle(405 + 160 * j, 105 + 100 * i, 150, 40, singleMenu[singleI].storage);
				}
				singleI++;
			}
		}

		//��ȡ��������µ�, �ָ����״̬
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
					settextstyle(30, 0, "΢���ź�");
					drawButton(10, 270, 380, 60, success);
					settextstyle(40, 0, "΢���ź�");
					drawButton(10, 210, 380, 60, singleMenu[mouseMessage-100].name);
				}
				else {
					clearrectangle(10, 210, 390, 330);
					settextstyle(30, 0, "΢���ź�");
					drawButton(10, 270, 380, 60, success);
					settextstyle(40, 0, "΢���ź�");
					drawButton(10, 210, 380, 60, comboMenu[mouseMessage-200].name);
				}
				onHand++;
			}
			if (capability == 0) {
				orders[orderFlag].state = 3;
				if (mouseMessage < 200) {
					clearrectangle(10, 210, 390, 330);
					settextstyle(30, 0, "΢���ź�");
					drawButton(10, 270, 380, 60, fail);
					settextstyle(40, 0, "΢���ź�");
					drawButton(10, 210, 380, 60, singleMenu[mouseMessage - 100].name);
				}
				else {
					clearrectangle(10, 210, 390, 330);
					settextstyle(30, 0, "΢���ź�");
					drawButton(10, 270, 380, 60, fail);
					settextstyle(40, 0, "΢���ź�");
					drawButton(10, 210, 380, 60, comboMenu[mouseMessage - 200].name);
				}
			}

			mouseMessage = 0;
		}


		for (i = flag; i <= orderFlag; i++) {//������
			if (orders[i].state == 1) {//���䵥Ʒ
				for (j = 0; j <= 4; j++) {
					if (orders[i].singleState[j] != -1) {
						if (singleMenu[orders[i].singleState[j]].storage > 0) {
							singleMenu[orders[i].singleState[j]].storage--;
							orders[i].singleState[j] = -1;
						}
					}
				}//��ɶ���
				if (orders[i].singleState[0] == -1 && orders[i].singleState[1] == -1 && orders[i].singleState[2] == -1 && orders[i].singleState[3] == -1 && orders[i].singleState[4] == -1) {
					orders[i].state = 2;
					orders[i].orderTime[0] = time[0];
					orders[i].orderTime[1] = time[1];
					orders[i].orderTime[2] = time[2];
					onHand--;

					clearrectangle(10, 550, 390, 790);
					settextstyle(40, 0, "΢���ź�");
					drawButton(10, 550, 380, 60, complete);
					settextstyle(30, 0, "΢���ź�");
					drawButton(10, 610, 380, 60, completeID);
					drawSingle(10, 670, 380, 60, i + 1);
					drawButton(10, 730, 380, 60, orders[i].name);
					PlaySound("finish.wav", NULL, SND_FILENAME | SND_ASYNC);
				}
			}
		}
		while (flag <= orderFlag && doBreak == 0) {//flag����ƶ���λ�ã����·���ϵ�����ɺ;ܾ��ӵ�����
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


		//����capability
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



	//��β
	fclose(fOutPut);
	WaitForSingleObject(hThread, 100);
	return 0;
}

