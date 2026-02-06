#include<iostream>
using namespace std;
struct Fraction{
	int a,b;
};
struct Result{
	double sum,diff,amass,sh;
};
Fraction huajian(int a,int b){	
    int min=a>b?b:a;
	for(int i=min;i>0;i--){
		if(a%i==0&&b%i==0){
			a/=i;
			b/=i;
			break;
		}
	}
	Fraction fra;
	fra.a=a;
	fra.b=b;
	return fra;
}
Result panduan(Fraction x,Fraction y){
	cout<<"分数大小比较 ";
	if(x.a*y.b-x.b*y.a>0)
	   cout<<">";
	else if(x.a*y.b-x.b*y.a<0)
	   cout<<"<";
	else cout<<"=";
	cout<<endl;
	Result resu;
	resu.sum=(x.a*y.b+x.b*y.a+0.0)/x.b/y.b;
	resu.diff=(x.a*y.b-x.b*y.a+0.0)/x.b/y.b;
	resu.amass=(x.a+0.0)*y.a/x.b/y.b;
	resu.sh=(x.a+0.0)*y.b/x.b/y.a;
	return resu;
}
int main(){
	int a,b,c,d;
	cin>>a>>b>>c>>d;
	Fraction t=huajian(a,b);
	Fraction z=huajian(c,d);
	Result rl=panduan(t,z);
	cout<<"和 "<<rl.sum<<" 差 "<<rl.diff<<" 积 "<<rl.amass<<" 商 "<<rl.sh;
	return 0;
} 
