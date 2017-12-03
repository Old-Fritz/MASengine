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

/*!
\addtogroup MemoryManagment
@{
*/
/*!
pool память
*/
class PoolAllocatorClass {
public:
	PoolAllocatorClass();
	PoolAllocatorClass(const PoolAllocatorClass&);
	~PoolAllocatorClass();

	/*!
	Инициализация класса /param[in] stackSize - размер стека в байтах
	/return false, если были ошибки
	*/
	bool Initialize(size_t elSize, long int poolSize);
	//! Удвоение памяти /return false, если были ошибки
	 bool doublePool();
	 //! Получение размера одной ячейки \return размер ячейки
	size_t getSize();

	//! Получение памяти /return указатель на полученную память
	 void* getMemory();
	 //! Освобождение блока памяти /param[in] pointer - указатель на удаляемый блок 
	 void deleteEl(void* pointer);

	//! Полная очистка памяти
	void clean();
	void Shutdown();
private:
	 static std::list<void*> m_occupEls; //!<Занятые ячейки памяти
	 static std::list<void*> m_freeEls; //!<Свободные ячейки памяти
	 size_t m_elSize; //!<Размер одной ячейки
	 int m_poolSize; //!<Количество ячеек
};
/*! @} */
#endif