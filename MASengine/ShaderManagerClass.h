///////////////////////////////////////////////////////////////////////////////
// Filename: ShaderManagerClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SHADERMANAGERCLASS_H_
#define _SHADERMANAGERCLASS_H_


//////////////
// INCLUDES //
//////////////
#include "GlobalManagerClass.h"
#include "InterfaceShaderClass.h"
#include "FontShaderClass.h"
#include "TerrainShaderClass.h"
#include "ModelShaderClass.h"
#include "WaterShaderClass.h"
#include "FillShaderClass.h"
#include "SkyShaderClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: ShaderManagerClass
////////////////////////////////////////////////////////////////////////////////

/*!
\defgroup ShaderManagment Управление шейдерами
\ingroup Graphics
@{
*/
/*!
Класс для управления шейдерами
*/
class ShaderManagerClass
{
public:
	ShaderManagerClass();
	ShaderManagerClass(const ShaderManagerClass&);
	~ShaderManagerClass();

	//! Инициализация шейдеров \param[in] device - графическое устройство \param[in] hwnd - идентификатор окна \return false, если были ошибки
	bool Initialize(ID3D11Device* device, HWND hwnd);
	void Shutdown();

	//getters
	//! Получение шейдера интерфейса \return Шейдер интерфейса
	InterfaceShaderClass* getInterfaceShader();
	//! Получение шейдера шрифтов \return Шейдер шрифтов
	FontShaderClass* getFontShader();
	//! Получение шейдера блоков \return Шейдер блоков
	TerrainShaderClass* getTerrainShader();
	//! Получение шейдера моделей \return Шейдер моделей
	ModelShaderClass* getModelShader();
	//! Получение шейдера воды \return Шейдер воды
	WaterShaderClass* getWaterShader();
	//! Получение шейдера заполнения \return Шейдер воды
	FillShaderClass* getFillShader();
	//! Получение шейдера неба \return Шейдер воды
	SkyShaderClass* getSkyShader();
private:
	//shaders
	InterfaceShaderClass* m_interfaceShader; //!<Шейдер интерфейса
	FontShaderClass* m_fontShader; //!<Шейдер шрифтов
	TerrainShaderClass* m_terrainShader; //!<Шейдер блоков
	ModelShaderClass* m_modelShader; //!<Шейдер моделей
	WaterShaderClass* m_waterShader; //!<Шейдер воды
	FillShaderClass* m_fillShader; //!<Шейдер заполнения
	SkyShaderClass* m_skyShader; //!<Шейдер неба
};
/*! @} */
#endif