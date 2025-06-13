#include <SFML/Graphics.hpp>
#include <ctime>
#include <cstdlib>

class Cloud {
public:
    Cloud(const sf::Texture& texture, float yPos);

    void update(float deltaTime);
    void draw(sf::RenderWindow& window);

private:
    sf::Sprite sprite;
    sf::Vector2f direction;
    float speed;
    float y;

    void reset() // ��ġ, ����, ������ �ٽ� �ʱ�ȭ
    {
        direction.x = (rand() % 2 == 0) ? 1.f : -1.f;
        speed = static_cast<float>(rand() % 200 + 100);
        float scale = static_cast<float>((rand() % 50 + 50)) / 100.f;

        sprite.setScale(direction.x < 0 ? -scale : scale, scale);
        float x = (direction.x > 0) ? -300.f : 1920.f + 300.f;
        sprite.setPosition(x, y);
    }
};



int main()
{
    srand((int)time(0));

    //printf("%d\n\n", RAND_MAX);

    for (int i = 0; i < 10; ++i)
    {
    printf("%d\n",rand() );
    }

    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Timber!");
   
    sf::Texture textureBackground;// graphics ���Ͽ��� �̹��� �ҷ�����
    textureBackground.loadFromFile("graphics/background.png");
    sf::Texture textureTree;
    textureTree.loadFromFile("graphics/tree.png");
    sf::Texture texturebee;
    texturebee.loadFromFile("graphics/bee.png");
    sf::Texture texturCloud;
    texturCloud.loadFromFile("graphics/cloud.png");
  

    //��������Ʈ

    sf::Sprite sprirteBackground;//�̹��� �׸���
    sprirteBackground.setTexture(textureBackground);
    sf::Sprite sprirteTree;//���� �׸���
    sprirteTree.setTexture(textureTree);
    //sprirteTree.setPosition(1920*0.5-textureTree.getSize().x*0.5f, 0);

    sf::Sprite sprirtecloud;//���� �׸���
    sprirtecloud.setTexture(texturCloud);
    sprirtecloud.setPosition(0,100);

    sf::Sprite sprirtecloud2;//���� �׸���2
    sprirtecloud2.setTexture(texturCloud);
    sprirtecloud2.setPosition(0, 250);

    sf::Sprite sprirtecloud3;//���� �׸���3
    sprirtecloud3.setTexture(texturCloud);
    sprirtecloud3.setPosition(0, 600);

    sf::Sprite spriteBee;//�� �׸���
    spriteBee.setTexture(texturebee);
    spriteBee.setPosition(1920*0.5f, 800);
    spriteBee.setScale(-1.f,1.f);//1.f 1.f

    //�� �̵�
    sf::Vector2f beeDirection = { 1.f,0.f };
    float beeSpeed = 200.f;
    float random = (float)rand() / RAND_MAX; //0.f~1.f
        if (random < 0.5f) 
        {
            beeDirection.x = 1.f;//������ �����ִ�
            spriteBee.setScale(-1.f, 1.f);
    }
        else {

            beeDirection.x = -1.f;//���� �������� �����ִ�
            spriteBee.setScale(1.f, 1.f);
        }
        beeSpeed = rand() % 200 + 100; //100~299
        spriteBee.setPosition(500, 700);
        
        //���� �̵�
        sf::Vector2f cloudDirection = { 1.f,0.f };
        float cloudSpeed = 200.f;
        float random = (float)rand() / RAND_MAX; //0.f~1.f
        if (random < 0.5f)
        {
            cloudDirection.x = 1.f;//������ �����ִ�
            sprirtecloud.setScale(-1.f, 1.f);
        }
        else {

            cloudDirection.x = -1.f;//�������� �����ִ�
           
            sprirtecloud.setScale(1.f, 1.f);
        }
        cloudSpeed = rand() % 200 + 100; //100~299
        sprirtecloud.setPosition(500, 700);



    sprirteTree.setOrigin(textureTree.getSize().x * 0.5f, 0.f);
    sprirteTree.setPosition(1920 * 0.5f, 0.f);

    

    sf::Vector2f v = { 200.f,0.f };
    //���� ���� ����
    sf::Vector2f cloudDirection1, cloudDirection2, cloudDirection3;
    float speed1 = static_cast<float>(std::rand() % 200 + 100); // 100~299
    float speed2 = static_cast<float>(std::rand() % 200 + 100);
    float speed3 = static_cast<float>(std::rand() % 200 + 100);
    //// �ʱ� ���� ���� ����
    //cloudDirection1.x = (rand() % 2 == 0) ? -1.f : 1.f,1.f;
    //cloudDirection2.x = (rand() % 2 == 0) ? -1.f : 1.f, 1.f;
    //cloudDirection3.x = (rand() % 2 == 0) ? -1.f : 1.f, 1.f;
    //// �¿� ���� ������ ����
    //sprirtecloud.setScale(cloudDirection1.x, 1.f);
    //sprirtecloud2.setScale(cloudDirection2.x, 1.f);
    //sprirtecloud3.setScale(cloudDirection3.x, 1.f);

    sf::Clock clock;
    while (window.isOpen())
    {
        sf::Time time= clock.restart(); //��ž��ġ ���
        float deltaTime = time.asSeconds(); //��(second)�� �Ǽ��� ��ȯ�ϴ� �Լ�
     

        // ���� �ʱ�ȭ �ڵ� ���� (���� + �߾� ���� ����)
        auto resetCloud = [](sf::Sprite& cloud, float& speed, sf::Vector2f& dir, float yPos) {
            dir.x = (rand() % 2 == 0) ? 1.f : -1.f;
            speed = static_cast<float>(rand() % 200 + 100);
            float scale = static_cast<float>((rand() % 50 + 50)) / 100.f;//0.5~1.0����


            cloud.setOrigin(cloud.getLocalBounds().width * 0.5f, 0.f);
            cloud.setScale(dir.x < 0 ? -scale : scale, scale);

            float x = (dir.x > 0) ? -300.f : 1920.f + 300.f;
            cloud.setPosition(x, yPos);
            };

        if (sprirtecloud.getPosition().x < -400 || sprirtecloud.getPosition().x > 1920 + 400)
            resetCloud(sprirtecloud, speed1, cloudDirection1, 150.f);

        if (sprirtecloud2.getPosition().x < -400 || sprirtecloud2.getPosition().x > 1920 + 400)
            resetCloud(sprirtecloud2, speed2, cloudDirection2, 400.f);

        if (sprirtecloud3.getPosition().x < -400 || sprirtecloud3.getPosition().x > 1920 + 400)
            resetCloud(sprirtecloud3, speed3, cloudDirection3, 600.f);
        //// ������ ȭ���� ����� �ٽ� ���ʿ��� ���� + �ӵ� ���� ����
        //if (sprirtecloud.getPosition().x > 1920)
        //{
        //    sprirtecloud.setPosition(-300.f, 150.f);
        //    speed1 = ((float)(rand() % 150) + 150);
        //}

        //if (sprirtecloud2.getPosition().x > 1920)
        //{
        //    sprirtecloud2.setPosition(-300.f, 400.f);
        //    speed2 = ((float)(rand() % 150) + 150);
        //}

        //if (sprirtecloud3.getPosition().x > 1920)
        //{
        //    sprirtecloud3.setPosition(-300.f, 600.f);
        //    speed3 =((float)(rand() % 150)+150);
        //}

        //�̺�Ʈ ����
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        //������Ʈ //�� �����Ӹ��� ����

        sf::Vector2f position = spriteBee.getPosition();

        position += beeDirection * beeSpeed * deltaTime;//����*���ǵ�*���� �ð� 
        spriteBee.setPosition(position);

        if (position.x < -200 || position.x > 1920 + 200)
        {
            float random = (float)rand() / RAND_MAX;
            if (random < 0.5f)
            {
                beeDirection.x = 1.f;
                spriteBee.setScale(-1.f, 1.f);
                spriteBee.setPosition(0 - 150, 800);
            }
            else
            {
                beeDirection.x = -1.f;
                spriteBee.setScale(1.f, 1.f);
                spriteBee.setPosition(1920 + 150, 700);
            }
            beeSpeed = rand() % 200 + 100;
        }
    
        //���� �̵�
        sprirtecloud.move(cloudDirection1.x * speed1 * deltaTime, 0.f);
        sprirtecloud2.move(cloudDirection2.x * speed2 * deltaTime, 0.f);
        sprirtecloud3.move(cloudDirection3.x * speed3 * deltaTime, 0.f);

        }

        //�׸���
        window.clear();
        window.draw(sprirteBackground);
        window.draw(sprirteTree);
        window.draw(sprirtecloud);
        window.draw(sprirtecloud2);
        window.draw(sprirtecloud3);
        window.draw(spriteBee);

        


        window.display();

        
    }

    return 0;
}