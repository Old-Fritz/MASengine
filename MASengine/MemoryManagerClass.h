////////////////////////////////////////////////////////////////////////////////
// Filename: MemoryManagerClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MEMORYMANAGERCLASS_H_
#define _MEMORYMANAGERCLASS_H_

//////////////
// INCLUDES //
//////////////

#include "PoolAllocatorClass.h"
#include "StackAllocatorClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: MemoryManagerClass
////////////////////////////////////////////////////////////////////////////////

class MemoryManagerClass {
public:
	MemoryManagerClass();
	MemoryManagerClass(const MemoryManagerClass&);
	~MemoryManagerClass();

	bool Initialize(int stackSize, int tempSize, int oneFrameSize, int poolSize);

	//allocate memory
	void* getStackMemory(size_t size);
	void* getOneFrameMemory(size_t size);
	void* getTempMemory(size_t size);
	void* getPoolMemory(size_t size);
	
	//delete element from memory
	void deleteStack(void* pointer, size_t size);
	void deletePool(void* pointer, size_t size);


	//clean type of memory
	void cleanStack();
	void cleanOneFrame();
	void cleanTemp();
	void cleanPool();

	void Shutdown();
private:

	StackAllocatorClass* m_stack;
	StackAllocatorClass* m_oneFrame;
	StackAllocatorClass* m_temp;
	std::list<PoolAllocatorClass*> m_pools;

	int m_stackSize;
	int m_oneFrameSize;
	int m_tempSize;
	int m_poolSize;
};

/////////////
// GLOBALS //
/////////////
static MemoryManagerClass MemoryManager;

#endif