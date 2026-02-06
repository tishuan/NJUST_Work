#include<iostream>
using namespace std;
bool checkagain(int rs[],int n,int m);
int getagainnnum(const int a[],int n,int m);
int getset(int rs[],int rsn[],const int a[],int n);
void showarray(int rs[],int n);
void showarray(int rs[],int rsn[],int n);
void sortarraybynum(int rs[],int rsn[],int n);
int main(){
	int a[8]={8,8,8,2,3,1,5,6};
	int rs[8]={0};
	int rsn[8]={0};
	int num=getset(rs,rsn,a,8);
	showarray(rs,num);
	showarray(rsn,num);
	sortarraybynum(rs,rsn,num);
	showarray(rs,rsn,num);
	return 0;
}
bool checkagain(int rs[],int n,int m){
	bool flag=false;
	for(int i=0;i<n;i++){
		if(rs[i]==m){
			flag=true;
			break;
		}
	}
	return flag;
}
int getagainnum(const int a[],int n,int m){
	int count=0;
	for(int i=0;i<n;i++){
		if(a[i]==m)
		count++;
	}
	return count;
}
int getset(int rs[],int rsn[],const int a[],int n){
	int j=0;
	for(int i=0;i<n;i++){
		if(checkagain(rs,j,a[i])==false){
			rs[j]=a[i];
			rsn[j]=getagainnum(a,n,rs[j]);
			j++;
		}
	}
	return j;
}
void showarray(int rs[],int n){
	for(int i=0;i<n;i++){
		cout<<rs[i]<<" ";
	}
	cout<<endl;
}
void showarray(int rs[],int rsn[],int n){
	for(int i=0;i<n;i++){
		cout<<rs[i]<<":"<<rsn[i]<<endl;
	}
}
void sortarraybynum(int rs[],int rsn[],int n){
	for(int i=0;i<n;i++){
		for(int j=0;j<n-i-1;j++){
			if(rsn[j]<rsn[j+1]){
				int tmp1=rsn[j];
				rsn[j]=rsn[j+1];
				rsn[j+1]=tmp1;
				int tmp2=rs[j];
				rs[j]=rs[j+1];
				rs[j+1]=tmp2;
			}
		}
	}
}


