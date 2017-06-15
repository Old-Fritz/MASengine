#include "MemoryManagerClass.h"

MemoryManagerClass::MemoryManagerClass()
{
	m_stackSize = 0;
	m_oneFrameSize = 0;
	m_tempSize = 0;
	m_poolSize = 0;

	m_stack = 0;
	m_oneFrame = 0;
	m_temp = 0;
}
MemoryManagerClass::MemoryManagerClass(const MemoryManagerClass &)
{
}
MemoryManagerClass::~MemoryManagerClass()
{
}

bool MemoryManagerClass::Initialize(int stackSize, int tempSize, int oneFrameSize, int poolSize)
{
	bool result;

	//MemoryManager = this;

	m_stackSize = stackSize;
	m_oneFrameSize = oneFrameSize;
	m_tempSize = tempSize;
	m_poolSize = poolSize;

	//initialize all stacks
	m_stack = new StackAllocatorClass;
	if (!m_stack)
	{
		LogManager.addLog("Error 1-4");
		return false;
	}

	result = m_stack->Initialize(m_stackSize);
	m_stack = new StackAllocatorClass;
	if (!result)
	{
		LogManager.addLog("Error 1-5");
		return false;
	}

	m_oneFrame = new StackAllocatorClass;
	if (!m_oneFrame)
	{
		LogManager.addLog("Error 1-4");
		return false;
	}

	result = m_oneFrame->Initialize(m_oneFrameSize);
	if (!result)
	{
		LogManager.addLog("Error 1-5");
		return false;
	}

	m_temp = new StackAllocatorClass;
	if (!m_temp)
	{
		LogManager.addLog("Error 1-4");
		return false;
	}

	result = m_temp->Initialize(m_tempSize);
	if (!result)
	{
		LogManager.addLog("Error 1-5");
		return false;
	}

	return true;

}

//allocate memory
void * MemoryManagerClass::getStackMemory(size_t size)
{
	return m_stack->getMemory(size);
}
void * MemoryManagerClass::getOneFrameMemory(size_t size)
{
	return m_oneFrame->getMemory(size);
}
void * MemoryManagerClass::getTempMemory(size_t size)
{
	return m_temp->getMemory(size);
}
void * MemoryManagerClass::getPoolMemory(size_t size)
{
	bool result;

	size_t poolSize = 1;
	//find best pool
	while (size > poolSize)
		poolSize *= 2;

	if (m_pools.size() > 0)
	{
		for (auto pool = m_pools.begin(); pool != m_pools.end();pool++)
		{
			if ((*pool)->getSize() == poolSize)
				return (*pool)->getMemory();
		}
	}

	//create new pool if no best pool
	PoolAllocatorClass* pool = new PoolAllocatorClass;
	if (!pool)
		return 0;
	result = pool->Initialize(size, m_poolSize);
	if (!result)
		return 0;
	m_pools.emplace_back(pool);

	return pool->getMemory();

}

//delete element from memory
void MemoryManagerClass::deleteStack(void * pointer, size_t size)
{
	m_stack->deleteEl(pointer,size);
	return;
}
void MemoryManagerClass::deletePool(void * pointer, size_t size)
{
	size_t poolSize = 1;
	//find correct pool
	while (size > poolSize)
		poolSize *= 2;

	for (auto pool = m_pools.begin();pool != m_pools.end();pool++)
	{
		if ((*pool)->getSize() == poolSize)
		{
			(*pool)->deleteEl(pointer);
			return;
		}
	}
}

//clean type of memory
void MemoryManagerClass::cleanStack()
{
	m_stack->clean();
	return;
}
void MemoryManagerClass::cleanOneFrame()
{
	m_oneFrame->clean();
	return;
}
void MemoryManagerClass::cleanTemp()
{
	m_temp->clean();
	return;
}
void MemoryManagerClass::cleanPool()
{
	for (auto pool = m_pools.begin();pool != m_pools.end();pool++)
		(*pool)->clean();
	return;
}

void MemoryManagerClass::Shutdown()
{
	//shutdown all stacks
	if(m_stack)
	{
		m_stack->Shutdown();
		delete m_stack;
		m_stack = 0;
	}
	if (m_oneFrame)
	{
		m_oneFrame->Shutdown();
		delete m_oneFrame;
		m_oneFrame = 0;
	}
	if (m_temp)
	{
		m_temp->Shutdown();
		delete m_temp;
		m_temp = 0;
	}
	//shutdown pools
	for (auto pool = m_pools.begin();pool != m_pools.end();pool++)
	{
		(*pool)->Shutdown();
		delete *pool;
		*pool = 0;
		m_pools.erase(pool);
	}
	
	return;
}
