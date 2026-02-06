#include<iostream>
using namespace std;
int main()
{
	int a,i,s=0;
	cout<<"请输入一个正整数"; 
	cin>>a;
	cout<<"该数的因子有"; 
	for(i=1;i<=a;i++)
	{
		if(a%i==0)
		{
			cout<<i<<" ";
		}
	}
cout<<"\n"<<"1-100之间的完全数有"; 
for(int n=1;n<=100;n++)
{
	int x=0;
	for(i=1;i<n;i++)
	{	
		if(n%i==0)
		{
			x+=i;
		}
	}
	if(n==x)
		{
			cout<<n<<" ";
		}
}
	cout<<endl;
	return 0;
	
}


