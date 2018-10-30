////////////////////////////////////////////////////////////////////////////////
// Filename: LayersClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _LAYERSCLASS_H_
#define _LAYERSCLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include"GM.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: ProvClass
////////////////////////////////////////////////////////////////////////////////

/*!
\addtogroup Provs
@{
* /
/*!
Управление слоями цвета в одной провинции
*/
class LayersClass
{

public:
	LayersClass();
	LayersClass(const LayersClass&);
	~LayersClass();

	//setters
	//! Установить новый основной цвет \param[in] type - тип региона, цвет которого будет выбран основным
	void setMainColor(GM::regionType type);
	//! Изменить цвет одного из слоев \param[in] type - тип региона, цвет слоя которого будет изменен \param[in] color - новый цвет
	void setColor(GM::regionType type, D3DXVECTOR4 color);

	//Getters
	//! Получить основной цвет \return Основной цвет
	D3DXVECTOR4 getMainColor();
	//! Получить цвет одного из слоев \param[in] - тип региона, цвет слоя которого будет возвращен \return Выбранный цвет
	D3DXVECTOR4 getColor(GM::regionType type); 
private:
	//changable main color
	D3DXVECTOR4* m_mainColor; //!<Основной цвет (ссылка на один из других цветов)

	//layers
	D3DXVECTOR4 m_baseColor; //!<Цвет слоя базового региона
	D3DXVECTOR4 m_blockColor; //!<Цвет слоя блокового региона
	D3DXVECTOR4 m_nationColor; //!<Цвет слоя региона страны
};
/*! @} */
#endif