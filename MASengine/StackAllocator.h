////////////////////////////////////////////////////////////////////////////////
// Filename: StackAllocator.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _StackAllocator_H_
#define _StackAllocator_H_

//////////////
// INCLUDES //
//////////////
#include "LogManager.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: StackAllocator
////////////////////////////////////////////////////////////////////////////////

/*!
\addtogroup MemoryManagment
@{
*/
/*!
Стек памяти
*/
class StackAllocator {
public:
	StackAllocator();
	StackAllocator(const StackAllocator&);
	~StackAllocator();

	/*!
	Инициализация класса /param[in] stackSize - размер стека в байтах
	/return false, если были ошибки
	*/
	bool Initialize(int stackSize);

	//! Получение памяти из стека /param[in] size - размер блока памяти /return указатель на полученную память
	 void* getMemory(size_t size);
	 //! Освобождение блока памяти из стека /param[in] pointer - указатель на удаляемый блок /param[in] size - размер блока памяти
	void deleteEl(void* pointer, size_t size);

	//! Полная очистка стека
	void clean();
	void Shutdown();
private:
	 void* m_stack; //!<Память для стека
	 long int m_position; //!<Текущий позиция в стеке
	 int m_stackSize; //!<Размер стека
};
/*! @} */
#endif