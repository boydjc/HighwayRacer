#include <SFML/graphics.hpp>
#include <iostream>

using namespace sf;

int main()
{
    VideoMode vm(1086, 679);

    RenderWindow window(vm, "Highway Racer", Style::Titlebar);

    // Load textures here
    Texture textureRoad;
    textureRoad.loadFromFile("assets/imgs/background.png");

    Texture textureRoadStripe;
    textureRoadStripe.loadFromFile("assets/imgs/roadStripe.png");

    Texture textureRock;
    textureRock.loadFromFile("assets/imgs/rock.png");

    Texture textureCarRed;
    textureCarRed.loadFromFile("assets/imgs/carRed.png");

    Texture textureTree;
    textureTree.loadFromFile("assets/imgs/tree.png");

    // set the road up
    Sprite spriteRoad;
    spriteRoad.setTexture(textureRoad);

    Sprite spriteRoadStripe[5];
    for(int i=0; i<5; i++)
    {
        spriteRoadStripe[i].setTexture(textureRoadStripe);
        spriteRoadStripe[i].setPosition(525, i*150);
    }

    // environment details
    //rocks
    const int NUMBEROFROCKS = 5;
    Sprite spriteRock[NUMBEROFROCKS];

    srand(int(time(0)));

    for(int i=0; i<NUMBEROFROCKS; i++)
    {
        spriteRock[i].setTexture(textureRock);
        // spawn the environment details are random locations

        // this makes sure that all the rocks are spawned on one side of the road
        if(i % 2 ==  0)
        {
            spriteRock[i].setPosition((rand() % 280), (rand() % -1) - 3000);
        }else
        {
            spriteRock[i].setPosition((rand() % 670) + 610, (rand() % -1) - 900);
        }
    }

    // trees
    const int NUMBEROFTREES = 20;
    Sprite spriteTree[NUMBEROFTREES];

    for(int i=0; i<NUMBEROFTREES; i++)
    {
        spriteTree[i].setTexture(textureTree);

        if(i % 2 == 0)
        {
            spriteTree[i].setPosition((rand() % 230), (rand() % 1000) - 3000);
        }
        else
        {
            spriteTree[i].setPosition((rand() % 810) + 780, (rand() % 1000) - 900);
        }

        spriteTree[i].setOrigin(94, 102);
        spriteTree[i].setRotation((rand() % 360) + 1);
    }



    // set player car up
    Sprite spriteCar;
    spriteCar.setTexture(textureCarRed);
    spriteCar.setPosition(575, 500);

    bool gameRunning = false;

    /* in the game, everything around the car moves instead of the car moving
    *  gameSpeed is the speed at which everything moves which can be considered
    * also to be how fast the car will appear to be moving to the player */
    const float MAXGAMESPEED = 1300.0f;
    float gameSpeed = 0.0f;

    Clock clock;

    while(window.isOpen())
    {
        /*****************************
        *   Handle the player's input
        ******************************/
        if(Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }else if(Keyboard::isKeyPressed(Keyboard::Enter))
        {
            gameRunning = true;

        }else if(Keyboard::isKeyPressed(Keyboard::Space) && gameRunning)
        {
            if(gameSpeed < MAXGAMESPEED)
            {
               gameSpeed += 0.1;
            }
        }else
        {
            if(gameSpeed > 0.0)
            {
                gameSpeed -= 0.05;
            }
        }

        // control player car
        if(Keyboard::isKeyPressed(Keyboard::Left) && gameRunning)
        {
            // make sure that the car can't leave the road
            if(spriteCar.getPosition().x > 370)
            {
                spriteCar.setPosition(spriteCar.getPosition().x - .1, spriteCar.getPosition().y);
            }
        }else if(Keyboard::isKeyPressed(Keyboard::Right) && gameRunning)
        {
            if(spriteCar.getPosition().x < 617)
            {
                spriteCar.setPosition(spriteCar.getPosition().x + .1, spriteCar.getPosition().y);
            }
        }



        /*****************************
        *   Update the scene
        ******************************/

        if(gameRunning)
        {
            // move everything in relation to the game speed

            Time dt = clock.restart();

            for(int i=0; i<5; i++)
            {
                // road stripes
                spriteRoadStripe[i].setPosition(spriteRoadStripe[i].getPosition().x, (spriteRoadStripe[i].getPosition().y + (gameSpeed * dt.asSeconds())));

                /* when the stripe gets off screen, put it back at the top to give the illusion of
                * endless road */
                if(spriteRoadStripe[i].getPosition().y > 679)
                {
                    spriteRoadStripe[i].setPosition(spriteRoadStripe[i].getPosition().x, -70);
                }
            }

            // do the same for the environment detail but move them at a slightly slower speed
            for(int i=0; i<NUMBEROFROCKS; i++)
            {
                spriteRock[i].setPosition(spriteRock[i].getPosition().x, (spriteRock[i].getPosition().y + (gameSpeed/1.5 * dt.asSeconds())));

                if(spriteRock[i].getPosition().y > 679)
                {
                    if(i % 2 == 0)
                    {
                        spriteRock[i].setPosition((rand() % 280), (rand() % -1) - 8000);
                    }
                    else
                    {
                        spriteRock[i].setPosition((rand() % 810) + 780, (rand() % -1) - 5000);
                    }
                }
            }

            for(int i=0; i<NUMBEROFTREES; i++)
            {
                spriteTree[i].setPosition(spriteTree[i].getPosition().x, (spriteTree[i].getPosition().y + (gameSpeed/1.5 * dt.asSeconds())));

                if(spriteTree[i].getPosition().y > 800)
                {
                    if(i % 2 == 0)
                    {
                        spriteTree[i].setPosition((rand() % 280), (rand() % -1) - 200);
                    }
                    else
                    {
                        spriteTree[i].setPosition((rand() % 770) + 750, (rand() % -1) - 200);
                    }
                }
            }
        }


        /*****************************
        *   Draw the scene
        ******************************/
        // clear what is currently on the screen
        window.clear();

        window.draw(spriteRoad);

        for(int i=0; i<5; i++)
        {
            window.draw(spriteRoadStripe[i]);
        }

        window.draw(spriteCar);

        for(int i=0; i<NUMBEROFROCKS; i++)
        {
            window.draw(spriteRock[i]);
        }

        for(int i=0; i<NUMBEROFTREES; i++)
        {
            window.draw(spriteTree[i]);
        }

        window.display();
    }
    return 0;
}
