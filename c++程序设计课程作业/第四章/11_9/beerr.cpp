#include<iostream>
using namespace std;
int main(){
	int gai,blank,pn,mon;//mon为现有的钱 
	cin>>mon;
	int k=2;
	pn=mon/2;
	gai=pn;
	blank=pn;
	while(1){	
		if(gai>=4){
			pn++;
			blank++; 
			gai-=3;
		}
		if(blank>=2){
			pn++;
			gai++; 
			blank-=1;
		}
		else break;		
	}
	cout<<pn;
	return 0;
}
