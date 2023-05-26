#pragma once
#ifndef _TEXTUREARRAYCLASS_H_
#define _TEXTUREARRAYCLASS_H_


//////////////
// INCLUDES //
//////////////
#include "stdafx.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: TextureArrayClass
////////////////////////////////////////////////////////////////////////////////
class TextureArrayClass
{
public:
	TextureArrayClass();
	TextureArrayClass(const TextureArrayClass&);
	~TextureArrayClass();

	bool Initialize(ID3D11Device*, WCHAR*, WCHAR*);
	void Shutdown();

	ID3D11ShaderResourceView** GetTextureArray();

private:
	//This is the two element texture array private variable.

	ID3D11ShaderResourceView* m_textures[2];
};

#endif