#include <iostream>


/*
	Date: 2019/8/15
	Author: starr
*/



/*
ADT ArrayList
Data 
	Template data type.
	Pointer to buf initialized to nullptr.
	Length currently and max value.
Operation:
	Insert data;
	Delete data;
	Get size;
	Get index of any data;
	Get data of any index;
	Check if it's empty;
	Modify data of any index.
*/

template <typename T>
class ArrayList
{
private:
	T *m_pBuf;
	size_t m_nCurLen;
	size_t m_nMaxLen;
public:
	ArrayList() :m_pBuf(nullptr), m_nCurLen(0), m_nMaxLen(0) {}
	~ArrayList()
	{
		clear();
	}

	/*
		Func name:
		Description:
		Parameters:
		Return:
	*/
	bool insert(size_t index, const T& data);

	void resize(size_t size);

	bool remove(size_t index, T* pOld = nullptr);

	size_t size() const { return m_nCurLen; }

	bool isEmpty() const { return m_nCurLen == 0; }

	size_t index(const T& data) const;

	T& get(size_t index) const;

	T& operator[](size_t index)
	{
		return get(index);
	}

	void clear()
	{
		delete[] m_pBuf;
		m_nCurLen = m_nMaxLen = 0;
	}

	template<typename T>
	friend std::ostream& operator<< (std::ostream& out, const ArrayList<T>& arr);

	void push_back(const T& data)
	{
		insert(m_nCurLen, data);
	}

	T pop_back()
	{
		T old;
		if (isEmpty())
		{
			throw std::exception("ArrayList is empty");
		}
		remove(m_nCurLen - 1, &old);
		return old;
	}
};


template<typename T>
bool ArrayList<T>::insert(size_t index, const T& data)
{
	// Param checking.
	if (index < 0 || index > m_nCurLen) return false;

	if (m_nCurLen == m_nMaxLen)
	{
		resize((m_nMaxLen + 1) * 2);
	}

	memmove(m_pBuf + index + 1,
			m_pBuf + index,
			sizeof(T) * (m_nCurLen - index) );

	m_pBuf[index] = data;
	++m_nCurLen;

	return true;
}

template<typename T>
void ArrayList<T>::resize(size_t size)
{
	if (m_pBuf == nullptr)
	{
		m_pBuf = new T[size];
		m_nMaxLen = size;
	}
	else
	{
		T* pNewBuf = new T[size];
		memcpy(pNewBuf,m_pBuf,
				(size > m_nMaxLen ? m_nMaxLen : size) * sizeof(T));
		delete[] m_pBuf;
		m_pBuf = pNewBuf;
		m_nMaxLen = size;

	}
}


template<typename T>
bool ArrayList<T>::remove(size_t index, T* pOld = nullptr)
{
	if (index < 0 || index > m_nCurLen)
	{
		return false;
	}
	if (pOld != nullptr)
	{
		*pOld = m_pBuf[index];
	}

	memmove(m_pBuf + index,
			m_pBuf + index + 1,
			sizeof(T) * (m_nCurLen - index - 1));

	--m_nCurLen;

	return true;
}


template<typename T>
std::ostream& operator<<(std::ostream& out, const ArrayList<T>& arr)
{
	out << "[";
	for (size_t i = 0; i < arr.m_nCurLen; ++i)
	{
		out << arr.m_pBuf[i];
		out << ",";
	}
	out << "]";
	return out;
}


template<typename T>
size_t ArrayList<T>::index(const T& data) const
{
	for (int i = 0; i < m_nCurLen; ++i) {
		if (m_pBuff[i] == data) {
			return i;
		}
	}
	return -1;
}

template<typename T>
 T& ArrayList<T>::get(size_t index) const
{
	if (index < 0 || index > m_nCurLen)
	{
		throw std::exception("index out of bound");
	}
	return m_pBuf[index];
}


 int main()
 {
	 ArrayList<int> arr;

	 arr.insert(0, 1);
	 arr.insert(0, 2);
	 arr.insert(0, 3);

	 try 
	 {
		std::cout << arr.get(1) << std::endl;
		std::cout << arr.get(-1) << std::endl;

	 }
	 catch (std::exception e)
	 {
		 std::cout << e.what() << std::endl;
	 }

	 std::cout << arr << std::endl;

	 arr.remove(2);
	 arr.remove(0);
	 arr.remove(0);


	 return 0;
 }
