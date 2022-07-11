#include <graphics.h>
#include <time.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <queue>
#include <math.h>
#define G 15  //长宽格数
#define S 50  //一格长宽（一般不改）
#define AD 0
#define WS 1
#define AW 2
#define DW 3
#define W 0
#define B 1

struct XY
{
    int X, Y;
};

IMAGE ch[15][2], board[10],menu[3];

int js;
int map[15][15];
int op1[13][4] = { {1,0,-1,2},{2,-1,0,1},{1,0,0,1},{2,-1,-1,2},{-1,2,-1,2},{2,-1,1,0},{2,-1,2,-1},{-2,3,0,1},{2,-1,-2,3},{1,0,-2,3},{-1,2,0,1},{-1,2,-2,3},{-2,3,-2,3} };
int op2[12][5] = { {1,0,1,1,0},{1,0,0,2,-1},{0,1,1,0,1},{0,1,2,0,1},{0,1,2,1,0},{0,1,2,-1,2},{0,1,1,-2,3},{0,1,1,-1,2},{0,1,0,-1,2},{0,1,0,-2,3},{0,1,2,2,-1},{0,1,2,-2,3} };
int score_b[10] = { 7,35,800,15000,3000000 };
int score_w[10] = { 0,15,400,2000,100000 };
void Load();
void Paint();
int Menu();
bool Start_PVE();
bool Start_PVP();
XY MiniQ();
long long Assess(int x, int y);
bool Win_Lost(int x, int y);
int main()
{
    MessageBox(NULL, TEXT("欢迎！"), TEXT("Welcom!"), MB_OK | MB_ICONINFORMATION);
    LOOP:
    int flag= Menu();
    bool result;
    do
    {
        initgraph(G * S + 40, G * S + 40);
        Load();
        Paint();
        if (flag)
            result = Start_PVE();
        else
            result = Start_PVP();
    } while (result);
    goto LOOP;
    closegraph();
    return 0;
}
void Load()
{
    setbkcolor(RGB(104, 152, 210));
    loadimage(&board[0], TEXT("./img/board.yjl"));
    loadimage(&board[1], TEXT("./img/board_a_w.yjl"));
    loadimage(&board[2], TEXT("./img/board_w.yjl"));
    loadimage(&board[3], TEXT("./img/board_d_w.yjl"));
    loadimage(&board[4], TEXT("./img/board_d.yjl"));
    loadimage(&board[5], TEXT("./img/board_d_s.yjl"));
    loadimage(&board[6], TEXT("./img/board_s.yjl"));
    loadimage(&board[7], TEXT("./img/board_a_s.yjl"));
    loadimage(&board[8], TEXT("./img/board_a.yjl"));
    loadimage(&ch[1][0], TEXT("./img/ch_w.yjl"));
    loadimage(&ch[2][0], TEXT("./img/ch_w_p.yjl"));
    loadimage(&ch[3][0], TEXT("./img/ch_w_p_a.yjl"));
    loadimage(&ch[4][0], TEXT("./img/ch_w_p_a_w.yjl"));
    loadimage(&ch[5][0], TEXT("./img/ch_w_p_w.yjl"));
    loadimage(&ch[6][0], TEXT("./img/ch_w_p_d_w.yjl"));
    loadimage(&ch[7][0], TEXT("./img/ch_w_p_d.yjl"));
    loadimage(&ch[8][0], TEXT("./img/ch_w_p_d_s.yjl"));
    loadimage(&ch[9][0], TEXT("./img/ch_w_p_s.yjl"));
    loadimage(&ch[10][0], TEXT("./img/ch_w_p_a_s.yjl"));
    loadimage(&ch[1][1], TEXT("./img/ch_b.yjl"));
    loadimage(&ch[2][1], TEXT("./img/ch_b_p.yjl"));
    loadimage(&ch[3][1], TEXT("./img/ch_b_p_a.yjl"));
    loadimage(&ch[4][1], TEXT("./img/ch_b_p_a_w.yjl"));
    loadimage(&ch[5][1], TEXT("./img/ch_b_p_w.yjl"));
    loadimage(&ch[6][1], TEXT("./img/ch_b_p_d_w.yjl"));
    loadimage(&ch[7][1], TEXT("./img/ch_b_p_d.yjl"));
    loadimage(&ch[8][1], TEXT("./img/ch_b_p_d_s.yjl"));
    loadimage(&ch[9][1], TEXT("./img/ch_b_p_s.yjl"));
    loadimage(&ch[10][1], TEXT("./img/ch_b_p_a_s.yjl"));
    loadimage(&menu[0], TEXT("./img/menu.yjl"));
    loadimage(&menu[1], TEXT("./img/menu_pvp.yjl"));
    loadimage(&menu[2], TEXT("./img/menu_pve.yjl"));
}
void Paint()
{
    cleardevice();
    setorigin(20, 20);
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            putimage(i * S, j * S, &board[0]);
        }
    }
    clearrectangle(0, 0, G * S, 23);
    clearrectangle(0, 0, 23, G * S);
    clearrectangle(0, G * S, G * S, G * S - 24);
    clearrectangle(G * S - 24, 0, G * S + 24, G * S);
}

bool Start_PVE()
{
    js = 0;
    memset(map, 0, sizeof(map));
    int fSelect = MessageBox(NULL, TEXT("想先下吗？"), TEXT("Tips"), MB_OKCANCEL | MB_ICONINFORMATION);
    if (fSelect == IDCANCEL)
    {
        MiniQ();
        js++;
    }
    MOUSEMSG m;
    int x = 5, y = 5;
    while (true)
    {
        // 获取一条鼠标消息
        m = GetMouseMsg();
        switch (m.uMsg)
        {
        case WM_MOUSEMOVE:
            if (m.x - 20 > G* S || m.y - 20 > G* S || m.x - 20 < 0 || m.y - 20 < 0)
                break;
            if (map[(m.y - 20) / S][(m.x - 20) / S] == 0 && (m.x - 20) > (m.x - 20) / S * S + 6 && (m.x - 20) < (m.x - 20) / S * S + 44 && (m.y - 20) > (m.y - 20) / S * S + 6 && (m.y - 20) < (m.y - 20) / S * S + 44)
            {
                if (map[y][x] == 0)
                {
                    if (x == 0 && y == 0)
                        putimage(x * S, y * S, &board[1]);
                    else if (x == 0 && y == 14)
                        putimage(x * S, y * S, &board[7]);
                    else if (x == 14 && y == 0)
                        putimage(x * S, y * S, &board[3]);
                    else if (x == 14 && y == 14)
                        putimage(x * S, y * S, &board[5]);
                    else if (x == 0)
                        putimage(x * S, y * S, &board[8]);
                    else if (x == 14)
                        putimage(x * S, y * S, &board[4]);
                    else if (y == 0)
                        putimage(x * S, y * S, &board[2]);
                    else if (y == 14)
                        putimage(x * S, y * S, &board[6]);
                    else
                        putimage(x * S, y * S, &board[0]);
                }
                ////////////////////////////////////////////////
                if ((m.x - 20) / S == 0 && (m.y - 20) / S == 0)
                    putimage((m.x - 20) / S * S + 6, (m.y - 20) / S * S + 6, &ch[4][0]);
                else if ((m.x - 20) / S == 0 && (m.y - 20) / S == 14)
                    putimage((m.x - 20) / S * S + 6, (m.y - 20) / S * S + 6, &ch[10][0]);
                else if ((m.x - 20) / S == 14 && (m.y - 20) / S == 0)
                    putimage((m.x - 20) / S * S + 6, (m.y - 20) / S * S + 6, &ch[6][0]);
                else if ((m.x - 20) / S == 14 && (m.y - 20) / S == 14)
                    putimage((m.x - 20) / S * S + 6, (m.y - 20) / S * S + 6, &ch[8][0]);
                else if ((m.x - 20) / S == 0)
                    putimage((m.x - 20) / S * S + 6, (m.y - 20) / S * S + 6, &ch[3][0]);
                else if ((m.x - 20) / S == 14)
                    putimage((m.x - 20) / S * S + 6, (m.y - 20) / S * S + 6, &ch[7][0]);
                else if ((m.y - 20) / S == 0)
                    putimage((m.x - 20) / S * S + 6, (m.y - 20) / S * S + 6, &ch[5][0]);
                else if ((m.y - 20) / S == 14)
                    putimage((m.x - 20) / S * S + 6, (m.y - 20) / S * S + 6, &ch[9][0]);
                else
                    putimage((m.x - 20) / S * S + 6, (m.y - 20) / S * S + 6, &ch[2][0]);
                x = (m.x - 20) / S, y = (m.y - 20) / S;
                ////////////////////////////////////////////////
            }
            else
            {
                if (map[y][x] == 0)
                {
                    if (x == 0 && y == 0)
                        putimage(x * S, y * S, &board[1]);
                    else if (x == 0 && y == 14)
                        putimage(x * S, y * S, &board[7]);
                    else if (x == 14 && y == 0)
                        putimage(x * S, y * S, &board[3]);
                    else if (x == 14 && y == 14)
                        putimage(x * S, y * S, &board[5]);
                    else if (x == 0)
                        putimage(x * S, y * S, &board[8]);
                    else if (x == 14)
                        putimage(x * S, y * S, &board[4]);
                    else if (y == 0)
                        putimage(x * S, y * S, &board[2]);
                    else if (y == 14)
                        putimage(x * S, y * S, &board[6]);
                    else
                        putimage(x * S, y * S, &board[0]);
                }
            }
            break;
        case WM_LBUTTONUP:
            if (m.x - 20 > G* S || m.y - 20 > G* S || m.x - 20 < 0 || m.y - 20 < 0 || map[(m.y - 20) / S][(m.x - 20) / S] != 0)
                break;
            if ((m.x - 20) > (m.x - 20) / S * S + 6 && (m.x - 20) < (m.x - 20) / S * S + 44 && (m.y - 20) > (m.y - 20) / S * S + 6 && (m.y - 20) < (m.y - 20) / S * S + 44)
            {
                putimage((m.x - 20) / S * S + 6, (m.y - 20) / S * S + 6, &ch[1][0]);
                map[(m.y - 20) / S][(m.x - 20) / S] = 1;
                js++;
                if (Win_Lost((m.x - 20) / S, (m.y - 20) / S))
                {
                    int nSelect = MessageBox(NULL, TEXT("恭喜您获胜了！要再来一局吗？"), TEXT("THE END"), MB_OKCANCEL | MB_ICONINFORMATION);
                    if (nSelect == IDOK)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                if (js == 225)
                {
                    int nSelect = MessageBox(NULL, TEXT("难得平局！要再来一局吗？"), TEXT("THE END"), MB_OKCANCEL | MB_ICONINFORMATION);
                    if (nSelect == IDOK)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                XY goal = MiniQ();
                js++;
                if (Win_Lost(goal.X, goal.Y))
                {
                    int nSelect = MessageBox(NULL, TEXT("哈哈哈！（来自电脑的无情嘲讽）要再来吗？"), TEXT("THE END"), MB_OKCANCEL | MB_ICONINFORMATION);
                    if (nSelect == IDOK)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                if (js == 225)
                {
                    int nSelect = MessageBox(NULL, TEXT("难得平局！要再来一局吗？"), TEXT("THE END"), MB_OKCANCEL | MB_ICONINFORMATION);
                    if (nSelect == IDOK)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
            }
            break;
        }
    }
}

bool Start_PVP()
{
    js = 0;
    int flag=0;
    memset(map, 0, sizeof(map));
    MessageBox(NULL, TEXT("白子先下"), TEXT("Tips"), MB_OK| MB_ICONINFORMATION);
    MOUSEMSG m;
    int x = 5, y = 5;
    while (true)
    {
        // 获取一条鼠标消息
        m = GetMouseMsg();
        switch (m.uMsg)
        {
        case WM_MOUSEMOVE:
            if (m.x - 20 > G* S || m.y - 20 > G* S || m.x - 20 < 0 || m.y - 20 < 0)
                break;
            if (map[(m.y - 20) / S][(m.x - 20) / S] == 0 && (m.x - 20) > (m.x - 20) / S * S + 6 && (m.x - 20) < (m.x - 20) / S * S + 44 && (m.y - 20) > (m.y - 20) / S * S + 6 && (m.y - 20) < (m.y - 20) / S * S + 44)
            {
                if (map[y][x] == 0)
                {
                    if (x == 0 && y == 0)
                        putimage(x * S, y * S, &board[1]);
                    else if (x == 0 && y == 14)
                        putimage(x * S, y * S, &board[7]);
                    else if (x == 14 && y == 0)
                        putimage(x * S, y * S, &board[3]);
                    else if (x == 14 && y == 14)
                        putimage(x * S, y * S, &board[5]);
                    else if (x == 0)
                        putimage(x * S, y * S, &board[8]);
                    else if (x == 14)
                        putimage(x * S, y * S, &board[4]);
                    else if (y == 0)
                        putimage(x * S, y * S, &board[2]);
                    else if (y == 14)
                        putimage(x * S, y * S, &board[6]);
                    else
                        putimage(x * S, y * S, &board[0]);
                }
                ////////////////////////////////////////////////
                if ((m.x - 20) / S == 0 && (m.y - 20) / S == 0)
                    putimage((m.x - 20) / S * S + 6, (m.y - 20) / S * S + 6, &ch[4][flag]);
                else if ((m.x - 20) / S == 0 && (m.y - 20) / S == 14)
                    putimage((m.x - 20) / S * S + 6, (m.y - 20) / S * S + 6, &ch[10][flag]);
                else if ((m.x - 20) / S == 14 && (m.y - 20) / S == 0)
                    putimage((m.x - 20) / S * S + 6, (m.y - 20) / S * S + 6, &ch[6][flag]);
                else if ((m.x - 20) / S == 14 && (m.y - 20) / S == 14)
                    putimage((m.x - 20) / S * S + 6, (m.y - 20) / S * S + 6, &ch[8][flag]);
                else if ((m.x - 20) / S == 0)
                    putimage((m.x - 20) / S * S + 6, (m.y - 20) / S * S + 6, &ch[3][flag]);
                else if ((m.x - 20) / S == 14)
                    putimage((m.x - 20) / S * S + 6, (m.y - 20) / S * S + 6, &ch[7][flag]);
                else if ((m.y - 20) / S == 0)
                    putimage((m.x - 20) / S * S + 6, (m.y - 20) / S * S + 6, &ch[5][flag]);
                else if ((m.y - 20) / S == 14)
                    putimage((m.x - 20) / S * S + 6, (m.y - 20) / S * S + 6, &ch[9][flag]);
                else
                    putimage((m.x - 20) / S * S + 6, (m.y - 20) / S * S + 6, &ch[2][flag]);
                x = (m.x - 20) / S, y = (m.y - 20) / S;
                ////////////////////////////////////////////////
            }
            else
            {
                if (map[y][x] == 0)
                {
                    if (x == 0 && y == 0)
                        putimage(x * S, y * S, &board[1]);
                    else if (x == 0 && y == 14)
                        putimage(x * S, y * S, &board[7]);
                    else if (x == 14 && y == 0)
                        putimage(x * S, y * S, &board[3]);
                    else if (x == 14 && y == 14)
                        putimage(x * S, y * S, &board[5]);
                    else if (x == 0)
                        putimage(x * S, y * S, &board[8]);
                    else if (x == 14)
                        putimage(x * S, y * S, &board[4]);
                    else if (y == 0)
                        putimage(x * S, y * S, &board[2]);
                    else if (y == 14)
                        putimage(x * S, y * S, &board[6]);
                    else
                        putimage(x * S, y * S, &board[0]);
                }
            }
            break;
        case WM_LBUTTONUP:
            if (m.x - 20 > G* S || m.y - 20 > G* S || m.x - 20 < 0 || m.y - 20 < 0 || map[(m.y - 20) / S][(m.x - 20) / S] != 0)
                break;
            if ((m.x - 20) > (m.x - 20) / S * S + 6 && (m.x - 20) < (m.x - 20) / S * S + 44 && (m.y - 20) > (m.y - 20) / S * S + 6 && (m.y - 20) < (m.y - 20) / S * S + 44)
            {
                putimage((m.x - 20) / S * S + 6, (m.y - 20) / S * S + 6, &ch[1][flag]);
                map[(m.y - 20) / S][(m.x - 20) / S] = flag?-1:1;
                js++;
                if (Win_Lost((m.x - 20) / S, (m.y - 20) / S))
                {
                    int nSelect;
                    if(flag)
                        nSelect = MessageBox(NULL, TEXT("黑方获胜！要再来一局吗？"), TEXT("THE END"), MB_OKCANCEL | MB_ICONINFORMATION);
                    else
                        nSelect = MessageBox(NULL, TEXT("白方获胜！要再来一局吗？"), TEXT("THE END"), MB_OKCANCEL | MB_ICONINFORMATION);
                    if (nSelect == IDOK)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                if (js == 225)
                {
                    int nSelect = MessageBox(NULL, TEXT("平局！要再来一局吗？"), TEXT("THE END"), MB_OKCANCEL | MB_ICONINFORMATION);
                    if (nSelect == IDOK)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                if (flag)
                    flag = 0;
                else
                    flag = 1;
            }
            break;
        }
    }
}

bool Win_Lost(int x, int y)
{
    int f = map[y][x];
    int ad = 0, ws = 0, aw = 0, dw = 0;
    for (int i = -4; i <= 0; i++)
    {
        if (x + i < 0 || x + i + 4> 14)
        {
            ad = -1, aw = -1, dw = -1;
        }
        if (y + i < 0 || y + i + 4>14)
        {
            ws = -1, aw = -1;
        }
        if (y - i > 14 || y - i - 4 < 0)
        {
            dw = -1;
        }
        if (ws == -1 && ad == -1)
        {
            ad = 0, ws = 0, aw = 0, dw = 0;
            continue;
        }
        for (int j = i; i - j >= -4; j++)
        {
            if (j == 0)
                continue;
            if (ad != -1)
            {
                if (map[y][x + j] == f)
                    ad++;
                else
                    ad = -1;
            }
            if (ws != -1)
            {
                if (map[y + j][x] == f)
                    ws++;
                else
                    ws = -1;
            }
            if (aw != -1)
            {
                if (map[y + j][x + j] == f)
                    aw++;
                else
                    aw = -1;
            }
            if (dw != -1)
            {
                if (map[y - j][x + j] == f)
                    dw++;
                else
                    dw = -1;
            }
        }
        if (ad == 4 || ws == 4 || aw == 4 || dw == 4)
            return true;
        else
            ad = 0, ws = 0, aw = 0, dw = 0;
    }
    return false;
}

long long Assess(int x, int y)
{
    long long total = 0;
    int dir[4][3] = { 0 };
    for (int i = -4; i <= 0; i++)
    {
        if (x + i < 0 || x + i + 4> 14)
        {
            dir[AD][2] = -1, dir[AW][2] = -1, dir[DW][2] = -1;
        }
        if (y + i < 0 || y + i + 4>14)
        {
            dir[WS][2] = -1, dir[AW][2] = -1;
        }
        if (y - i > 14 || y - i - 4 < 0)
        {
            dir[DW][2] = -1;
        }
        if (dir[WS][2] == -1 && dir[AD][2] == -1)
        {
            memset(dir, 0, sizeof(dir));
            continue;
        }
        for (int j = i; i - j >= -4; j++)
        {
            if (j == 0)
                continue;
            if (dir[AD][2] != -1)
            {
                if (map[y][x + j] == -1)
                    dir[AD][B]++;
                else if (map[y][x + j] == 1)
                    dir[AD][W]++;
            }
            if (dir[WS][2] != -1)
            {
                if (map[y + j][x] == -1)
                    dir[WS][B]++;
                else if (map[y + j][x] == 1)
                    dir[WS][W]++;
            }
            if (dir[AW][2] != -1)
            {
                if (map[y + j][x + j] == -1)
                    dir[AW][B]++;
                else if (map[y + j][x + j] == 1)
                    dir[AW][W]++;
            }
            if (dir[DW][2] != -1)
            {
                if (map[y - j][x + j] == -1)
                    dir[DW][B]++;
                else if (map[y - j][x + j] == 1)
                    dir[DW][W]++;
            }
        }
        for (int k = 0; k < 4; k++)
        {
            if ((dir[k][B] != 0 && dir[k][W] != 0) || dir[k][2] == -1)
                continue;
            else
            {
                total += (long long)score_b[dir[k][B]] + (long long)score_w[dir[k][W]];
            }
        }
        memset(dir, 0, sizeof(dir));
    }
    return total;
}

int Choose(int n)
{
    srand((unsigned)time(NULL));
    return rand() % n;
}

XY MiniQ()
{
    XY xy = { 7,7 };
    if (js == 0)
    {
        map[xy.Y][xy.X] = -1;
        putimage(xy.X * S + 6, xy.Y * S + 6, &ch[1][1]);
        return xy;
    }
    if (js == 2)
    {
        printf("0\n");
        XY w1 = { 7,7 }, b1 = { 7,7 };
        int a;
        for (int i = 0; i < 15; i++)
        {
            for (int j = 0; j < 15; j++)
            {
                if (map[i][j] == 1)
                    w1.X = j, w1.Y = i;
                if (map[i][j] == -1)
                    b1.X = j, b1.Y = i;
            }
        }
        if (abs(b1.X - w1.X) == 1 && abs(b1.Y - w1.Y) == 1)
        {
            a = Choose(13);
            xy.X = op1[a][0] * w1.X + op1[a][1] * b1.X;
            xy.Y = op1[a][2] * w1.Y + op1[a][3] * b1.Y;
            map[xy.Y][xy.X] = -1;
            putimage(xy.X * S + 6, xy.Y * S + 6, &ch[1][1]);
            return xy;
        }
        else if (b1.X == w1.X && abs(b1.Y - w1.Y) == 1)
        {
            a = Choose(12);
            xy.X = op2[a][0] * w1.X + op2[a][1] * b1.X + (b1.X >= 7 ? -op2[a][2] : op2[a][2]);
            xy.Y = op2[a][3] * w1.Y + op2[a][4] * b1.Y;
            map[xy.Y][xy.X] = -1;
            putimage(xy.X * S + 6, xy.Y * S + 6, &ch[1][1]);
            return xy;
        }
        else if (abs(b1.X - w1.X) == 1 && b1.Y == w1.Y)
        {
            a = Choose(12);
            xy.Y = op2[a][0] * w1.Y + op2[a][1] * b1.Y + (b1.Y >= 7 ? -op2[a][2] : op2[a][2]);
            xy.X = op2[a][3] * w1.X + op2[a][4] * b1.X;
            map[xy.Y][xy.X] = -1;
            putimage(xy.X * S + 6, xy.Y * S + 6, &ch[1][1]);
            return xy;
        }
    }
    long long max = 0;
    long long score = 0;
    std::queue<XY> qscore;
    for (int i = 0; i < 15; i++)//y
    {
        for (int j = 0; j < 15; j++)//x
        {
            if (map[i][j] == 0)
            {
                score = Assess(j, i);
                xy = { j,i };
                if (score > max)
                {
                    max = score;
                    while (qscore.size())
                    {
                        qscore.pop();
                    }
                    qscore.push(xy);
                }
                else if (score == max)
                {
                    qscore.push(xy);
                }
            }
        }
    }
    int n = Choose((int)qscore.size());
    for (int l = 1; l <= n; l++)
    {
        if (!qscore.empty())
            qscore.pop();
    }
    xy = { qscore.front().X ,qscore.front().Y };
    map[xy.Y][xy.X] = -1;
    putimage(xy.X * S + 6, xy.Y * S + 6, &ch[1][1]);
    return xy;
}

int Menu()
{
    initgraph(500, 500);
    loadimage(&menu[0], TEXT("./img/menu.yjl"));
    loadimage(&menu[1], TEXT("./img/menu_pvp.yjl"));
    loadimage(&menu[2], TEXT("./img/menu_pve.yjl"));
    putimage(0, 0, &menu[0]);
    MOUSEMSG m;
    while (true)
    {
        m = GetMouseMsg();
        if (m.uMsg == WM_MOUSEMOVE)
        {
            if (m.y > 100)
            {
                putimage(0, 0, &menu[2]);
            }
            else
            {
                putimage(0, 0, &menu[1]);
            }
        }
        else if (m.uMsg == WM_LBUTTONUP)
        {
            if (m.y > 100)
            {
                closegraph();
                return 1;
            }
            else
            {
                closegraph();
                return 0;
            }
        }
    }
    closegraph();
}