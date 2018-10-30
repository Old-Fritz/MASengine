#include "MemoryManager.h"

MemoryManager * MemoryManager::m_instance = 0;
bool MemoryManager::m_isInit = false;

MemoryManager::MemoryManager()
{
	m_stackSize = 0;
	m_oneFrameSize = 0;
	m_tempSize = 0;
	m_poolSize = 0;

	m_stack = 0;
	m_oneFrame = 0;
	m_temp = 0;
}
MemoryManager::MemoryManager(const MemoryManager &)
{
}
MemoryManager::~MemoryManager()
{
}

bool MemoryManager::Initialize(int stackSize, int tempSize, int oneFrameSize, int poolSize)
{
	bool result;

	//MemoryManager = this;

	m_stackSize = stackSize;
	m_oneFrameSize = oneFrameSize;
	m_tempSize = tempSize;
	m_poolSize = poolSize;

	//initialize all stacks
	m_stack = new StackAllocator;
	if (!m_stack)
	{
		GM::LM()->addLog("Error 1-4");
		return false;
	}

	result = m_stack->Initialize(m_stackSize);
	if (!result)
	{
		GM::LM()->addLog("Error 1-5");
		return false;
	}

	m_oneFrame = new StackAllocator;
	if (!m_oneFrame)
	{
		GM::LM()->addLog("Error 1-4");
		return false;
	}

	result = m_oneFrame->Initialize(m_oneFrameSize);
	if (!result)
	{
		GM::LM()->addLog("Error 1-5");
		return false;
	}

	m_temp = new StackAllocator;
	if (!m_temp)
	{
		GM::LM()->addLog("Error 1-4");
		return false;
	}

	result = m_temp->Initialize(m_tempSize);
	if (!result)
	{
		GM::LM()->addLog("Error 1-5");
		return false;
	}

	m_isInit = true;

	return true;

}

MemoryManager* MemoryManager::getI()
{
	if (!m_instance)
		m_instance = new(5) MemoryManager;
	return m_instance;
}

bool MemoryManager::isInit()
{
	return m_isInit;
}

//allocate memory
void * MemoryManager::getStackMemory(size_t size)
{
	return m_stack->getMemory(size);
}
void * MemoryManager::getOneFrameMemory(size_t size)
{
	return m_oneFrame->getMemory(size);
}
void * MemoryManager::getTempMemory(size_t size)
{
	return m_temp->getMemory(size);
}
void * MemoryManager::getPoolMemory(size_t size)
{
	bool result;

	size_t poolSize = 1;
	void* mem;

	//find best pool
	m_isInit = false;
	while (size > poolSize)
		poolSize *= 2;

	if (m_pools.size() > 0)
	{
		for (auto pool = m_pools.begin(); pool != m_pools.end();pool++)
		{
			if ((*pool)->getSize() == poolSize)
			{
				mem = (*pool)->getMemory();
				m_isInit = true;
				return mem;
			}
				
		}
	}

	//create new pool if no best pool
	PoolAllocator* pool = new PoolAllocator;
	if (!pool)
		return 0;
	result = pool->Initialize(poolSize, m_poolSize);
	if (!result)
		return 0;
	m_pools.emplace_back(pool);

	mem = pool->getMemory();
	m_isInit = true;
	return mem;

}

//delete element from memory
void MemoryManager::deleteStack(void * pointer, size_t size)
{
	m_stack->deleteEl(pointer,size);
	return;
}
void MemoryManager::deletePool(void * pointer, size_t size)
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
void MemoryManager::cleanStack()
{
	m_stack->clean();
	return;
}
void MemoryManager::cleanOneFrame()
{
	m_oneFrame->clean();
	return;
}
void MemoryManager::cleanTemp()
{
	m_temp->clean();
	return;
}
void MemoryManager::cleanPool()
{
	for (auto pool = m_pools.begin();pool != m_pools.end();pool++)
		(*pool)->clean();
	return;
}

void MemoryManager::Shutdown()
{
	m_isInit = false;
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
	}
	m_pools.clear();
	
	return;
}



void * operator new(size_t size, int type)
{
	

	switch (type)
	{
	case(1):
		return ::operator new(size,MemoryManager::getI()->getStackMemory(size));
	case(2):
		return ::operator new(size, MemoryManager::getI()->getOneFrameMemory(size));
	case(3):
		return ::operator new(size, MemoryManager::getI()->getTempMemory(size));
	//case(4):
		//return ::operator new(size, MemoryManager::getI()->getPoolMemory(size));
	default:
		return malloc(size);
		break;
	}
}

void* operator new[](size_t size, int type)
{
	

	switch (type)
	{
	case(1):
		return ::operator new[](size, MemoryManager::getI()->getStackMemory(size));
	case(2):
		return ::operator new[](size, MemoryManager::getI()->getOneFrameMemory(size));
	case(3):
		return ::operator new[](size, MemoryManager::getI()->getTempMemory(size));
	//case(4):
		//return ::operator new[](size, MemoryManager::getI()->getPoolMemory(size));
	default:
		return malloc(size);
		break;
	}
}

void operator delete(void * mem, size_t size, int type)
{
	
	switch (type)
	{
	case(1):
		MemoryManager::getI()->deleteStack(mem,size);
		break;
	//case(2):
	//	MemoryManager::getI()->deletePool(mem, size);
		//break;
	default:
		free(mem);
		break;
	}
}


void operator delete[](void * mem, size_t size, int type)
{
	

	switch (type)
	{
	case(1):
		MemoryManager::getI()->deleteStack(mem, size);
		break;
	//case(2):
		//MemoryManager::getI()->deletePool(mem, size);
		//break;
	default:
		free(mem);
		break;
	}
}
