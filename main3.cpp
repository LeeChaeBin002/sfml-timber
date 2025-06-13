#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <vector>

// 유틸 함수: 랜덤 방향 반환
float getRandomDirection() {
	return (rand() % 2 == 0) ? 1.f : -1.f;
}



// ------------------- Cloud 클래스 -------------------
class Cloud {
public:
	sf::Sprite sprite;
	sf::Vector2f direction;
	float speed;

	Cloud(const sf::Texture &texture, float yPos) {
		sprite.setTexture(texture);
		reset(yPos);
	}

	void update(float dt) {
		sprite.move(direction.x * speed * dt, 0.f);
		if (sprite.getPosition().x < -400 || sprite.getPosition().x > 1920 + 400)
			reset(sprite.getPosition().y);
	}

private:
	void reset(float yPos) {
		direction.x = getRandomDirection();//방향랜덤
		speed = static_cast<float>(rand() % 200 + 100);//속도 랜덤 100~299
		float scale = static_cast<float>(rand() % 50 + 50) / 100.f; // 0.5 ~ 1.0
		sprite.setOrigin(sprite.getLocalBounds().width * 0.5f, 0.f);

		if (direction.x < 0) {
			sprite.setScale(scale, scale);
			sprite.setPosition(1920.f + 300.f, yPos);
		}
		else {
			sprite.setScale(-scale, scale);
			sprite.setPosition(-300.f, yPos);
		}
	}
};

// ------------------- Bee 클래스 -------------------
class Bee {
public:
	sf::Sprite sprite;
	sf::Vector2f direction;
	float speed;
	float elapsedTime;//포물선 시간 추적용

	Bee(const sf::Texture& texture) {
		sprite.setTexture(texture);
		reset();
	}
	void update(float dt) {
		elapsedTime += dt;
		sprite.move(direction.x * speed * dt, 0.f);

		float ampliutude = 50.f;//포물선 높이 ,진폭
		float frequency = 2.f;//곡선 주기 조절,빈도
		float baseY = (direction.x > 0) ? 800.f : 700.f;
		float offsetY = ampliutude * std::sin(frequency * elapsedTime);

		sf::Vector2f pos = sprite.getPosition();
		sprite.setPosition(pos.x, baseY + offsetY);


		if (sprite.getPosition().x < -200 || sprite.getPosition().x > 1920 + 200)
			reset();
	}
private:
	void reset() {
		direction.x = getRandomDirection();
		speed = static_cast<float>(rand() % 200 + 100);
		float x = (direction.x > 0) ? -150.f : 1920.f + 150.f;
		float y = (direction.x > 0) ? 800.f : 700.f;
		sprite.setPosition(x, y);
		sprite.setScale(-direction.x, 1.f);
		elapsedTime = 0.f;
	}
};

enum class Side { LEFT, RIGHT, NONE }; //데이터형은 열거형(enum) 3개의 리터럴

void updateBranches(Side* branches, int size)
{
	for (int i = size - 1; i > 0; --i)
	{
		branches[i] = branches[i - 1];
	}

	int r = rand() % 3;
	switch (r)
	{
	case 0:
		branches[0] = Side::LEFT;
		break;
	case 1:
		branches[0] = Side::RIGHT;
		break;
	default:
		branches[0] = Side::NONE;
		break;
	}
}
bool checkCollision(Side* branches, Side playerSide, int numBranches)
{
	// 가장 아래쪽 나뭇가지 (화면에서 플레이어와 같은 위치)의 방향과 플레이어의 방향이 같으면 충돌
	return branches[numBranches - 1] == playerSide;
}
//전역변수
bool isGamePaused = false;
sf::Font font;
sf::Text pauseText;

// ------------------- 메인 함수 -------------------
int main() {
	
	bool showAxe = false;

	Side side;//데이터형 변수명
	side = Side::LEFT;
	if (side == Side::RIGHT) {


	}
	else {


	}
	Side sidePlayer = Side::LEFT; //플레이어 현재 방향 초기화
	Side prevSidePlayer = sidePlayer;//이전 방향 변수 초기화



	srand(static_cast<unsigned>(time(0)));

	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Timber!");

	// 텍스처 로드
	sf::Texture texBackground, textureTree, texCloud, texBee, axe;
	texBackground.loadFromFile("graphics/background.png");
	textureTree.loadFromFile("graphics/tree.png");
	texCloud.loadFromFile("graphics/cloud.png");
	texBee.loadFromFile("graphics/bee.png");
	sf::Texture texturePlayer;//그래픽 파일에서 이미지 불러오기
	texturePlayer.loadFromFile("graphics/player.png");
	sf::Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");
	sf::Texture textureaxe;
	textureaxe.loadFromFile("graphics/axe.png");
	font.loadFromFile("fonts/KOMIKAP_.ttf");//사용중인 폰트파일
	pauseText.setFont(font);
	pauseText.setString("충돌! 엔터를 눌러 다시 시작");
	pauseText.setCharacterSize(80);
	pauseText.setFillColor(sf::Color::Red);
	pauseText.setPosition(500.f, 400.f); // 원하는 위치로 조정

	// 배경, 나무
	sf::Sprite background(texBackground);
	sf::Sprite spirteTree(textureTree);
	spirteTree.setOrigin(textureTree.getSize().x * 0.5f, 0.f);
	spirteTree.setPosition(1920 * 0.5f, 0.f);

	Bee bee(texBee);
	const int NUM_CLOUDS = 3;
	Cloud clouds[NUM_CLOUDS] = {
		Cloud(texCloud, 150.f),
		Cloud(texCloud, 300.f),
		Cloud(texCloud, 500.f)
	};

	sf::Sprite spriteplayer;//캐릭터 설정
	spriteplayer.setTexture(texturePlayer);
	spriteplayer.setOrigin(texturePlayer.getSize().x * 0.5f, texturePlayer.getSize().y);
	spriteplayer.setPosition(1920 * 0.5f, 900.f);

	sf::Sprite spriteaxe;//도끼 설정
	spriteaxe.setTexture(textureaxe);
	spriteaxe.setOrigin(textureaxe.getSize().x * 0.5f, textureaxe.getSize().y);

	const int NUM_BRANCHES = 6;//const는 이름이 있는 상수로 만들어준다. 상수는 수정불가. 읽기만 가능하다
	sf::Sprite spriteBranch[NUM_BRANCHES];
	Side sideBranch[NUM_BRANCHES] = { Side::LEFT,Side::RIGHT,Side::NONE,Side::LEFT,Side::RIGHT,Side::NONE};


	for (int i = 0; i < NUM_BRANCHES; ++i) 
	{

		spriteBranch[i].setTexture(textureBranch);
		spriteBranch[i].setOrigin(textureTree.getSize().x * -0.5f, 0.f);
		spriteBranch[i].setPosition(1920 * 0.5f, i * 150.f);
		spriteBranch[i].setScale(-1.f, 1.f);
	}
	////왼쪽 오른쪽 방향키
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	//{
	//	spriteplayer.setScale(-1.f, 1.f); // 왼쪽을 바라보도록 반전
	//	spriteplayer.setPosition(1920 * 0.3f, 900.f); // 왼쪽 위치로 이동
	//}
	//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	//{
	//	spriteplayer.setScale(1.f, 1.f); // 오른쪽을 바라보도록
	//	spriteplayer.setPosition(1920 * 0.7f, 900.f); // 오른쪽 위치로 이동
	//
	//Side sidePlayer = Side::LEFT;

	sf::Clock clock; // Ensure the clock is defined before the main loop

		bool isLeft = false;
		bool isRight = false;

		

	while (window.isOpen()) {
		sf::Time dt = clock.restart(); // Restart the clock and get the elapsed time
		float deltaTime = dt.asSeconds(); // Convert elapsed time to seconds

		// 이벤트 루프 매프레임마다 호출되는 것
		sf::Event event;
			bool isLeftDown = false;
			bool isLeftUp = false;
			bool isRightDown = false;
			bool isRightUp = false;
		while (window.pollEvent(event)) 
		{

			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				switch (event.key.code)
				{
				case sf::Keyboard::Left:
					if(!isLeft)//왼쪽이 처음 눌리는 순간
					{
						isLeftDown = true;
						break;
					}
					isLeft = true;
					break;
				
				case sf::Keyboard::Right:
					if (!isRight) 
					{
						isRightDown = true;
						break;
					}
					isRight = true;
					break;
			
					if (event.key.code == sf::Keyboard::Enter && isGamePaused) {
						// 게임 상태 초기화
						isGamePaused = false;
						showAxe = false;
						sidePlayer = Side::LEFT;
						prevSidePlayer = sidePlayer;

						// 나뭇가지 초기화
						for (int i = 0; i < NUM_BRANCHES; ++i)
							sideBranch[i] = Side::NONE;

						// 플레이어 위치 초기화
						spriteplayer.setScale(-1.f, 1.f);
						spriteplayer.setPosition(spirteTree.getPosition().x - 300.f, 950.f);
					}
					if (!isGamePaused) {
						bee.update(deltaTime); // Use deltaTime instead of dt
						for (int i = 0; i < NUM_CLOUDS; ++i)
							clouds[i].update(deltaTime);

						// 나뭇가지 방향 처리 등 추가적인 업데이트들도 여기에만 위치

					}
					//sidePlayer = Side::RIGHT;
					//updateBranches(sideBranch, NUM_BRANCHES);
				}
				break;
			case sf::Event::KeyReleased://안눌려져있음
				switch (event.key.code) {
				case sf::Keyboard::Left:
					isLeft = false;
					isLeftUp = true;
					break;
				case sf::Keyboard::Right:
					isRight = false;
					isRightUp = true;
					break;
				}

				break;
			
			}


			if (event.type == sf::Event::Closed)
				window.close();


		}

		//누르고 있을때만
		showAxe = sf::Keyboard::isKeyPressed(sf::Keyboard::Left) 
			|| sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
	
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			sidePlayer = Side::LEFT;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			sidePlayer = Side::RIGHT;
		}

	
		// 업데이트

		if (isRightDown || isLeftDown)
		{


			if (isLeftDown)
			{
				sidePlayer = Side::LEFT;
			}
			if (isRightDown)
			{
				sidePlayer = Side::RIGHT;
			}
			//충돌 시 게임 멈춤 처리
			if (sidePlayer != prevSidePlayer) {
				updateBranches(sideBranch, NUM_BRANCHES);

					printf("충돌!\n");
				if (sidePlayer == sideBranch[NUM_BRANCHES - 1]) 
				{
					isGamePaused = true;
					showAxe = false;
				}

				prevSidePlayer = sidePlayer;
			
			}
		}

			//방향이 바뀌었을 때만 나뭇가지 업데이트

			
			//if (sidePlayer != prevSidePlayer) 
			//{
			//	updateBranches(sideBranch, NUM_BRANCHES);

			//	if (sidePlayer == sideBranch[NUM_BRANCHES - 1]) {
			//	}

			//	prevSidePlayer = sidePlayer; // 이전 방향 갱신
			//}
		//}
		//일시정지가 아니라면
		if (!isGamePaused) {

	
		

        for (int i = 0; i < NUM_BRANCHES; ++i)
        {
            switch (sideBranch[i])
            {
            case Side::LEFT:
                spriteBranch[i].setScale(-1.f, 1.f);
                break;
            case Side::RIGHT:
                spriteBranch[i].setScale(1.f, 1.f);
                break;
            }
        }
		switch (sidePlayer)//플레이어 움직임 업데이트
		{
		case Side::LEFT:
			spriteplayer.setScale(-1.f, 1.f);
			spriteaxe.setScale(-1.f, 1.f);
			spriteplayer.setPosition(spirteTree.getPosition().x - 300.f, 950.f);

			break;
		case Side::RIGHT:
			spriteplayer.setScale(1.f, 1.f);
			spriteaxe.setScale(1.f, 1.f);
			spriteplayer.setPosition(spirteTree.getPosition().x + 300.f, 950.f);
			break;
		}
		spriteaxe.setPosition
		(spirteTree.getPosition().x + ((sidePlayer == Side::LEFT) ? -150.f : 150.f),
			spriteplayer.getPosition().y - 50.f
		);
		for (int i = 0; i < NUM_CLOUDS; ++i)
			clouds[i].update(deltaTime);

		for (int i = 0; i < NUM_BRANCHES; ++i) 
		{
			switch (sideBranch[i]) {

			case Side::LEFT:
				spriteBranch[i].setScale(-1.f, 1.f);
					break;
			case Side::RIGHT:
				spriteBranch[i].setScale(1.f, 1.f);
				
			}
		}

		// 그리기
		window.clear();
		window.draw(background);

		window.draw(spirteTree);
		for (int i = 0; i < NUM_BRANCHES; ++i) //나뭇가지

		{
			if (sideBranch[i] != Side::NONE)
			{
				window.draw(spriteBranch[i]);
			}
		}


		for (int i = 0; i < NUM_CLOUDS; ++i)
		{
			window.draw(clouds[i].sprite);
		}
		if (showAxe)
		{
		window.draw(spriteaxe);
		}
		//플레이어,벌
		window.draw(spriteplayer);
		window.draw(bee.sprite);
		if (isGamePaused) {
			window.draw(pauseText);
		}
		window.display();

	}
	return 0;
}
