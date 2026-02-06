#include<iostream>
#include<math.h>
using namespace std;
struct Jie{
	double x1,x2;
};
Jie f(int a,int b,int c){
	Jie j;
	double t=sqrt(b*b-4*a*c);
	j.x1=(-b+t)/2/a;
	j.x2=(-b-t)/2/a;
	return j;
}
int main(){
	int a,b,c;
	cin>>a>>b>>c;
	if(b*b-4*a*c<0)
	  cout<<"该函数无解";
	else{
	    Jie j=f(a,b,c);
	cout<<j.x1<<" "<<j.x2;
	}	

	return 0;
} 
