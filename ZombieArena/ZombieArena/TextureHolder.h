#pragma once
#include <SFML/Graphics.hpp>
#include <map>

using namespace std;
using namespace sf;

class TextureHolder
{
private:
	static TextureHolder* mInstance;
	map<string, Texture> mTextures;

public:
	TextureHolder();
	~TextureHolder();
	static Texture& GetTexture(string const filename);
};

