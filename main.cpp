#include <SFML/graphics.hpp>

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

    Sprite spriteRoad;
    spriteRoad.setTexture(textureRoad);

    Sprite spriteRoadStripe[5];
    for(int i=0; i<5; i++)
    {
        spriteRoadStripe[i].setTexture(textureRoadStripe);
        spriteRoadStripe[i].setPosition(525, i*150);
    }

    bool gameRunning = false;

    /* in the game, everything around the car moves instead of the car moving
    *  gameSpeed is the speed at which everything moves which can be considered
    * also to be how fast the car will appear to be moving to the player */
    float gameSpeed = 520.0f;

    Clock clock;

    while(window.isOpen())
    {

        /*****************************
        *   Handle the player's input
        ***************************/
        if(Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }else if(Keyboard::isKeyPressed(Keyboard::Enter))
        {
            gameRunning = true;
        }



        /*****************************
        *   Update the scene
        ******************************/

        if(gameRunning)
        {
            // move everything in relation to the game speed
            // road stripes

            Time dt = clock.restart();

            for(int i=0; i<5; i++)
            {
                spriteRoadStripe[i].setPosition(spriteRoadStripe[i].getPosition().x, (spriteRoadStripe[i].getPosition().y + (gameSpeed * dt.asSeconds())));

                /* when the stripe gets off screen, put it back at the top to give the illusion of
                * endless road */
                if(spriteRoadStripe[i].getPosition().y > 679)
                {
                    spriteRoadStripe[i].setPosition(spriteRoadStripe[i].getPosition().x, -70);
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

        window.display();
    }
    return 0;
}
