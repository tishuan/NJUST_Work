#include<iostream>
#include<stdlib.h>
#define len 10
using namespace std;
void getRandom(int a[],int n);
int getMax(const int a[],int n);
int main(){
	int a[]={0};
	getRandom(a,len);
	getMax(a,len);
	return 0;
}
void getRandom(int a[],int n){
	for(int i=0;i<n;i++){
		a[i]=rand()%100;
		cout<<a[i]<<" ";
	}
	cout<<endl;
}
int getMax(const int a[],int n){
	int max=a[0];
	int index=0;
	for(int i=0;i<n;i++){
		if(a[i]>max){
			max=a[i];
			index=i;
		}		
	}
	cout<<"find max "<<max<<" in array ["<<index<<"]";
}
