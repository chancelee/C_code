
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
		//	�������Ҷ�ӽڵ�: ��������ѡһ��Ҷ�ӽڵ�.
		//    �����Ҷ�ӽڵ��ֵ�ͱ��ڵ��ֵ���н���.
		//    �����ɾ��Ҷ�ӽڵ�. 
		//	һ�������߶ȱȽϴ����������Ҷ�ӽڵ�.
		//  ���뾡�����������б��
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

// ����ת(���ڽ���б����ת��ƽ����)
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
	* �����ı��ָ��:
	* 1. N1->pRight �����T
	* 3. T->pLeft   �����N1->pRight
	**/
	Node* N1 = T->pLeft;
	T->pLeft = N1->pRight;
	N1->pRight = T;
	T = N1;
}

// ����ת(���ڽ���б����ת��ƽ����)
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

// ������(����б��������������)
void BinTree::rotateRightLeft(Node*& T) {
	/*      T[10]						 T[10]	 ===> ��T��Ϊ����(�ҵ�б)
	*        \						      \		   ��������
	*		  (N1)20   => ��N1��Ϊ   ==>   N2[15]
	*		   /		 ����(��б)��������	 \
	*		 N2[15]							N1[20]
	*
	*/
	rotateRight(T->pRight);
	rotateLeft(T);
}

// ������(����б��������������)
void BinTree::rotateLeftRight(Node*& T) {
	/*	   T[10]					 T[10]   => ��T��Ϊ����(��б)
	/						  /			   ��������
	N1[5]  => ��N1��Ϊ����  =>   N2[8]
	\     (��б)��������      /
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