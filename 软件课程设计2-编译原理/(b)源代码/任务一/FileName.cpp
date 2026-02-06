#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstring>
#include <cstdio>
#include <algorithm>
#include <stack>
#include <fstream>
#include <utility> 

using namespace std;
const int keywordCount = 24;//关键词的个数
const int oneOperatorCount = 11;//单目运算符的个数
const int multiOperatorCount = 12;//双目运算符的个数
const int delimiterCount = 9;//界符的个数
const char keyword[50][12] = { "break","case","char","continue","do","default","double","else","float","for","if","int","include","long","main","return","switch","typedef","void","unsigned","while","iostream","using","namespace"};
const char oneOperator[20] = { '+','-','*','/','!','%','~','&','|','^','=' };   //单目运算符
const char multiOperator[20][5] = { "++","--","&&","||","<=","!=","==",">=","+=","-=","*=","/=" }; //双目运算符
const char delimiter[20] = { ',','(',')','{','}',';','<','>','#' }; //界符
FILE* file_source = NULL;
ifstream input;
ofstream output;

char state[100];
int stateCount;//状态个数
char start;//初态
char final[100];//终结符
int finalCount;//终态个数
bool is_final[150];//判断是否是终态

struct NFA_set
{
	char set[100];
	int len = 0;
};

NFA_set from[100][100];//状态1通过终结符所到达的状态集2
NFA_set subset[100];//子集族
int subsetCount = 0;//子集族的个数
int dfa[150][150];//dfa里状态1通过终结符所到达的状态2

//用于任务2的源程序转换
const char keyword_map[] = {
	'b', 'c', 'a', 'o', 'd', 'n', 'z', 'e', 'y', 'f', 'i', 'h', 'p', 'l', 'm',
	'r', 's', 't', 'v', 'u', 'w', 'k','g','q'
};
//将keyword_map与关键词匹配，用于任务2的源程序转换
char match(char str[]) {
	// 遍历 keyword 数组并查找匹配
	for (int i = 0; i < 24; ++i) {
		if (strcmp(str, keyword[i]) == 0) {
			return keyword_map[i];
		}
	}

	return '\0';  // 如果没有找到对应的关键字，返回 '\0'
}

//判断是否已经加入状态
bool Is_In_state(char a)
{
	for (int i = 0; i < stateCount; ++i)
	{
		if (a == state[i])
			return true;
	}
	return false;
}
//判断该终结符是否已经存在
bool Is_In_final(char a)
{
	for (int i = 0; i < finalCount; ++i)
	{
		if (a == final[i])
			return true;
	}
	return false;
}
//判断该状态是否已经在该集合里出现过
bool Is_in_set(char a, NFA_set temp)
{
	for (int i = 0; i < temp.len; ++i)
	{
		if (a == temp.set[i])
			return true;
	}
	return false;
}

//根据输入的正规文法生成nfa
void createNFA()
{

	int N;
	bool flag = true;   //是不是第一个
	char ch;    //用来读 文法左边的
	char nouse;
	char str[10];    //用来读 文法 右边的
	input.open("wenfa.txt");
	input >> N;
	while (N--)
	{
		input >> ch >> nouse >> nouse >> str;
		if (flag)
		{
			start = ch;
			flag = false;
		}
		if (!Is_In_state(ch))
		{
			state[stateCount++] = ch;
		}
		if (!Is_In_final(str[0]))
		{
			final[finalCount++] = str[0];
		}
		if (strlen(str) > 1)
		{
			int length = from[ch][str[0]].len;
			from[ch][str[0]].set[length] = str[1];
			from[ch][str[0]].len++;
		}
		else
		{
			int length = from[ch][str[0]].len;
			from[ch][str[0]].set[length] = 'Y';  //终态
			from[ch][str[0]].len++;
		}
	}
}
void showNFA()
{
	for (int i = 0; i < 100; ++i)
	{
		for (int j = 0; j < 100; ++j)
		{
			for (int k = 0; k < 100; ++k)
			{
				if (from[i][j].set[k] != '#')
					cout << char(i) << " " << char(j) << " " << k << " " << from[i][j].set[k] << endl;
			}
		}
	}
}

//和已有的subset有没有重复的，有就返回重复的编号
int inSubset(NFA_set temp)   
{
	bool flag[100];
	bool flag1;
	for (int i = 0; i < temp.len; ++i)
	{
		flag[i] = false;
	}
	for (int i = 0; i < subsetCount; ++i)
	{
		for (int k = 0; k < temp.len; ++k)
		{
			for (int j = 0; j < subset[i].len; ++j)
			{
				if (temp.set[k] == subset[i].set[j])
				{
					flag[k] = true;
				}
			}
		}
		flag1 = true;
		for (int m = 0; m < temp.len; ++m)
		{
			if (flag[m] == false)
			{
				flag1 = false;
				break;
			}
		}
		if (flag1 == true)
			return i;
		for (int m = 0; m < temp.len; ++m)
		{
			flag[m] = false;
		}
	}
	return -1;
}
void get_closure(NFA_set& temp)    //得到一个完整的子集 
{
	for (int i = 0; i < temp.len; ++i)
	{
		for (int j = 0; j < from[temp.set[i]]['@'].len; ++j)
		{
			if (!Is_in_set(from[temp.set[i]]['@'].set[j], temp))
			{
				temp.set[temp.len++] = from[temp.set[i]]['@'].set[j];
			}
		}
	}
}
bool Is_contained_Y(NFA_set temp)   //判断计算的子集是否是终态（只要包含终态Y就是终态）
{
	for (int i = 0; i < temp.len; ++i)
	{
		if (temp.set[i] == 'Y')
			return true;
	}
	return false;
}
void convertToDFA()
{
	subsetCount = 0;
	NFA_set work_set;//子集（初始为空，当前状态集）
	NFA_set worked_set;//下一个状态集
	work_set.set[work_set.len++] = start;
	worked_set.len = 0;
	stack<NFA_set> s;
	get_closure(work_set);//得到初态的闭包
	s.push(work_set);//加入栈
	subset[subsetCount++] = work_set;//成为dfa的第一个状态
	//初始
	for (int i = 0; i < 150; ++i)
	{
		for (int j = 0; j < 150; ++j)
		{
			dfa[i][j] = '-1';
		}
	}
	for (int i = 0; i < 150; ++i)
		::is_final[i] = false;
	//判断该子集是否包含Y，包含即为终态
	if (Is_contained_Y(work_set))
		::is_final[subsetCount - 1] = true;
	while (!s.empty())
	{
		work_set = s.top();
		s.pop();
		for (int i = 0; i < finalCount; ++i)//遍历所有的终结符
		{
			//遍历当前状态集的所有状态
			for (int j = 0; j < work_set.len; ++j)
			{
				for (int k = 0; k < from[work_set.set[j]][final[i]].len; ++k)//该状态在所有终结符下能到达的所有状态
				{
					if (from[work_set.set[j]][final[i]].set[k] != '#' && from[work_set.set[j]][final[i]].set[k] != 'Y' && !Is_in_set(from[work_set.set[j]][final[i]].set[k], worked_set))
					{
						worked_set.set[worked_set.len++] = from[work_set.set[j]][final[i]].set[k];
					}
					if (from[work_set.set[j]][final[i]].set[k] == 'Y' && !Is_in_set(from[work_set.set[j]][final[i]].set[k], worked_set))
					{
						worked_set.set[worked_set.len++] = 'Y';    //用Y表示终态
					}
				}
			}
			get_closure(worked_set);//计算闭包，也就是加上通过ε能到达的状态
			if (worked_set.len > 0 && inSubset(worked_set) == -1)
			{
				dfa[subsetCount - 1][final[i]] = subsetCount;//用数字表示即可，因为dfa通过某一个终结符只可能到达一个状态
				s.push(worked_set);
				subset[subsetCount++] = worked_set;
				if (Is_contained_Y(worked_set))
				{
					::is_final[subsetCount - 1] = true;
				}
			}
			if (worked_set.len > 0 && inSubset(worked_set) > -1 && final[i] != '@')
			{
				//给已经存在的集合建立dfa转移关系
				dfa[inSubset(work_set)][final[i]] = inSubset(worked_set);
			}
			worked_set.len = 0;
		}
	}

}
//能否被dfa接受
bool isAcceptedByDFA(char str[])
{
	char now_state = 0;
	for (int i = 0; i < strlen(str); ++i)
	{
		now_state = dfa[now_state][str[i]];
		if (now_state == -1)
			return false;
	}
	//如果走到的最后一个状态是终态
	if (::is_final[now_state] == true)
		return true;
	return false;
}

//是否是整数
bool isNum(char a)
{
	if (a >= '0' && a <= '9')
		return true;
	return false;
}
//是否是字母
bool isLetter(char a)
{
	if ((a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z'))
		return true;
	return false;
}
//是否是关键词
bool IsKeyword(char a[])
{
	int len = strlen(a);
	for (int j = 0; j < keywordCount; ++j)
	{
		if (strlen(keyword[j]) == len)
		{
			if (strcmp(keyword[j], a) == 0)
				return true;
		}
	}
	return false;
}
//是否是单目运算符
bool isOne(char a)
{
	for (int i = 0; i < oneOperatorCount; ++i)
	{
		if (oneOperator[i] == a)
			return true;
	}
	return false;
}
//是否是双目运算符
bool isMulti(char a[])
{
	for (int i = 0; i < multiOperatorCount; ++i)
	{
		if (strcmp(multiOperator[i], a) == 0)
			return true;
	}
	return false;
}
//是否是界符
bool IsDelimiter(char a)
{
	for (int i = 0; i < delimiterCount; ++i)
	{
		if (delimiter[i] == a)
			return true;
	}
	return false;
}

//识别程序
void scanCode()
{
	char str[100];//存取读到的内容
	char ch;
	int i, j;
	int point;//下标
	int flag;// 标志当前读取的内容是什么类型

	ch = fgetc(file_source);
	bool finish = false;
	int line = 1;
	while (!finish)
	{
		flag = -1;
		point = 0;
		if (isNum(ch))     //读一个ch
		{
			flag = 1;
			str[point++] = ch;
			ch = fgetc(file_source);
			while (isLetter(ch) || isNum(ch) || ch == '.' || ch == '+' || ch == '-')
			{
				flag = 1;
				str[point++] = ch;
				ch = fgetc(file_source);
			}
			str[point] = '\0';
		}
		if (flag == 1)
		{
			bool isPureInt = true;
			for (int i = 0; str[i]; ++i)
			{
				if (!isdigit(str[i])) // 含有非数字字符，不是纯整数
				{
					isPureInt = false;
					break;
				}
			}

			// 如果是多位整数且第一个字符为0，则非法
			if (isPureInt && strlen(str) > 1 && str[0] == '0')
			{
				cout << line << "  " << "出错，整数不能有前导0 " << str << endl;
			}
			else if (isAcceptedByDFA(str))
			{
				cout << line << "  " << "常量  " << " " << str << endl;
				output << 3;
			}
			else
			{
				cout << line << "  " << "出错，不是常量" << " " << str  << endl;
			}
			point = 0;
			flag = -1;
		}
		if (isLetter(ch))
		{
			flag = 2;
			str[point++] = ch;
			ch = fgetc(file_source);
			while (isLetter(ch) || isNum(ch))
			{
				flag = 2;
				str[point++] = ch;
				ch = fgetc(file_source);
			}
			str[point] = '\0';
		}
		if (flag == 2)
		{
			if (IsKeyword(str))
			{
				cout << line << "  " << "关键字" << " " << str  << endl;
				output << match(str);
			}
			else
			{
				if (isAcceptedByDFA(str))
				{
					cout << line << "  " << "标识符" << " " << str << endl;
					output << 2;
				}
				else
				{
					cout << line << "  " << "出错，不是标识符" << " " << str << endl;
				}
			}
			point = 0;
			flag = -1;
		}
		
		if (IsDelimiter(ch))
		{
			cout << line << "  " << "界符  " << " " << ch  << endl;
			if (ch == '#')
				output << '!';
			else
				output << ch;
			if ((ch = fgetc(file_source)) == EOF)
			{
				finish = true;
				break;
			}
		}
		if (isOne(ch))
		{
			str[point++] = ch;
			if ((ch = fgetc(file_source)) == EOF)
			{
				finish = true;
			}
			str[point++] = ch;
			str[point] = '\0';
			if (finish == false && isMulti(str))
			{
				cout << line << "  " << "运算符" << " " << str << endl;
				//output<<4;
				ch = fgetc(file_source);
			}
			else
			{
				cout << line << "  " << "运算符" << " " << str[0]  << endl;
				output << str[0];
			}
			point = 0;
		}

		if (ch == ' ' || ch == '\n' || ch == '\t')
		{
			if (ch == '\n') {
				line++;
			}
			if ((ch = fgetc(file_source)) == EOF)
			{
				finish = true;
				break;
			}
			continue;
		}
	}
	output << '#';
}
void init()
{
	finalCount = 0;
	stateCount = 0;
	for (int i = 0; i < 100; ++i)
	{
		//is_final[i]=false;
		for (int j = 0; j < 100; ++j)
			for (int k = 0; k < 100; ++k)
				from[i][j].set[k] = '#';
	}
}
void show()
{
	for (int i = 0; i < subsetCount; ++i)
	{
		cout << i << "的set" << endl;
		for (int j = 0; j < subset[i].len; ++j)
		{

			cout << subset[i].set[j] << " ";
		}
		cout << endl;
	}
}
int main()
{
	init();
	finalCount = 0;
	stateCount = 0;
	createNFA();
	convertToDFA();
	file_source = fopen("yuan.txt", "r+");
	output.open("output.txt");
	scanCode();
	fclose(file_source);
	output.close();
	return 0;
}