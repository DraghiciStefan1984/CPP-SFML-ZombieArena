#pragma once
#include <SFML/Graphics.hpp>
#include <map>

using namespace std;
using namespace sf;

class TextureHolder
{
private:
	std::map<std::string, sf::Texture> m_Textures;
	static TextureHolder* m_s_Instance;

public:
	TextureHolder();
	static sf::Texture& GetTexture(std::string const& filename);
};