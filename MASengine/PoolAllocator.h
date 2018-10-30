////////////////////////////////////////////////////////////////////////////////
// Filename: PoolAllocator.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _PoolAllocator_H_
#define _PoolAllocator_H_

#include <memory.h>
#include <list>
#include "LogManager.h"

//////////////
// INCLUDES //
//////////////


////////////////////////////////////////////////////////////////////////////////
// Class name: StackAllocator
////////////////////////////////////////////////////////////////////////////////

/*!
\addtogroup MemoryManagment
@{
*/
/*!
pool память
*/
class PoolAllocator {
public:
	PoolAllocator();
	PoolAllocator(const PoolAllocator&);
	~PoolAllocator();

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