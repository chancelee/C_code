#pragma once

#include<iostream>
#define N 20
class ECC{
private:
	int a,b;
	int p;
	
	
public:
	ECC(int a,int b, int p);
	int getA();
	int getP();
	void printAB(); 
	
	class Point{
	private:
		int x,y;
	public:
		Point();
		Point(int x, int y);
		~Point(); 
		int getX();
		int getY();
		Point* PP(Point *p , int a, int c);  //point plus
		bool operator==(Point &p);
		friend std::ostream& operator<<(std::ostream &out,Point &p);
	};
	static int* enc(int order,Point *points,int privkey);
	static void dec(int privkey,int x1,int cipher, int order,Point*points);
};


