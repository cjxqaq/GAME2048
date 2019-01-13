#include"g2048.h" 
#include<iostream> 
#include<ctime> 
#include<cstdlib> //����������������й�
#include<iomanip> //����ʶ
using namespace std;
G2048::G2048()
{
	initPane();
}

G2048::~G2048() { }

void G2048::rotate90(Pane& p1, Pane& p2)//˳ʱ��90��
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			p2[i][j] = p1[j][i];
		}//ת��
	}
	rotate180(p2, p1);//���ҷ�ת
}

void G2048::rotate_90(Pane& p1, Pane& p2)// ��ʱ��90��
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			p2[i][j] = p1[j][i];
		}
	}
	rotate_180(p2, p1);//���·�ת
}
void G2048::rotate180(Pane& p1, Pane& p2) //���ҷ�ת
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			p2[i][j] = p1[i][SIZE - j - 1];
		}
	}
}
void G2048::rotate_180(Pane& p1, Pane& p2) //���·�ת
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			p2[i][j] = p1[SIZE - i - 1][j];
		}
	}
}

bool G2048::gameOver() //����Ԫ�ؾ���Ϊ0ʱ����
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
}//������������������հ״�����������

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
}//��Ϸ�̳�ʼ�������������

void G2048::disPane(Pane& p)
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			cout << setw(4) << p[i][j] << ' ';
		} cout << endl;
	}
}//��ӡ����

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

void G2048::coreAlgorithm(Pane& p)//����ĵ��㷨�����ƺ��������������ƶ�ͨ������任ʵ�֣�������Ϸ�Ĺؼ����ڣ�
{
	for (int i = 0; i < SIZE; i++)//����ÿһ��
	{
		for (int j = SIZE - 1; j > 0; j--)
		{
			if (p[i][j - 1] == 0)//���ĳԪ��Ϊ0���ӵ����ڶ�����ʼ��
			{
				int k = j - 1;
				for (; k < SIZE - 1; k++)//��0Ԫ�غ����Ԫ��������һ��
				{
					p[i][k] = p[i][k + 1];
				}
				p[i][k] = 0;
			}
			else if (p[i][j - 1] == p[i][j])//�����Ԫ�������Ҳ��Ԫ�����
			{
				p[i][j - 1] *= 2;//��2
				int k = j;
				for (; k < SIZE - 1; k++)//�Ѹ�Ԫ�غ���ĺ����Ԫ��������һ��û�ж���֣�
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
		rotate_90(pane1, pane2);//��ʱ��ת90��
		coreAlgorithm(pane1);//���ƴ���
		rotate90(pane1, pane2);//˳ʱ��90�Ȼ�ԭ
	} break;
	//ͨ������任ʹ�����ƶ���Ϊ�����ƶ����ٻ�ԭ���������ƣ�������Ϸ�Ĺؼ����룩
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
			system("cls");//����
			cout << "-----game over-----" << endl;
			showPane(pane1);
			exit(0);
		}
		int tmpx = 0;
		int tmpy = 0;
		int num = 0;
		while (!num) //�������ĳ��λ�ã�ֱ����λ��Ϊ0��ʹ����ֵΪ2��4
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
				cin.clear();//������벻��ȷ�ַ���ȥ�����������״̬
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