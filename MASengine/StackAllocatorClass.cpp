////////////////////////////////////////////////////////////////////////////////
// Filename: StackAllocatorClass.cpp
////////////////////////////////////////////////////////////////////////////////

#include "StackAllocatorClass.h"

StackAllocatorClass::StackAllocatorClass()
{
	m_stack = 0;
	m_position = 0;
}
StackAllocatorClass::StackAllocatorClass(const StackAllocatorClass &)
{
}
StackAllocatorClass::~StackAllocatorClass()
{
}

bool StackAllocatorClass::Initialize(int stackSize)
{
	m_stack = new char[stackSize];
	m_stackSize = stackSize;
	if (!m_stack)
	{
		LogManager.addLog("Error 1-3");
		return false;
	}
	else
		return true;
}

void* StackAllocatorClass::getMemory(size_t size)
{
	//check if stack is full
	if (m_position + size + sizeof(size) + 1 > m_stackSize)
		return 0;

	void* ptr = (char*)m_stack + m_position;

	m_position += size;

	//add size of last element after it
	((size_t*)((char*)m_stack + m_position))[0] = size;
	m_position += sizeof(size);

	//add bool byte to show tat thsi area not free
	((char*)((char*)m_stack + m_position))[0] = bool(1);
	m_position++;

	return ptr;
}

void StackAllocatorClass::deleteEl(void* pointer, size_t size)
{
	
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

void StackAllocatorClass::clean()
{
	m_position = 0;

	return;
}

void StackAllocatorClass::Shutdown()
{
	clean();
	delete m_stack;
	m_stack = 0;

	return;
}
