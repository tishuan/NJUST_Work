#include<iostream>
using namespace std;
int f(int x){
	if(x==0) return 0;
	cout<<x%16;
	return f(x/16);
}
int main(){
	int x;
	long long a;
	cin>>x>>a;
	cout<<f(x)<<"\n";
	cout<<f(a);
	return 0;
}
