#pragma once

#include <iostream>
#include <iomanip	>
using T = int;
//template<typename T>
class BinTree
{
public:

	class Node
	{
	public:
		T	data;
		Node*	pLeft;
		Node*	pRight;

		Node():data(),pLeft(),pRight(){}

		Node(const T& d) : data(d),pLeft(nullptr),pRight(nullptr){}
	};

private:
	Node* m_pRoot = nullptr;
	int m_nCount;

private:

	bool insert(Node*& tree, const T& d);
	bool remove(Node*& tree, const T& d);
	void clear(Node*& tree);


	int height(const Node* tree);

	/*
		Get the max/min value int the tree's nodes.
	*/
	Node*& maxNode(Node*& tree);
	Node*& minNode(Node*& tree);

	Node*& find(Node*& root, const T& d);

	void sort(Node*& tree);

	/*
		Rotate
	*/
	void rotateRight(Node*& T);
	void rotateLeft(Node*& T);
	void rotateRightLeft(Node*& T);
	void rotateLeftRight(Node*& T);

	/*
		Output methods.
	*/
	void preOrder(Node*& tree);
	void inOrder(Node*& tree);
	void postOrder(Node*& tree);
	void printTree(Node*& tree, char ch, int width);

public:
	bool insert(const T& d);

	bool remove(const T& d);
	void clear();
	int height();

	void preOrder();
	void inOrder();
	void postOrder();
	void printTree();

};