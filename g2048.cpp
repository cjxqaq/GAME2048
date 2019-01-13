#include"g2048.h" 
#include<iostream> 
#include<ctime> 
#include<cstdlib> //这两个库与随机数有关
#include<iomanip> //不认识
using namespace std;
G2048::G2048()
{
	initPane();
}

G2048::~G2048() { }

void G2048::rotate90(Pane& p1, Pane& p2)//顺时针90度
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			p2[i][j] = p1[j][i];
		}//转置
	}
	rotate180(p2, p1);//左右翻转
}

void G2048::rotate_90(Pane& p1, Pane& p2)// 逆时针90度
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			p2[i][j] = p1[j][i];
		}
	}
	rotate_180(p2, p1);//上下翻转
}
void G2048::rotate180(Pane& p1, Pane& p2) //左右翻转
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			p2[i][j] = p1[i][SIZE - j - 1];
		}
	}
}
void G2048::rotate_180(Pane& p1, Pane& p2) //上下翻转
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			p2[i][j] = p1[SIZE - i - 1][j];
		}
	}
}

bool G2048::gameOver() //所有元素均不为0时结束
{
	int tmp = 0; for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			tmp += (pane1[i][j]) ? 0 : 1;
		}
	}
	if (!tmp) return true;
	else return false;
}

bool G2048::isEmpty(int x, int y)
{
	return (!pane1[x][y]) ? true : false;
}
int G2048::generateNum(int x, int y)
{
	if (isEmpty(x, y)) return rand() % 2 ? 2 : 4; else return 0;
}//以上两个函数在随机空白处生成新数字

void G2048::initPane()
{
	srand(time(NULL));
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			pane1[i][j] = 0; pane2[i][j] = 0;
		}
	}
	score = 0;
}//游戏盘初始化，播种随机数

void G2048::disPane(Pane& p)
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			cout << setw(4) << p[i][j] << ' ';
		} cout << endl;
	}
}//打印矩阵

void G2048::copyP1P2()
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			pane1[i][j] = pane2[i][j];
		}
	}
}
void G2048::showPane(Pane& p)
{
	disPane(p);
	resetScore();
	cout << "score:" << getScore(p) << endl;
}
int G2048::getScore(Pane& p)
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			score += p[i][j];
		}
	}
	return score;
}

void G2048::resetScore() { score = 0; }

void G2048::coreAlgorithm(Pane& p)//最核心的算法，左移函数，其他方向移动通过矩阵变换实现，整个游戏的关键所在，
{
	for (int i = 0; i < SIZE; i++)//对于每一行
	{
		for (int j = SIZE - 1; j > 0; j--)
		{
			if (p[i][j - 1] == 0)//如果某元素为0（从倒数第二个开始）
			{
				int k = j - 1;
				for (; k < SIZE - 1; k++)//把0元素后面的元素向左移一格
				{
					p[i][k] = p[i][k + 1];
				}
				p[i][k] = 0;
			}
			else if (p[i][j - 1] == p[i][j])//如果该元素与其右侧的元素相等
			{
				p[i][j - 1] *= 2;//乘2
				int k = j;
				for (; k < SIZE - 1; k++)//把该元素后面的后面的元素向左移一格（没有多打字）
				{
					p[i][k] = p[i][k + 1];
				}
				p[i][k] = 0;
			}
		}
	}
}
void G2048::handleOps(Direction& d)
{
	if (d == QUIT)
	{
		cout << endl;
		cout << "Quit!" << endl; 
		exit(0);
	}
	switch (d)
	{
	case UP:
	{
		rotate_90(pane1, pane2);//逆时针转90度
		coreAlgorithm(pane1);//左移处理
		rotate90(pane1, pane2);//顺时针90度还原
	} break;
	//通过矩阵变换使所有移动变为向左移动，再还原，以下类似（整个游戏的关键代码）
	case DOWN:
	{
		rotate90(pane1, pane2);
		coreAlgorithm(pane1);
		rotate_90(pane1, pane2);
	}break;

	case LEFT:
	{
		coreAlgorithm(pane1);
	}break;

	case RIGHT:
	{
		rotate180(pane1, pane2);
		coreAlgorithm(pane2);
		rotate180(pane2, pane1);
	}break;
	}
}
void G2048::run()
{
	cout << "game start..." << endl;
	cout << "-------------" << endl;
	while (true)
	{
		if (gameOver())
		{
			system("cls");//清屏
			cout << "-----game over-----" << endl;
			showPane(pane1);
			exit(0);
		}
		int tmpx = 0;
		int tmpy = 0;
		int num = 0;
		while (!num) //随机尝试某个位置，直到该位置为0，使其数值为2或4
		{
			tmpx = rand() % SIZE;
			tmpy = rand() % SIZE;
			num = generateNum(tmpx, tmpy);
			if (num) setNum(pane1, tmpx, tmpy, num);
		}
		showPane(pane1);
		cout << "---------" << endl;
		cout << "please enter the direction:-> | <- (w,s,a,d)" << endl;
		Direction d;
		char dirt;
		while (cin.good())
		{
			cin.clear();
			dirt = cin.get();
			switch (dirt)
			{
			case 'w':d = UP; break;
			case 's':d = DOWN; break;
			case 'a':d = LEFT; break;
			case 'd':d = RIGHT; break;
			case 'q':d = QUIT; break;
			default: d = ERR; break;
			}
			if(d == ERR)
			{
				char ch;
				while ((ch = cin.get()) != '\n')continue;
				cin.clear();//如果输入不正确字符，去除并清除错误状态
			}
			else
			{	
				system("cls");
				handleOps(d);
				char ch;
				while ((ch = cin.get()) != '\n')continue;
				cin.clear();
				break;
			}
		}
	}
	
	showPane(pane1);
	cout << "-----------------------" << endl<<endl;
}

void G2048::setNum(Pane& p, int x, int y, int num)
{
	p[x][y] = num;
}