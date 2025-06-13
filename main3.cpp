#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <vector>

// ��ƿ �Լ�: ���� ���� ��ȯ
float getRandomDirection() {
	return (rand() % 2 == 0) ? 1.f : -1.f;
}



// ------------------- Cloud Ŭ���� -------------------
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
		direction.x = getRandomDirection();//���ⷣ��
		speed = static_cast<float>(rand() % 200 + 100);//�ӵ� ���� 100~299
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

// ------------------- Bee Ŭ���� -------------------
class Bee {
public:
	sf::Sprite sprite;
	sf::Vector2f direction;
	float speed;
	float elapsedTime;//������ �ð� ������

	Bee(const sf::Texture& texture) {
		sprite.setTexture(texture);
		reset();
	}
	void update(float dt) {
		elapsedTime += dt;
		sprite.move(direction.x * speed * dt, 0.f);

		float ampliutude = 50.f;//������ ���� ,����
		float frequency = 2.f;//� �ֱ� ����,��
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

enum class Side { LEFT, RIGHT, NONE }; //���������� ������(enum) 3���� ���ͷ�

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
	// ���� �Ʒ��� �������� (ȭ�鿡�� �÷��̾�� ���� ��ġ)�� ����� �÷��̾��� ������ ������ �浹
	return branches[numBranches - 1] == playerSide;
}
//��������
bool isGamePaused = false;
sf::Font font;
sf::Text pauseText;

// ------------------- ���� �Լ� -------------------
int main() {
	
	bool showAxe = false;

	Side side;//�������� ������
	side = Side::LEFT;
	if (side == Side::RIGHT) {


	}
	else {


	}
	Side sidePlayer = Side::LEFT; //�÷��̾� ���� ���� �ʱ�ȭ
	Side prevSidePlayer = sidePlayer;//���� ���� ���� �ʱ�ȭ



	srand(static_cast<unsigned>(time(0)));

	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Timber!");

	// �ؽ�ó �ε�
	sf::Texture texBackground, textureTree, texCloud, texBee, axe;
	texBackground.loadFromFile("graphics/background.png");
	textureTree.loadFromFile("graphics/tree.png");
	texCloud.loadFromFile("graphics/cloud.png");
	texBee.loadFromFile("graphics/bee.png");
	sf::Texture texturePlayer;//�׷��� ���Ͽ��� �̹��� �ҷ�����
	texturePlayer.loadFromFile("graphics/player.png");
	sf::Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");
	sf::Texture textureaxe;
	textureaxe.loadFromFile("graphics/axe.png");
	font.loadFromFile("fonts/KOMIKAP_.ttf");//������� ��Ʈ����
	pauseText.setFont(font);
	pauseText.setString("�浹! ���͸� ���� �ٽ� ����");
	pauseText.setCharacterSize(80);
	pauseText.setFillColor(sf::Color::Red);
	pauseText.setPosition(500.f, 400.f); // ���ϴ� ��ġ�� ����

	// ���, ����
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

	sf::Sprite spriteplayer;//ĳ���� ����
	spriteplayer.setTexture(texturePlayer);
	spriteplayer.setOrigin(texturePlayer.getSize().x * 0.5f, texturePlayer.getSize().y);
	spriteplayer.setPosition(1920 * 0.5f, 900.f);

	sf::Sprite spriteaxe;//���� ����
	spriteaxe.setTexture(textureaxe);
	spriteaxe.setOrigin(textureaxe.getSize().x * 0.5f, textureaxe.getSize().y);

	const int NUM_BRANCHES = 6;//const�� �̸��� �ִ� ����� ������ش�. ����� �����Ұ�. �б⸸ �����ϴ�
	sf::Sprite spriteBranch[NUM_BRANCHES];
	Side sideBranch[NUM_BRANCHES] = { Side::LEFT,Side::RIGHT,Side::NONE,Side::LEFT,Side::RIGHT,Side::NONE};


	for (int i = 0; i < NUM_BRANCHES; ++i) 
	{

		spriteBranch[i].setTexture(textureBranch);
		spriteBranch[i].setOrigin(textureTree.getSize().x * -0.5f, 0.f);
		spriteBranch[i].setPosition(1920 * 0.5f, i * 150.f);
		spriteBranch[i].setScale(-1.f, 1.f);
	}
	////���� ������ ����Ű
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	//{
	//	spriteplayer.setScale(-1.f, 1.f); // ������ �ٶ󺸵��� ����
	//	spriteplayer.setPosition(1920 * 0.3f, 900.f); // ���� ��ġ�� �̵�
	//}
	//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	//{
	//	spriteplayer.setScale(1.f, 1.f); // �������� �ٶ󺸵���
	//	spriteplayer.setPosition(1920 * 0.7f, 900.f); // ������ ��ġ�� �̵�
	//
	//Side sidePlayer = Side::LEFT;

	sf::Clock clock; // Ensure the clock is defined before the main loop

		bool isLeft = false;
		bool isRight = false;

		

	while (window.isOpen()) {
		sf::Time dt = clock.restart(); // Restart the clock and get the elapsed time
		float deltaTime = dt.asSeconds(); // Convert elapsed time to seconds

		// �̺�Ʈ ���� �������Ӹ��� ȣ��Ǵ� ��
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
					if(!isLeft)//������ ó�� ������ ����
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
						// ���� ���� �ʱ�ȭ
						isGamePaused = false;
						showAxe = false;
						sidePlayer = Side::LEFT;
						prevSidePlayer = sidePlayer;

						// �������� �ʱ�ȭ
						for (int i = 0; i < NUM_BRANCHES; ++i)
							sideBranch[i] = Side::NONE;

						// �÷��̾� ��ġ �ʱ�ȭ
						spriteplayer.setScale(-1.f, 1.f);
						spriteplayer.setPosition(spirteTree.getPosition().x - 300.f, 950.f);
					}
					if (!isGamePaused) {
						bee.update(deltaTime); // Use deltaTime instead of dt
						for (int i = 0; i < NUM_CLOUDS; ++i)
							clouds[i].update(deltaTime);

						// �������� ���� ó�� �� �߰����� ������Ʈ�鵵 ���⿡�� ��ġ

					}
					//sidePlayer = Side::RIGHT;
					//updateBranches(sideBranch, NUM_BRANCHES);
				}
				break;
			case sf::Event::KeyReleased://�ȴ���������
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

		//������ ��������
		showAxe = sf::Keyboard::isKeyPressed(sf::Keyboard::Left) 
			|| sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
	
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			sidePlayer = Side::LEFT;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			sidePlayer = Side::RIGHT;
		}

	
		// ������Ʈ

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
			//�浹 �� ���� ���� ó��
			if (sidePlayer != prevSidePlayer) {
				updateBranches(sideBranch, NUM_BRANCHES);

					printf("�浹!\n");
				if (sidePlayer == sideBranch[NUM_BRANCHES - 1]) 
				{
					isGamePaused = true;
					showAxe = false;
				}

				prevSidePlayer = sidePlayer;
			
			}
		}

			//������ �ٲ���� ���� �������� ������Ʈ

			
			//if (sidePlayer != prevSidePlayer) 
			//{
			//	updateBranches(sideBranch, NUM_BRANCHES);

			//	if (sidePlayer == sideBranch[NUM_BRANCHES - 1]) {
			//	}

			//	prevSidePlayer = sidePlayer; // ���� ���� ����
			//}
		//}
		//�Ͻ������� �ƴ϶��
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
		switch (sidePlayer)//�÷��̾� ������ ������Ʈ
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

		// �׸���
		window.clear();
		window.draw(background);

		window.draw(spirteTree);
		for (int i = 0; i < NUM_BRANCHES; ++i) //��������

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
		//�÷��̾�,��
		window.draw(spriteplayer);
		window.draw(bee.sprite);
		if (isGamePaused) {
			window.draw(pauseText);
		}
		window.display();

	}
	return 0;
}
