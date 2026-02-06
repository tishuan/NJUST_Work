#include <iostream>
#include <fstream>
#include <stack>
#include <cstdio>
#include <cstring>
#include <iomanip>
#include <string>
using namespace std;
//堆栈
stack<int> state_stack;
stack<char> sign_stack;

char G[300][300];     //二型文法第i条产生式的第j个符号
int length[300];    //第i条产生式的长度
int number = 0;    //文法产生式的条数
bool isV[300];      //是否出现过，出现就加到非终结符或终结符中
char Vn[300];        //非终结符集
int size_vn = 0;
char Vt[300];       //终结符集
int size_vt = 0;
bool first[300][300];    //first集，如非终结符E的first集包含i,那么first['E']['i']=true
char buffer[300];
int size1 = 0;
struct T        //转换表  项目集转换
{
	int begin;
	int next;
	char ch;
};
struct project      //项目集
{
	int num;	//文法的第几条产生式
	int now;	//.的位置
	char search;	//向前搜索符
};
struct action
{
	char ch;
	int next_state;
};


T trans[300];//记录当前状态到已有状态的转移路径
int size_trans = 0;
project items[300][300]; //第 T 个项目集中的第 i 个项目
int count1 = 0;
int size_item[300]; //第 T 个项目集中的项目个数

action action_table[300][300];
int size_act_table[300];

//文件定义
ifstream grammar_file;
ifstream input_file;
ofstream items_file;
ofstream action_file;
ofstream firstset_file;
ofstream procedure_file;

void processInput()     //读文法
{
	char nouse, temp;//nouse是要跳过的内容，如->
	int i, j = 0;
	G[0][0] = 'S';    //S'->S 扩展文法
	length[0] = 2;
	grammar_file >> number;
	for (i = 1; i <= number; ++i)
	{
		j = 0;
		grammar_file >> temp;
		grammar_file >> nouse >> nouse;
		//识别第i条产生式，$代表
		while (temp != '!')
		{
			G[i][j] = temp;
			isV[temp] = true;
			grammar_file >> temp;
			j++;
		}
		length[i] = j;
	}
	G[0][1] = G[1][0];//构造出S->P （扩展文法）
	//如果是大写字母就是非终结符，其他的是终结符
	for (int i = 0; i < 128; ++i)
	{
		if (isV[i])
		{
			if (i >= 'A' && i <= 'Z')  // 非终结符：大写字母
				Vn[size_vn++] = i;
			else                      // 其它：终结符
				Vt[size_vt++] = i;
		}
	}
}
//判断是否在非终结符中出现过
bool Is_in_vn(char a)
{
	for (int i = 0; i < size_vn; ++i)
	{
		if (Vn[i] == a)
		{
			return true;
		}
	}
	return false;
}
//求first集
void firstSet()
{
	bool done = true;    
	int t, k;
	bool isempty;
	while (done)
	{
		done = false;
		for (int i = 1; i <= number; i++)//遍历每条产生式
		{
			t = 1;//从右部开始分析
			isempty = true;//是否可以推出空
			while (isempty && t < length[i])
			{
				isempty = false;//第一次可以进入，之后如果可以推出空串就继续像右看
				if (G[i][t] >= 'A' && G[i][t] <= 'Z')//是非终结符
				{
					for (k = 0; k <= 63; ++k)
					{
						//非终结符G[i][t]的first集不在G[i][0]里，就把它加入到G[i][0]的first集
						if (first[G[i][t] - 'A'][k] == true && !first[G[i][0] - 'A'][k])
						{
							first[G[i][0] - 'A'][k] = true;
							done = true;
						}
					}
					if (first[G[i][t] - 'A'][64] == true)    //@表示空
					{
						isempty = true;
						++t;//向右看
					}
					//没有加入就再次加入
					for (k = 0; k < 128; ++k)
					{
						if (k >= 'A' && k <= 'Z') continue; // 跳过非终结符
						if (first[G[i][t] - 'A'][k] && !first[G[i][0] - 'A'][k])
						{
							first[G[i][0] - 'A'][k] = true;
							done = true;
						}
					}
				}
				//如果右部是终结符，就直接加入到first集
				else if (first[G[i][0] - 'A'][G[i][t]] == false)
				{
					done = true;
					first[G[i][0] - 'A'][G[i][t]] = true;
				}
			}
			//如果每一个都能推导出空，就把空@加入到first集
			if (length[i] == t)
			{
				first[G[i][0] - 'A']['@'] = true;
			}
		}
	}
}
//打印first集
void printfirstset()
{
	for (int i = 0; i < 26; ++i)
	{
		char ch = char(i + 'A');
		if (Is_in_vn(ch))  // 是非终结符
		{
			firstset_file << "FIRST(" << ch << ") = {";

			bool first_element = true;
			for (int j = 0; j < 128; ++j)
			{
				if (first[i][j])
				{
					if (!first_element)
						firstset_file << ", ";

					// 将空串 '@' 显示为 ε
					if (j == '@')
						firstset_file << "ε";
					else
						firstset_file << char(j);

					first_element = false;
				}
			}

			firstset_file << "}" << endl;
		}
	}

}


void headSearch(project temp)    //得到向前搜索符
{
	size1 = 0;
	bool flag = true;
	int nownow = temp.now;
	int i;
	while (flag == true)//能推出空，就一直向右找
	{
		flag = false;
		//.后面没有符号了，直接把当前项目的搜索符加到buffer里
		if (nownow + 1 >= length[temp.num])
		{
			buffer[size1++] = temp.search;
			return;
		}
		//如果后面是终结符，就直接加入buffer里
		else if (G[temp.num][nownow + 1] < 'A' || G[temp.num][nownow + 1]>'Z')
		{
			buffer[size1++] = G[temp.num][nownow + 1];
			return;
		}
		//如果后面是非终结符，就要把非终结符的first集加入buffer里
		else if (G[temp.num][nownow + 1] >= 'A' && G[temp.num][nownow + 1] <= 'Z')
		{
			char B = G[temp.num][nownow + 1]; // 获取当前非终结符
			int B_index = B - 'A';

			for (i = 0; i < 128; ++i)
			{
				// 跳过非终结符 A~Z以及@
				if (i >= '@' && i <= 'Z') continue;

				if (first[B_index][i])
				{
					buffer[size1++] = i;
				}
			}

			// 如果 FIRST(B) 含有空串 '@'，继续向右分析下一个符号
			if (first[B_index]['@'])  // ASCII 64
			{
				++nownow;
				flag = true;
			}
		}
	}
}

//判断项目 temp 是否已经存在于第 T 个项目集中。
bool is_in(project temp, int T)   
{
	int i;
	for (i = 0; i < size_item[T]; ++i)
	{
		//是同一条产生式，而且.的位置一样，而且向前搜索符一样，则认为该项目已经存在
		if (items[T][i].num == temp.num && items[T][i].now == temp.now && items[T][i].search == temp.search)
		{
			return true;
		}
	}
	return false;
}
//.后是非终结符的情况，需要扩展
void project_closure(int T)  
{
	project temp;
	int i, j, k;
	for (i = 0; i < size_item[T]; ++i)
	{
		//.后是非终结符的情况
		if (G[items[T][i].num][items[T][i].now] >= 'A' && G[items[T][i].num][items[T][i].now] <= 'Z')
		{
			//遍历所有文法产生式
			for (j = 0; j < 300; ++j)
			{
				size1 = 0;
				//如果产生式的左部刚好是该非终结符
				if (G[j][0] == G[items[T][i].num][items[T][i].now])
				{
					headSearch(items[T][i]);
					for (k = 0; k < size1; ++k)
					{
						temp.num = j;
						temp.now = 1;//让.在产生式右部的最左侧
						temp.search = buffer[k];
						if (is_in(temp, T) == false)//没有在项目集中出现过，就加入
						{
							items[T][size_item[T]++] = temp;
						}
					}
				}
			}
		}
	}
	return;
}

//判断当前构造的项目集是否已存在，跟已有的比较
int is_contained()   
{
	int i;
	int sum = 0;
	int j;
	int k;
	for (i = 0; i < count1; ++i)
	{
		sum = 0;        //记录有多少是匹配的
		if (size_item[count1] == size_item[i])
		{
			for (j = 0; j < size_item[count1]; ++j)
			{
				for (k = 0; k < size_item[i]; ++k)
				{
					if (items[i][k].num == items[count1][j].num && items[i][k].now == items[count1][j].now && items[i][k].search == items[count1][j].search)
					{
						++sum;
						break;
					}
				}
			}
		}
		if (sum == size_item[count1])
		{
			return i;
		}
	}
	return 0;
}

//构造项目集族，计算goto
void make_set()
{
	//对于初始扩展产生式
	items[0][0].num = 0;
	items[0][0].now = 1;
	items[0][0].search = '#';
	size_item[0] = 1;
	project_closure(0);
	project buf[50];
	int buf_size = 0;
	project tp;
	int i, j, k;
	int nextt_state;
	items_file << "I0:" << endl;
	for (i = 0; i < size_item[0]; ++i)
	{
		project item = items[0][i];
		items_file << "  " << G[item.num][0] << "->";

		for (int p = 1; p < length[item.num]; ++p)
		{
			if (p == item.now) items_file << "·";
			items_file << G[item.num][p];
		}
		if (item.now == length[item.num]) items_file << "·"; // 点在末尾

		items_file << " , " << item.search << endl;
	}
	items_file << "--------------------------------------------------" << endl;
	int index;
	int p;
	int t;
	//遍历当前已有的项目集
	for (index = 0; index < count1 + 1; ++index)
	{
		for (j = 0; j < size_vt; ++j)
		{
			buf_size = 0;
			for (p = 0; p < size_item[index]; ++p)
			{
				if (items[index][p].now < length[items[index][p].num] && G[items[index][p].num][items[index][p].now] == Vt[j])
				{
					
					tp.num = items[index][p].num;
					tp.search = items[index][p].search;
					tp.now = items[index][p].now + 1;
					//将所有点后是 Vt[j] 的项目收集进 buf[]
					buf[buf_size++] = tp;
				}
			}
			//产生一个新的项目集
			if (buf_size != 0)    
			{
				count1++;
				for (t = 0; t < buf_size; ++t)
				{
					items[count1][size_item[count1]++] = buf[t];
				}
				project_closure(count1);
				nextt_state = is_contained();        //检查第count个项目集是否重复
				if (nextt_state != 0)           //已经有了，不新建状态，只记录当前状态到已有状态的转移路径
				{
					size_item[count1--] = 0;
					trans[size_trans].begin = index;//当前
					trans[size_trans].next = nextt_state;
					trans[size_trans++].ch = Vt[j];
				}
				else//没有就新建
				{
					items_file << "I" << count1 << ":" << endl;
					//写入文件
					for (i = 0; i < size_item[count1]; ++i)
					{
						project item = items[count1][i];
						items_file << "  " << G[item.num][0] << "->";

						for (int p = 1; p < length[item.num]; ++p)
						{
							if (p == item.now) items_file << "·";
							items_file << G[item.num][p];
						}
						if (item.now == length[item.num]) items_file << "·"; // 点在末尾

						items_file << " , " << item.search << endl;

					}
					items_file << "--------------------------------------------------" << endl;
					//记录转移关系
					trans[size_trans].begin = index;
					trans[size_trans].next = count1;
					trans[size_trans++].ch = Vt[j];
				}
			}
		}

		for (j = 0; j < size_vn; ++j)
		{
			buf_size = 0;
			for (p = 0; p < size_item[index]; ++p)//第index项目集中的项目个数
			{
				//还没到产生式末尾
				if (items[index][p].now < length[items[index][p].num] && G[items[index][p].num][items[index][p].now] == Vn[j])
				{
					//点右移
					tp.num = items[index][p].num;
					tp.search = items[index][p].search;
					tp.now = items[index][p].now + 1;
					buf[buf_size++] = tp;
				}
			}
			//构造新的项目集
			if (buf_size != 0)
			{
				++count1;
				for (t = 0; t < buf_size; ++t)
				{
					items[count1][size_item[count1]++] = buf[t];
				}
				project_closure(count1);
				nextt_state = is_contained();

				if (nextt_state != 0)
				{
					size_item[count1--] = 0;
					trans[size_trans].begin = index;
					trans[size_trans].next = nextt_state;
					trans[size_trans++].ch = Vn[j];
				}
				else
				{
					items_file << "I" << count1 << ":" << endl;
					for (i = 0; i < size_item[count1]; ++i)
					{
						project item = items[count1][i];
						items_file << "  " << G[item.num][0] << "->";

						for (int p = 1; p < length[item.num]; ++p)
						{
							if (p == item.now) items_file << "·";
							items_file << G[item.num][p];
						}
						if (item.now == length[item.num]) items_file << "·";

						items_file << " , " << item.search << endl;

					}
					items_file << "--------------------------------------------------" << endl;
					trans[size_trans].begin = index;
					trans[size_trans].next = count1;
					trans[size_trans++].ch = Vn[j];
				}
			}
		}
	}
}
//分析表
void get_action()
{
	int i, j;
	int t1, t2, t;
	char tp;
	for (i = 0; i < 300; ++i)
	{
		size_act_table[i] = 0;
	}
	//处理规约项
	for (i = 0; i <= count1; ++i)
	{
		for (j = 0; j < size_item[i]; ++j)
		{
			//如果点在产生式尾或者只含空，则可以规约
			if (items[i][j].now == length[items[i][j].num] || (items[i][j].now == 1 && length[items[i][j].num] == 2 && G[items[i][j].num][1] == '@'))
			{
				action_table[i][size_act_table[i]].ch = items[i][j].search;
				action_table[i][size_act_table[i]++].next_state = items[i][j].num * (-1);//使用负数表示规约
			}
		}
	}
	//建立分析表
	for (i = 0; i < size_trans; ++i)
	{
		t1 = trans[i].begin;
		t2 = trans[i].next;
		tp = trans[i].ch;
		action_table[t1][size_act_table[t1]].ch = tp;
		action_table[t1][size_act_table[t1]++].next_state = t2;
	}
	bool has_end_marker = false;
	for (int i = 0; i < size_vt; ++i)
		if (Vt[i] == '#') has_end_marker = true;

	if (!has_end_marker)
		Vt[size_vt++] = '#'; // 加入结束符号

	//输出到文件
	// 输出表头：终结符 + 非终结符
	action_file << "State\t";
	for (int i = 0; i < size_vt; ++i) action_file << Vt[i] << "\t";
	for (int i = 0; i < size_vn; ++i) action_file << Vn[i] << "\t";
	action_file << endl;

	for (int i = 0; i <= count1; ++i)
	{
		action_file << "  " << i << "  " << "\t";

		for (int j = 0; j < size_vt + size_vn; ++j)
		{
			char symbol;
			if (j < size_vt) symbol = Vt[j];
			else symbol = Vn[j - size_vt];

			bool found = false;
			for (int k = 0; k < size_act_table[i]; ++k)
			{
				if (action_table[i][k].ch == symbol)
				{
					int next = action_table[i][k].next_state;

					if (j < size_vt)
					{
						if (next > 0)
							action_file << "S" << next << "\t";
						else if (next == 0)
							action_file << "acc\t";  // 接受状态
						else
							action_file << "r" << -next << "\t";
					}
					else
					{
						action_file << next << "\t"; // GOTO 表
					}

					found = true;
					break;
				}
			}

			if (!found)
			{
				action_file << "-\t";
			}
		}

		action_file << endl;
	}

}
//写入分析过程中
void write_stack(int x)
{
	if (x == 1) // 状态栈
	{
		std::stack<int> temp;
		while (!state_stack.empty())
		{
			temp.push(state_stack.top());
			state_stack.pop();
		}
		
		while (!temp.empty())
		{
			procedure_file << temp.top() << ',';
			state_stack.push(temp.top());
			temp.pop();
		}
	}
	else if (x == 2) // 符号栈
	{
		std::stack<char> temp;
		while (!sign_stack.empty())
		{
			temp.push(sign_stack.top());
			sign_stack.pop();
		}
		while (!temp.empty())
		{
			procedure_file << temp.top() << ',';
			sign_stack.push(temp.top());
			temp.pop();
		}
	}
}

void judge()
{
	size1 = 0;
	//读到#为止
	while (input_file >> buffer[size1] && buffer[size1++] != '#');
	int    work_sta = 0;
	int    index_buf = 0;
	bool   error = false;
	bool   done = false;
	char now_in;
	now_in = buffer[0];
	//先把第0个状态加入，同时把#加入符号栈
	state_stack.push(0);
	sign_stack.push('#');
	// 输出到 procedure_file
	procedure_file << left << setw(17) << "状态栈"
		<< setw(17) << "符号栈"
		<< setw(17) << "输入串"
		<< setw(17) << "ACTION"
		<< setw(17) << "GOTO" << endl;
	//procedure_file << "状态栈                 符号栈                  输入串               ACTION                  GOTO" << endl;
	int i, j, k, m;
	int tp, len;
	int aa;
	while (done == false && error == false)
	{
		work_sta = state_stack.top();
		write_stack(1);//输出状态栈到文件
		procedure_file << "               ";
		//procedure_file << std::left << std::setw(25);
		write_stack(2);//输出符号栈到文件
		procedure_file << "               ";
		//procedure_file << std::setw(50);
		for (i = index_buf; i < size1; ++i)
		{
			procedure_file << buffer[i];//输出当前剩余输入串
		}
		procedure_file << "              ";
		error = true;
		for (i = 0; i < size_act_table[work_sta]; ++i)
			if (action_table[work_sta][i].ch == now_in)
			{
				error = false;
				if (action_table[work_sta][i].next_state == 0)//分析完成
				{
					procedure_file << "YES" << endl;
					cout << "YES" << endl;
					done = true;
					break;
				}
				else if (action_table[work_sta][i].next_state > 0)   //移进
				{
					procedure_file  << 'S' << action_table[work_sta][i].next_state << endl;
					//加入状态栈和符号栈
					state_stack.push(action_table[work_sta][i].next_state);
					sign_stack.push(action_table[work_sta][i].ch);
					++index_buf;
					now_in = buffer[index_buf];
					break;
				}
				else if (action_table[work_sta][i].next_state < 0)	//规约
				{
					tp = action_table[work_sta][i].next_state * (-1);
					procedure_file  << 'r' << tp << "              ";
					len = length[tp] - 1;//产生式右边的符号数
					if (G[tp][1] == '@')
					{
						--len;
					}
					for (k = 0; k < len; ++k)
					{
						state_stack.pop();
						sign_stack.pop();
					}
					//产生式的左部加入到符号栈
					sign_stack.push(G[tp][0]);
					aa = state_stack.top();
					for (m = 0; m < size_act_table[aa]; ++m)
					{
						if (action_table[aa][m].ch == G[tp][0])
						{
							state_stack.push(action_table[aa][m].next_state);
							procedure_file  << action_table[aa][m].next_state << endl;
						}
					}
					break;
				}
			}
	}
	if (!done)
	{
		cout << "NO" << endl;
		cout << "出错原因是未找到：";
		for (i = 0; i < size_act_table[work_sta]; ++i)
		{
			if (action_table[work_sta][i].ch == '2') {
				cout << "标识符 ";
			}
			else if (action_table[work_sta][i].ch == '3') {
				cout << "常量 ";
			}
			else cout << action_table[work_sta][i].ch << " ";
		}
		cout << endl;
	}
}
void init()
{
	int i, j;
	for (i = 0; i < 300; ++i)
	{
		isV[i] = false;
	}
	for (i = 0; i < 300; ++i)
	{
		size_item[i] = 0;
	}
	for (i = 0; i < 300; ++i)
	{
		for (j = 0; j < 300; ++j)
		{
			first[i][j] = false;
		}
	}
	size1 = 0;
}
void open_file()
{
	grammar_file.open("wenfa.txt");
	input_file.open("input.txt");
	items_file.open("项目集.txt");
	action_file.open("分析表.txt");
	firstset_file.open("first集.txt");
	procedure_file.open("分析过程.txt");
}
void LR1()
{
	processInput();
	firstSet();
	printfirstset();
	make_set();
	get_action();
	judge();
}
void close_file()
{
	grammar_file.close();
	input_file.close();
	items_file.close();
	action_file.close();
	firstset_file.close();
	procedure_file.close();
}
int main()
{
	init();
	open_file();
	LR1();
	close_file();
	return 0;
}

