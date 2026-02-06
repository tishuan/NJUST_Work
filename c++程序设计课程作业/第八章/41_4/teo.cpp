#include<iostream>
#include<string.h>
#include<memory.h>
using namespace std;
void print(char *p[],int n);
void delet(char **a,int n);
void compare(char * *p,int n);
void cinn(char **p,int n,char *buf);
bool judge(char *p[],char b[],int n);
int main(){
	int n=0;
	cin>>n;
	char * *p=new char* [n];
	char *buf=new char [1000];
	cin.getline(buf,1000);
	cinn(p,n,buf);
	delete []buf;
	compare(p,n);
    print(p,n);
    delet(p,n);
	return 0; 
}
void cinn(char **p,int n,char *buf){
	for(int i=0;i<n;i++){
		memset(buf,0,1000);
		cin.getline(buf,1000);
		int len=strlen(buf);
		p[i]=new char [len+1];
		strncpy(p[i],buf,len+1);
	}
}
void compare(char * *p,int n){
	for(int i=0;i<n;i++){
		for(int j=0;j<n-i-1;j++){
			if(strcmp(p[j],p[j+1])>0){
		     	char *tmp=p[j];
		    	p[j]=p[j+1];
			    p[j+1]=tmp;
		    }
		}
	} 
}
void print(char **a,int n){
	for(int i=0;i<n;i++){
		cout<<a[i]<<" ";
	}
	cout<<endl;
}
void delet(char **a,int n){
	for(int i=0;i<n;i++){
		delete []a[i];
	}
}
