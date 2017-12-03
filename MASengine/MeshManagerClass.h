////////////////////////////////////////////////////////////////////////////////
// Filename: MeshManagerClass.h
////////////////////////////////////////////////////////////////////////////////

#ifndef _MESHMANAGERCLASS_H_
#define _MESHMANAGERCLASS_H_

//////////////
// INCLUDES //
//////////////


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "meshclass.h"
#include "ModManagerClass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: MeshManagerClass
////////////////////////////////////////////////////////////////////////////////

/*!
\addtogroup ResourceManagment
@{
*/
/*!
Класс для управления мешами, синглтон
*/
class MeshManagerClass
{
public:
	void Shutdown();
	//! \brief Получить экземпляр класса
	static MeshManagerClass& getI();

	/*!
	Добавление меша \param[in] device - графическое устройство
	\param[in] filename - путь до шрифта
	\param[in] sizeX, sizeY, sizeZ - scale меша (1 по умолчанию)
	\return false, если были ошибки
	*/
	bool addModel(ID3D11Device* device, PathClass* filename, float sizeX = 1, float sizeY = 1, float sizeZ = 1);
	//! Получение меша \param[in] filename - путь до меша \return меш
	MeshClass* getModel(PathClass* filename);
	//! см. getModel, но по хэшу пути
	MeshClass* getModel(int hash);

private:
	MeshManagerClass();
	MeshManagerClass(const MeshManagerClass&);
	~MeshManagerClass();
private:
	std::map<int, MeshClass*> m_models; //!<меши

	static MeshManagerClass* m_instance; //!<Единственный экземпляр класса
};
/*! @} */
#endif