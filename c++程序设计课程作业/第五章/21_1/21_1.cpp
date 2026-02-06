#include<iostream>
using namespace std;
int digitsum(int x){
	if(x==0) return 0;

cout<<x%10<<"+";
return x%10+digitsum(x/10);
}
int main(){
	int x;
	cin>>x;
	cout<<"\b"<<" "<<"="<<digitsum(x);
	return 0;
}
