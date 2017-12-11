////////////////////////////////////////////////////////////////////////////////
// Filename: lightclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _LIGHTCLASS_H_
#define _LIGHTCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3dx10math.h>
#include <vector>


////////////////////////////////////////////////////////////////////////////////
// Class name: LightClass
////////////////////////////////////////////////////////////////////////////////

/*!
\addtogroup Systems
@{
*/



/*!
Свет
*/
class LightClass
{

public:
	//! Точечный свет
	struct PointLightType {
		D3DXVECTOR4 ambientColor; //!<цвет обтекающего света
		D3DXVECTOR4 diffuseColor; //!<цвет диффузного света
		D3DXVECTOR3 position; //!<позиция света
		D3DXVECTOR4 specularColor; //!<цвет зеркального света
		float specularPower; //!<мощность зеркального света
	};
	//! Направленный свет
	struct DirectedLightType {
		D3DXVECTOR4 ambientColor; //!<цвет обтекающего света
		D3DXVECTOR4 diffuseColor; //!<цвет диффузного света
		D3DXVECTOR3 direction; //!<направление света
		D3DXVECTOR4 specularColor; //!<цвет зеркального света
		float specularPower; //!<мощность зеркального света
	};

public:
	LightClass();
	LightClass(const LightClass&);
	~LightClass();

	//! Добавление точечного света \param[in] light - источник света
	void addPointLight(PointLightType* light);
	//! Добавление направленного света \param[in] light - источник света
	void addDirectedLight(DirectedLightType* light);

	//! Получение текущих точечных источников света \return источники света в виде вектора
	std::vector<PointLightType*> getPointLights();
	//! Получение текущих направленных источников света \return источники света в виде вектора
	std::vector<DirectedLightType*> getDirectedLights();

private:
	std::vector<PointLightType*> m_pointLights; //!< Точечные источники света
	std::vector<DirectedLightType*> m_directedLights; //!< Направленные источники света
};
/*! @} */
#endif
