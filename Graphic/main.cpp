#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;

struct point {
    int x, y;
};


int main()
{
    srand(time(0));

    RenderWindow app(VideoMode(1794, 1248), "Team PR 1");
    app.setFramerateLimit(60);

    Texture t1, t2, t3;
    t1.loadFromFile("/Users/arno/Desktop/ohmygodness/images/background.png");
    t2.loadFromFile("/Users/arno/Desktop/ohmygodness/images/avatar.gif");
    t3.loadFromFile("/Users/arno/Desktop/ohmygodness/images/dmdld.png");
    Sprite sBackground(t1), sPers(t2), sObstacle(t3);
    sPers.setPosition(100,100);
    sObstacle.setPosition(900, 805);

    int x = 100, y = 800, dx = 5;

    bool isFlipped = false;

    point Obstacle[5];
    for (int i = 0; i < 5; i++)
    {
        Obstacle[i].x = rand() % 1700;
        Obstacle[i].y = 805;
    }

    while (app.isOpen())
    {
        Event e;
        while (app.pollEvent(e))
        {
            if (e.type == Event::Closed)
                app.close();
        }

        if (Keyboard::isKeyPressed(Keyboard::Right)) {
            x+=3;
            isFlipped = true;
            if (x > 1000)
                for (int i = 0; i < 5;i++)
                {
                    x = 1000;
                    Obstacle[i].x  = Obstacle[i].x - dx;
                    if (Obstacle[i].x < 0) {
                        Obstacle[i].x = 0;
                        Obstacle[i].x = rand() % 10 + 1500;
                    }
                }
        }
        if (Keyboard::isKeyPressed(Keyboard::Left)) {
            x -= 3;
            isFlipped = false;
            if (x < 400)
                for (int i = 0; i < 5;i++)
                {
                    x = 400;
                    Obstacle[i].x = Obstacle[i].x + dx;
                    if (Obstacle[i].x > 1794) {
                        Obstacle[i].x = 0;
                        Obstacle[i].x = rand() % 70;
                    }
                }
        }
        if (Keyboard::isKeyPressed(Keyboard::Down)) {
            y += 100;
        }
        if (Keyboard::isKeyPressed(Keyboard::Up)) {
            y -= 10;
        }

        if (y > 800) {
            y = 800;
        }
        if (y < 0) {
            y = 0;
        }
        for (int i = 0; i < 5; i++) { //TODO: перенести в спец функцию для проверки препятствий
            if ((x >= Obstacle[i].x - 60) && (x < Obstacle[i].x + 15) && y >= 680) {
                x -= 6;
            }
            if ((x >= Obstacle[i].x + 15) && (x < Obstacle[i].x + 140) && y >= 680) {
                x += 10;
            }
        }

        if (x > 1000)
            for (int i = 0; i < 5;i++)
            {
                x = 1000;
                Obstacle[i].x =Obstacle[i].x - dx;
                if (Obstacle[i].x < -40) {
                    Obstacle[i].x = 0;
                    Obstacle[i].x = rand() % 300 + 1500;
                }
            }

        if (x < 400)
            for (int i = 0; i < 5;i++)
            {
                x = 400;
                Obstacle[i].x =Obstacle[i].x - dx;
                if (Obstacle[i].x > 1794) {
                    Obstacle[i].x = 0;
                    Obstacle[i].x = rand() % 70;
                }
            }

        if (isFlipped) {
            sPers.setScale(sf::Vector2f(-1, 1));
            sPers.setPosition(x + 75, y);
        } else {
            sPers.setScale(sf::Vector2f(1, 1));
            sPers.setPosition(x - 75, y);
        }

        app.draw(sBackground);
        app.draw(sPers);
        for (int i = 0; i < 5; i++)
        {
            sObstacle.setPosition(Obstacle[i].x,Obstacle[i].y);
            app.draw(sObstacle);
        }
        app.display();
    }

    return 0;
}