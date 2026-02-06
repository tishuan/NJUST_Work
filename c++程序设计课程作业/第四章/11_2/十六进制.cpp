#include<iostream>
using namespace std;
int main()
{
	int n;
	cin>>n;
	for(int i=7;i>=0;i--){
		int tmp=n>>(i*4);
		tmp=tmp&0xf;
		if(tmp>=0xa&&tmp<=0xf){
			int t=tmp-9;
			switch(t){
				case 1:cout<<'A';
				       break;
				case 2:cout<<'B';
				       break;
				case 3:cout<<'C';
				       break;
				case 4:cout<<'D';
				       break;
				case 5:cout<<'E';
				       break;
				case 6:cout<<'F';
				       break;
			}
		}
		else cout<<tmp;
	} 

	return 0;
}
