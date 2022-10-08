#include <SFML/Graphics.hpp>
#include <cmath>

#include "headers/Game.hpp"

#define levelboxColumns 5
#define levelboxSpacing 36

Game::Game(sf::RenderWindow *_window, sf::View *_view)
{
	window = _window;
	view   = _view;

	viewport.width  = viewWidth;
	viewport.height = viewHeight;

	editor     = Editor(window, view, &viewport, &sprites, &text, &mousePosition, &handyCursor, &paused);
	level      = Level(window, view, &viewport, &audio, &sprites, &transition, &paused);
	text       = Text(window);
	transition = Transition(window, view, state);

	state = MainMenu;

	pauseShape.setSize(sf::Vector2f(viewWidth, viewHeight));
	pauseShape.setFillColor(pauseColor);

	rng.seed(std::random_device{}());
}



void Game::resetView()
{
	view->setCenter(viewWidth * 0.5, viewHeight * 0.5);
}

void Game::updateViewport()
{
	viewport.left = view->getCenter().x - viewWidth  * 0.5;
	viewport.top  = view->getCenter().y - viewHeight * 0.5;
}



void Game::processKeyboardInput()
{
	if ((state == LevelEditor || state == LevelPlay) && !transition.transitioning)
	{
		handlePress(pressing(pause), pausePress, pausePressed);
		if (pausePress) { toggle(paused); }
	}
}

void Game::processMouseInput()
{
	mousePosition = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

	handlePress(pressing(sf::Mouse::Left  ),   leftClick,   leftClicked);
	handlePress(pressing(sf::Mouse::Middle), middleClick, middleClicked);
	handlePress(pressing(sf::Mouse::Right ),  rightClick,  rightClicked);
}



void Game::createLevelboxes()
{
	unsigned int currentColumn = 0;
	unsigned int currentRow    = 0;

	unsigned int lastRow = std::floor((float)defaultMaps.size() / levelboxColumns);

	sf::Vector2f position;

	for (unsigned int i = 1; i <= defaultMaps.size(); i++)
	{
		unsigned int columns;

		if (currentRow == lastRow)
		{ columns = defaultMaps.size() - levelboxColumns * lastRow; }
		else
		{ columns = levelboxColumns; }

		if (columns % 2 == 0) // even
		{
			position.x = viewWidth * 0.5 + (currentColumn -            columns / 2 ) * levelboxSpacing + levelboxSpacing / 2;
		}
		else                  // odd
		{
			position.x = viewWidth * 0.5 + (currentColumn - std::floor(columns / 2)) * levelboxSpacing;
		}

		position.y = viewWidth * 0.25 + 48 + levelboxSpacing * currentRow;

		menu.push_back(new Levelbox(level, i, position));

		currentColumn++;

		if (currentColumn == levelboxColumns)
		{
			currentColumn = 0;
			currentRow++;
		}
	}
}

void Game::createMenu()
{
	/*
	the ones commented out do not have any menuboxes
	*/
	switch (state)
	{
		// case SplashScreen:
		// 	break;

		// case ExitScreen:
		// 	break;

		case MainMenu:
			menu.push_back(new Menubox(StoryLevels , "Story"  , sf::Vector2f(60, 20), End   , End  , sf::Vector2f(viewHeight * 0.5 - (30 + 10), viewWidth * 0.75 - 5)));
			menu.push_back(new Menubox(CustomLevels, "Custom" , sf::Vector2f(60, 20), Center, End  , sf::Vector2f(viewHeight * 0.5            , viewWidth * 0.75 - 5)));
			menu.push_back(new Menubox(LevelEditor  , "Editor" , sf::Vector2f(60, 20), Start , End  , sf::Vector2f(viewHeight * 0.5 + (30 + 10), viewWidth * 0.75 - 5)));
			menu.push_back(new Menubox(Options     , "Options", sf::Vector2f(60, 20), End   , Start, sf::Vector2f(viewHeight * 0.5 - 5        , viewWidth * 0.75 + 5)));
			menu.push_back(new Menubox(ExitScreen  , "Exit"   , sf::Vector2f(60, 20), Start , Start, sf::Vector2f(viewHeight * 0.5 + 5        , viewWidth * 0.75 + 5)));
			break;

		case Options:
			menu.push_back(new Menubox(MainMenu, "Back", sf::Vector2f(60, 20), Start, Start, sf::Vector2f(10, 10)));
			break;

		case LevelEditor:
			menu.push_back(new Menubox(MainMenu, "Back", sf::Vector2f(60, 20), Start, Start, sf::Vector2f(10, 10)));
			break;

		case StoryLevels:
			menu.push_back(new Menubox(MainMenu, "Back", sf::Vector2f(60, 20), Start, Start, sf::Vector2f(10, 10)));
			createLevelboxes();
			break;
		
		case CustomLevels:
			menu.push_back(new Menubox(MainMenu, "Back", sf::Vector2f(60, 20), Start, Start, sf::Vector2f(10, 10)));
			break;

		case LevelPlay:
			menu.push_back(new Menubox(StoryLevels, "Back", sf::Vector2f(60, 20), Start, Start, sf::Vector2f(10, 10)));
			break;

		// case LevelClear:
		// 	break;
	}
}

void Game::updateMenu()
{
	if (changedState)
	{
		// avoiding a memory leak by deleting the pointers in the vector
		for (Menubox *menubox : menu)
		{
			delete menubox;
		}
		menu.clear();
	}

	if (menu.empty())
	{
		createMenu();
	}

	// disallowing presses
	if (transition.transitioning || (state == LevelEditor || state == LevelPlay) && !paused) { return; }

	for (Menubox *box : menu)
	{
		box->active = false;
		if (box->bounds.contains(mousePosition))
		{
			if (leftClick)
			{
				audio.click.play();
				box->action();
				transition.to(box->destination);
			}
			else
			{
				box->active = true;
				handyCursor = true;
			}
		}
	}
}

void Game::drawMenu()
{
	if ((state == LevelEditor || state == LevelPlay) && !paused) { return; }
	for (Menubox *box : menu)
	{
		box->draw(window, view, &text);
	}
}



void Game::updateCursor()
{
	if (handyCursor)
	{
		cursor.loadFromSystem(sf::Cursor::Hand);
	}
	else
	{
		cursor.loadFromSystem(sf::Cursor::Arrow);
	}

	window->setMouseCursor(cursor);

	handyCursor = false;
}



void Game::updateFPS()
{
	if (frameUpdateClock.getElapsedTime().asSeconds() >= frameUpdateDelta) {
		frameUpdateClock.restart();
		FPS.str("");
		FPS << "FPS: " << std::floor(1 / (frameClock.getElapsedTime().asSeconds() - displayClock.getElapsedTime().asSeconds()));
	}
	frameClock.restart();
}

void Game::drawFPS()
{
	text.draw(FPS.str(), Start, Start, relativeViewPosition(*view, {0, 0}));
}



/*
moved these to States.cpp
*/
// void Game::updateSplashScreen()
// { ... }
// void Game::updateExitScreen()
// { ... }
// void Game::updateMainMenu()
// { ... }
// void Game::updateOptions()
// { ... }
// void Game::update()
// { ... }
// void Game::updateLevelPlay()
// { ... }
// void Game::updateLevelClear()
// { ... }
// void Game::updateState()
// { ... }

// void Game::drawSplashScreen()
// { ... }
// void Game::drawExitScreen()
// { ... }
// void Game::drawMainMenu()
// { ... }
// void Game::drawOptions()
// { ... }
// void Game::draw()
// { ... }
// void Game::drawLevelPlay()
// { ... }
// void Game::drawLevelClear()
// { ... }
// void Game::drawState()
// { ... }



void Game::update()
{
	updateFPS();
	if (window->hasFocus())
	{
		processKeyboardInput();
		processMouseInput();
	}
	if (transition.transitioning)
	{
		transition.update();
	}
	updateMenu();
	updateState();
	updateCursor();
	updateViewport();

	if (state != lastState)
	{
		changedState = true;
		if (state != LevelEditor && state != LevelPlay)
		{
			resetView();
		}
	}
	else
	{
		changedState = false;
	}

	if (changedState)
	{
		paused       = false;
		pausePress   = false;
		pausePressed = false;

		if (lastState == LevelEditor)
		{
			sprites.resetScale();
		}
	}

	lastState = state;
}

void Game::draw()
{
	drawState();
	if (paused)
	{
		pauseShape.setPosition(relativeViewPosition(*view, {0, 0}));
		window->draw(pauseShape);
		text.draw("Paused", Center, Center, relativeViewPosition(*view, {viewWidth * 0.5, viewHeight * 0.25}), {2, 2});
	}
	drawMenu();
	if (transition.transitioning)
	{
		transition.draw();
	}
	drawFPS();
	displayClock.restart();
}
