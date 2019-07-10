#include<iostream>
#include <iomanip>
#include "cstdlib"
#include "ctime"
#include "ecc.h"

using namespace std;

#define PRIVKEY 3
int main(int argc, char *argv[])
{
	ECC ecc(-2,-3,7);
	ECC::Point G(3,2);
	ECC::Point *p = &G;
	ECC::Point O(0,0); 
	ECC::Point *points = (ECC::Point*)malloc(sizeof(ECC::Point)*N);
	int order = 1; //½× 
	points[order-1] = G;
	int a = ecc.getA();
	int c = ecc.getP();
	
	cout<<setw(2)<<order<<"G:";
	cout<<G<<endl;
	while(a<0){
		a += c;
	}
	while(1){
		p = points[order-1].PP(&G,a,c);
		if( *p == O ){
			++order;
			cout<<setw(2)<<order<<"G:";
			cout<<*p<<endl;
			
			break;
		}
		points[order++] = *p;
		cout<<setw(2)<<order<<"G:";
		cout<<*p<<endl;
		delete p;
		p = NULL;
	}
	cout<<"the order of G is ";
	cout<<order<<endl; 

	
	cin.get();
	cout<<"begin simulating encrypting the symmetric key..."<<endl;
	int *ret = NULL;
	ret = ECC::enc(order,points,PRIVKEY);
	/*
	ret[0] = privkey;
	ret[1] = symkey;
	ret[2] = x1;
	ret[3] = c;
	*/
	printf("x1:%d\n",ret[0]);
	printf("cipher:%d\n",ret[1]);
	int x1 = ret[0];
	int cipher = ret[1];
	cout<<"Get the cipher: [ ";
	cout<<points[x1-1]<<",";
	cout<<points[cipher-1];
	cout<<" ]"<<endl;
	
	cin.get(); 
	cout<<"begin simulating decrypting the symmetric key..."<<endl;
	ECC::dec(ret[0],x1,cipher,order,points);
	
	
	cin.get();
	return 0;
}

