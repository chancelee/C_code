#include<iostream>
#include "ecc.h"
using namespace std;


/*
	functions of class Point
*/
ECC::Point::Point(){
	this->x = 0;
	this->y = 0;
}
ECC::Point::Point(int x, int y){
	this->x = x;
	this->y = y;
}
ECC::Point::~Point(){}
int ECC::Point::getX(){
	return this->x;
}
int ECC::Point::getY(){
	return this->y;
}

bool ECC::Point::operator==(Point& p){
	if( this->getX() == p.getX() && this->getY() == p.getY() )
		return true;
	else
		return false;
}

ECC::Point* ECC::Point::PP(Point *p,int a,int c){
	int x,y;
	int m;
	
	if( *this == *p ){
		if( this->getY() == 0)
			return new Point(0,0); 
		m = ( 3 * this->getX() * this->getX() + a ) / ( 2 * this->getY() );
	}
	else{

		if( this->getX() == p->getX() ) 
			return new Point(0,0);
		m = ( this->getY() - p->getY() ) / ( this->getX() - p->getX() );
	}
	x = m * m - this->getX() - p->getX();
	y = m * ( this->x - x ) - this->y;
	while( x < 0 ){
		x += c;
	}
	x = x % c;
	while( y < 0 ){
		y += c;
	}
	y = y % c;
	return new Point(x,y);
}


ostream& operator<<(std::ostream &out,ECC::Point& p){
	printf("(%d,%d)",p.getX(),p.getY());
	return out;
}

/*
	functions of class ECC
*/
ECC::ECC(int a, int b, int p){
	this->a = a;
	this->b = b;
	this->p = p;
}

int ECC::getA(){
	return this->a;
}
int ECC::getP(){
	return this->p;
}

void ECC::printAB(){
	cout<<"a:"<<this->a<<endl;
	cout<<"b:"<<this->b<<endl;
}

int* ECC::enc(int order,Point *points,int privkey){
	int *ret = (int*)malloc(sizeof(int)*2);
	srand(time(0));
//	int privkey = rand() % 	(order - 1) + 1;
	int symkey = rand() % (order - 1) + 1;
	cout<<"generate the symmetric key to be encrypted:"<<symkey<<","<<points[symkey-1]<<endl;
	int x1;
	int x2;
	int c;
	do{
		x1 = 3;//rand() % (order - 1) + 1;
		x2 = x1 * privkey%order; 	
	}while(points[x2-1].getX() == 0 || x2 == 0);
	c = points[x2-1].getX() * symkey % order; 

	ret[0] = x1;
	ret[1] = c;
	
	return ret;
}

void ECC::dec(int privkey,int x1,int cipher, int order,Point*points){
	int x2 = privkey * x1 % order;
	x2 = points[x2-1].getX();
	cout<<"x2:"<<x2<<endl;
	int i = 1 ;
	while( x2 * i % order != 1 ) i++;
	cout<<"i:"<<i<<endl;
	int plain = cipher * i % order;
	cout<<"the symkey decrypted:"<<plain<<","<<points[plain-1];
}



