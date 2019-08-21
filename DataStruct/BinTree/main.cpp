#include "BinTree.h"

int main()
{
	BinTree tree;
	tree.insert(10);
	tree.insert(8);
	tree.insert(15);
	tree.insert(4);
	tree.insert(3);
	tree.printTree();

	std::cout << std::endl;

	tree.remove(4);
	tree.printTree();

	return 0;
}