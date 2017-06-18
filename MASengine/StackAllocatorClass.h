////////////////////////////////////////////////////////////////////////////////
// Filename: StackAllocatorClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _STACKALLOCATORCLASS_H_
#define _STACKALLOCATORCLASS_H_

//////////////
// INCLUDES //
//////////////
#include "LogManagerClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: StackAllocatorClass
////////////////////////////////////////////////////////////////////////////////

class StackAllocatorClass {
public:
	StackAllocatorClass();
	StackAllocatorClass(const StackAllocatorClass&);
	~StackAllocatorClass();

	bool Initialize(int stackSize);

	 void* getMemory(size_t size);
	void deleteEl(void* pointer, size_t size);

	void clean();
	void Shutdown();
private:
	 void* m_stack;
	 long int m_position;
	 int m_stackSize;
};

#endif