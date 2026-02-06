#include<iostream>
using namespace std;
#define HUM_NUM 21
int main(void){	
	int count=0;
	int livenum=HUM_NUM;
	int i=0;
	bool hlive[HUM_NUM];
	for(int i=0;i<HUM_NUM;i++){
		hlive[i]=true;
	}//游戏开始时所有人都为true，都活着； 
	while(livenum>1){
		if(hlive[i]==true){
			count++;
			if(count%5==0){
				hlive[i]=false;
				livenum--;
			}
		}
		i++;
		i%=HUM_NUM;//以HUM_NUM为一个轮回； 
	}
	for(i=0;i<HUM_NUM;i++){
		if(hlive[i]==true){
			cout<<i+1<<endl;
		}
	}
	return 0;
}
