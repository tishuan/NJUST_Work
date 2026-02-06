#include<iostream>
using namespace std;
int main()
{
	char ch;
	int count=0;
while(1)
{

		cin>>ch;
		if(ch>='0'&&ch<='9')
		count++;
		else if(ch=='q')
		   break;
}
	cout<<count<<endl;
	return 0;
 } 
