////////////////////////////////////////////////////////////////////////////////
// Filename: terrainclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TERRAINCLASS_H_
#define _TERRAINCLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "ProvManagerClass.h"
#include "GlobalManagerClass.h"
#include "ProvRegionManagerClass.h"
#include "TerrainShaderClass.h"
#include "WaterShaderClass.h"
#include "FillShaderClass.h"
#include "RenderTextureClass.h"
#include "SkyModelClass.h"

//////////////
// GLOBALS //
/////////////
const int NUM_OF_LVLS = 4;
const int MAX_WATER_FILL_LEVEL = 2;

////////////////////////////////////////////////////////////////////////////////
// Class name: TerrainClass
////////////////////////////////////////////////////////////////////////////////

/*!
\defgroup Blocks Блоки ландшафта
\ingroup Graphics
@{
*/
/*!
Блок с ландшафтом, оснвгая структурная единица
*/
class TerrainClass
{
public:
	TerrainClass();
	TerrainClass(const TerrainClass&);
	~TerrainClass();

	//! Загрузка блока \param[in] device, deviceContext - графическое устройство \param[in] blockFilename - путь до файла блока
	//! \param[in] id - ID блока  \return false, если были ошибки
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, PathClass* blockFilename, int id);
	void Shutdown();
	//! Установка рендрируемых текстур из главного менджера \param[in] fillTexture - текстура заполнения блока \param[in] skyTexture -  текстура отраженного неба для воды
	//! \param[in] skyModel - глобальная модель неба
	void setRenderTextures(RenderTextureClass* skyTexture);

	/*!
	Прорисовка блок \param[in] D3D - API Directx для рендринга в текстуру \param[in] terrainShader - шейдер блоков 	\param[in] waterShader - шейдер воды 
	\param[in] fillShader - шейдер заполнения \param[in] skyShader - шейдер неба  \param[in] worldMatrix, viewMatrix, projectionMatrix - матрицы с параметрам 
	\param[in] topViewMatrix - 10 матриц для камеры сверху блока   \param[in] mapTextures - текстуры физ. карты
	\param[in] lights - источники света \param[in] cameraPosition - позиция камеры
	\param[in] SCREEN_DEPTH - глубина экрана \param[in] frustum - конус усечения  \param[in] waterHeight - уровень воды
	\param[in] waterTranslation - смещение воды     \return false, если были ошибки
	*/
	bool Render(D3DClass* D3D, TerrainShaderClass* terrainShader, WaterShaderClass* waterShader, FillShaderClass* fillShader, SkyShaderClass* skyShader, D3DXMATRIX worldMatrix,
		D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, std::vector<D3DXMATRIX> topViewMatrix, ID3D11ShaderResourceView** mapTextures,
		std::vector<LightClass::PointLightType*> lights, D3DXVECTOR3 cameraPosition,
		float SCREEN_DEPTH, float waterHeight, float waterTranslation, FrustumClass* frustum);

	/*
	Прорисовка блока сверху в текстуру  
	\param[in] D3D - API Directx для рендринга в текстуру \param[in] fillShader - шейдер заполнения
	\param[in] worldMatrix, viewMatrix, projectionMatrix - матрицы с параметрами \param[in] topViewMatrix - 10 матриц для камеры сверху блока 
	\param[in] terrainMesh - меш ландшафта \param[in] waterHeight - уровень воды \param[in] lvl - уровень детализации \return false, если были ошибки
	*/
	bool renderToTexture(D3DClass* D3D, FillShaderClass* fillShader, SkyShaderClass* skyShader, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix,
		D3DXMATRIX projectionMatrix, std::vector<D3DXMATRIX> topViewMatrix, MeshClass* terrainMesh, float waterHeight,int lvl);

	//pick actions
	//! Получение провинции, выбранной при клике \param[in] deviceContext - графическое устройство \param[in] mouseX, mouseY - позиция курсора 
	//! \param[out] provNum - индекс выбранной провинции \param[out] point - точка пересечения
	//! \return false, если не был выбран ни один элемент, true, если был
	bool pick(ID3D11DeviceContext * deviceContext, D3DXVECTOR3 rayOrigin,D3DXVECTOR3 rayDirection, int& provNum, D3DXVECTOR3& point);

	//! Получение цвета пикселя из изображения \param[in] x,y - координаты пикселя \param[out] color - цвет пикселя 
	//! \param[in] - путь до изображения \return false, если были ошибки
	bool getColorFromBMP(float x, float y, D3DXVECTOR3& color, PathClass* filename);

	//Getters
	//! Получение позиции блока \return позиция блока
	D3DXVECTOR3 getPosition();

	//! Получение длины вертикальной стороны \return terrainHeight
	float getTerrainHeight();

	//! Получение длины горизонтальной стороны \return terrainWidth
	float getTerrainWidth();

	
private:
	/*!
	Прорисовка воды \param[in] waterShader - шейдер воды \param[in] deviceContext - графическое устройство
	\param[in] worldMatrix, viewMatrix, projectionMatrix - матрицы с параметрами
	\param[in] lights - источники света \param[in] cameraPosition - позиция камеры  \param[in] waterHeight - уровень воды
	\param[in] waterTranslation - смещение воды \param[in] lvl - уровень детализации \return false, если были ошибки
	*/
	bool renderWater(WaterShaderClass* waterShader, ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix,
		D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix,  std::vector<LightClass::PointLightType*> lights, D3DXVECTOR3 cameraPosition,
		 float waterHeight, float waterTranslation,  int lvl);

	//block info
	//! Получение данных из файла \param[in] blockFilename - путь до файла блока \return false, если были ошибки
	bool readFromFile(PathClass* filename);
	//! Получение массива цветов пров в блоке \return массив цветов в прове
	D3DXVECTOR4* getProvColor();
	//! Расчет уровня прорисовка в зависимости от дистанции \param[in] dist - расстояние от блока до камеры \return уровень прорисовки
	int getLvlByDist(float dist);
	//! Получение номера провинции по её индексу в регионе \param[in] - индекс провы в регионе
	int getProvNum(int index);
private:
	int m_id; //!< ID блока

	float m_terrainWidth, m_terrainHeight;  //!<Ширина и высота блока 
	int m_meshHash[NUM_OF_LVLS]; //!<Хэши мешей всех уровней
	int m_waterHash; //!<Хэш водяного меша
	PathClass* m_hmapFilenames[NUM_OF_LVLS]; //!<Пути файлов с картами высот
	PathClass* m_waterMeshFilename; //!<Путь до файла водяного меша
	std::string m_hmapFilenameBase; //!<Базовое имя, к которому прибаляются индексы для расчета путей

	//textures
	int m_provTextureHash; //!<Хэш текстуры  провами
	PathClass* m_provFilename; //!<Путь до текстуры с провами

	int m_physTextureHash; //!<Хэш физической текстуры
	PathClass* m_physFilename; //!<ПУть до физической текстуры

	int m_depthTextureHash; //!<Хэш водной текстуры
	PathClass* m_depthFilename; //!<Путь до водной текстуры

	int m_waterNormalTexureHash; //!<Хэш нормальной водной текстуры
	PathClass*  m_waterNormalFilename; //!<Путь до нормальной водной текстуры

	int m_waterTextureHash; //!<Хэш водной текстуры

	//info from file
	D3DXVECTOR3 m_position; //!<Позиция блока на карте

	std::pair<int, RenderTextureClass*> m_fillTexture; //!< Текстуры заполнения блока разного уровня детализации с модификатором заполненности
	RenderTextureClass* m_skyTexture; //!< Текстура отраженного неба для воды

};
/*! @} */
#endif
