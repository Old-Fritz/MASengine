////////////////////////////////////////////////////////////////////////////////
// Filename: ModelManagerClass.h
////////////////////////////////////////////////////////////////////////////////

#ifndef _MODELMANAGERCLASS_H_
#define _MODELMANAGERCLASS_H_

//////////////
// INCLUDES //
//////////////


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "GM.h"
#include "ModelClass.h"
#include "ModelShaderClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: ModelManagerClass
////////////////////////////////////////////////////////////////////////////////

/*!
\addtogroup Models
@{
*/
/*!
Менеджер моделей, синглтон
*/
class ModelManagerClass
{
public:
	void Shutdown();

	//! \brief Получить экземпляр класса
	static ModelManagerClass& getI();

	//! Добавление готовой модели \param[in] model - добавляемая модель
	void addModel(ModelClass* model);
	//! Удаление модели \param[in] model - удаляемая модель
	void deleteModel(ModelClass* model);

	/*!
	Прорисовка блока \param[in] modelShader - шейдер моделелей \param[in] deviceContext - графическое устройство
	\param[in] worldMatrix, viewMatrix, projectionMatrix - матрицы с параметрами
	\param[in] lights - источники света \param[in] cameraPosition - позиция камеры
	\param[in] SCREEN_DEPTH - глубина экрана \param[in] frustum - конус усечения  \return false, если были ошибки
	*/
	bool Render(ModelShaderClass* modelShader, ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix,
		D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, std::vector<LightClass::PointLightType*> lights, D3DXVECTOR3 cameraPosition,
		float SCREEN_DEPTH, FrustumClass* frustum);
private:
	ModelManagerClass();
	ModelManagerClass(const ModelManagerClass&);
	~ModelManagerClass();
private:
	std::set<ModelClass*> m_models; //!<Модели

	static ModelManagerClass* m_instance; //!<Единственный экземпляр класса

};
/*! @} */
#endif