//By Oct10th
//Based on EasyX
//Version 1.1

#include <iostream>
#include <conio.h>
#include <graphics.h>
#include <cmath>

#define WINDOW_WIDTH  1280  //���ڿ��
#define WINDOW_HEIGHT 480   //���ڸ߶�

#define CANDLE_HEIGHT 75    //����߶�
#define FOCAL_LENGTH 100   //����

int DrawPicture(int x, int y);
int DrawCandle(int x1, int y1, int x2, int y2, bool upsidedown = false);

int main() {

    //���ô���
    initgraph(WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    setbkcolor(0xCCCCCC);
    setbkmode(TRANSPARENT);
    setlinecolor(BLACK);
    cleardevice();
    HWND hWND = GetHWnd();
    SetWindowText(hWND, _T("������"));
    settextstyle(20, 0, _T("����"));

    int objDis = 2 * FOCAL_LENGTH;  //���

    //����ԭ�㵽�����м�
    setorigin(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

    DrawPicture(-objDis, -CANDLE_HEIGHT);

    ExMessage mouse;
    bool isLDown = false;

    while(true){
        //������������ʱ���������
        getmessage(&mouse, EX_MOUSE);
        switch (mouse.message) {
            //���������������϶�ʱ���������������
            case WM_MOUSEMOVE:
                if(isLDown) {
                    if (mouse.x <= WINDOW_WIDTH / 2)
                        objDis = WINDOW_WIDTH / 2 - mouse.x;
                    else objDis = 0;
                    cleardevice();
                    DrawPicture(-objDis, -CANDLE_HEIGHT);
                }
                break;

            //���������ʱ���������������
            case WM_LBUTTONDOWN:
                isLDown = true;
                if (mouse.x <= WINDOW_WIDTH / 2)
                    objDis = WINDOW_WIDTH / 2 - mouse.x;
                else objDis = 0;
                cleardevice();
                DrawPicture(-objDis, -CANDLE_HEIGHT);
                break;
            
            case WM_LBUTTONUP:
                isLDown = false;
                break;
        }
    }
    return 0;
}

//��ͼ��ĺ�����ʹ�ô���������������ĵ������
int DrawPicture(int x, int y) {
    int imgX,              //��ĺ�����
        imgY;              //���������
    
    if (std::abs(x) != FOCAL_LENGTH) {
        imgX = (x * FOCAL_LENGTH) / (x + FOCAL_LENGTH);
        imgY = (y * FOCAL_LENGTH) / (x + FOCAL_LENGTH);  
    }
    else imgX = imgY = 0;

    //��ʼ������ͼ
    BeginBatchDraw();

    //����͹͸��
    setlinestyle(PS_DASH);
    line(0, -WINDOW_HEIGHT / 2, 0, WINDOW_HEIGHT / 2);
    setlinestyle(PS_SOLID, 3);
    ellipse(-36 / 2, -276 / 2, 36 / 2, 276 / 2);
    setlinestyle(PS_SOLID);

    //����������
    const DWORD dash_dot[4] = {10, 5, 1, 5};
    setlinestyle(PS_USERSTYLE, 1, dash_dot, 4);
    line(-WINDOW_WIDTH / 2, 0, WINDOW_WIDTH / 2, 0);
    setlinestyle(PS_SOLID);

    //��������Ͷ�������
    setfillcolor(BLACK);
    solidcircle(FOCAL_LENGTH, 0, 2);
    solidcircle(-FOCAL_LENGTH, 0, 2);
    solidcircle(2 * FOCAL_LENGTH, 0, 2);
    solidcircle(-2 * FOCAL_LENGTH, 0, 2);

    //��������
    setlinecolor(BLUE);
    DrawCandle(x, 0, x, y);
    setlinecolor(BLACK);

    //�������򷢳�����������ƽ�еĹ�
    line(x, y, 0, y);

    if (std::abs(x) > FOCAL_LENGTH) { //�������ڽ���ʱ

        //����ƽ�й������Ĺ���
        line(0, y, imgX, imgY);  

        //�������򾭹����ĵĹ���
        line(x, y, imgX, imgY);

        //����ʵ��
        setlinecolor(DARKGRAY);
        DrawCandle(imgX, 0, imgX, imgY, true);
        setlinecolor(BLACK);

    }else if(std::abs(x) < FOCAL_LENGTH) { //�����С�ڽ���ʱ

        //������ʽ��Ϊ����
        setlinestyle(PS_DASH, 1);

        //����ƽ�й������Ĺ��ߵķ����ӳ���
        line(x, y, imgX, imgY);

        //�����������ĵĹ�ķ����ӳ���
        line(0, y, imgX, imgY);

        //��������
        setlinecolor(DARKGRAY);
        DrawCandle(imgX, 0, imgX, imgY);
        setlinecolor(BLACK);

        //������ʽ��Ϊʵ��
        setlinestyle(PS_SOLID);

        //�����������ĵĹ�
        if (x != 0)
            line(x, y, WINDOW_WIDTH, (static_cast<double>(y) / x) * WINDOW_WIDTH);

        //����ƽ�йⷴ���Ĺ���
        line(0, y, 
            WINDOW_WIDTH, (static_cast<double>(-y) / FOCAL_LENGTH * WINDOW_WIDTH) + y);
    }else{ //�������ڽ���ʱ

        //�����������ĵĹ�
         line(x, y, WINDOW_WIDTH, (static_cast<double>(y) / x) * WINDOW_WIDTH);

        //����ƽ�йⷴ���Ĺ���
        line(0, y, 
            WINDOW_WIDTH, (static_cast<double>(-y) / FOCAL_LENGTH * WINDOW_WIDTH) + y);
    }
    //��ʼ���ַ���
    TCHAR   objDisShow[8]  = _T(""),
            focLenShow[8]  = _T(""),
            imgXShow[13] = _T("v ������");

    //����ࡢ���ࡢ���д���Ӧ���ַ�����
    std::_stprintf(objDisShow, _T("u = %d"), std::abs(x));
    std::_stprintf(focLenShow, _T("f = %d"), FOCAL_LENGTH);
    if (std::abs(x) != FOCAL_LENGTH)
        std::_stprintf(imgXShow, _T("v = %d"), std::abs(imgX));
    
    //����Ļ�ϴ�ӡ�����ַ���
    outtextxy(-WINDOW_WIDTH / 2 + 10, -WINDOW_HEIGHT / 2 + 10, objDisShow);
    outtextxy(-WINDOW_WIDTH / 2 + 10, -WINDOW_HEIGHT / 2 + 30, focLenShow);
    outtextxy(-WINDOW_WIDTH / 2 + 10, -WINDOW_HEIGHT / 2 + 50, imgXShow);

    //����������ͼ
    EndBatchDraw();

    return 0;
}


//������ĺ���
int DrawCandle(int x1, int y1, int x2, int y2, bool upsidedown) {

    //������������
    line(x1, y1, x2, y2);

    //����������棨��ʵ���Ǽ�ͷ��
    if(!upsidedown){
        line(x2 - 5, y2 + 10, x2, y2);
        line(x2 + 5, y2 + 10, x2, y2);
    }else{
        line(x2 - 5, y2 - 10, x2, y2);
        line(x2 + 5, y2 - 10, x2, y2);
    }
    return 0;
}