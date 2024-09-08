//By Oct10th
//Based on EasyX
//Version 1.1

#include <iostream>
#include <conio.h>
#include <graphics.h>
#include <cmath>

#define WINDOW_WIDTH  1280  //窗口宽度
#define WINDOW_HEIGHT 480   //窗口高度

#define CANDLE_HEIGHT 75    //蜡烛高度
#define FOCAL_LENGTH 100   //焦距

int DrawPicture(int x, int y);
int DrawCandle(int x1, int y1, int x2, int y2, bool upsidedown = false);

int main() {

    //设置窗口
    initgraph(WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    setbkcolor(0xCCCCCC);
    setbkmode(TRANSPARENT);
    setlinecolor(BLACK);
    cleardevice();
    HWND hWND = GetHWnd();
    SetWindowText(hWND, _T("物理大坑"));
    settextstyle(20, 0, _T("宋体"));

    int objDis = 2 * FOCAL_LENGTH;  //物距

    //设置原点到窗口中间
    setorigin(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

    DrawPicture(-objDis, -CANDLE_HEIGHT);

    ExMessage mouse;
    bool isLDown = false;

    while(true){
        //当按下鼠标左键时，设置物距
        getmessage(&mouse, EX_MOUSE);
        switch (mouse.message) {
            //当按下鼠标左键并拖动时，设置物距跟随鼠标
            case WM_MOUSEMOVE:
                if(isLDown) {
                    if (mouse.x <= WINDOW_WIDTH / 2)
                        objDis = WINDOW_WIDTH / 2 - mouse.x;
                    else objDis = 0;
                    cleardevice();
                    DrawPicture(-objDis, -CANDLE_HEIGHT);
                }
                break;

            //当按下左键时，设置物距跟随鼠标
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

//画图像的函数，使用代数方法，接受物的点的坐标
int DrawPicture(int x, int y) {
    int imgX,              //像的横坐标
        imgY;              //像的纵坐标
    
    if (std::abs(x) != FOCAL_LENGTH) {
        imgX = (x * FOCAL_LENGTH) / (x + FOCAL_LENGTH);
        imgY = (y * FOCAL_LENGTH) / (x + FOCAL_LENGTH);  
    }
    else imgX = imgY = 0;

    //开始批量绘图
    BeginBatchDraw();

    //画出凸透镜
    setlinestyle(PS_DASH);
    line(0, -WINDOW_HEIGHT / 2, 0, WINDOW_HEIGHT / 2);
    setlinestyle(PS_SOLID, 3);
    ellipse(-36 / 2, -276 / 2, 36 / 2, 276 / 2);
    setlinestyle(PS_SOLID);

    //画出主光轴
    const DWORD dash_dot[4] = {10, 5, 1, 5};
    setlinestyle(PS_USERSTYLE, 1, dash_dot, 4);
    line(-WINDOW_WIDTH / 2, 0, WINDOW_WIDTH / 2, 0);
    setlinestyle(PS_SOLID);

    //画出焦点和二倍焦点
    setfillcolor(BLACK);
    solidcircle(FOCAL_LENGTH, 0, 2);
    solidcircle(-FOCAL_LENGTH, 0, 2);
    solidcircle(2 * FOCAL_LENGTH, 0, 2);
    solidcircle(-2 * FOCAL_LENGTH, 0, 2);

    //画出蜡烛
    setlinecolor(BLUE);
    DrawCandle(x, 0, x, y);
    setlinecolor(BLACK);

    //画出蜡烛发出的与主光轴平行的光
    line(x, y, 0, y);

    if (std::abs(x) > FOCAL_LENGTH) { //当物距大于焦距时

        //画出平行光折射后的光线
        line(0, y, imgX, imgY);  

        //画出蜡烛经过光心的光线
        line(x, y, imgX, imgY);

        //画出实像
        setlinecolor(DARKGRAY);
        DrawCandle(imgX, 0, imgX, imgY, true);
        setlinecolor(BLACK);

    }else if(std::abs(x) < FOCAL_LENGTH) { //当物距小于焦距时

        //将线样式改为虚线
        setlinestyle(PS_DASH, 1);

        //画出平行光折射后的光线的反向延长线
        line(x, y, imgX, imgY);

        //画出经过光心的光的反向延长线
        line(0, y, imgX, imgY);

        //画出虚像
        setlinecolor(DARKGRAY);
        DrawCandle(imgX, 0, imgX, imgY);
        setlinecolor(BLACK);

        //将线样式改为实线
        setlinestyle(PS_SOLID);

        //画出经过光心的光
        if (x != 0)
            line(x, y, WINDOW_WIDTH, (static_cast<double>(y) / x) * WINDOW_WIDTH);

        //画出平行光反射后的光线
        line(0, y, 
            WINDOW_WIDTH, (static_cast<double>(-y) / FOCAL_LENGTH * WINDOW_WIDTH) + y);
    }else{ //当物距等于焦距时

        //画出经过光心的光
         line(x, y, WINDOW_WIDTH, (static_cast<double>(y) / x) * WINDOW_WIDTH);

        //画出平行光反射后的光线
        line(0, y, 
            WINDOW_WIDTH, (static_cast<double>(-y) / FOCAL_LENGTH * WINDOW_WIDTH) + y);
    }
    //初始化字符串
    TCHAR   objDisShow[8]  = _T(""),
            focLenShow[8]  = _T(""),
            imgXShow[13] = _T("v 不存在");

    //将物距、焦距、像距写入对应的字符串中
    std::_stprintf(objDisShow, _T("u = %d"), std::abs(x));
    std::_stprintf(focLenShow, _T("f = %d"), FOCAL_LENGTH);
    if (std::abs(x) != FOCAL_LENGTH)
        std::_stprintf(imgXShow, _T("v = %d"), std::abs(imgX));
    
    //在屏幕上打印三条字符串
    outtextxy(-WINDOW_WIDTH / 2 + 10, -WINDOW_HEIGHT / 2 + 10, objDisShow);
    outtextxy(-WINDOW_WIDTH / 2 + 10, -WINDOW_HEIGHT / 2 + 30, focLenShow);
    outtextxy(-WINDOW_WIDTH / 2 + 10, -WINDOW_HEIGHT / 2 + 50, imgXShow);

    //结束批量绘图
    EndBatchDraw();

    return 0;
}


//画蜡烛的函数
int DrawCandle(int x1, int y1, int x2, int y2, bool upsidedown) {

    //画出蜡烛主干
    line(x1, y1, x2, y2);

    //画出蜡烛火焰（其实就是箭头）
    if(!upsidedown){
        line(x2 - 5, y2 + 10, x2, y2);
        line(x2 + 5, y2 + 10, x2, y2);
    }else{
        line(x2 - 5, y2 - 10, x2, y2);
        line(x2 + 5, y2 - 10, x2, y2);
    }
    return 0;
}