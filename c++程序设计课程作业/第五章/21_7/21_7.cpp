#include<iostream>
using namespace std;
void cal(int n){
   if(n==0) return ;
   cout<<n%10;
   cal(n/10);
}
int main(){
	int n;
	cin>>n;
    cal(n);
	return 0;
}
