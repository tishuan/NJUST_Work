#include<iostream>
using namespace std;
int getWordcount(char a[]);
int main(){
	char a[20]={0};
	cin.getline(a,20);
	int count=getWordcount(a);
	cout<<count;
	return 0;
}
int getWordcount(char a[]){
	int count=0;
	int i=0;
	while(a[i]!='\0'){
		while(a[i]==' '&&a[i]!='\0'){			
			i++;		    
		}
		count++;
		if(a[i-1]==' '&&a[i]=='\0')
		    count--;
		while(a[i]!=' '&&a[i]!='\0'){
			i++;
		}			
	}
	return count;		
}			   

