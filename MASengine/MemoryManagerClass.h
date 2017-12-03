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
#include "Utils.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: MemoryManagerClass
////////////////////////////////////////////////////////////////////////////////

void* operator new (size_t size, int type);
void* operator new[] (size_t size, int type);

void operator delete(void* mem, size_t size, int type);
void operator delete[](void* mem, size_t size, int type);

/*!
\defgroup MemoryManagment Управление памятью
\ingroup GlobalManager
@{
*/
/*!
Класс для управления памятью, синглтон
*/
class MemoryManagerClass {
public:
	/*!
	Инициализация класса /param[in] stackSize - размер основного стека в байтах
	/param[in] tempSize - размер временного стека в байтах
	/param[in] oneFrameSize - размер покадрового стека в байтах
	/param[in] poolSize - количество начальных ячеек в pool памяти
	/return false, если были ошибки
	*/
	bool Initialize(int stackSize, int tempSize, int oneFrameSize, int poolSize);

	//! \brief Получить экземпляр класса
	static MemoryManagerClass& getI();

	//! Проверка инициализации /return true, если класс был инициализирован, false, если нет
	static bool isInit();

	//allocate memory
	//! Получение памяти из основного стека /param[in] size - размер блока памяти /return указатель на полученную память
	 void* getStackMemory(size_t size);
	 //! Получение памяти из временного стека /param[in] size - размер блока памяти /return указатель на полученную память
	 void* getOneFrameMemory(size_t size);
	 //! Получение памяти из покадрового стека /param[in] size - размер блока памяти /return указатель на полученную память
	 void* getTempMemory(size_t size);
	 //! Получение памяти pool памяти /param[in] size - размер блока памяти /return указатель на полученную память
	 void* getPoolMemory(size_t size);
	
	//delete element from memory
	//! Освобождение блока памяти из основного стека /param[in] pointer - указатель на удаляемый блок /param[in] size - размер блока памяти
	void deleteStack(void* pointer, size_t size);
	//! Освобождение блока памяти из pool памяти /param[in] pointer - указатель на удаляемый блок /param[in] size - размер блока памяти
	void deletePool(void* pointer, size_t size);


	//clean type of memory
	//! Полностью очистить основной стек
	void cleanStack();
	//! Полностью очистить покадровый стек
	void cleanOneFrame();
	//! Полностью очистить временный стек
	void cleanTemp();
	//! Полностью очистить pool память
	void cleanPool();

	void Shutdown();

private:
	MemoryManagerClass();
	MemoryManagerClass(const MemoryManagerClass&);
	~MemoryManagerClass();
private:
	static MemoryManagerClass* m_instance; //!<Единственный экземпляр класса

	 StackAllocatorClass* m_stack; //!<Основной стек
	 StackAllocatorClass* m_oneFrame; //!<Покадровый стек
	 StackAllocatorClass* m_temp; //!<Временный стек
	 std::list<PoolAllocatorClass*> m_pools; //!<pool память

	int m_stackSize; //!<Размер основного стека
	int m_oneFrameSize; //!<Размер покадрового стека
	int m_tempSize; //!<Размер временного стека
	int m_poolSize; //!<Размер pool памяти

	static bool m_isInit; //!<Статус инициализации
};
/*! @} */
#endif