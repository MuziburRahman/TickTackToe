#ifndef TICKTACKTOE_ALGORITHM_H_INCLUDED
#define TICKTACKTOE_ALGORITHM_H_INCLUDED
#include<string>
#include<sstream>
#define END_OF_STRING -1

int ctoi(char x) //if fails, return an integer greater than 10;
{
    switch (x)
    {
        case '1':return 1;
        case '2':return 2;
        case '3':return 3;
        case '4':return 4;
        case '5':return 5;
        case '6':return 6;
        case '7':return 7;
        case '8':return 8;
        case '9':return 9;
        case '0':return 0;
        default: return 12;
    }
}
int stoi(string x)
{
    int y=0,b=x.length()-1;
    while(b>=0)
    {
        y=y+pow(10,b)*ctoi(x[x.length()-b-1]);
        b--;
    }
    return y;
}
string itos(double i)
{
	stringstream ss;
	ss <<i;
	return ss.str();
}

class data
{
public:
    string me;
    int totalNums;
    int* all;

    void append (int i)
    {
        me.append("-");
        me.append(itos(i));
        totalNums++;
    }
    int ExtractNumAfter(int* i)
    {
        int tobeReturned=0,p=0,q=0,j=*i;
        bool started=false;
        while(j<me.length())
        {
            if(me[j]>=48&&me[j]<=57)    // '0' er int man 48.... '9' er 57
            {
                if(!started)
                {
                    started=true;
                    p=j;
                }
            }
            else
            {
                if (started)
                {
                    q=j;
                    break;
                }
            }

            j++;
        }
        if(j>=me.length()&&!started) return END_OF_STRING;
        *i=j;
        return stoi(me.substr(p,q-p));
    }
    data(string s)
    {
        me=s;
        int i=0,j=0,l=0;
        while(ExtractNumAfter(&i)!=END_OF_STRING)
        {
            j++;
        }
        totalNums=j;
        all= new int[j];
        for(int k=0;k<totalNums;k++)
            all[k]=ExtractNumAfter(&l);
    }
    int ran()
    {
        return all[ran()%totalNums+1];
    }
};
class Algorithm
{
    int nr=0, nc=0;

    public:Block * blk;
    int totalInputs=0,totalNeighbours=0,userValue,comValue;
     bool init(int nRow, int nColoumn)
    {
        nr=nRow;nc=nColoumn;
        blk= new Block[nc*nr];
        for(int i=0;i<nColoumn*nRow;i++)
        {
            blk[i+1].myRow=ceil((i+1.0)/nColoumn);
            blk[i+1].myColoumn=(i+1)-(ceil((i+1.0)/nColoumn)-1)*nColoumn;
        }
        return true;
    };
    inline int rowOf(int j)
    {
        double i=j,nC=nc;
        return ceil(i/nC);
    }
    inline int coloumnOf(int i)
    {
        return i-(rowOf(i)-1)*nc;
    }
    inline int up(double i)
    {
        if(i>nc) return i-nc;
        else return 0;
    }
    inline int down(int i)
    {
        if(i<=nc*nr-nc) return i+nc;
        else return 0;
    }
    inline bool isCornered(int i)
    {
        if(i==1||i==nc||i==nc*nRow||i==nc*nr-nc+1) return true;
        return false;
    }
    bool isAlly(int i, int j)
    {
        if(coloumnOf(i)==coloumnOf(j)||rowOf(i)==rowOf(j)||rowOf(i)==coloumnOf(i)&&rowOf(j)==coloumnOf(j)||rowOf(i)+coloumnOf(i)==rowOf(j)+coloumnOf(j))
            return true;
        return false;
    }
    DWORD AllyType(int i,int j)
    {
        if(nc!=nr)
            return NOT_ALLY;
        if(coloumnOf(i)==coloumnOf(j))
            return SAME_COLOUMN;
        if(rowOf(i)==rowOf(j))
            return SAME_ROW;
        if(rowOf(i)==coloumnOf(i)&&rowOf(j)==coloumnOf(j))
            return CROSS_ALLY_TAN_NEG;
        if(rowOf(i)+coloumnOf(i)==rowOf(j)+coloumnOf(j))
            return CROSS_ALLY_TAN_POS;
        return NOT_ALLY;
    }
    int inputAtRow(int i)
    {
        int re=0;
        for(int j=(rowOf(i)-1)*nc+1;  j<=rowOf(i)*nc;  j++)
        {
            if(blk[j].value!=2)
                re++;
        }
        return re;
    }
    int inputAtColoumn(int i)
    {
        int re=0;
        for(int j=coloumnOf(i);j<=nr*nc-nc+coloumnOf(i);j+=nc)
        {
            if(blk[j].value!=2)
                re++;
        }
        return re;
    }
    /*void CloneAt(TickTackToe* ttt)
    {
        ttt=&me;
    }*/
    data neighbours_of(int i)
    {
        data s("");
        if(up(i)!=0) s.append(up(i));
        if(down(i)!=0) s.append(down(i));
        if(i%nc!=0) s.append(i+1);// right block
        if((i-1)%nc!=0) s.append(i-1);// left block
        if(up(i)!=0&&i%nc!=0) s.append(up(i)+1);
        if(down(i)!=0&&i%nc!=0) s.append(down(i)+1);// lower right block
        if((i-1)%nc!=0&&down(i)!=0) s.append(down(i)-1);// lower left block
        if(up(i)!=0&&(i-1)%nc!=0) s.append(up(i)-1);
        return s;
    }
    data giveDataOfRow(int i)
    {
        data re("");
        for(int j=(rowOf(i)-1)*nc+1;  j<=rowOf(i)*nc;  j++)
        {
            if(blk[j].value==2)
                re.append(j);
        }
        return re;
    }
    data giveDataOfCol(int i)
    {
        data re("");
        for(int j=coloumnOf(i);j<=nr*nc-nc+coloumnOf(i);j+=nc)
        {
            if(blk[j].value==2)
                re.append(j);
        }
        return re;
    }
    int output(int id)
    {
        int re_id=0;
        if(totalInputs==1)
            re_id=neighbours_of(id).ran();
        else
        {
            if(inputAtColoumn(coloumnOf(id))>1)
            {
                data col=giveDataOfCol(id);
                re_id=col.ran();
            }
        }
        blk[re_id].value=comValue;
        return re_id;
    }
};
#endif // TICKTACKTOE_ALGORITHM_H_INCLUDED
