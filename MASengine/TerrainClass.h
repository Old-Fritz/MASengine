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

//////////////
// GLOBALS //
/////////////
const int NUM_OF_LVLS = 4;


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
	/*!
	Прорисовка блока \param[in] terrainShader - шейдер блоков \param[in] waterShader - шейдер воды \param[in] deviceContext - графическое устройство
	\param[in] worldMatrix, viewMatrix, projectionMatrix - матрицы с параметрами  \param[in] mapTextures - текстуры физ. карты
	\param[in] lightDirection - направление света \param[in] ambientColor - цвет обтеквющего света \param[in] diffuseColor - цвет диффузного света
	\param[in] cameraPosition - позиция камеры \param[in] specularColor - цвет зеркального света \param[in] specularPower - мощность зеркального света
	\param[in] SCREEN_DEPTH - глубина экрана \param[in] frustum - конус усечения  \param[in] waterHeight - уровень воды
	\param[in] waterTranslation - смещение воды     \return false, если были ошибки
	*/
	bool Render(TerrainShaderClass* terrainShader, WaterShaderClass* waterShader, ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix,
		D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView** mapTextures, D3DXVECTOR3 lightDirection,
		D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor, D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float specularPower,
		float SCREEN_DEPTH, float waterHeight, float waterTranslation, FrustumClass* frustum);


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

	
private:
	/*!
	Прорисовка воды \param[in] waterShader - шейдер воды \param[in] deviceContext - графическое устройство
	\param[in] worldMatrix, viewMatrix, projectionMatrix - матрицы с параметрами
	\param[in] lightDirection - направление света \param[in] ambientColor - цвет обтекающего света \param[in] diffuseColor - цвет диффузного света
	\param[in] cameraPosition - позиция камеры \param[in] specularColor - цвет зеркального света \param[in] specularPower - мощность зеркального света
	\param[in] SCREEN_DEPTH - глубина экрана \param[in] frustum - конус усечения  \param[in] waterHeight - уровень воды
	\param[in] waterTranslation - смещение воды \return false, если были ошибки
	*/
	bool renderWater(WaterShaderClass* waterShader, ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix,
		D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, D3DXVECTOR3 lightDirection,
		D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor, D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float specularPower,
		float SCREEN_DEPTH, float waterHeight, float waterTranslation, FrustumClass* frustum);

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

	int m_waterTextureHash; //!<Хэш водной текстуры
	PathClass* m_waterFilename; //!<Путь до водной текстуры

	int m_waterNormalTexureHash; //!<Хэш нормальной водной текстуры
	PathClass*  m_waterNormalFilename; //!<Путь до нормальной водной текстуры

	//info from file
	D3DXVECTOR3 m_position; //!<Позиция блока на карте
};
/*! @} */
#endif
