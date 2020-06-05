#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <vector>
using namespace std;
#define MAXS 50
int NONE[MAXS] = { 0 };
int visit[MAXS] = { 0 };
string strings;// 产生式
string Vn;// 非终结符
string Vt;// 终结符
string first[MAXS];// 用于存放每个终结符的first 集
string First[MAXS];// 用于存放每个非终结符的First 集
string Follow[MAXS]; // 用于存放每个非终结符的Follow 集
int N = 0;// 产生式个数
char initialstate;
int character = 0;
struct LRS
{
	string left;
	string right;
	string select;
};
class First_Follow_Select_LL
{
public:
	void VNVT(LRS *p);
	string Letter_First(LRS *p, char ch);
	string Letter_Follow(LRS *p, char ch);
	void Letter_Select(LRS *p);
	void Open_File(int &n, LRS *p);
	void Display(LRS *p);
	int Judge_LL(LRS *p);
	void Select_Show(LRS *p);
	void Change_LL(LRS *p, int info);
	void Combine(LRS *p);
	void Extract(LRS *p);
	int Easy_Judge(LRS *p);
};
//求VN 和VT
void First_Follow_Select_LL::VNVT(LRS *p)
{
	int i, j;
	Vn.clear();
	Vt.clear();
	for (i = 0; i < MAXS; i++)
	{
		first[i].clear();
		First[i].clear();
		Follow[i].clear();
	}
	for (i = 0; i < N; i++)
	{
		if ((p[i].left[0] >= 'A'&&p[i].left[0] <= 'Z'))
			if (Vn.find(p[i].left[0]) == string::npos)
				Vn += p[i].left[0];
		for (j = 0; j < p[i].right.size(); j++)
		{
			if (p[i].right[j] >= 'A'&&p[i].right[j] <= 'Z')
			{
				if (Vn.find(p[i].right[j]) == string::npos)
					Vn += p[i].right[j];
			}
			else
			{
				if (Vt.find(p[i].right[j]) == string::npos)
					Vt += p[i].right[j];
			}

		}
	}
}
//对每个文法符号求first 集
string First_Follow_Select_LL::Letter_First(LRS *p, char ch)
{
	int t;

	if (Vt.find(ch) != string::npos)
	{
		first[Vt.find(ch)] = ch;
		return first[Vt.find(ch)];
	}
	if (Vn.find(ch) != string::npos)
	{
		visit[Vn.find(ch)]++;
		if (visit[Vn.find(ch)] == 2)
		{
			visit[Vn.find(ch)] = 1;
			return First[Vn.find(ch)];
		}
		for (int i = 0; i < N; i++)
		{
			if (p[i].left[0] == ch)
			{
				if (Vt.find(p[i].right[0]) != string::npos)
				{
					if (First[Vn.find(ch)].find(p[i].right[0]) == string::npos)
					{
						First[Vn.find(ch)] += p[i].right[0];
					}
				}
				if (p[i].right[0] == '*')
				{
					if (First[Vn.find(ch)].find('*') == string::npos)
					{
						First[Vn.find(ch)] += '*';
					}
				}
				if (Vn.find(p[i].right[0]) != string::npos)
				{
					if (p[i].right.size() == 1)
					{
						string ff;
						if (First[Vn.find(p[i].right[0])] != "")
						{
							ff = First[Vn.find(p[i].right[0])];
						}
						else
						{
							ff = Letter_First(p, p[i].right[0]);
							visit[Vn.find(p[i].right[0])]++;
						}
						for (int ii = 0; ii < ff.size(); ii++)
						{
							if (First[Vn.find(ch)].find(ff[ii]) == string::npos)
							{
								First[Vn.find(ch)] += ff[ii];
							}
						}
					}
					else
					{

						for (int j = 0; j < p[i].right.size(); j++)
						{
							if (p[i].right[0] == p[i].left[0])
								break;
							string TT;
							if (First[Vn.find(p[i].right[j])] != "")
							{
								TT = First[Vn.find(p[i].right[j])];
							}
							else
							{
								TT = Letter_First(p, p[i].right[j]);
								visit[Vn.find(p[i].right[j])]++;
							}

							if (TT.find('*') != string::npos && (j + 1) < p[i].right.size())
							{
								sort(TT.begin(), TT.end());
								string tt;
								for (int t = 1; t < TT.size(); t++)
								{
									tt += TT[t];
								}
								TT = tt;
								tt = "";
								for (t = 0; t < TT.size(); t++)
								{
									if (First[Vn.find(ch)].find(TT[t]) == string::npos)
									{
										First[Vn.find(ch)] += TT[t];
									}
								}
							}
							else
							{
								for (t = 0; t < TT.size(); t++)
								{
									if (First[Vn.find(ch)].find(TT[t]) == string::npos)
									{
										First[Vn.find(ch)] += TT[t];
									}
								}
								break;
							}
						}
					}
				}
			}
		}
		return First[Vn.find(ch)];
	}
}
// 求每个非终结符的Follow 集
string First_Follow_Select_LL::Letter_Follow(LRS *p, char ch)
{
	int t, k;
	NONE[Vn.find(ch)]++;
	if (NONE[Vn.find(ch)] == 2)
	{
		NONE[Vn.find(ch)] = 1;
		return Follow[Vn.find(ch)];
	}
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < p[i].right.size(); j++)
		{
			if (p[i].right[j] == ch)
			{
				if (j + 1 == p[i].right.size())
				{
					string gg;
					gg = Letter_Follow(p, p[i].left[0]);
					NONE[Vn.find(p[i].left[0])] = 1;
					for (int k = 0; k < gg.size(); k++)
					{
						if (Follow[Vn.find(ch)].find(gg[k]) == string::npos)
						{
							Follow[Vn.find(ch)] += gg[k];
						}
					}
				}
				else
				{
					string FF;
					for (int jj = j + 1; jj < p[i].right.length(); jj++)
					{
						string TT;
						TT = Letter_First(p, p[i].right[jj]);
						if ((TT.find('*') != string::npos) && (jj + 1) < p[i].right.size())
						{
							sort(TT.begin(), TT.end());
							string tt;
							for (int t = 1; t < TT.size(); t++)
							{
								tt += TT[t];
							}
							TT = tt;
							tt = "";
							for (t = 0; t < TT.size(); t++)
							{
								if (FF.find(TT[t]) == string::npos)
									//&& TT[t] != '*'
								{
									FF += TT[t];
								}
							}
						}
						else
						{
							for (t = 0; t < TT.size(); t++)
							{
								if (FF.find(TT[t]) == string::npos)
								{
									FF += TT[t];
								}
							}
							break;
						}
					}
					if (FF.find('*') == string::npos)
					{
						for (k = 0; k < FF.size(); k++)
						{
							if (Follow[Vn.find(ch)].find(FF[k]) == string::npos)
							{
								Follow[Vn.find(ch)] += FF[k];
							}
						}
					}
					else
					{
						for (k = 0; k < FF.size(); k++)
						{
							if ((Follow[Vn.find(ch)].find(FF[k]) == string::npos) && FF[k] != '*')
							{
								Follow[Vn.find(ch)] += FF[k];
							}
						}
						string dd;
						dd = Letter_Follow(p, p[i].left[0]);
						NONE[Vn.find(p[i].left[0])] = 1;
						for (k = 0; k < dd.size(); k++)
						{
							if (Follow[Vn.find(ch)].find(dd[k]) == string::npos)
							{
								Follow[Vn.find(ch)] += dd[k];
							}
						}
					}
				}
			}
		}
	}
	return Follow[Vn.find(ch)];
}
//打开文件，并将读入的文法显示出来
void First_Follow_Select_LL::Open_File(int &n, LRS *p)
{
	string temp;
	int i;
	ifstream myfile("C://Users//88304//Desktop//lixiang.txt");
	if (!myfile)
	{
		cout << "你所要打开的文件不存在" << endl;
	}
	getline(myfile, temp);
	initialstate = temp[0];
	temp.clear();
	while (!myfile.eof())
	{
		getline(myfile, temp);
		p[n].left = temp.substr(0, 1);
		p[n].right = temp.substr(3, temp.size() - 3);
		n++;
		temp.clear();
	}
	myfile.close();
	cout << "您输入的文法是" << endl;
	for (i = 0; i < N; i++)
	{
		cout << p[i].left << "->" << p[i].right << endl;
	}
}
//求每一个产生式的select集
void First_Follow_Select_LL::Letter_Select(LRS *p)
{
	int i, j, k;
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < p[i].right.size(); j++)
		{
			if (Vt.find(p[i].right[0]) != string::npos)
			{
				if (p[i].right[0] != '*')
					p[i].select = p[i].right[0];
				else
					p[i].select = Follow[Vn.find(p[i].left[0])];
				break;
			}
			else if (Vn.find(p[i].right[j]) != string::npos)
			{
				if (First[Vn.find(p[i].right[j])].find("*") == string::npos)
				{
					p[i].select += First[Vn.find(p[i].right[j])];
					break;
				}
				else
				{
					for (k = 0; k < First[Vn.find(p[i].right[j])].size(); k++)
					{
						if (First[Vn.find(p[i].right[j])][k] != '*'&&p[i].select.find(First[Vn.find(p[i].right[j])][k]) == string::npos)
							p[i].select += First[Vn.find(p[i].right[j])][k];
					}
					if (j + 1 == p[i].right.size())
						for (k = 0; k < Follow[Vn.find(p[i].left[0])].size(); k++)
						{
							if (p[i].select.find(Follow[Vn.find(p[i].left[0])][k]) == string::npos)
								p[i].select += Follow[Vn.find(p[i].left[0])][k];
						}
				}
			}
		}
	}
}
//将select集显示出来
void First_Follow_Select_LL::Select_Show(LRS *p)
{
	int i, j;
	string temp = "{";
	string temp1;
	cout << setw(10) << "文法" << setw(22) << "SELECT" << endl;
	for (i = 0; i < N; i++)
	{
		if (p[i].select.size() != 0)
		{
			for (j = 0; j + 1 < p[i].select.size(); j++)
			{
				temp += p[i].select[j];
				temp.append(",");
			}
			temp += p[i].select[p[i].select.size() - 1];
			temp.append("}");
			temp1 = p[i].left + "->" + p[i].right;
			cout << setw(10) << temp1 << setw(22) << temp << endl;
			temp = "{";
			temp1 = "";
		}
		else
		{
			temp1 = p[i].left + "->" + p[i].right;
			cout << setw(10) << temp1 << setw(22) << "不存在" << endl;
			temp1 = "";
		}
	}
	cout << "--------------------------------------------------------------" << endl;
}
//将结果进行显示
void First_Follow_Select_LL::Display(LRS *p)
{
	int i, j, k;
	cout << endl;
	cout << "--------------------------------------------------------------" << endl;
	cout << "   Vn   " << setw(18) << "FIRST" << setw(22) << "FOLLOW" << endl;
	Follow[Vn.find(initialstate)] += '*';
	for (i = 0; i < Vn.size(); i++)
	{
		cout << "   " << Vn[i];
		string temp1, temp2, temp3 = "{";
		temp1 = Letter_First(p, Vn[i]);
		if (temp1 != "")
		{
			for (j = 0; j + 1 < temp1.size(); j++)
			{
				temp3 += temp1[j];
				temp3.append(",");
			}
			temp3 += temp1[temp1.size() - 1];
			temp3.append("}");
			cout << setw(22) << temp3;
			temp3 = "{";
		}
		else
		{
			cout << setw(22) << "不存在！";
		}
		temp2 = Letter_Follow(p, Vn[i]);
		//if (temp2[0] == '#')
		//temp2[0] = '*';
		for (k = 0; k < Vn.size(); k++)
		{
			NONE[k] = 0;
			visit[k] = 0;
		}

		for (k = 0; k + 1 < temp2.size(); k++)
		{
			//if (temp2[k] == '#')
			//temp2[k] = '*';
			temp3 += temp2[k];
			temp3.append(",");
		}
		if (temp2.size() != 0)
		{
			temp3 += temp2[temp2.size() - 1];
			temp3.append("}");
		}
		else
		{
			temp3 += "不存在}";
		}
		cout << setw(22) << temp3 << endl;
	}
	//for (i = 0; i < Vn.size(); i++)
	//for (j = 0; j < Follow[i].size(); j++)
	//if (Follow[i][j] == '#')
	//Follow[i][j] = '*';
	cout << "--------------------------------------------------------------" << endl;
}
//判断是不是LL（1）文法
int First_Follow_Select_LL::Judge_LL(LRS *p)
{
	int i, j, k;
	string temp1 = "";
	for (i = 0; i < Vn.size(); i++)
	{
		for (j = 0; j < N; j++)
		{
			if (p[j].left[0] == Vn[i])
			{
				if (temp1 == "")
					temp1 = p[j].select;
				else
				{
					for (k = 0; k < temp1.size(); k++)
					{
						if (p[j].select.find(temp1[k]) != string::npos)
						{
							cout << "该文法不是LL（1）文法！" << endl;
							return 0;
						}
					}
				}
			}
		}
		temp1 = "";
	}
	cout << "该文法是LL（1）文法！" << endl;
	return 1;
}
//重新整合文法的存储，去除空串，去除无用产生式，去除经不同转换转换出的相同的产生式，将select置空
void First_Follow_Select_LL::Combine(LRS *p)
{
	int i, j;
	string temp;
	temp += initialstate;
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < p[i].right.size(); j++)
		{
			if (temp.find(p[i].right[j]) == string::npos&&p[i].right[j] >= 'A'&&p[i].right[j] <= 'Z')
			{
				temp += p[i].right[j];
			}
		}
	}
	for (i = 0; i < N; i++)
	{
		if (temp.find(p[i].left[0]) == string::npos)
		{
			p[i].left = "";
			p[i].right = "";
		}
	}
	temp.clear();
	temp += initialstate;
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < p[i].right.size(); j++)
		{
			if (temp.find(p[i].right[j]) == string::npos&&p[i].right[j] >= 'A'&&p[i].right[j] <= 'Z')
			{
				temp += p[i].right[j];
			}
		}
	}
	for (i = 0; i < N; i++)
	{
		if (p[i].left != "")
			for (j = i + 1; j < N; j++)
			{
				if (p[i].left == p[j].left&&p[i].right == p[j].right)
				{
					p[j].left = "";
					p[j].right = "";
				}
			}
	}
	LRS *q = new LRS[MAXS];
	for (i = 0, j = 0; i < N; i++)
	{
		if (p[i].left != "")
		{
			q[j].left = p[i].left;
			q[j].right = p[i].right;
			q[j].select = "";
			j++;
		}
	}
	N = j;
	int count = 0;
	for (j = 0; j < temp.size(); j++)
	{
		for (i = 0; i < N; i++)
		{
			if (q[i].left[0] == temp[j])
			{
				p[count].left = q[i].left;
				p[count].right = q[i].right;
				p[count].select = q[i].select;
				count++;
			}
		}
	}
	N = count;	
	delete[]q;
}
//将非LL（1）文法转换成LL（1)文法
void First_Follow_Select_LL::Change_LL(LRS *p, int info)
{
	char q[20];
	int i, j, count = 1, count1 = N, flag = 0, m, x;
	q[0] = p[0].left[0];
	for (i = 1; i < N; i++)
	{
		for (j = 0; j < i; j++)
		{
			if (p[i].left == p[j].left)
				break;
		}
		if (j == i)
			q[count++] = p[i].left[0];
	}
	count--;
	for (i = 0; i < N; i++)// 判断第一个非终结符是否存在直接左递归
		if (p[i].left[0] == q[0] && p[i].left[0] == p[i].right[0])
			flag++;
	if (flag != 0)// 消除第一个非终结符的直接左递归
	{
		for (i = 0; i < N; i++)
		{
			if (p[i].left[0] == q[0])
			{
				while (Vn.find(char(character + 65)) != string::npos)
					character++;
				if (p[i].left[0] == p[i].right[0])
				{
					//p[i].left = p[i].left + "'";
					p[i].left = char(character + 65);
					p[i].right = p[i].right.substr(1, p[i].right.length()) + p[i].left;
				}
				else
					//p[i].right = p[i].right + p[i].left + "'";
					p[i].right = p[i].right + char(character + 65);
			}
		}
		//p[count1].left = p[0].left;
		p[count1].left = char(character + 65);
		p[count1++].right = "*";// 用*代替空产生式
		character++;
	}
	//替换第一个非终结符的间接左递归
	if (info == 1)
	{
		for (i = 0; i < count1; i++)
		{
			if (p[i].left[0] == q[0] && p[i].right[0] != p[i].left[0] && p[i].right[0] >= 'A'&&p[i].right[0] <= 'Z')
			{
				for (j = 0; j < count1; j++)
				{
					if (p[j].left[0] == p[i].right[0])
					{
						if (p[j].right != "*")
						{
							p[count1].left = p[i].left;
							p[count1].right = p[j].right + p[i].right.substr(1, p[i].right.size() - 1);
							count1++;
						}
						else
						{
							if (p[i].right.size() > 1)
							{
								p[count1].left = p[i].left;
								p[count1].right = p[i].right.substr(1, p[i].right.length());
								count1 = count1 + 1;
							}
							else
							{
								p[count1].left = p[i].left;
								p[count1].right = p[j].right;
								count1 = count1 + 1;
							}
						}
					}
				}
				p[i].left = "";
				p[i].right = "";
			}

		}
	}
	//消一切左递归
	for (m = 0; m <= count; m++)
	{
		for (i = 0; i < N; i++)
		{
			if (p[i].left[0] == q[m])
			{
				for (j = 0; j < count1; j++)
				{
					for (x = m + 1; x <= count; x++)
						if (p[j].left[0] == q[x] && p[j].right[0] == q[m])
						{
							if (p[i].right != "*")
							{
								p[count1].left = p[j].left;
								p[count1].right = p[i].right + p[j].right.substr(1, p[j].right.length());
								count1 = count1 + 1;
							}
							else
							{
								if (p[j].right.size() > 1)
								{
									p[count1].left = p[j].left;
									p[count1].right = p[j].right.substr(1, p[j].right.length());
									count1 = count1 + 1;
								}
								else
								{
									p[count1].left = p[j].left;
									p[count1].right = p[i].right;
									count1 = count1 + 1;
								}
							}
						}
				}
			}
		}
		for (j = 0; j < count1; j++)
		{
			for (x = m + 1; x <= count; x++)
				if (p[j].right[0] == q[m] && p[j].left[0] == q[x])
				{
					p[j].right = "";
					p[j].left = "";
				}
		}
		for (x = 0, flag = 0; x < count1; x++)// 判断第m 个非终结符是否存在直接左递归
		{
			if (p[x].left[0] == q[m] && p[x].left[0] == p[x].right[0])
				flag++;	//消直接左递归
		}
		if (flag != 0)
		{
			for (i = 0; i < count1; i++)
			{
				if (p[i].left[0] == q[m])
				{
					while (Vn.find(char(character + 65)) != string::npos)
						character++;
					if (p[i].left[0] == p[i].right[0])
					{
						//p[i].left = p[i].left + "'";
						p[i].left = char(character + 65);
						p[i].right = p[i].right.substr(1, p[i].right.length()) + p[i].left;
						p[count1].left = p[i].left;
						p[count1].right = "*";// 用*代替空产生式
					}
					else
						//p[i].right = p[i].right + p[i].left + "'";
						p[i].right = p[i].right + char(character + 65);
				}
			}
			character++;
			count1 = count1 + 1;
		}
	}
	count1;
	//提取左公因子
	N = count1;
}
//判断文法中是否有左公因子
int First_Follow_Select_LL::Easy_Judge(LRS *p)
{
	int i, j;
	for (i = 0; i < N; i++)
	{
		for (j = i + 1; j < N; j++)
		{
			if (p[i].left[0] == p[j].left[0] && p[i].right[0] == p[j].right[0])
				return 0;
		}
	}
	return 1;
}
//提取左公因子
void First_Follow_Select_LL::Extract(LRS *p)
{
	int i, j, k, count = N, l;
	string Vntemp, Vttemp;
	for (i = 0; i < N; i++)
	{
		if (Vntemp.find(p[i].left[0]) == string::npos&&p[i].left[0] >= 'A'&&p[i].left[0] <= 'Z')
			Vntemp += p[i].left[0];
		for (j = 0; j < p[i].right.size(); j++)
			if (Vttemp.find(p[i].right[j]) == string::npos && (p[i].right[j] >= 'a'&&p[i].right[j] <= 'z' || p[i].right[j] == '*'))
				Vttemp += p[i].right[j];
	}
	for (i = 0; i < Vntemp.size(); i++)
	{
		for (j = 0; j < N; j++)
		{
			if (p[j].left[0] == Vntemp[i])
			{
				vector<string> temp;
				temp.push_back(p[j].right);
				for (k = j + 1; k < N; k++)
				{
					if (p[k].right[0] == p[j].right[0] && p[k].left[0] == Vntemp[i])
					{
						for (l = 1; l < p[k].right.size();)
						{
							if (p[j].right[l] == p[k].right[l])
							{
								l++;
								if (l<p[k].right.size())
									continue;
								else
								{
									for (vector<string>::iterator it = temp.begin(); it != temp.end();)
									{
										if (*it == p[j].right)
											it = temp.erase(it);
										else
											it++;
									}
									while (Vntemp.find(char(character + 65)) != string::npos)
										character++;

									p[count].left = char(character + 65);
									if (p[k].right.substr(l, p[k].right.size() - l) != "")
										p[count].right = p[k].right.substr(l, p[k].right.size() - l);
									else
										p[count].right = "*";
									p[k].left = "";
									p[k].right = "";
									count++;
									p[count].left = char(character + 65);
									if (p[j].right.substr(l, p[j].right.size() - 1) != "")
										p[count].right = p[j].right.substr(l, p[j].right.size() - 1);
									else
										p[count].right = "*";
									count++;
									p[j].right = p[j].right.substr(0, l) + char(character + 65);
									temp.push_back(p[j].right);
									break;
								}
							}
							else
							{
								if (l > 1)
								{
									for (vector<string>::iterator it = temp.begin(); it != temp.end();)
									{
										if (*it == p[j].right)
											it = temp.erase(it);
										else
											it++;
									}
									while (Vntemp.find(char(character + 65)) != string::npos)
										character++;

									p[count].left = char(character + 65);
									if (p[k].right.substr(l, p[k].right.size() - l) != "")
										p[count].right = p[k].right.substr(l, p[k].right.size() - l);
									else
										p[count].right = "*";
									p[k].left = "";
									p[k].right = "";
									count++;
									p[count].left = char(character + 65);
									if (p[j].right.substr(l, p[j].right.size() - 1) != "")
										p[count].right = p[j].right.substr(l, p[j].right.size() - 1);
									else
										p[count].right = "*";
									count++;
									p[j].right = p[j].right.substr(0, l) + char(character + 65);
									temp.push_back(p[j].right);
								}
								else
								{
									temp.push_back(p[k].right);
									p[k].left = "";
									p[k].right = "";
								}
								break;
							}
						}
						N = count;
					}
				}
				if (temp.size() > 1)
				{
					while (Vntemp.find(char(character + 65)) != string::npos)
						character++;
					p[j].right = p[j].right.substr(0, 1) + char(character + 65);
					for (k = 0; k < temp.size(); k++)
					{
						p[count].left = char(character + 65);
						if (temp.at(k).substr(1, temp.at(k).size() - 1) != "")
							p[count].right = temp.at(k).substr(1, temp.at(k).size() - 1);
						else
							p[count].right = "*";
						count++;
					}
					N = count;
				}
				temp.clear();
			}
		}
	}
}
//主函数
int main(int argc, char *argv[])
{
	LRS *p = new LRS[MAXS];
	int flag;
	First_Follow_Select_LL fifo;
	fifo.Open_File(N, p);
	fifo.Combine(p);
	fifo.VNVT(p);//求VN 和VT
	fifo.Display(p);//将结果进行显示
	fifo.Letter_Select(p);//求每一个产生式的select集
	fifo.Select_Show(p);//将select集显示出来
	int iterout = 0;
	while (!fifo.Judge_LL(p))//判断是不是LL（1）文法
	{
		cout << "开始进行转换。。。";
		flag = 1;
		int iterin = 0;
		fifo.Change_LL(p, flag);
		fifo.Combine(p);
		while (!fifo.Easy_Judge(p))
		{
			iterin++;
			fifo.Extract(p);
			fifo.Combine(p);
			if (iterin >= 10)
				break;
		}
		fifo.VNVT(p);
		fifo.Display(p);
		fifo.Letter_Select(p);
		fifo.Select_Show(p);
		iterout++;
		if (iterout >= 10)
		{
			cout << "该文法无法转换成LL（1）文法！" << endl;
			break;
		}
	}
	return 0;
}

