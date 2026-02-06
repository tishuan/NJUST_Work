#include<iostream>
using namespace std;
bool issmith(int n);
int main(){	
	int n;cin>>n;
	issmith(n);	
	return 0;
}
bool issmith(int n){
	int a[]={0};
	if(n<2) return 0;
	int i=2,t=n,count=0,k[]={0};
	while(i<=n&&t!=1){
		if(t%i!=0) i++;
		else{
			a[count]=i;
			t/=i;
			k[count]=a[count];			 			
			cout<<k[count]<<"*";
			count++;
		}
	}//分解质因数存入数组
	cout<<"\b"<<"="<<n;
	cout<<endl;
	int x,b,num1=0,num2=0,y=0;
	while(n>0){		
		x=n%10;
		n/=10;
		cout<<x<<"+";
		num1+=x;
	} 
cout<<"\b"<<" "<<endl;
	while(y<=count){
		for(int y=0;k[y]>0&&y<=count;y++){
			for(int j=0;k[y]>0&&y<count;j++){
				b=k[y]%10;
				cout<<b<<"+";
		        k[y]/=10;
		        num2+=b;
			}
		}
		break;
		}
		cout<<"\b"<<" "<<endl;			
    if(num1==num2){
		cout<<"yes";				
		}
	if(num1!=num2){			
		cout<<"no";
		}		
}
