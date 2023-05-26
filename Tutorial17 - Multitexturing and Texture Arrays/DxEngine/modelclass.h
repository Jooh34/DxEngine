#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_


//////////////
// INCLUDES //
//////////////
#include "stdafx.h"
#include "textureclass.h"
#include "texturearrayclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class ModelClass
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*, char*, WCHAR*, WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	ID3D11ShaderResourceView** GetTextureArray();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	bool LoadTextures(ID3D11Device*, WCHAR*, WCHAR*);
	void ReleaseTextures();

	bool LoadModel(char*);
	void ReleaseModel();

	
private:
	ID3D11Buffer * m_vertexBuffer, * m_indexBuffer;
	int m_vertexCount, m_indexCount;

	TextureClass* m_Texture;

	ModelType* m_model;

	TextureArrayClass* m_TextureArray;
};

#endif