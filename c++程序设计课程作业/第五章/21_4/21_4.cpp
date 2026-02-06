#include<iostream>
using namespace std;
bool sushu(int n){
	if(n==1){
		return false;
	}
	for(int i=2;i<n/2;i++){
		if(n%i==0){
			return false;
		}
	}
	return true;
}
int main(){
	int n=500;
	for(int i=3;i<n-1;i+=2){
		if(sushu(i)&&sushu(i+2)){
			cout<<i<<" "<<i+2<<endl;
		}
	}
	return 0;
}
