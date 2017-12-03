﻿////////////////////////////////////////////////////////////////////////////////
// Filename: BlockRegionClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _BLOCKREGIONCLASS_H_
#define _BLOCKREGIONCLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "ProvRegionClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: ProvClass
////////////////////////////////////////////////////////////////////////////////

/*!
\addtogroup ProvRegions
@{
* /
/*!
Блоковый регион
*/
class BlockRegionClass : public ProvRegionClass
{
public:
	//! Добавление провинции в регион \param[in] provID - ID добавляемой провинции
	void add(int provID);
};
/*! @} */
#endif