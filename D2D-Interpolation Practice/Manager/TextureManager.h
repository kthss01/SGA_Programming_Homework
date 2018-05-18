#pragma once
class TextureManager
{
private:
	TextureManager();
	~TextureManager();

	map<wstring, LPDIRECT3DTEXTURE9> textures;
	static TextureManager* instance;
public:
	static TextureManager * Get();
	static void Delete();

	void AddTexture(wstring keyName, LPDIRECT3DTEXTURE9 pTex);
	LPDIRECT3DTEXTURE9 GetTexture(wstring keyName);
};

