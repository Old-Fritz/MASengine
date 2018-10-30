#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: MemoryManager.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MemoryManager_H_
#define _MemoryManager_H_

//////////////
// INCLUDES //
//////////////

#include "PoolAllocator.h"
#include "StackAllocator.h"
#include "Utils.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: MemoryManager
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
class MemoryManager {
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
	static MemoryManager* getI();

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
	MemoryManager();
	MemoryManager(const MemoryManager&);
	~MemoryManager();
private:
	static MemoryManager* m_instance; //!<Единственный экземпляр класса

	 StackAllocator* m_stack; //!<Основной стек
	 StackAllocator* m_oneFrame; //!<Покадровый стек
	 StackAllocator* m_temp; //!<Временный стек
	 std::list<PoolAllocator*> m_pools; //!<pool память

	int m_stackSize; //!<Размер основного стека
	int m_oneFrameSize; //!<Размер покадрового стека
	int m_tempSize; //!<Размер временного стека
	int m_poolSize; //!<Размер pool памяти

	static bool m_isInit; //!<Статус инициализации
};
/*! @} */
#endif