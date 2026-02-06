#include<iostream>
#include"mytool.h"
using namespace std;
int main(){
	int a,b,x;
	cin>>a>>b;
	x=max(a,b);
	cout<<max(a,b)<<"\n";
	tohex(x);
 return 0;
}

