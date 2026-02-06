#include<iostream>
using namespace std;
int main()
{
	double i=3.0,j=2.0,s=0.5;
	cout<<"每一项分母"<<2<<" "<<2; 
	for(int n=1;n<20;n++)
	{
		s+=i/j;
     	i=i+j;
		j=i-j;
	cout<<" "<<j;
	}
	cout<<"\n"<<s;
	return 0;
}
