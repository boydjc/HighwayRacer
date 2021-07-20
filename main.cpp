#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    sf::VideoMode vm(1086, 679);

    sf::RenderWindow window(vm, "Highway Racer", sf::Style::Titlebar);

    // Load textures here
    sf::Texture textureRoad;
    textureRoad.loadFromFile("assets/imgs/background.png");

    sf::Texture textureRoadStripe;
    textureRoadStripe.loadFromFile("assets/imgs/roadStripe.png");

    sf::Texture textureRock;
    textureRock.loadFromFile("assets/imgs/rock.png");

    sf::Texture textureCars[16];
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

    sf::Texture textureTree;
    textureTree.loadFromFile("assets/imgs/tree.png");

    sf::Texture textureSpeedometer;
    textureSpeedometer.loadFromFile("assets/imgs/speedometer.png");

    // set the road up
    sf::Sprite spriteRoad;
    spriteRoad.setTexture(textureRoad);

    sf::Sprite spriteRoadStripe[5];
    for(int i=0; i<5; i++)
    {
        spriteRoadStripe[i].setTexture(textureRoadStripe);
        spriteRoadStripe[i].setPosition(525, i*150);
    }

    // environment details
    //rocks
    const int NUMBEROFROCKS = 2;
    sf::Sprite spriteRock[NUMBEROFROCKS];

    srand(int(time(0)));

    for(int i=0; i<NUMBEROFROCKS; i++)
    {
        spriteRock[i].setTexture(textureRock);
        // spawn the environment details in random locations

        // this makes sure that all the rocks spawn on different sides of the road
        if(i == 0)
        {
            spriteRock[i].setPosition((rand() % 280), (rand() % 900) - 1500);
        }else
        {
            spriteRock[i].setPosition((rand() % 770) + 710, (rand() % 900) - 900);
        }
    }

    // trees
    const int NUMBEROFTREES = 20;
    sf::Sprite spriteTree[NUMBEROFTREES];

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
    sf::Sprite spritePlayer;
    spritePlayer.setTexture(textureCars[0]);
    spritePlayer.setPosition(575, 500);
    CarState playerCarState = CarState::STOPPED;

    // set up the other cars that will be in the way
    // there will only be a few sprites that will just change textures randomly
    const int NUMBEROFNPCS = 2;
    sf::Sprite spriteNpc[NUMBEROFNPCS];

    for(int i=0; i<NUMBEROFNPCS; i++)
    {
        int textureChoice = rand() % 15;
        spriteNpc[i].setTexture(textureCars[textureChoice]);
        if(i == 0)
        {
            spriteNpc[i].setPosition(400, (rand() % 1) - 2000);
        }
        else
        {
            spriteNpc[i].setPosition(580, (rand() % 1) - 4000);
        }
    }

    // set up the HUD
    sf::Sprite spriteSpeedometer;
    spriteSpeedometer.setTexture(textureSpeedometer);
    spriteSpeedometer.setPosition(840, 520);

    // a triangle that will represent the speedometer needle
    sf::ConvexShape dometerNeedle(3);
    dometerNeedle.setPoint(0, sf::Vector2f(0, 10));
    dometerNeedle.setPoint(1, sf::Vector2f(5, 80));
    dometerNeedle.setPoint(2, sf::Vector2f(-5, 80));

    dometerNeedle.setPosition(915, 593);
    dometerNeedle.setFillColor(sf::Color::Red);
    sf::FloatRect dometerBounds = dometerNeedle.getLocalBounds();
    dometerNeedle.setOrigin(dometerBounds.left + dometerBounds.width - 5, dometerBounds.top + dometerBounds.height - 5);
    dometerNeedle.setRotation(225);

    bool gameRunning = false;

    /* in the game, everything around the car moves instead of the car moving
    *  gameSpeed is the speed at which everything moves which can be considered
    * also to be how fast the car will appear to be moving to the player */
    const float MAXGAMESPEED = 1800.0f;
    float gameSpeed = 0.0f;

    sf::Font font;
    font.loadFromFile("assets/fonts/Bungee-Regular.ttf");

    sf::Text score;
    score.setFont(font);
    score.setCharacterSize(30);
    score.setFillColor(sf::Color::White);
    score.setPosition(20, 20);

    sf::Clock clock;

    while(window.isOpen())
    {
        /*****************************
        *   Handle the player's input
        ******************************/

	sf::Event event;

	while(window.pollEvent(event))
	{
	    if(event.type == sf::Event::Closed)
	    {
		window.close();
	    }else if(event.type == sf::Event::KeyPressed)
	    {
		if(event.key.code == sf::Keyboard::Enter)
		{
		    if(!gameRunning)
		    {
			gameRunning = true;
			std::cout << "Game Running" << std::endl;
		    }else
		    {
			gameRunning = false;
			std::cout << "Game Paused" << std::endl;
		    }
		}
	    }
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
	    window.close();
	}
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && gameRunning)
        {

            playerCarState = CarState::ACCELERATE;

            if(gameSpeed < MAXGAMESPEED)
            {
               gameSpeed += 0.55;
            }
        }else
        {
	    if(gameRunning)
	    {
                playerCarState = CarState::DECELERATE;

                if(gameSpeed > 0.0)
                {
                    gameSpeed -= 0.38;
                }
	    }
        }

        // control player car
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && gameRunning)
        {
            // make sure that the car can't leave the road
            if(spritePlayer.getPosition().x > 370)
            {
                spritePlayer.setPosition(spritePlayer.getPosition().x - .8, spritePlayer.getPosition().y);
            }
        }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && gameRunning)
        {
            if(spritePlayer.getPosition().x < 633)
            {
                spritePlayer.setPosition(spritePlayer.getPosition().x + .8, spritePlayer.getPosition().y);
            }
        }



        /*****************************
        *   Update the scene
        ******************************/

	sf::Time dt = clock.restart();

        if(gameRunning)
        {

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

            // move the npc cars
	    for(int i=0; i<NUMBEROFNPCS; i++)
	    {
		spriteNpc[i].setPosition(spriteNpc[i].getPosition().x, (spriteNpc[i].getPosition().y - (5 * dt.asSeconds())));

		// if the we pass the npc, move the npc back to the front of the road
		if(spriteNpc[i].getPosition().y > 800)
		{
		    if(i == 0){
			spriteNpc[i].setPosition(400, (rand() % 1) - 2000);
		    }else{
			spriteNpc[i].setPosition(580, (rand() % 1) - 4000);
		    }

		    // set a new texture for the sprite
		    int textureChoice = rand() % 15;
		    spriteNpc[i].setTexture(textureCars[textureChoice]);
		}
	    }


	    if(playerCarState == CarState::ACCELERATE)
	    {
	        // move the odometer meter
	        if(gameSpeed < MAXGAMESPEED)
	        {
		    dometerNeedle.setRotation(dometerNeedle.getRotation() + (gameSpeed/26 * dt.asSeconds()));
	        }
	    }
	    else if(playerCarState == CarState::DECELERATE)
	    {
		// move the odometer the opposite way 
		if(gameSpeed > 0.0)
		{
		    dometerNeedle.setRotation(dometerNeedle.getRotation() - (gameSpeed/37.5 * dt.asSeconds()));
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

        for(int i=0; i<NUMBEROFNPCS; i++)
        {
            window.draw(spriteNpc[i]);
        }

	window.draw(score);

        window.display();
    }
    return 0;
}
