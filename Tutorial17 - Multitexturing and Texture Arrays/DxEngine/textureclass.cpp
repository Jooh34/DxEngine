#include "textureclass.h"
//The class constructor will initialize the texture shader resource pointer to null.

TextureClass::TextureClass()
{
	m_texture = 0;
}

TextureClass::TextureClass(const TextureClass& other)
{
}

TextureClass::~TextureClass()
{
}
//Initialize takes in the Direct3D deviceand file name of the textureand then loads the texture file into the shader resource variable called m_texture.The texture can now be used to render with.

bool TextureClass::Initialize(ID3D11Device* device, WCHAR* filename)
{
	HRESULT result;


	// Load the texture in.
	ScratchImage scratchImage;
	TexMetadata  metadata;
	result = LoadFromDDSFile(filename, DDS_FLAGS::DDS_FLAGS_NONE, &metadata, scratchImage);
	if (FAILED(result))
	{
		return false;
	}

	result = CreateShaderResourceView(device, scratchImage.GetImages(), scratchImage.GetImageCount(), metadata, &m_texture);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}
//The Shutdown function releases the texture resource if it has been loadedand then sets the pointer to null.

void TextureClass::Shutdown()
{
	// Release the texture resource.
	if (m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}

	return;
}
//GetTexture is the function that is called by other objects that need access to the texture shader resource so that they can use the texture for rendering.

ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_texture;
}
