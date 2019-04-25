#ifndef WINDOWSIMPLIFIED_H_INCLUDED
using namespace std;
bool error(string s)
{
    if(s=="") s="Something has gone wrong!";
    MessageBox(NULL,s.c_str(),"Error!",MB_OK);
}
void ErrorAndExit(string s)
{
    if(s=="") s="Sorry for your inconvenience,but something has gone wrong! Unavoidable exit.";
    MessageBox(NULL,s.c_str(),"Error!",MB_OK);
    exit(0);
}
string lpstrtostring(LPSTR x)
{
    string s;
    s.clear();
    s.insert(0,x);
    return s;
}
LPSTR strtolpstr(string x)
{
    LPSTR y=const_cast<char*>(x.c_str());
    return y;
}
BOOL MouseIsOnWindow(HWND hwnd)
{
    RECT rect;
    GetWindowRect(hwnd,&rect);
    POINT p;
    GetCursorPos(&p);
    if(PtInRect(&rect,p)) return true;
    else return false;
}
bool MouseIsOnRgn(HRGN h)
{
    POINT p;
    GetCursorPos(&p);
    if(PtInRegion(h,p.x,p.y)) return true;
    else return false;
}
bool GetText(HWND h, string* s)
{
    LPSTR l;
    l=(LPSTR)GlobalAlloc(GPTR,GetWindowTextLength(h)+1);
    GetWindowText(h,l,GetWindowTextLength(h)+1);
    *s=lpstrtostring(l);
    GlobalFree(l);
    return true;
}
#define WINDOWSIMPLIFIED_H_INCLUDED
#endif // WINDOWSIMPLIFIED_H_INCLUDED
