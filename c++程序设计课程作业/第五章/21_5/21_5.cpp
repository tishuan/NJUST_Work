#include<iostream>
using namespace std;
int main(){
int n;
	cin>>n;
	cout<<n<<"=";
	int i=1;
	while(1){
		i++;
		bool issushu=true;
		for(int j=2;j<=i/2;j++){
			if(i%j==0)
			issushu=false;
		}
		if(issushu==false)
		continue;
		if(i>n)
		break;
		while(n%i==0){
			cout<<i<<"*";
			n/=i;
		}
	}
	cout<<"\b"<<" ";
return 0;

}
