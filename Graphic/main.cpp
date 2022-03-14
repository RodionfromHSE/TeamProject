#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;

int number_of_obstacles = 3;
int speed = 10;
int gravitation = 50;
int horizontal = 1700;
int vertical = 800;
int left_border = 600;
int jump = 50;

struct point {
    int x, y;
};


int main()
{
    srand(time(0));

    RenderWindow app(VideoMode(1794, 1248), "Team PR 1");
    app.setFramerateLimit(60);

    Texture t1, t2, t3;
    t1.loadFromFile("background.png");
    t2.loadFromFile("avatar.gif");
    t3.loadFromFile("flower.png");
    Sprite sBackground(t1), sPers(t2), sObstacle(t3);
    sPers.setPosition(1000,800);
    sObstacle.setPosition(900, 805);

    int x = 100, y = 800, dx = 10;

    bool isFlipped = false;

    point Obstacle[number_of_obstacles];
    for (int i = 0; i < number_of_obstacles; i++)
    {
        Obstacle[i].x = rand() % horizontal;
        Obstacle[i].y = vertical + 5;
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
            x+=speed;
            isFlipped = true;
            if (x > 1000)
                for (int i = 0; i < number_of_obstacles; i++)
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
            x -= speed;
            isFlipped = false;
            if (x < left_border)
                for (int i = 0; i < number_of_obstacles ;i++)
                {
                    x = left_border;
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
            y -= 100;
        }

        if (y > 800) {
            y = 800;
        }
        if (y < 0) {
            y = 0;
        }
        if (y < 800) {
            y += gravitation;
        }

        for (int i = 0; i < number_of_obstacles; i++) { //TODO: перенести в спец функцию для проверки препятствий
            if ((x >= Obstacle[i].x - 60) && (x < Obstacle[i].x + 15) && y >= 680) {
                x -= 6;
            }
            if ((x >= Obstacle[i].x + 15) && (x < Obstacle[i].x + 140) && y >= 680) {
                x += 10;
            }
        }

        if (x > 1000)
            for (int i = 0; i < number_of_obstacles;i++)
            {
                x = 1000;
                Obstacle[i].x =Obstacle[i].x - dx;
                if (Obstacle[i].x < -40) {
                    Obstacle[i].x = 0;
                    Obstacle[i].x = rand() % 300 + 1500;
                }
            }

        if (x < left_border)
            for (int i = 0; i < number_of_obstacles;i++)
            {
                x = left_border;
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
        for (int i = 0; i < number_of_obstacles; i++)
        {
            sObstacle.setPosition(Obstacle[i].x,Obstacle[i].y);
            app.draw(sObstacle);
        }
        app.display();
    }

    return 0;
}