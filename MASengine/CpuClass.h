///////////////////////////////////////////////////////////////////////////////
// Filename: cpuclass.h
///////////////////////////////////////////////////////////////////////////////
#ifndef _CPUCLASS_H_
#define _CPUCLASS_H_

/////////////
// LINKING //
/////////////
#pragma comment(lib, "pdh.lib")


//////////////
// INCLUDES //
//////////////
#include <pdh.h>

///////////////////////////////////////////////////////////////////////////////
// Class name: CpuClass
///////////////////////////////////////////////////////////////////////////////
/*!
\addtogroup SystemStateManagment
@{
*/
/*!
Класс для получения сведений о ЦП
*/
class CpuClass
{
public:
	CpuClass();
	CpuClass(const CpuClass&);
	~CpuClass();

	//! Инициализировать класс
	void Initialize();
	void Shutdown();
	//! Обновить данные, выполняется на каждом кадре
	void Frame();
	//! Получить загруженность процессора \return число от 0 до 100 - загруженность процессора в процентах
	int GetCpuPercentage();

private:
	bool m_canReadCpu;
	HQUERY m_queryHandle;
	HCOUNTER m_counterHandle;
	unsigned long m_lastSampleTime;
	long m_cpuUsage;
};
/*! @} */
#endif
