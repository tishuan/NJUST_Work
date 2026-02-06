#include<iostream>
#include<string.h>
#define MAX 3
using namespace std;
int getrev(char a[]);
struct ming{
	char a[20];
	int b;
};
int main(){
	ming ch[MAX];
	for(int i=0;i<MAX;i++){
		cin>>ch[i].a;
		ch[i].b=getrev(ch[i].a);
	}
	for(int i=0;i<MAX;i++){
		for(int j=0;j<MAX-i-1;j++){
			if(ch[j].b>ch[j+1].b){
				ming tmp=ch[j];
				ch[j]=ch[j+1];
				ch[j+1]=tmp;
			}
		}		
	}
	for(int i=0;i<MAX;i++){
		cout<<ch[i].a<<" ";
		cout<<ch[i].b<<endl;
	}
	
	return 0;
} 
int getrev(char a[]){
    int len=strlen(a);
	int right=len-1;
	int count=0;
	for(int left=0;left<right;left++){
		for(int i=1;i<right-left+1;i++){
			if(a[left]>a[left+i])
			count++;
		}
	}
	return count;
}
