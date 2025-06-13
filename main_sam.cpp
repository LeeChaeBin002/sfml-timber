#include <SFML/Graphics.hpp>
#include <ctime>
#include <cstdlib>

enum class Side { LEFT, RIGHT, NONE };

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


int main()
{
    srand((int)time(0));

    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Timber!");

    //
    sf::Texture textureBackgroud;
    textureBackgroud.loadFromFile("graphics/background.png");
    sf::Texture textureTree;
    textureTree.loadFromFile("graphics/tree.png");
    sf::Texture textureBee;
    textureBee.loadFromFile("graphics/bee.png");
    sf::Texture textureCloud;
    textureCloud.loadFromFile("graphics/cloud.png");
    sf::Texture texturePlayer;
    texturePlayer.loadFromFile("graphics/player.png");
    sf::Texture textureBranch;
    textureBranch.loadFromFile("graphics/branch.png");

    // 

    sf::Sprite spirteBackground;
    spirteBackground.setTexture(textureBackgroud);
    sf::Sprite spirteTree;
    spirteTree.setTexture(textureTree);
    //spirteTree.setPosition(1920 * 0.5f - textureTree.getSize().x * 0.5f, 0);
    spirteTree.setOrigin(textureTree.getSize().x * 0.5f, 0.f);
    spirteTree.setPosition(1920 * 0.5f, 0.f);

    sf::Sprite spriteBackgroundElement[4];
    float speedElement[4];
    sf::Vector2f directionElement[4];

    int cloudCount = 3;

    for (int i = 0; i < 4; ++i)
    {
        if (i < cloudCount)
        {
            spriteBackgroundElement[i].setTexture(textureCloud);
            speedElement[i] = rand() % 200 + 100;
            float random = (float)rand() / RAND_MAX; // 0.f ~ 1.f
            if (random < 0.5f)
            {
                directionElement[i].x = 1.f;
                spriteBackgroundElement[i].setScale(-1.f, 1.f);
                spriteBackgroundElement[i].setPosition(-150, rand() % 300 + 100);
            }
            else
            {
                directionElement[i].x = -1.f;
                spriteBackgroundElement[i].setScale(1.f, 1.f);
                spriteBackgroundElement[i].setPosition(1920 + 150, rand() % 300 + 100);
            }
        }
        else
        {
            spriteBackgroundElement[i].setTexture(textureBee);
            float random = (float)rand() / RAND_MAX; // 0.f ~ 1.f
            if (random < 0.5f)
            {
                directionElement[i].x = 1.f;
                spriteBackgroundElement[i].setScale(-1.f, 1.f);
            }
            else
            {
                directionElement[i].x = -1.f;
                spriteBackgroundElement[i].setScale(1.f, 1.f);
            }
            speedElement[i] = rand() % 200 + 100;
            spriteBackgroundElement[i].setPosition(500, 700);
        }
    }

    sf::Sprite spritePlayer;
    spritePlayer.setTexture(texturePlayer);
    spritePlayer.setOrigin(texturePlayer.getSize().x * 0.5f, texturePlayer.getSize().y);
    spritePlayer.setPosition(1920 * 0.5f, 950.f);

    const int NUM_BRANCHES = 6;
    sf::Sprite spriteBranch[NUM_BRANCHES];
    Side sideBranch[NUM_BRANCHES];
    for (int i = 0; i < NUM_BRANCHES; ++i)
    {
        spriteBranch[i].setTexture(textureBranch);
        spriteBranch[i].setOrigin(textureTree.getSize().x * -0.5f, 0.f);
        spriteBranch[i].setPosition(1920 * 0.5f, i * 150.f);

        int r = rand() % 3;
        switch (r)
        {
        case 0:
            sideBranch[i] = Side::LEFT;
            break;
        case 1:
            sideBranch[i] = Side::RIGHT;
            break;
        default:
            sideBranch[i] = Side::NONE;
            break;
        }
    }
    sideBranch[NUM_BRANCHES - 1] = Side::NONE;


    Side sidePlayer = Side::LEFT;
    switch (sidePlayer)
    {
    case Side::LEFT:
        spritePlayer.setScale(-1.f, 1.f);
        spritePlayer.setPosition(spirteTree.getPosition().x - 300.f, 950.f);
        break;
    case Side::RIGHT:
        spritePlayer.setScale(1.f, 1.f);
        spritePlayer.setPosition(spirteTree.getPosition().x + 300.f, 950.f);
        break;
    }

    sf::Clock clock;
    while (window.isOpen())
    {
        sf::Time time = clock.restart();
        float deltaTime = time.asSeconds();

        // 이벤트 루프
        sf::Event event;
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
                    sidePlayer = Side::LEFT;
                    updateBranches(sideBranch, NUM_BRANCHES);
                    break;
                case sf::Keyboard::Right:
                    sidePlayer = Side::RIGHT;
                    updateBranches(sideBranch, NUM_BRANCHES);
                    break;
                }
                break;
            case sf::Event::KeyReleased:
                break;
            }
        }

        // 업데이트
        for (int i = 0; i < 3; ++i)
        {
            sf::Vector2f position = spriteBackgroundElement[i].getPosition();
            position += directionElement[i] * speedElement[i] * deltaTime;
            spriteBackgroundElement[i].setPosition(position);
            if (position.x < -200 || position.x > 1920 + 200)
            {
                float random = (float)rand() / RAND_MAX;
                if (random < 0.5f)
                {
                    directionElement[i].x = 1.f;
                    spriteBackgroundElement[i].setScale(-1.f, 1.f);
                    spriteBackgroundElement[i].setPosition(0 - 150, rand() % 300 + 100);
                }
                else
                {
                    directionElement[i].x = -1.f;
                    spriteBackgroundElement[i].setScale(1.f, 1.f);
                    spriteBackgroundElement[i].setPosition(1920 + 150, rand() % 300 + 100);
                }
                speedElement[i] = rand() % 200 + 100;
            }
        }

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

        switch (sidePlayer)
        {
        case Side::LEFT:
            spritePlayer.setScale(-1.f, 1.f);
            spritePlayer.setPosition(spirteTree.getPosition().x - 300.f, 950.f);
            break;
        case Side::RIGHT:
            spritePlayer.setScale(1.f, 1.f);
            spritePlayer.setPosition(spirteTree.getPosition().x + 300.f, 950.f);
            break;
        }

        // 그리기
        window.clear();
        window.draw(spirteBackground);
        for (int i = 0; i < cloudCount; ++i)
        {
            window.draw(spriteBackgroundElement[i]);
        }
        window.draw(spirteTree);
        for (int i = 0; i < NUM_BRANCHES; ++i)
        {
            if (sideBranch[i] != Side::NONE)
            {
                window.draw(spriteBranch[i]);
            }
        }

        for (int i = cloudCount; i < 4; ++i)
        {
            window.draw(spriteBackgroundElement[i]);
        }

        window.draw(spritePlayer);

        window.display();
    }

    return 0;
}