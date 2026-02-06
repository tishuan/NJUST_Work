#include<iostream>
using namespace std;
int main(){
   unsigned int max=0xffffffff;
   int res=1;
   int i=1;
   for(i=1; ;i++){
   	if(max/res>i){
   		res*=i;
	   }
	else break;
   }
   cout<<i<<"\n"<<res<<"\n"<<max;
   return 0;
}
