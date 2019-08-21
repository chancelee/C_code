#include <iostream>

template<typename T>
class Node
{
public:
	T m_data;
	Node* m_pNext;
	Node(const T& data) : m_data(data), m_pNext(nullptr) {}
};

template<typename T>
/*
	The member m_pHead doesn't store data, while just pointing to the first node.
*/
class LinkList
{
private:
	Node<T>* m_pHead;
	size_t m_nLen;
public:
	LinkList() :m_pHead(nullptr), m_nLen(0) {}
	~LinkList()
	{
		clear();
	}

	void clear();

	bool insert(size_t index,const T& data);

	bool remove(size_t index,T* pOlddata);

	size_t size() const{ return m_nLen; }

	bool isEmpty() const{ return m_nLen == 0; }

	void push_back(const T& data) { insert(m_nLen, data); }

	T pop_back() 
	{
		T data;
		if (m_nLen == 0)
		{
			throw std::exception("List is empty.");
		}
		remove(m_nLen - 1, &data);
		return node;
	}

	template <typename T>
	friend std::ostream& operator<<(std::ostream& out, const LinkList<T>& list);
};


template<typename T>
bool LinkList<T>::insert(size_t index, const T& data)
{
	if (index < 0 || index > m_nLen)
	{
		return false;
	}

	Node<T>* pNewNode = new Node<T>(data);

	if (m_pHead == nullptr)
	{
		m_pHead = new Node<T>(0);
		m_pHead->m_pNext = pNewNode;
	}
	else
	{
		Node<T> *pPre = m_pHead;
		for (size_t i = 0; i < index ; ++i)
		{
			pPre = pPre->m_pNext;
		}
		pNewNode->m_pNext = pPre->m_pNext;
		pPre->m_pNext = pNewNode;
	}

	++m_nLen;
	return true;
}

template<typename T>
bool LinkList<T>::remove(size_t index, T* pOlddata)
{
	if (index < 0 || index > m_nLen)
	{
		return false;
	}

	if (m_pHead->m_pNext == nullptr)
	{
		throw std::exception("List is empty.");
	}

	Node<T> *pPre = m_pHead,*pCur = m_pHead->m_pNext;

	for (size_t i = 0; i < index; ++i)
	{
		pPre = pCur;
		pCur = pCur->m_pNext;
	}
	pPre->m_pNext = pCur->m_pNext;

	delete pCur;

	*pOlddata = pCur->m_data;
	--m_nLen;
	return true;
}

template<typename T>
void LinkList<T>::clear()
{
	while (m_pHead->m_pNext)
	{
		Node<T> *pDel = m_pHead->m_pNext;
		m_pHead->m_pNext = pDel->m_pNext;
		delete pDel;
	}
	m_nLen = 0;
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const LinkList<T>& list)
{
	if (list.isEmpty())
	{
		out << "[]" << std::endl;
	}

	out << "[";
	for (Node<T> *pCur = list.m_pHead->m_pNext; pCur; pCur = pCur->m_pNext)
	{
		out << pCur->m_data;
		out << ",";
	}
	out << "\b]" << std::endl;

	return out;
}


int main()
{
	LinkList<int> list;

	list.insert(0, 10);
	list.insert(0, 11);
	list.insert(2, 12);
	list.insert(1, 13);

	std::cout << list << std::endl;

	int oldData = 0;
	list.remove(1, &oldData);
	list.remove(0, &oldData);
	list.remove(1, &oldData);

	return 0;
}