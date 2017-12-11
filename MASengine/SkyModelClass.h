////////////////////////////////////////////////////////////////////////////////
// Filename: SkyModelClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SKYMODELCLASS_H_
#define _SKYMODELCLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "ProvManagerClass.h"
#include "GlobalManagerClass.h"
#include "ProvRegionManagerClass.h"
#include "SkyShaderClass.h"
#include "MeshClass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: SkyModelClass
////////////////////////////////////////////////////////////////////////////////

/*!
\defgroup Blocks ����� ���������
\ingroup Graphics
@{
*/
/*!
���� � ����������, ������� ����������� �������
*/
class SkyModelClass
{
public:
	SkyModelClass();
	SkyModelClass(const SkyModelClass&);
	~SkyModelClass();

	//! �������� ����� \param[in] device, deviceContext - ����������� ���������� \param[in] skyFilename - ���� �� ����� ����  \return false, ���� ���� ������
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, PathClass* skyFilename);
	void Shutdown();

	/*!
	���������� ���� \param[in] deviceContext - ����������� ���������� \param[in] skyShader - ������ ���� 
	\param[in] worldMatrix, viewMatrix, projectionMatrix - ������� � ���������� \return false, ���� ���� ������
	*/
	bool Render(ID3D11DeviceContext* deviceContext, SkyShaderClass* skyShader, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix);
private:
	//! ��������� ������ �� ����� \param[in] blockFilename - ���� �� ����� ����� \return false, ���� ���� ������
	bool readFromFile(PathClass* filename);
private:
	int m_id; //!< ID �����

	float m_skyWidth, m_skyHeight;  //!<������ � ������ ����� 
	int m_meshHash; //!<���� ���� ��������� ����
	PathClass* m_meshFilename; //!<���� �� ����� ���� ��������� ����

	//textures
	int m_texture1Hash; //!<��� �������� ������� ������� ������
	PathClass* m_texture1Filename; //!<���� �� �������� ������� ������� ������

	int m_texture2Hash; //!<��� �������� ������� ������� ������
	PathClass* m_texture2Filename; //!<���� �� �������� ������� ������� ������

	//info from file
	D3DXVECTOR3 m_position; //!<������� ���� �� �����
};
/*! @} */
#endif
