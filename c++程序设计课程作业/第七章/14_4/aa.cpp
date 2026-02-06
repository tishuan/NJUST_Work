#include<iostream>
using namespace std;
struct Course{
	char number[20];
	char name[20];
	enum way{
		dengji,
		percent
	};
};
enum Dengji{
	A=0,B,C,D,E 
};
struct student{
	int score;
	char stuname[20];
	char coursename[20];
	Course kaohe;
};
int main(){
	
	return 0;
}
