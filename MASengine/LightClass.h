////////////////////////////////////////////////////////////////////////////////
// Filename: lightclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _LIGHTCLASS_H_
#define _LIGHTCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3dx10math.h>


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
	LightClass();
	LightClass(const LightClass&);
	~LightClass();

	//! Установка цвета обтекающего света \param[in] color - цвет обтекающего света
	void SetAmbientColor(D3DXVECTOR4 color);
	//! Установка цвета диффузного света \param[in] color - цвет диффузного света
	void SetDiffuseColor(D3DXVECTOR4 color);
	//! Установка направления света \param[in] direction - направление света
	void SetDirection(D3DXVECTOR3 direction);
	//! Установка цвета зеркального света \param[in] color - цвет зеркального света
	void SetSpecularColor(D3DXVECTOR4 color);
	//! Установка мощности зеркального света \param[in] power - мощность зеркального света
	void SetSpecularPower(float power);

	//! Получение цвета обтекающего света \return цвет обтекающего света
	D3DXVECTOR4 GetAmbientColor();
	//! Получение цвета диффузного света \return цвет диффузного света
	D3DXVECTOR4 GetDiffuseColor();
	//! Получение направления света \return направление света
	D3DXVECTOR3 GetDirection();
	//! Получение цвета зеркального света \return цвет зеркального света
	D3DXVECTOR4 GetSpecularColor();
	//! Получение мощности зеркального света \return мощность зеркального света
	float GetSpecularPower();

private:
	D3DXVECTOR4 m_ambientColor; //!<цвет обтекающего света
	D3DXVECTOR4 m_diffuseColor; //!<цвет диффузного света
	D3DXVECTOR3 m_direction; //!<направление света
	D3DXVECTOR4 m_specularColor; //!<цвет зеркального света
	float m_specularPower; //!<мощность зеркального света
};
/*! @} */
#endif
