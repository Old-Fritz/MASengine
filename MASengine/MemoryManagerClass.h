#pragma once
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

void* operator new (size_t size, int type);
void* operator new[] (size_t size, int type);

void operator delete(void* mem, size_t size, int type);
void operator delete[](void* mem, size_t size, int type);
	
class MemoryManagerClass {
public:

	bool Initialize(int stackSize, int tempSize, int oneFrameSize, int poolSize);

	static MemoryManagerClass& getI();

	static bool isInit();

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
	MemoryManagerClass();
	MemoryManagerClass(const MemoryManagerClass&);
	~MemoryManagerClass();
private:
	static MemoryManagerClass* m_instance;

	 StackAllocatorClass* m_stack;
	 StackAllocatorClass* m_oneFrame;
	 StackAllocatorClass* m_temp;
	 std::list<PoolAllocatorClass*> m_pools;

	int m_stackSize;
	int m_oneFrameSize;
	int m_tempSize;
	int m_poolSize;

	static bool m_isInit;
};

#endif