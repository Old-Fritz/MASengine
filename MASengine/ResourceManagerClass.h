
////////////////////////////////////////////////////////////////////////////////
// Filename: ResourceManagerClass.h
////////////////////////////////////////////////////////////////////////////////

#ifndef _RESOURCEMANAGERCLASS_H_
#define _RESOURCEMANAGERCLASS_H_

//////////////
// INCLUDES //
//////////////


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "TextureManagerClass.h"
#include "TextManagerClass.h"
#include "MeshManagerClass.h"
#include "FontManagerClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: ResourceManagerClass
////////////////////////////////////////////////////////////////////////////////

/*!
\addtogroup ResourceManagment
@{
*/
/*!
\defgroup ResourceManagment Управление ресурсами
\ingroup GlobalManager
@{
*/
/*!
 Класс для управления всеми блоками ресурсов, синглтон
*/
class ResourceManagerClass
{
public:

	//! Инициализация класса \return false, если были ошибки
	bool Initialize();
	void Shutdown();
	//! \brief ѕолучить экземпляр класса
	static ResourceManagerClass& getI();

private:
	ResourceManagerClass();
	ResourceManagerClass(const ResourceManagerClass&);
	~ResourceManagerClass();

private:

	static ResourceManagerClass* m_instance;  //!<Единственный экземпляр класса
};
/*! @} */
#endif
