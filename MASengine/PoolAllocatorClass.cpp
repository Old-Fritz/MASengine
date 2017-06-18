#include "PoolAllocatorClass.h"

PoolAllocatorClass::PoolAllocatorClass()
{
	m_elSize = 0;
	m_poolSize = 0;
}

PoolAllocatorClass::PoolAllocatorClass(const PoolAllocatorClass &)
{
}

PoolAllocatorClass::~PoolAllocatorClass()
{
}

bool PoolAllocatorClass::Initialize(size_t elSize, long int poolSize)
{
	m_elSize = elSize;
	m_poolSize = poolSize;

	char* mem = new char[elSize*poolSize];
	if (!mem)
	{
		LogManagerClass::getI().addLog("Error 1-1");
		return false;
	}
	//create new blocks of free memory
	for (int i = 0;i < poolSize;i++)
	{
		void* element = mem + i*elSize;

		m_freeEls.emplace_back(element);
	}

	return true;

}

bool PoolAllocatorClass::doublePool()
{
	char* mem = new char[m_elSize*m_poolSize];
	if (!mem)
	{
		LogManagerClass::getI().addLog("Error 1-2");
		return false;
	}
	//create new blocks of free memory
	for (int i = 0;i < m_poolSize;i++)
	{
		void* element = mem + i*m_elSize;

		m_freeEls.emplace_back(element);
	}

	m_poolSize *= 2;

	return true;
}

size_t PoolAllocatorClass::getSize()
{
	return m_elSize;
}

void * PoolAllocatorClass::getMemory()
{
	//check for existing of size
	if (!m_freeEls.size())
		doublePool();

	//delete first element from free and add it to list of occupied
	void* pointer = m_freeEls.front();

	m_freeEls.erase(m_freeEls.begin());

	m_occupEls.emplace_back(pointer);

	return pointer;
}

void PoolAllocatorClass::deleteEl(void * pointer)
{
	//search for element in list end replace it
	for(auto ptr = m_occupEls.begin();ptr!= m_occupEls.end();ptr++)
		if (*ptr == pointer)
		{
			m_freeEls.emplace_back(pointer);
			m_occupEls.erase(ptr);
			return;
		}
	return;
}

void PoolAllocatorClass::clean()
{
	//sreplace all elements
	for (auto ptr = m_occupEls.begin();ptr != m_occupEls.end();ptr++)
	{
		m_freeEls.emplace_back(*ptr);
		m_occupEls.erase(ptr);
	}
	return;
}

void PoolAllocatorClass::Shutdown()
{
	//delete all elements in both lists
	for (auto ptr = m_occupEls.begin();ptr != m_occupEls.end();ptr++)
	{
		delete *ptr;
		m_occupEls.erase(ptr);
	}
	for (auto ptr = m_freeEls.begin();ptr != m_freeEls.end();ptr++)
	{
		delete *ptr;
		m_freeEls.erase(ptr);
	}

	m_elSize = 0;
	m_poolSize = 0;

	return;
}

