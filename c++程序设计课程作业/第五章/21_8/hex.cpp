#include<iostream>
#include"mytool.h"
using namespace std;
int max(int a,int b){
	return a>b?a:b;
}
int tohex(int x){
	for(int i=7;i>=0;i--){
		int tmp=x>>(i*4);
		tmp=tmp&0x0f;
		if(tmp>=0xa&&tmp<=0xf){
			int t=tmp-10;			
			switch(t){
				case 0:cout<<'A';break;				  
				case 1:cout<<'B';break;
				case 2:cout<<'C';break;
				case 3:cout<<'D';break;
				case 4:cout<<'E';break;
				case 5:cout<<'F';break;
				default:break;
			}
		}
		else cout<<tmp;
	}
}

