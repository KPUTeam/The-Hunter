#pragma once

#include "Global.h"

template<typename T, int Size = 100>

class CCircleQueue
{
private:
	T		m_Queue[Size];
	UINT	m_iHead;
	UINT	m_iTail;
	UINT	m_iSize;

public:
	CCircleQueue()
	{
		m_iHead = m_iTail = 0;
		m_iSize = 0;
	}

	~CCircleQueue()
	{
	}

public:
	bool push(const T& data)
	{
		if (full())
			return false;

		UINT  idx = (m_iTail + 1) % Size;

		m_Queue[idx] = data;

		m_iTail = idx;

		++m_iSize;

		return true;
	}

	bool pop(T& data)
	{
		if (empty())
			return false;

		UINT	idx = (m_iHead + 1) % Size;

		data = m_Queue[idx];

		m_iHead = idx;

		--m_iSize;

		return true;
	}

	bool empty()
	{
		return m_iHead == m_iTail;
	}

	bool full()
	{
		returm((m_iTail + 1) % Size) == m_iHead;
	}

	UINT size()
	{
		return m_iSize;
	}
};

