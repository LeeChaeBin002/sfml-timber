#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <vector>

int main()
{
	srand((int)time(0));//매번 다른 결과
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Timber!"); //제목

	//1. sf::Texture 이미지 불러오기 
	sf::Texture textureBackground;
	textureBackground.loadFromFile("graphics/background.png");
	sf::Texture textureTree;
	textureTree.loadFromFile("graphics/tree.png");
	sf::Texture textureBee;
	textureTree.loadFromFile("graphics/bee.png");
	sf::Texture texturecloud;
	textureTree.loadFromFile("graphics/cloud.png");
	
	//2.sf::sprite 이미지 뿌리기

	sf::Sprite spriteBackground;
	spriteBackground.setTexture(textureBackground);
	sf::Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	sf::Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	sf::Sprite spritecloud;
	spritecloud.setTexture(texturecloud);

	//3.setOrigin,setPosition 설정




}