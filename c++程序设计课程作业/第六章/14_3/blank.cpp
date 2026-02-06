#include<iostream>
#include<string.h>
#define len 20
using namespace std;
void trim(char a[],int n);
int main(){
    char a[len]={0};
    cin.getline(a,len);
    trim(a,len); 
	return 0;
}
void trim(char a[],int n){
	n=strlen(a);
	int i=0,j=n-1;
	while(a[i++]==' ');  //两个等于号 
	i--;    //先增完后发现不等于break，所以要减一 
	while(a[j--]==' ');
	j++;
	for(int k=i;k<=j;k++){
		cout<<a[k];
	}
	strncpy(a,a+i,j-i+1);
	a[j-i+1]=0;
}
