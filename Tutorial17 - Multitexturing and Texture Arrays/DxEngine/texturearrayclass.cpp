////////////////////////////////////////////////////////////////////////////////
// Filename: texturearrayclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "texturearrayclass.h"
#include "DirectXTex.h"
//The class constructor initializes the texture array elements to null.

TextureArrayClass::TextureArrayClass()
{
	m_textures[0] = 0;
	m_textures[1] = 0;
}


TextureArrayClass::TextureArrayClass(const TextureArrayClass& other)
{
}


TextureArrayClass::~TextureArrayClass()
{
}
//Initialize takes in the two texture file namesand creates two texture resources in the texture array from those files.

bool TextureArrayClass::Initialize(ID3D11Device* device, WCHAR* filename1, WCHAR* filename2)
{
	HRESULT result;


	// Load the first texture in.
	ScratchImage scratchImage;
	TexMetadata  metadata;
	result = LoadFromDDSFile(filename1, DDS_FLAGS::DDS_FLAGS_NONE, &metadata, scratchImage);
	if (FAILED(result))
	{
		return false;
	}

	result = CreateShaderResourceView(device, scratchImage.GetImages(), scratchImage.GetImageCount(), metadata, &m_textures[0]);
	if (FAILED(result))
	{
		return false;
	}

	// Load the second texture in.
	result = LoadFromDDSFile(filename2, DDS_FLAGS::DDS_FLAGS_NONE, &metadata, scratchImage);
	if (FAILED(result))
	{
		return false;
	}

	result = CreateShaderResourceView(device, scratchImage.GetImages(), scratchImage.GetImageCount(), metadata, &m_textures[1]);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}
//The Shutdown function releases each element in the texture array.

void TextureArrayClass::Shutdown()
{
	// Release the texture resources.
	if (m_textures[0])
	{
		m_textures[0]->Release();
		m_textures[0] = 0;
	}

	if (m_textures[1])
	{
		m_textures[1]->Release();
		m_textures[1] = 0;
	}

	return;
}
//GetTextureArray returns a pointer to the texture array so calling objects can have access to the textures in the texture array.

ID3D11ShaderResourceView** TextureArrayClass::GetTextureArray()
{
	return m_textures;
}