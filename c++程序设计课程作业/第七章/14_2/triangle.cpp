#include<iostream>
#include<math.h>
using namespace std;

struct point{
	int x;
	int y;

};
struct bian{
	double a1;
	double a2;
	double a3;
};
double distan(struct point a,struct point b);
double zhouchang(bian a,bian b,bian c);
int main(){
	point a,b,c;
	point ch[3];
	for(int i=0;i<3;i++){
		cin>>ch[i].x>>ch[i].y;
	}
    bian bi;	
	bi.a1=distan(ch[0],ch[1]);
	bi.a2=distan(ch[1],ch[2]);
	bi.a3=distan(ch[0],ch[2]);
	if(bi.a1+bi.a2<=bi.a3){
		cout<<"buneng";		
	}
	else{
		//double zhouchang=bi.a1+bi.a2+bi.a3;
		zhouchang(bi.a1+bi.a2+bi.a3);
		double hai=(bi.a1+bi.a2+bi.a3)/2.0;
		cout<<hai<<endl;
		double t=hai*(hai-bi.a1)*(hai-bi.a2)*(hai-bi.a3);
		double mianji=sqrt(t);
		cout<<zhouchang<<" "<<mianji;
	}	
	return 0;
}
double zhouchang(bian a.a1,bian b.a2,bian c.a3){
	bian add=a+b+c;
	cout<<add;	
}
double distan(struct point a,struct point b){
	double c,d;
	d=pow(a.x-b.x,2)+pow(a.y-b.y,2);
	c=sqrt(d);
	return c;
}

