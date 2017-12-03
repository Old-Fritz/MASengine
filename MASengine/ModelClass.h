////////////////////////////////////////////////////////////////////////////////
// Filename: ModelClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "ModelShaderClass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////

/*!
\defgroup Models 3D-модели
\ingroup Graphics
@{
*/
/*!
Статическая модель
*/
class ModelClass
{
public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	//! Загрузка блока \param[in] device, deviceContext - графическое устройство \param[in] meshFilename - путь до файла меша
	//! \param[in] textureFilename - путь до файла текстуры  \return false, если были ошибки
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, PathClass* meshFilename, PathClass* textureFilename);
	void Shutdown();
	/*!
	Прорисовка блока \param[in] modelShader - шейдер моделелей \param[in] deviceContext - графическое устройство
	\param[in] worldMatrix, viewMatrix, projectionMatrix - матрицы с параметрами 
	\param[in] lightDirection - направление света \param[in] ambientColor - цвет обтекающего света \param[in] diffuseColor - цвет диффузного света
	\param[in] cameraPosition - позиция камеры \param[in] specularColor - цвет зеркального света \param[in] specularPower - мощность зеркального света
	\param[in] SCREEN_DEPTH - глубина экрана \param[in] frustum - конус усечения  \return false, если были ошибки
	*/
	bool Render(ModelShaderClass* modelShader, ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix,
		D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, D3DXVECTOR3 lightDirection,
		D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor, D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float specularPower,
		float SCREEN_DEPTH, FrustumClass* frustum);

	//pick actions
	//bool pick(ID3D11DeviceContext * deviceContext, D3DXVECTOR3 rayOrigin, D3DXVECTOR3 rayDirection, int& provNum, D3DXVECTOR3& point);

	//Getters
	//! Получение позиция модели \return координаты модели
	D3DXVECTOR3 getPosition();
	//! Получение поворота модели \return координаты поворота модели (ypr)
	D3DXVECTOR3 getRotation();

	//Setters
	//! Установка позиция модели \param[in] position - координаты модели
	void setPosition(D3DXVECTOR3 position);
	//! Установка поворота модели \param[in] rotation - координаты поворота модели (ypr)
	void setRotation(D3DXVECTOR3 rotation);
private:
	//mesh
	int m_meshHash; //!<Хэш меша

	//textures
	int m_textureHash; //!<Хэш текстуры

	//info
	D3DXVECTOR3 m_position; //!<Позиция в мире
	D3DXVECTOR3 m_rotation; //!<Поворот в мире
};
/*! @} */
#endif
