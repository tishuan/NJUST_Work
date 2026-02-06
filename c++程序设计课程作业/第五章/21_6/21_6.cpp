#include<iostream>
using namespace std;
int min(int a,int b){
	return a>b?b:a;
}
int main(){
	int a,b,n,x,m;
	double sum=1;
	cin>>a>>b;	
	for(int i=1;i<min(a,b);i++){
		if(a%i==0&&b%i==0)
		{
		    x=i;	
		}	
	m=a*b/x;	//a和b都除以最大公约数，然后将得到的结果相乘，再乘以最大公约数即M=(a/x)*(b/x)*x 。 
		}
	cout<<x<<"\n"<<m;
	return 0;
}
