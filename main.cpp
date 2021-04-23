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

    Texture textureCars[16];
    textureCars[0].loadFromFile("assets/imgs/carOne.png");
    textureCars[1].loadFromFile("assets/imgs/carTwo.png");
    textureCars[2].loadFromFile("assets/imgs/carThree.png");
    textureCars[3].loadFromFile("assets/imgs/carFour.png");
    textureCars[4].loadFromFile("assets/imgs/carFive.png");
    textureCars[5].loadFromFile("assets/imgs/carSix.png");
    textureCars[6].loadFromFile("assets/imgs/carSeven.png");
    textureCars[7].loadFromFile("assets/imgs/carEight.png");
    textureCars[8].loadFromFile("assets/imgs/carNine.png");
    textureCars[9].loadFromFile("assets/imgs/carTen.png");
    textureCars[10].loadFromFile("assets/imgs/carEleven.png");
    textureCars[11].loadFromFile("assets/imgs/carTwelve.png");
    textureCars[12].loadFromFile("assets/imgs/carThirteen.png");
    textureCars[13].loadFromFile("assets/imgs/carFourteen.png");
    textureCars[14].loadFromFile("assets/imgs/carFifteen.png");
    textureCars[15].loadFromFile("assets/imgs/carSixteen.png");

    Texture textureTree;
    textureTree.loadFromFile("assets/imgs/tree.png");

    Texture textureSpeedometer;
    textureSpeedometer.loadFromFile("assets/imgs/speedometer.png");

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
    const int NUMBEROFROCKS = 2;
    Sprite spriteRock[NUMBEROFROCKS];

    srand(int(time(0)));

    for(int i=0; i<NUMBEROFROCKS; i++)
    {
        spriteRock[i].setTexture(textureRock);
        // spawn the environment details are random locations

        // this makes sure that all the rocks spawn on different sides of the road
        if(i ==  0)
        {
            spriteRock[i].setPosition((rand() % 280), (rand() % 900) - 1500);
        }else
        {
            spriteRock[i].setPosition((rand() % 770) + 710, (rand() % 900) - 900);
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
            spriteTree[i].setPosition((rand() % 230), (rand() % 1000) - 500);
        }
        else
        {
            spriteTree[i].setPosition((rand() % 810) + 780, (rand() % 1000) - 500);
        }

        spriteTree[i].setOrigin(94, 102);
        spriteTree[i].setRotation((rand() % 360) + 1);
    }

    enum class CarState{ ACCELERATE, DECELERATE, STOPPED };

    // set player car up
    Sprite spritePlayer;
    spritePlayer.setTexture(textureCars[0]);
    spritePlayer.setPosition(575, 500);
    CarState playerCarState = CarState::STOPPED;

    // set up the other cars that will be in the way
    // there will only be a few sprites that will just change textures randomly
    Sprite spriteNpc[3];

    // set up the HUD
    Sprite spriteSpeedometer;
    spriteSpeedometer.setTexture(textureSpeedometer);
    spriteSpeedometer.setPosition(840, 520);

    // a triangle that will represent the speedometer needle
    ConvexShape dometerNeedle(3);
    dometerNeedle.setPoint(0, Vector2f(0, 10));
    dometerNeedle.setPoint(1, Vector2f(5, 80));
    dometerNeedle.setPoint(2, Vector2f(-5, 80));

    dometerNeedle.setPosition(915, 593);
    dometerNeedle.setFillColor(Color::Red);
    FloatRect dometerBounds = dometerNeedle.getLocalBounds();
    dometerNeedle.setOrigin(dometerBounds.left + dometerBounds.width - 5, dometerBounds.top + dometerBounds.height - 5);
    dometerNeedle.setRotation(225);

    bool gameRunning = false;

    /* in the game, everything around the car moves instead of the car moving
    *  gameSpeed is the speed at which everything moves which can be considered
    * also to be how fast the car will appear to be moving to the player */
    const float MAXGAMESPEED = 1800.0f;
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

            playerCarState = CarState::ACCELERATE;

            if(gameSpeed < MAXGAMESPEED)
            {
               gameSpeed += 0.1;
            }
        }else
        {

            playerCarState = CarState::DECELERATE;

            if(gameSpeed > 0.0)
            {
                gameSpeed -= 0.07;
            }
        }

        // control player car
        if(Keyboard::isKeyPressed(Keyboard::Left) && gameRunning)
        {
            // make sure that the car can't leave the road
            if(spritePlayer.getPosition().x > 370)
            {
                spritePlayer.setPosition(spritePlayer.getPosition().x - .2, spritePlayer.getPosition().y);
            }
        }else if(Keyboard::isKeyPressed(Keyboard::Right) && gameRunning)
        {
            if(spritePlayer.getPosition().x < 633)
            {
                spritePlayer.setPosition(spritePlayer.getPosition().x + .2, spritePlayer.getPosition().y);
            }
        }



        /*****************************
        *   Update the scene
        ******************************/

        if(gameRunning && gameSpeed > 0.0)
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
                    if(i == 0)
                    {
                        spriteRock[i].setPosition((rand() % 280), (rand() % -1) - 1000);
                    }
                    else
                    {
                        spriteRock[i].setPosition((rand() % 810) + 780, (rand() % -1) - 500);
                    }
                }
            }

            for(int i=0; i<NUMBEROFTREES; i++)
            {
                spriteTree[i].setPosition(spriteTree[i].getPosition().x, (spriteTree[i].getPosition().y + (gameSpeed/1.5 * dt.asSeconds())));

                if(spriteTree[i].getPosition().y > 800)
                {
                    if(i == 0)
                    {
                        spriteTree[i].setPosition((rand() % 280), (rand() % -1) - 200);
                    }
                    else
                    {
                        spriteTree[i].setPosition((rand() % 770) + 750, (rand() % -1) - 200);
                    }
                }
            }

            // move the odometer meter
            if(gameSpeed < MAXGAMESPEED && playerCarState == CarState::ACCELERATE)
            {
                dometerNeedle.setRotation(dometerNeedle.getRotation() + (gameSpeed/25 * dt.asSeconds()));
            }else if(gameSpeed > 0 && playerCarState == CarState::DECELERATE)
            {
                dometerNeedle.setRotation(dometerNeedle.getRotation() - (gameSpeed/35.4 * dt.asSeconds()));
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

        window.draw(spritePlayer);

        for(int i=0; i<NUMBEROFROCKS; i++)
        {
            window.draw(spriteRock[i]);
        }

        for(int i=0; i<NUMBEROFTREES; i++)
        {
            window.draw(spriteTree[i]);
        }

        window.draw(spriteSpeedometer);
        window.draw(dometerNeedle);

        window.display();
    }
    return 0;
}
