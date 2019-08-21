
#include "BinTree.h"


/*
	Private methods.
*/

bool BinTree::insert(BinTree::Node*& tree, const T& d)
{
	bool ret = false;
	if (tree == nullptr)
	{
		tree = new BinTree::Node(d);
		++m_nCount;
		return true;
	}
	
	if (d < tree->data)
	{
		ret = insert(tree->pLeft, d);
	}
	else if (d > tree->data)
	{
		ret = insert(tree->pRight, d);
	}
	else
	{
		ret = false;
	}

	if (ret == true)
	{
		sort(tree);
	}
	return ret;
}


bool BinTree::remove(BinTree::Node*& tree, const T& d)
{
	bool ret = false;
	BinTree::Node*& pFindNode = find(tree, d);
	if (pFindNode == nullptr)
	{
		return false;
	}

	/*
		If it's a leaf Node, just delete it.
	*/
	if (pFindNode->pLeft == nullptr || pFindNode->pRight == nullptr)
	{
		delete pFindNode;
		--m_nCount;
		pFindNode = nullptr;
		ret = true;
	}
	else
	{
		//	如果不是叶子节点: 在子树中选一个叶子节点.
		//    将这个叶子节点的值和本节点的值进行交换.
		//    最后再删除叶子节点. 
		//	一般在树高度比较大的子树中找叶子节点.
		//  必须尽量避免树变成斜树
		int nLeftHeight = height(pFindNode->pLeft);
		int nRightHeight = height(pFindNode->pRight);

		if (nLeftHeight > nRightHeight)
		{
			BinTree::Node*& pLeftMaxNode = maxNode(pFindNode->pLeft);
			pFindNode->data = pLeftMaxNode->data;
			ret = remove(pFindNode->pLeft, pLeftMaxNode->data);
		}
		else
		{
			Node*& pRightMinNode = minNode(pFindNode->pRight);
			pFindNode->data = pRightMinNode->data;
			ret = remove(pFindNode->pRight, pRightMinNode->data);
		}

	}

	if (ret == true)
	{
		sort(tree);
	}
	return ret;
}


void BinTree::clear(BinTree::Node*& tree)
{
	/*
		Delete via post traverse.
	*/
	if (tree == nullptr)
	{
		return;
	}
	clear(tree->pLeft);
	clear(tree->pRight);
	delete tree;
	tree = nullptr;
}


int BinTree::height(const BinTree::Node* tree)
{
	if (tree == nullptr)
	{
		return 0;
	}
	int leftHeight = height(tree->pLeft);
	int rightHeight = height(tree->pRight);
	return (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}


BinTree::Node*& BinTree::maxNode(BinTree::Node*& tree)
{

	if (tree == nullptr)
	{
		return tree;
	}
	while (tree->pRight)
	{
		tree = tree->pRight;
	}
	
	return tree;
}


BinTree::Node*& BinTree::minNode(BinTree::Node*& tree)
{
	if (tree == nullptr)
	{
		return tree;
	}
	while (tree->pLeft)
	{
		tree = tree->pLeft;
	}

	return tree;
}


BinTree::Node*& BinTree::find(BinTree::Node*& tree, const T& d)
{
	if (tree == nullptr)
	{
		return tree;
	}
	if (tree->data == d)
	{
		return tree;
	}
	else if(d < tree->data)
	{
		return find(tree->pLeft, d);
	}
	else
	{
		return find(tree->pRight, d);
	}
}


void BinTree::sort(BinTree::Node*& tree)
{
	if (tree == nullptr)
	{
		return ;
	}
	int nLeftHeight = height(tree->pLeft);
	int nRightHeight = height(tree->pRight);

	if(nLeftHeight - nRightHeight > 1)
	{
		//	The left child tree oblique
		nLeftHeight = height(tree->pLeft->pLeft);
		nRightHeight = height(tree->pLeft->pRight);
		(nLeftHeight >= nRightHeight) ? 
			rotateRight(tree)
		:	rotateLeftRight(tree);
	}
	else if(nRightHeight - nLeftHeight > 1)
	{
		//	The right child tree oblique
		nLeftHeight = height(tree->pRight->pLeft);
		nRightHeight = height(tree->pRight->pRight);
		(nRightHeight >= nLeftHeight) ?
			  rotateLeft(tree)
			: rotateRightLeft(tree);
	}
}

// 右旋转(用于将左斜树旋转成平衡树)
void BinTree::rotateRight(Node*& T) {
	/*
	*   		T[10]				N1[5]
	*        /     \		  	  /      \
	*   	N1[5]	12	  	   N2[3]    [T]10
	*      /   \		==>	      \	    /   \
	*   N2[3]   8		  		  4    8    12
	*   	 \
	*   	  4
	*
	* 发生改变的指针:
	* 1. N1->pRight 变成了T
	* 3. T->pLeft   变成了N1->pRight
	**/
	Node* N1 = T->pLeft;
	T->pLeft = N1->pRight;
	N1->pRight = T;
	T = N1;
}

// 左旋转(用于将右斜树旋转成平衡树)
void BinTree::rotateLeft(Node*& T) {
	/*
	*	   T[10]					   N1(15)
	*	  /     \					  /       \
	*	 8	   N1[15]			   T[10]	  N2[18]
	*	 	   /    \	   ==> 	  /	   \       /
	*	 	 12    N2[18]		 8     12     16
	*	 		   /
	*	 		  16
	*/
	Node* N1 = T->pRight;
	T->pRight = N1->pLeft;
	N1->pLeft = T;
	T = N1;
}

// 右左旋(将右斜树先右旋再左旋)
void BinTree::rotateRightLeft(Node*& T) {
	/*      T[10]						 T[10]	 ===> 将T作为顶点(右单斜)
	*        \						      \		   进行左旋
	*		  (N1)20   => 将N1作为   ==>   N2[15]
	*		   /		 顶点(左斜)进行右旋	 \
	*		 N2[15]							N1[20]
	*
	*/
	rotateRight(T->pRight);
	rotateLeft(T);
}

// 左右旋(将左斜树先左旋再右旋)
void BinTree::rotateLeftRight(Node*& T) {
	/*	   T[10]					 T[10]   => 将T作为顶点(左单斜)
	/						  /			   进行右旋
	N1[5]  => 将N1作为顶点  =>   N2[8]
	\     (右斜)进行左旋      /
	N2[8]		 		  N1[5]
	*/
	rotateLeft(T->pLeft);
	rotateRight(T);
}

void BinTree::preOrder(BinTree::Node*& tree)
{
	if (tree == nullptr) return;

	std::cout << tree->data << std::endl;
	preOrder(tree->pLeft);
	preOrder(tree->pRight);
}


void BinTree::inOrder(BinTree::Node*& tree)
{
	if (tree == nullptr) return;

	preOrder(tree->pLeft);
	std::cout << tree->data << std::endl;
	preOrder(tree->pRight);
}


void BinTree::postOrder(BinTree::Node*& tree)
{
	if (tree == nullptr) return;

	preOrder(tree->pLeft);
	preOrder(tree->pRight);
	std::cout << tree->data << std::endl;
}


void BinTree::printTree(BinTree::Node*& tree, char ch, int width)
{
	if (tree == nullptr) return;
	/*
		Output via midOrder.
	*/
	printTree(tree->pRight,'/',width + 2);
	std::cout << std::setw(width)
		<< ch
		<< ' '
		<< tree->data
		<< std::endl;
	printTree(tree->pLeft, '\\', width + 2);
}



/*
	Public methods.
*/


bool BinTree::insert(const T& d)
{
	return insert(m_pRoot, d);
}


bool BinTree::remove(const T& d)
{
	return remove(m_pRoot,d);
}


void BinTree::clear()
{
	clear(m_pRoot);
	m_pRoot = nullptr;
	m_nCount = 0;
}


int BinTree::height()
{
	return height(m_pRoot);
}


void BinTree::preOrder()
{
	preOrder(m_pRoot);
}


void BinTree::inOrder()
{
	inOrder(m_pRoot);
}


void BinTree::postOrder()
{
	postOrder(m_pRoot);
}


void BinTree::printTree()
{
	printTree(m_pRoot, '-', 1);
}