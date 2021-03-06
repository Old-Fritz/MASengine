#include "PoolAllocatorClass.h"

 std::list<void*> PoolAllocatorClass::m_occupEls;
 std::list<void*> PoolAllocatorClass::m_freeEls;

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

	
	//create new blocks of free memory
	for (int i = 0;i < poolSize;i++)
	{
		void* element = new char[elSize];
		if (!element)
		{
			LogManagerClass::getI().addLog("Error 1-1");
			return false;
		}

		m_freeEls.emplace_back(element);
	}

	return true;

}

bool PoolAllocatorClass::doublePool()
{
	
	//create new blocks of free memory
	for (int i = 0;i < m_poolSize;i++)
	{
		void* element = new char[m_elSize];
		if (!element)
		{
			LogManagerClass::getI().addLog("Error 1-1");
			return false;
		}

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
		delete (char*)*ptr;
	}
	m_occupEls.clear();
	for (auto ptr = m_freeEls.begin();ptr != m_freeEls.end();ptr++)
	{
		delete (char*)*ptr;
	}
	m_freeEls.clear();

	m_elSize = 0;
	m_poolSize = 0;

	return;
}

