#define cross 1
#define zero 0
#define SAME_ROW 1
#define SAME_COLOUMN 2
#define CROSS_ALLY_TAN_POS 3
#define CROSS_ALLY_TAN_NEG 4
#define NOT_ALLY 5

double rectW,rectH;
HDC hdc;
int nColoumn,nRow;

int BlkIdFromPnt(POINT p, int nColoumn)
{
    int r,c;
    r=ceil(p.y/rectH);
    c=ceil(p.x/rectW);
    return (r-1)*nColoumn+c;
}

class Block
{
    public:
        Rect myRect,ClrRect;
        //HRGN myRgn, OkCrclRgn,OkRgn;
    int myColoumn=0, myRow=0,value=2;
    bool isFocused=false;

    void eraseBk()
    {
        SolidBrush BkBrush(Color(255,131,121,134));
        Graphics g(hdc);
        g.FillRectangle(&BkBrush,ClrRect);
    }
    void DrawCrossOnMe()
    {
        Graphics g(hdc);
        eraseBk();
        MoveToEx(hdc,(myColoumn-1)*rectW,(myRow-1)*rectH,NULL);
        LineTo(hdc,myColoumn*rectW,myRow*rectH);
        MoveToEx(hdc,(myColoumn-1)*rectW,myRow*rectH,NULL);
        LineTo(hdc,myColoumn*rectW,(myRow-1)*rectH);
    }
    void DrawZeroOnMe()
    {
        eraseBk();
        Graphics g(hdc);
        SolidBrush EllipseBrush(Color(200,150,180,0));
        g.FillEllipse(&EllipseBrush,myRect);
    }
    bool Draw()
    {
        if(value==2) return false;
        SolidBrush BkBrush(Color(255,131,121,134)),OkBrush(Color(100,255,0,0));
        if(value==zero)
            DrawZeroOnMe();
        else if(value==cross)
            DrawCrossOnMe();
        //g.FillPie(&OkBrush,OkRect,180.0f,90.0f);
    }
    /*bool IsOkClicked(POINT p)
    {
        myRgn=CreateRectRgn((myColoumn-1)*rectW,     (myRow-1)*rectH,     myColoumn*rectW,    myRow*rectH);
        OkCrclRgn=CreateEllipticRgn(OkRect.X,   OkRect.Y,    OkRect.X+OkRect.Width-1,   OkRect.Height+OkRect.Y-1);

        if(CombineRgn(OkRgn,myRgn,OkCrclRgn,RGN_AND)==ERROR) cout<<"whadda";

        cout<<" okrect.x= "<<OkRect.X;
        FillRgn(hdc,OkCrclRgn,(HBRUSH) CreateSolidBrush(RGB(0,0,0)));Sleep(1000);
        bool b=PtInRegion(OkRgn,p.x,p.y);
        DeleteObject(myRgn);
        DeleteObject(OkRgn);
        DeleteObject(OkCrclRgn);
        if(b)
        {
            return true;
        }

        else return false;
    }*/
    bool makeFocused()
    {
        Graphics g(hdc);
        Pen borderPen(Color(100,255,2,2));
        borderPen.SetWidth(3.0f);

        Point lt((myColoumn-1)*rectW+2,(myRow-1)*rectH+2),     rt(myColoumn*rectW-2,(myRow-1)*rectH+2);
        Point lb((myColoumn-1)*rectW+2,myRow*rectH-2),     rb(myColoumn*rectW-2,myRow*rectH-2);

        g.DrawLine(&borderPen,lt,rt);
        g.DrawLine(&borderPen,rt,rb);
        g.DrawLine(&borderPen,lb,rb);
        g.DrawLine(&borderPen,lt,lb);
        //cout<<"\n executing.. rectW= "<<rectW;
    }
};
class TickTackToe
{
    private: WINDOWINFO winfo;
    public: int period=1,height,width,totalBlocks=0,prevFocus=0,currentFocus=0,totalInput=0,userValue,comValue;
    Block * block;
    bool RefreshDisplayData=false;
    public: bool AssignWindow(HWND window)
    {
        GetWindowInfo(window,&winfo);
        hdc= GetDC(window);
        height= winfo.rcClient.bottom-winfo.rcClient.top;
        width=winfo.rcClient.right-winfo.rcClient.left;
        return true;
    };
    bool initialize (void)
    {
        RECT welcome;
        welcome.top=height/3;
        welcome.left=width/4;
        welcome.bottom=2*height/3;
        welcome.right=3*width/4;

        HFONT font=CreateFont(0,0,0,0,FW_BOLD,0,0,0,ANSI_CHARSET,OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,DEFAULT_PITCH | FF_DONTCARE, TEXT("Tahoma"));
        SelectObject(hdc,font);
        HBRUSH greenbrush=CreateSolidBrush(RGB(150,180,0));
        FillRect(hdc,&welcome,greenbrush);
        DeleteObject(greenbrush);
        SetBkMode(hdc,TRANSPARENT);
        TextOut(hdc,width/2-7.5*LOWORD(GetDialogBaseUnits()),height/2-20,TEXT("Tick! Tack! Toe!"),16);
        return true;
    }
    bool deployTable()
    {
        static int flag=1;
        for(int i=1;i<nColoumn;i+=1)
        {
            rectH=height/nColoumn;
            rectW=width/nRow;
            MoveToEx(hdc,0,i*rectH,NULL);
            LineTo(hdc,width,i*rectH);
            MoveToEx(hdc,i*rectW,0,NULL);
            LineTo(hdc,i*rectW,height);
        }
        if(flag==1||RefreshDisplayData)
        {
            if(flag==1) block= new Block[nColoumn*nRow];
            for(int i=0;i<nColoumn*nRow;i++)
            {
                block[i+1].myRow=ceil((i+1.0)/nColoumn);
                block[i+1].myColoumn=(i+1)-(ceil((i+1.0)/nColoumn)-1)*nColoumn;

                /*block[i+1].OkRect.X=(block[i+1].myColoumn-0.25)*rectW;
                block[i+1].OkRect.Y=(block[i+1].myRow-0.25)*rectH;
                block[i+1].OkRect.Width=rectW/2;
                block[i+1].OkRect.Height=rectH/2;*/

                block[i+1].myRect.X=(block[i+1].myColoumn-1)*rectW;
                block[i+1].myRect.Y=(block[i+1].myRow-1)*rectH;
                block[i+1].myRect.Width=rectW;
                block[i+1].myRect.Height=rectH;

                block[i+1].ClrRect.X=(block[i+1].myColoumn-1)*rectW+1;
                block[i+1].ClrRect.Y=(block[i+1].myRow-1)*rectH+1;
                block[i+1].ClrRect.Width=rectW-1;
                block[i+1].ClrRect.Height=rectH-1;
            }
            RefreshDisplayData=false;
        }
        flag++;
        return true;
    }
};
