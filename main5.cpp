// 개선된 구조의 SFML Timber! 게임 코드

#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>

// ------------------- 유틸 함수 -------------------
float getRandomDirection() {
    return (rand() % 2 == 0) ? 1.f : -1.f;
}

// ------------------- 열거형 -------------------
enum class Side { LEFT, RIGHT, NONE };

// ------------------- 클래스 정의 -------------------
class Cloud {
public:
    sf::Sprite sprite;
    sf::Vector2f direction;
    float speed;

    Cloud(const sf::Texture& texture, float yPos) {
        sprite.setTexture(texture);
        reset(yPos);
    }

    void update(float dt) {
        sprite.move(direction.x * speed * dt, 0.f);
        if (sprite.getPosition().x < -400 || sprite.getPosition().x > 2320)
            reset(sprite.getPosition().y);
    }

private:
    void reset(float yPos) {
        direction.x = getRandomDirection();
        speed = static_cast<float>(rand() % 200 + 100);
        float scale = static_cast<float>(rand() % 50 + 50) / 100.f;
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

class Bee {
public:
    sf::Sprite sprite;
    sf::Vector2f direction;
    float speed;
    float elapsedTime;

    Bee(const sf::Texture& texture) {
        sprite.setTexture(texture);
        reset();
    }

    void update(float dt) {
        elapsedTime += dt;
        sprite.move(direction.x * speed * dt, 0.f);
        float baseY = (direction.x > 0) ? 800.f : 700.f;
        float offsetY = 50.f * std::sin(2.f * elapsedTime);
        sprite.setPosition(sprite.getPosition().x, baseY + offsetY);

        if (sprite.getPosition().x < -200 || sprite.getPosition().x > 2120)
            reset();
    }

private:
    void reset() {
        direction.x = getRandomDirection();
        speed = static_cast<float>(rand() % 200 + 100);
        float x = (direction.x > 0) ? -150.f : 2070.f;
        float y = (direction.x > 0) ? 800.f : 700.f;
        sprite.setPosition(x, y);
        sprite.setScale(-direction.x, 1.f);
        elapsedTime = 0.f;
    }
};

// ------------------- 함수 정의 -------------------
void updateBranches(Side* branches, int size) {
    for (int i = size - 1; i > 0; --i)
        branches[i] = branches[i - 1];

    int r = rand() % 3;
    branches[0] = (r == 0) ? Side::LEFT : (r == 1) ? Side::RIGHT : Side::NONE;
}

bool checkCollision(Side* branches, Side playerSide, int numBranches) {
    return branches[numBranches - 1] == playerSide;
}

// ------------------- 메인 함수 -------------------
int main() {
    srand(static_cast<unsigned>(time(0)));

    const int WIDTH = 1920, HEIGHT = 1080, NUM_BRANCHES = 6, NUM_CLOUDS = 3;
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Timber!");

    sf::Texture texBackground, texTree, texCloud, texBee, texPlayer, texBranch, texAxe;
    texBackground.loadFromFile("graphics/background.png");
    texTree.loadFromFile("graphics/tree.png");
    texCloud.loadFromFile("graphics/cloud.png");
    texBee.loadFromFile("graphics/bee.png");
    texPlayer.loadFromFile("graphics/player.png");
    texBranch.loadFromFile("graphics/branch.png");
    texAxe.loadFromFile("graphics/axe.png");

    sf::Font font;
    font.loadFromFile("graphics/KOMIKAP_.ttf");

    sf::Text pauseText("충돌! 엔터를 눌러 다시 시작", font, 80);
    pauseText.setFillColor(sf::Color::Red);
    pauseText.setPosition(500.f, 400.f);

    sf::Sprite background(texBackground);
    sf::Sprite tree(texTree);
    tree.setOrigin(texTree.getSize().x * 0.5f, 0.f);
    tree.setPosition(WIDTH * 0.5f, 0.f);

    Bee bee(texBee);
    Cloud clouds[NUM_CLOUDS] = {
        Cloud(texCloud, 150.f), Cloud(texCloud, 300.f), Cloud(texCloud, 500.f)
    };

    sf::Sprite player(texPlayer);
    player.setOrigin(texPlayer.getSize().x * 0.5f, texPlayer.getSize().y);
    player.setPosition(WIDTH * 0.5f, 900.f);

    sf::Sprite axe(texAxe);
    axe.setOrigin(texAxe.getSize().x * 0.5f, texAxe.getSize().y);

    Side branches[NUM_BRANCHES] = {};
    sf::Sprite branchSprites[NUM_BRANCHES];
    for (int i = 0; i < NUM_BRANCHES; ++i) {
        branchSprites[i].setTexture(texBranch);
        branchSprites[i].setOrigin(texTree.getSize().x * -0.5f, 0.f);
        branchSprites[i].setPosition(WIDTH * 0.5f, i * 150.f);
    }

    bool isGamePaused = false, showAxe = false;
    Side playerSide = Side::LEFT, prevPlayerSide = playerSide;

    sf::Clock clock;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter && isGamePaused) {
                    isGamePaused = false;
                    showAxe = false;
                    playerSide = Side::LEFT;
                    prevPlayerSide = playerSide;
                    for (int i = 0; i < NUM_BRANCHES; ++i)
                        branches[i] = Side::NONE;
                    player.setScale(-1.f, 1.f);
                    player.setPosition(tree.getPosition().x - 300.f, 950.f);
                }
                if (!isGamePaused) {
                    if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Right) {
                        playerSide = (event.key.code == sf::Keyboard::Left) ? Side::LEFT : Side::RIGHT;
                       /* if (playerSide != prevPlayerSide) {*/
                            updateBranches(branches, NUM_BRANCHES);
                            if (checkCollision(branches, playerSide, NUM_BRANCHES))
                                isGamePaused = true;
                            prevPlayerSide = playerSide;
                            showAxe = true;
                    }
                }
            }
        }

        if (!isGamePaused) {
            bee.update(dt);
            for (auto& c : clouds) c.update(dt);

            player.setScale((playerSide == Side::LEFT) ? -1.f : 1.f, 1.f);
            axe.setScale((playerSide == Side::LEFT) ? -1.f : 1.f, 1.f);
            player.setPosition(tree.getPosition().x + ((playerSide == Side::LEFT) ? -300.f : 300.f), 950.f);
            axe.setPosition(tree.getPosition().x + ((playerSide == Side::LEFT) ? -150.f : 150.f), player.getPosition().y - 50.f);
        }

        for (int i = 0; i < NUM_BRANCHES; ++i) {
            branchSprites[i].setScale((branches[i] == Side::LEFT) ? -1.f : 1.f, 1.f);
        }

        window.clear();
        window.draw(background);
        window.draw(tree);
        for (int i = 0; i < NUM_BRANCHES; ++i)
            if (branches[i] != Side::NONE)
                window.draw(branchSprites[i]);
        for (auto& c : clouds) window.draw(c.sprite);
        if (showAxe) window.draw(axe);
        window.draw(player);
        window.draw(bee.sprite);
        if (isGamePaused) window.draw(pauseText);
        window.display();
    }
    return 0;
}
