#include<iostream>
#include"mytool.h"
using namespace std;
int main(){
	int sum=0;
	int i;
	cin>>i;
	for(int n=1;n<=i;n++){
		sum+=jiechen(n);
	}
	cout<<sum;
	return 0;
}
