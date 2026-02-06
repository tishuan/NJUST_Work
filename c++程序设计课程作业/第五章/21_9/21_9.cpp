#include<iostream>
#include<math.h>
using namespace std;
int s(int a,int b,int c){
	return (a+b+c)/2;
}
int area(int a,int b,int c){
	return s(a,b,c)*(s(a,b,c)-a)*(s(a,b,c)-b)*(s(a,b,c)-c);
}
int main(){
	int a,b,c;
	double f;
	cin>>a>>b>>c;
	f=sqrt(area(a,b,c));
	cout<<f;
	return 0;
}
