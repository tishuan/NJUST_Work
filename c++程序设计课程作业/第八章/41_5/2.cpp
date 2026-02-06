#include<iostream>
#include<math.h>
#include<string.h>
using namespace std;
int main(){
	char a[100]={0};
	cin.getline(a,100);
	int len=strlen(a);
	int i=len;
	int j=0,count=0,sum=0;
	int q[100]={0};
	int b[100]={0};
	while(i--){
		if(a[i]>='0'&&a[i]<='9'){
			sum+=(a[i]-'0')*pow(10,count);
			count++;
			if(a[i-1]<'0'||a[i-1]>'9'){
				q[j]=sum;
				sum=0;
				j++;
			}
		}
		else{
			count=0;
		}		
	}
	for(int i=0,t=j-1;i<j;i++,t--){
		b[i]=q[t];
	}
	for(int i=0;i<j;i++){
		cout<<b[i]<<" ";
	}

	return 0;
}

