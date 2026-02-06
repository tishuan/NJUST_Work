#include<iostream>
using namespace std;
void switchh(int a[],int n);
bool judge(int a[],int n);
bool check(int b[],int n,int m);
void sort(int a[],int n);
int  fuzhi(int a[],int b[],int n);
void print(int a[],int n);
int main(){
	int a[14]={3,5,4,7,3,6,7,4,'J','A','K',9,3};
	int b[14]={0};
	switchh(a,13);
	print(a,13);
	int num=fuzhi(a,b,13);
	sort(b,num);
	print(b,num);
	bool flag=judge(b,num);
	if(flag==true)
	  cout<<"有连续牌";
	else cout<<"无连续牌";
	return 0;
}
void print(int a[],int n){
	for(int i=0;i<n;i++){
		cout<<a[i]<<" ";
	}
	cout<<endl;
}
void switchh(int a[],int n){
	for(int i=0;i<n;i++){
		switch(a[i]){
			case 'J':a[i]=11;
					 break;
			case 'Q':a[i]=12;
					 break;
			case 'K':a[i]=13;
					 break;
			case 'A':a[i]=14;
					 break;
		    default: break;

		}
	}
}
int  fuzhi(int a[],int b[],int n){
	int j=0;
	for(int i=0;i<n;i++){
		if(check(b,n,a[i])==true){
			b[j]=a[i];
			j++;
		}
	}
	return j;	
}
bool judge(int a[],int n){
	bool flag=false;
	for(int i=0;i<n;i++){
		if(a[i+4]-a[i]==4){
			flag=true;
			break;
		}
	}
	return flag;	
}
void sort(int a[],int n){
	for(int i=0;i<n;i++){
		for(int j=0;j<n-i-1;j++){
			if(a[j]>a[j+1])
			  swap(a[j],a[j+1]);
		}
	}
}
bool check(int b[],int n,int m){
	bool flag=true;
	for(int i=0;i<n;i++){
		if(b[i]==m){
			flag=false;
			break;
		}
	}
	return flag;
}

