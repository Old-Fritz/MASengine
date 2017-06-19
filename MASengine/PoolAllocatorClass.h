////////////////////////////////////////////////////////////////////////////////
// Filename: PoolAllocatorClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _POOLALLOCATORCLASS_H_
#define _POOLALLOCATORCLASS_H_

#include <memory.h>
#include <list>
#include "LogManagerClass.h"

//////////////
// INCLUDES //
//////////////


////////////////////////////////////////////////////////////////////////////////
// Class name: StackAllocatorClass
////////////////////////////////////////////////////////////////////////////////

class PoolAllocatorClass {
public:
	PoolAllocatorClass();
	PoolAllocatorClass(const PoolAllocatorClass&);
	~PoolAllocatorClass();

	bool Initialize(size_t elSize, long int poolSize);
	 bool doublePool();
	size_t getSize();

	 void* getMemory();
	 void deleteEl(void* pointer);

	void clean();
	void Shutdown();
private:
	 static std::list<void*> m_occupEls;
	 static std::list<void*> m_freeEls;
	 size_t m_elSize;
	 int m_poolSize;
};

#endif