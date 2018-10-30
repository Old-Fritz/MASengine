////////////////////////////////////////////////////////////////////////////////
// Filename: StackAllocator.cpp
////////////////////////////////////////////////////////////////////////////////

#include "StackAllocator.h"

StackAllocator::StackAllocator()
{
	m_stack = 0;
	m_position = 0;
}
StackAllocator::StackAllocator(const StackAllocator &)
{
}
StackAllocator::~StackAllocator()
{
}

bool StackAllocator::Initialize(int stackSize)
{
	m_stack = new char[stackSize];
	m_stackSize = stackSize;
	if (!m_stack)
	{
		GM::LM()->addLog("Error 1-3");
		return false;
	}
	else
		return true;
}

void* StackAllocator::getMemory(size_t size)
{
	//check if stack is full
	if (m_position + size + sizeof(size) + 1 > m_stackSize)
		return 0;

	void* ptr = (char*)m_stack + m_position;

	m_position += size;

	return ptr;

	//add size of last element after it
	((size_t*)((char*)m_stack + m_position))[0] = size;
	m_position += sizeof(size);

	//add bool byte to show tat thsi area not free
	((char*)((char*)m_stack + m_position))[0] = bool(1);
	m_position++;

	return ptr;
}

void StackAllocator::deleteEl(void* pointer, size_t size)
{
	m_position -= size;
	
	return;
	// check if element is in stack
	if (pointer < m_stack || pointer>=(void*)((char*)m_stack + m_stackSize))
		return;

	//set bool byte to 0 to show that this area is free
	((char*)((char*)pointer + size + sizeof(size_t) + 1))[0] = bool(0);

	//if this is last element, replace position
	while (m_position > 0 && ((size_t*)((char*)m_stack + m_position - 1))[0] == 0)
		m_position -= ((size_t*)((char*)m_stack + m_position - 1-sizeof(size_t)))[0] + sizeof(size_t) + 1;

	return;
}

void StackAllocator::clean()
{
	m_position = 0;

	return;
}

void StackAllocator::Shutdown()
{
	clean();
	delete m_stack;
	m_stack = 0;

	return;
}


