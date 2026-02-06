#include<iostream>
#include<string.h>
using namespace std;
void trans(char ch); 
void tcatleft(int index);
void tcatright(int index);
int main(){
	char a[]="1234.56";
	int len=strlen(a);
	int index=0;
	while(a[index++]!='.');
	index--;//indexÎª4; 
	int i=0;
	while(i<index){//Ð¡Êýµã×ó±ß 
		trans(a[i]);//°Ñ×ó±ßµÄ»»³É´óÐ´£» 
		tcatleft(index-i); 
		i++;
	}
	for(i=index+1;i<index+3;i++){
		trans(a[i]);
		tcatright(i-index);
	}
	cout<<a;
	return 0;
}

void trans(char ch){
	switch(ch){
		case'0':
			cout<<"Áã";
			break;
		case'1':
			cout<<"Ò¼";
			break;
		case'2':
			cout<<"·¡";
			break;
		case'3':
			cout<<"Èþ";
			break;
		case'4':
			cout<<"ËÁ";
			break;
		case'5':
			cout<<"Îé";
			break;
		case'6':
			cout<<"Â½";
			break;
		case'7':
			cout<<"Æâ";
			break;
		case'8':
			cout<<"°Æ";
			break;
		case'9':
			cout<<"¾Á";	
		default:
			
			break;
				
	}
}
void tcatleft(int index){
	switch(index){
		case 1:
			cout<<"Ôª";
			break;
		case 2:
			cout<<"Ê°";
			break;
		case 3:
			cout<<"°Û";
			break;
		case 4:
			cout<<"Çª";
			break;
		default: break;
	}
}
void tcatright(int index){
	switch(index){
		case 1:
			cout<<"½Ç";
			break;
		case 2:
			cout<<"·Ö";
			break;
		default: break;
	}
}
