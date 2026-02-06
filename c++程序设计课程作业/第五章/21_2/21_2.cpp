#include<iostream>
using namespace std;
int f(int m){
	if(m==1) return 1;
	return m*f(m-1);
}
int c(int n,int k){
	return f(n)/f(k)/f(n-k);	
}
int main(){
	cout<<"c(4,2)="<<c(4,2)<<"\n";
	cout<<"c(6,4)="<<c(6,4)<<"\n";
	cout<<"c(8,7)="<<c(8,7);
	return 0;
}
