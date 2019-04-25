#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include<iostream>
#include<windowsx.h>
#include<math.h>
#include<gdiplus.h>
#include<stdlib.h>


using namespace std;
using namespace Gdiplus;

#include"class.cpp"
#include"TickTackToe_Algorithm.h"

LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
HWND maine,button,editC,editR;
POINT posD,posU;
Rect slkt_id_zero ,slkt_id_cross;

TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");
TickTackToe ttt;
Algorithm Alg;

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    MSG messages;
    WNDCLASSEX wincl;
    GdiplusStartupInput gdiplusStartupInput;
   ULONG_PTR           gdiplusToken;

   // Initialize GDI+.
   GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;
    wincl.style = CS_DBLCLKS;
    wincl.cbSize = sizeof (WNDCLASSEX);
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    wincl.hbrBackground = CreateSolidBrush(RGB(131,121,134));

    if (!RegisterClassEx (&wincl))
        return 0;

    maine = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("TickTackToe"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           429,                 /* The programs width */
           400,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL        /* No Window Creation data */
           );

    ShowWindow (maine, nCmdShow);

    while (GetMessage (&messages, NULL, 0, 0))
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }
    GdiplusShutdown(gdiplusToken);
    return messages.wParam;
}

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static bool buttondown;
    switch (message)
    {
        case WM_CREATE:
        {
            button=CreateWindow("button","START",WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON,ttt.width/2-50,4*ttt.height/5,100,20,hwnd,(HMENU)1,NULL,NULL);
            break;
        }
        case WM_SIZE:
        {
            ttt.RefreshDisplayData=true;
            InvalidateRect(hwnd,NULL,TRUE);
            UpdateWindow(hwnd);
            break;
        }
        case WM_PAINT:
        {
            ttt.AssignWindow(hwnd);
            PAINTSTRUCT ps;
            BeginPaint(hwnd,&ps);
            if(ttt.period<3)
            {
                 ttt.initialize();
                 if(ttt.period==1)
                    MoveWindow(button,ttt.width/2-50,4*ttt.height/5,100,20,true);
                 else if(ttt.period==2)
                 {
                     MoveWindow(button,ttt.width/2+50,4*ttt.height/5,100,20,true);
                     MoveWindow(editC,ttt.width/2-50,4*ttt.height/5,30,20,true);
                     MoveWindow(editR,ttt.width/2,4*ttt.height/5,30,20,true);
                 }
            }
            else if (ttt.period==3)
            {
                Graphics g(GetDC(hwnd));
                SolidBrush brtxt(Color(200,0,170,250));
                FontFamily ff(L"Times New Roman");
                Font font(&ff,34,FontStyleRegular,UnitPixel);
                PointF point(40,20);
                g.DrawString(L"Choose Your Option:",-1,&font,point,&brtxt);
                Image img_zero(L"zero.png"),img_cross(L"cross.png");

                slkt_id_zero.X=ttt.width/8;
                slkt_id_zero.Y=ttt.height/2;
                slkt_id_zero.Width=ttt.width/4;
                slkt_id_zero.Height=ttt.height/4;

                slkt_id_cross.X=5*ttt.width/8;
                slkt_id_cross.Y=ttt.height/2;
                slkt_id_cross.Width=ttt.width/4;
                slkt_id_cross.Height=ttt.height/4;

                g.DrawImage(&img_zero,slkt_id_zero);
                g.DrawImage(&img_cross,slkt_id_cross);
                //g.DrawImage()
            }
            else if (ttt.period==4)
            {
                ttt.deployTable();
                for(int i=1;i<ttt.totalBlocks;i++)
                    ttt.block[i].Draw();
            }
            EndPaint(hwnd,&ps);
            break;
        }
        case WM_COMMAND:
        {
            if(wParam==1)
            {
                DestroyWindow(button);
                ttt.period=2;
                button=CreateWindow("button","START",WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON,ttt.width/2+50,4*ttt.height/5,100,20,hwnd,(HMENU)2,NULL,NULL);
                editC=CreateWindow("edit","",WS_VISIBLE|WS_CHILD|ES_NUMBER,ttt.width/2-50,4*ttt.height/5,30,20,hwnd,NULL,NULL,NULL);
                editR=CreateWindow("edit","",WS_VISIBLE|WS_CHILD|ES_NUMBER,ttt.width/2,4*ttt.height/5,30,20,hwnd,NULL,NULL,NULL);
            }

            else if(wParam==2)
            {
                ttt.period=3;
                DestroyWindow(button);
                InvalidateRect(hwnd,NULL,true);
                TCHAR textC[4],textR[4];
                GetWindowText(editC,textC,4);
                GetWindowText(editR,textR,4);
                DestroyWindow(editC);
                DestroyWindow(editR);
                nColoumn=_ttoi(textC),nRow=_ttoi(textR);
                ttt.totalBlocks=nColoumn*nRow;
                Alg.init(nRow,nColoumn);
            }
            break;
        }
        case WM_LBUTTONDOWN:
        {
            posD.x= GET_X_LPARAM(lParam);
            posD.y= GET_Y_LPARAM(lParam);
            buttondown=true;
            break;
        }
        case WM_LBUTTONUP:
        {
            posU.x= GET_X_LPARAM(lParam);
            posU.y= GET_Y_LPARAM(lParam);
            if(buttondown&&ttt.period==4)
            {
                ttt.prevFocus=ttt.currentFocus;
                int i=BlkIdFromPnt(posU,nColoumn);
                ttt.currentFocus=i;

                if(ttt.userValue==cross)
                {
                    ttt.block[i].DrawCrossOnMe();
                    ttt.block[i].value=cross;
                }
                else
                {
                    ttt.block[i].DrawZeroOnMe();
                    ttt.block[i].value=zero;
                }

                ttt.block[i].makeFocused();
            }
            if(buttondown&&ttt.period==3)
            {
                RECT gdiRct;
                gdiRct.left=slkt_id_cross.GetLeft();
                gdiRct.top=slkt_id_cross.GetTop();
                gdiRct.right=slkt_id_cross.GetRight();
                gdiRct.bottom=slkt_id_cross.GetBottom();



/* here I ended*/








                if(PtInRect(posU))
            }

            //ttt.block[i].IsOkClicked(posU);
            //cout<<"\n row= "<<ttt.block[i].myRow<<" coloumn= "<<ttt.block[i].myColoumn<<" ID = "<<i<<" value= "<<ttt.block[i].value;

            break;
        }
        case WM_KEYDOWN:
        {
            if(wParam==VK_LEFT)
            {
                if(ttt.currentFocus==0) ttt.currentFocus=ttt.totalBlocks;
                else if(ttt.currentFocus==1) break;
                else
                {
                    ttt.prevFocus=ttt.currentFocus;
                    ttt.currentFocus--;
                }
            }
            else if(wParam==VK_RIGHT)
            {
                if(ttt.currentFocus==ttt.totalBlocks) break;
                else
                {
                    ttt.prevFocus=ttt.currentFocus;
                    ttt.currentFocus++;
                }
            }
            else if(wParam==VK_UP)
            {
                if(ttt.block[ttt.currentFocus].myRow==1) break;
                else
                {
                    ttt.prevFocus=ttt.currentFocus;
                    ttt.currentFocus-=nColoumn;
                }
            }
            else if(wParam==VK_DOWN)
            {
                if(ttt.block[ttt.currentFocus].myRow==nRow) break;
                else
                {
                    ttt.prevFocus=ttt.currentFocus;
                    ttt.currentFocus+=nColoumn;
                }
            }
            if(wParam==VK_DOWN||wParam==VK_UP||wParam==VK_RIGHT||wParam==VK_LEFT)
            {
                ttt.block[ttt.prevFocus].eraseBk();
                ttt.block[ttt.currentFocus].DrawZeroOnMe();
                ttt.block[ttt.currentFocus].makeFocused();
            }
            /*if(wParam==VK_NUMPAD0)
            {
                if(ttt.block[ttt.currentFocus].tmpValue==cross)
                {
                    ttt.block[ttt.currentFocus].DrawCrossOnMe();
                    ttt.block[ttt.currentFocus].tmpValue=zero;
                }
                else
                {
                    ttt.block[ttt.currentFocus].DrawZeroOnMe();
                    ttt.block[ttt.currentFocus].tmpValue=cross;
                }
                ttt.block[ttt.currentFocus].makeFocused();
            }*/
            else if(wParam==VK_RETURN)
            {
                if(ttt.currentFocus==0) break;
                //if(ttt.block[ttt.currentFocus].side!=NO_SIDE) break;

                ttt.block[ttt.currentFocus].value=ttt.userValue;
                ttt.totalInput++;

                Alg.totalInputs++;
                Alg.blk[ttt.currentFocus].value=ttt.userValue;
                int com_id=Alg.output(ttt.currentFocus);
                ttt.block[com_id].value=ttt.comValue;
                ttt.block[com_id].Draw();
            }
            break;
        }
        case WM_DESTROY:
            PostQuitMessage (0);
            break;
        default:
            return DefWindowProc (hwnd, message, wParam, lParam);
    }
    return 0;
}
