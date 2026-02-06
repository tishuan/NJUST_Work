#include<iostream>
#include<string.h>
#define len 4
using namespace std;
bool issorted(const int a[],int n);//判断是否升序 
int input(int a[],int n);//输入数组 
int main(){
    int a[]={0};
	input(a,len);     
    issorted(a,len);
	return 0;
}
int input(int a[],int n){
	int shu;
	for(int i=0;i<n;i++){
		cin>>shu;
		a[i]=shu;
	}
}
bool issorted(const int a[],int n){
	int num=0;
	for(int i=0;i<n;i++){
		if(a[i]>a[i+1])
		num++;
	}
	if(num>0)
	cout<<"不是按升序排列"; 
	if(num==0)
	cout<<"是按升序排列"; 
}
