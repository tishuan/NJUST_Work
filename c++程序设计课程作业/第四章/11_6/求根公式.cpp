#include<iostream>
#include<math.h>
using namespace std;
int main()
{
	double x;
    int a,b,c;
	cin>>a>>b>>c;
	cout<<"a*x*x+b*x+c=0";
	double k=b*b-4*a*c,f;
	f=sqrt(k);
	if(k<0)
	{
		cout<<"此方程无解"; 
	}
	if(k==0)
	{
		x=-b/2*a,
		cout<<"此方程有一根"<<x;
	}
	if(k>0)
	{  
		double  x1=(-b-f)/2/a,x2=(-b+f)/2/a;
		cout<<"此方程有两根"<<"x1="<<x1<<"\n"<<"x2="<<x2; 
	}
	return 0;
}
