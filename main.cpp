//by QHC
//Version 1.0

#include <iostream>
#include <conio.h>
#include <graphics.h>
#define WINDOW_WIDTH  1280  //���ڿ��
#define WINDOW_HEIGHT 480   //���ڸ߶�

#define CANDLE_HEIGHT 75    //����߶�
#define FOCAL_LENGTH  100   //����

int DrawPicture(int objDis);
int DrawCandle(int x1, int y1, int x2, int y2, bool upsidedown = false);

int main() {

    //���ô���
    initgraph(WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    setbkcolor(0xCCCCCC);
    setbkmode(TRANSPARENT);
    setlinecolor(BLACK);
    //initgraph(WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    HWND hWND = GetHWnd();
    SetWindowText(hWND, _T("������"));

    int objDis = 2 * FOCAL_LENGTH;  //���

    //����ԭ�㵽�����м�
    setorigin(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

    //����y������������
    setaspectratio(1, 1);

    DrawPicture(objDis);

    ExMessage mouse;
    bool isLDown = false;

    settextstyle(20, 0, _T("����"));
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
                    DrawPicture(objDis);
                }
                break;

            //���������ʱ���������������
            case WM_LBUTTONDOWN:
                isLDown = true;
                if (mouse.x <= WINDOW_WIDTH / 2)
                    objDis = WINDOW_WIDTH / 2 - mouse.x;
                else objDis = 0;
                cleardevice();
                DrawPicture(objDis);
                break;
            
            case WM_LBUTTONUP:
                isLDown = false;
                break;
        }
    }
    return 0;
}

int DrawPicture(int objDis) {
    int imgDis,              //���
        imgHeight;           //��ĸ߶�
    
    if (objDis != FOCAL_LENGTH) {
        imgDis = objDis * FOCAL_LENGTH / (objDis - FOCAL_LENGTH);
        imgHeight = -CANDLE_HEIGHT * FOCAL_LENGTH / (objDis - FOCAL_LENGTH);  
    }
    else imgDis = imgHeight = 0;

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
    DrawCandle(-objDis, 0, -objDis, -CANDLE_HEIGHT);
    setlinecolor(BLACK);

    //�������򷢳���������ƽ�еĹ�
    line(-objDis, -CANDLE_HEIGHT, 0, -CANDLE_HEIGHT);

    if (objDis > FOCAL_LENGTH) { //�������ڽ���ʱ

        //����ƽ�й������Ĺ���
        line(0, -CANDLE_HEIGHT, imgDis, -imgHeight);  

        //�������򾭹����ĵĹ���
        line(-objDis, -CANDLE_HEIGHT, imgDis, -imgHeight);

        //����ʵ��
        setlinecolor(DARKGRAY);
        DrawCandle(imgDis, 0, imgDis, -imgHeight, true);
        setlinecolor(BLACK);

    }else if(objDis < FOCAL_LENGTH) { //�����С�ڽ���ʱ

        //������ʽ��Ϊ����
        setlinestyle(PS_DASH, 1);

        //����ƽ�й������Ĺ��ߵķ����ӳ���
        line(-objDis, -CANDLE_HEIGHT, imgDis, -imgHeight);

        //�����������ĵĹ�ķ����ӳ���
        line(0, -CANDLE_HEIGHT, imgDis, -imgHeight);

        //��������
        setlinecolor(DARKGRAY);
        DrawCandle(imgDis, 0, imgDis, -imgHeight);
        setlinecolor(BLACK);

        //������ʽ��Ϊʵ��
        setlinestyle(PS_SOLID);

        //�����������ĵĹ�
        if (objDis != 0)
            line(-objDis, -CANDLE_HEIGHT, WINDOW_WIDTH, (1.0 * CANDLE_HEIGHT / objDis) * WINDOW_WIDTH);

        //����ƽ�йⷴ���Ĺ���
        line(0, -CANDLE_HEIGHT, 
            WINDOW_WIDTH, (1.0 * CANDLE_HEIGHT / FOCAL_LENGTH * WINDOW_WIDTH) - FOCAL_LENGTH + 26);
    }else{
        //�����������ĵĹ�
         line(-objDis, -CANDLE_HEIGHT, WINDOW_WIDTH, (1.0 * CANDLE_HEIGHT / objDis) * WINDOW_WIDTH);

        //����ƽ�йⷴ���Ĺ���
        line(0, -CANDLE_HEIGHT, 
            WINDOW_WIDTH, (1.0 * CANDLE_HEIGHT / FOCAL_LENGTH * WINDOW_WIDTH) - FOCAL_LENGTH + 26);
    }
    //��ʼ���ַ���
    TCHAR   objDisShow[8]  = _T(""),
            focLenShow[8]  = _T(""),
            imgDisShow[13] = _T("v ������");

    //����ࡢ���ࡢ���д���Ӧ���ַ�����
    _stprintf(objDisShow, _T("u = %d"), objDis);
    _stprintf(focLenShow, _T("f = %d"), FOCAL_LENGTH);
    if (objDis != FOCAL_LENGTH)
        _stprintf(imgDisShow, _T("v = %d"), imgDis);
    
    //����Ļ�ϴ�ӡ�����ַ���
    outtextxy(-WINDOW_WIDTH / 2 + 10, -WINDOW_HEIGHT / 2 + 10, objDisShow);
    outtextxy(-WINDOW_WIDTH / 2 + 10, -WINDOW_HEIGHT / 2 + 30, focLenShow);
    outtextxy(-WINDOW_WIDTH / 2 + 10, -WINDOW_HEIGHT / 2 + 50, imgDisShow);

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