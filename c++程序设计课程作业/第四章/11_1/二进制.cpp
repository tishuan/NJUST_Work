#include<iostream>
using namespace std;
int main()
{
int i,x=0,a;
cin>>x;
if(x<0)
{
	x=-x;
	cout<<"-";
}
for (i=31;i>=0;i--)
{
	int a=x>>i;
	a=a&000000001;
	cout<<a;
}
cout<<endl;
return 0;
}

