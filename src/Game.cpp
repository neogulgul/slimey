#include <cmath>
#include <cstring>
#include <set>

#include "headers/Game.hpp"

#define levelboxColumns 5

#define customLevelsScrollDelta 30

#define backMenubox MainMenu, "Back", {60, 20}, Start, Start, {10, 10}

Game::Game(sf::RenderWindow* _window, sf::View* _view)
{
	window = _window;
	view   = _view;

	state = SplashScreen;

	viewport.width  = viewWidth;
	viewport.height = viewHeight;

	editor     = Editor(window, view, &viewport, &audio, &level, &sprites, &text,
	                    &mousePosition, &handyCursor, &leftClick,
	                    &pressingControl, &pressingShift, &pressingAlt, &paused, &transition.transitioning);
	level      = Level(window, view, &viewport, &audio, &sprites, &text, &transition, &paused, &options.debug);
	text       = Text(window);
	transition = Transition(window, view, state);

	audio.updateMusicVolume(options.music, options.volumeMusic);
	audio.updateSFXVolume(options.SFX, options.volumeSFX);

	pauseRect.setSize({viewWidth, viewHeight});
	pauseRect.setFillColor(pauseColor);

	rng.seed(std::random_device{}());

	// buttons
	bookButton = Button(&sprites.book, End, End, {viewWidth - 5, viewHeight - 5});
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

	pressingControl = false;
	pressingShift   = false;
	pressingAlt     = false;
	if (pressing(sf::Keyboard::LControl) || pressing(sf::Keyboard::RControl))
	{
		pressingControl = true;
	}
	if (pressing(sf::Keyboard::LShift) || pressing(sf::Keyboard::RShift))
	{
		pressingShift = true;
	}
	if (pressing(sf::Keyboard::LAlt) || pressing(sf::Keyboard::RAlt))
	{
		pressingAlt = true;
	}
}

void Game::processMouseInput()
{
	mousePosition = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

	handlePress(pressing(sf::Mouse::Left  ),   leftClick,   leftClicked);
	handlePress(pressing(sf::Mouse::Middle), middleClick, middleClicked);
	handlePress(pressing(sf::Mouse::Right ),  rightClick,  rightClicked);
}



void Game::handleCustomLevelsScroll(sf::Event event)
{
	sf::Vector2f viewPosition = view->getCenter();

	if (event.mouseWheel.delta == MouseWheel::Forward)
	{
		viewPosition.y -= customLevelsScrollDelta;
	}
	else if (event.mouseWheel.delta == MouseWheel::Backward)
	{
		viewPosition.y += customLevelsScrollDelta;
	}

	view->setCenter(viewPosition);

	limitCustomLevelsScroll();
}

void Game::limitCustomLevelsScroll()
{
	sf::Vector2f viewPosition = view->getCenter();

	if (viewPosition.y < viewHeight * 0.5)
	{
		viewPosition.y = viewHeight * 0.5;
	}
	if (lastCustomLevelVerticalPosition + 24 > viewHeight)
	{
		if (viewPosition.y > lastCustomLevelVerticalPosition + 24 - viewHeight * 0.5)
		{
			viewPosition.y = lastCustomLevelVerticalPosition + 24 - viewHeight * 0.5;
		}
	}
	else
	{
		if (viewPosition.y > viewHeight * 0.5)
		{
			viewPosition.y = viewHeight * 0.5;
		}
	}

	view->setCenter(viewPosition);
}



void Game::createStoryLevelboxes()
{
	unsigned int currentColumn = 0;
	unsigned int currentRow    = 0;

	unsigned int lastRow = std::floor((float)storyLevels.size() / levelboxColumns);

	sf::Vector2f position;

	for (unsigned int i = 1; i <= storyLevels.size(); i++)
	{
		unsigned int columns;

		if (currentRow == lastRow)
		{ columns = storyLevels.size() - levelboxColumns * lastRow; }
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

		position.y = viewHeight * 0.25 + 48 + levelboxSpacing * currentRow;

		menu.push_back(new Levelbox(&level, i, position));

		currentColumn++;

		if (currentColumn == levelboxColumns)
		{
			currentColumn = 0;
			currentRow++;
		}
	}
}

void replaceSubstringInString(std::string &string, std::string substring)
{
	string.replace(string.find(substring), substring.size(), "");
}

void Game::createCustomLevelboxes()
{
	lastCustomLevelVerticalPosition = 0;
	customLevelsCount = 0;
	if (!fs::is_directory("custom_levels")) { return; }

	std::set<fs::path> levels_sorted_by_name;

	for (fs::directory_entry entry : fs::directory_iterator("custom_levels"))
	{
		levels_sorted_by_name.insert(entry.path());
	}

	for (fs::path path : levels_sorted_by_name)
	{
		std::stringstream conversionStream;
		conversionStream << path;
		std::string levelName = conversionStream.str();

		// if the file is not a .txt we skip it
		if (!strstr(levelName.c_str(), ".txt")) { continue; }

		// this removes a two \ from the name, which appears on windows
		if (strstr(levelName.c_str(), "\\"))
		{
			replaceSubstringInString(levelName, "\\");
			replaceSubstringInString(levelName, "\\");
		}
		// this removes a / from the name
		else
		{
			replaceSubstringInString(levelName, "/");
		}
		replaceSubstringInString(levelName, "custom_levels");
		replaceSubstringInString(levelName, ".txt");
		replaceSubstringInString(levelName, "\""); // we do this two times, since there
		replaceSubstringInString(levelName, "\""); // is one quotation mark on each side

		float verticalPosition = viewHeight * 0.25 + 48 + levelboxSpacing * customLevelsCount;

		customLevelboxes.push_back(
			Levelbox(&level, levelName, {viewWidth * 0.5, verticalPosition})
		);
		customLevelRemoveButtons.push_back(
			Button(&sprites.xMark, Start, Center, {viewWidth * 0.5 + 114 / 2 + 10, verticalPosition}, false)
		);

		lastCustomLevelVerticalPosition = verticalPosition;

		customLevelsCount++;
	}
}

void Game::createMenu()
{
	switch (state)
	{
		case SplashScreen:
			break;

		case ExitScreen:
			break;

		case MainMenu:
			menu.push_back(new Menubox(StoryLevels  , "Story"  , {60, 20}, End   , End  , {viewHeight * 0.5 - (30 + 10), viewWidth * 0.75 - 5}));
			menu.push_back(new Menubox(CustomLevels , "Custom" , {60, 20}, Center, End  , {viewHeight * 0.5            , viewWidth * 0.75 - 5}));
			menu.push_back(new Menubox(LevelEditor  , "Editor" , {60, 20}, Start , End  , {viewHeight * 0.5 + (30 + 10), viewWidth * 0.75 - 5}));
			menu.push_back(new Menubox(OptionsScreen, "Options", {60, 20}, End   , Start, {viewHeight * 0.5 - 5        , viewWidth * 0.75 + 5}));
			menu.push_back(new Menubox(ExitScreen   , "Exit"   , {60, 20}, Start , Start, {viewHeight * 0.5 + 5        , viewWidth * 0.75 + 5}));
			break;

		case OptionsScreen:
			menu.push_back(new Menubox(backMenubox));
			break;

		case LevelEditor:
			menu.push_back(new Menubox(backMenubox));
			break;

		case StoryLevels:
			menu.push_back(new Menubox(backMenubox));
			createStoryLevelboxes();
			break;
		
		case CustomLevels:
			menu.push_back(new Menubox(backMenubox));
			createCustomLevelboxes();
			break;

		case LevelPlay:
			if (level.destination == LevelClear)
			{
				menu.push_back(new Menubox(StoryLevels, "Back", {60, 20}, Start, Start, {10, 10}));
			}
			else
			{
				menu.push_back(new Menubox(level.destination, "Back", {60, 20}, Start, Start, {10, 10}));
			}
			break;

		case LevelClear:
			menu.push_back(new Menubox(LevelPlay  , "Continue", {80, 20}, Center, Center, {viewWidth / 2, viewHeight / 2     }));
			menu.push_back(new Menubox(LevelPlay  , "Restart" , {80, 20}, Center, Center, {viewWidth / 2, viewHeight / 2 + 30}));
			menu.push_back(new Menubox(StoryLevels, "Go back" , {80, 20}, Center, Center, {viewWidth / 2, viewHeight / 2 + 60}));
			break;
	}
}

void Game::updateMenu()
{
	if (state == MainMenu && bookIsOpen) { return; }

	if (changedState)
	{
		// avoiding a memory leak by deleting the pointers in this vector
		for (Menubox* menubox : menu)
		{
			delete menubox;
		}
		menu.clear();

		if (lastState == CustomLevels)
		{
			customLevelboxes.clear();
			customLevelRemoveButtons.clear();
		}
	}

	if (menu.empty())
	{
		createMenu();
	}

	// disallowing presses
	if (transition.transitioning || (state == LevelEditor || state == LevelPlay) && !paused) { return; }

	for (Menubox* box : menu)
	{
		box->update(mousePosition);
		if (box->active)
		{
			handyCursor = true;
			if (leftClick)
			{
				audio.click.play();
				box->action();
				transition.to(box->destination);
			}
		}
	}
}

void Game::drawMenu()
{
	if ((state == LevelEditor || state == LevelPlay) && !paused) { return; }
	for (Menubox* box : menu)
	{
		box->draw(window, view, &text);
	}
}



void Game::drawBookIsOpen()
{
	pauseRect.setPosition(relativeViewPosition(view, {0, 0}));
	window->draw(pauseRect);

	text.draw("Level", Center, Start, {viewWidth / 2, 10}, {1.5, 1.5});
	text.draw("Move - WASD or Arrow Keys", Start, Start, {10, 25}, {0.8, 0.8});
	text.draw("Jump - J or Space"        , Start, Start, {10, 35}, {0.8, 0.8});
	text.draw("Reset - R"                , Start, Start, {10, 45}, {0.8, 0.8});
	text.draw("Pause - Escape"           , Start, Start, {10, 55}, {0.8, 0.8});

	text.draw("Editor", Center, Start, {viewWidth / 2, 70}, {1.5, 1.5});
	text.draw("Place / Erase - Left Click"                      , Start, Start, {10,  85}, {0.8, 0.8});
	text.draw("Move View - (Middle Click or Shift) + Mouse Move", Start, Start, {10,  95}, {0.8, 0.8});
	text.draw("Zoom View - Scroll Wheel"                        , Start, Start, {10, 105}, {0.8, 0.8});
	text.draw("Eyedropper - Alt + Left Click"                   , Start, Start, {10, 115}, {0.8, 0.8});
	text.draw("Equip Brush - B"                                 , Start, Start, {10, 125}, {0.8, 0.8});
	text.draw("Equip Fill - F"                                  , Start, Start, {10, 135}, {0.8, 0.8});
	text.draw("Toggle Erase - E"                                , Start, Start, {10, 145}, {0.8, 0.8});
	text.draw("Toggle Crosshair - C"                            , Start, Start, {10, 155}, {0.8, 0.8});
	text.draw("Reset View - R"                                  , Start, Start, {10, 165}, {0.8, 0.8});
	text.draw("Clear Level - Control + Shift + R"                 , Start, Start, {10, 175}, {0.8, 0.8});
	text.draw("Save Level - Control + S or Hit the save button"   , Start, Start, {10, 185}, {0.8, 0.8});
	text.draw("Load Level - Control + L or Hit the load button"   , Start, Start, {10, 195}, {0.8, 0.8});
	text.draw("Pause - Escape"                                  , Start, Start, {10, 205}, {0.8, 0.8});
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
		FPS = std::floor(1 / (frameClock.getElapsedTime().asSeconds() - displayClock.getElapsedTime().asSeconds()));
	}
	frameClock.restart();
}

void Game::drawFPS()
{
	text.draw("FPS:" + std::to_string(FPS), Start, Start, relativeViewPosition(view, {1, 1}));
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
// void Game::updateOptionsScreen()
// { ... }
// void Game::updateLevelEditor()
// { ... }
// void Game::updateStoryLevels()
// { ... }
// void Game::updateCustomLevels()
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
// void Game::drawOptionsScreen()
// { ... }
// void Game::drawLevelEditor()
// { ... }
// void Game::drawStoryLevels()
// { ... }
// void Game::drawCustomLevels()
// { ... }
// void Game::drawLevelPlay()
// { ... }
// void Game::drawLevelClear()
// { ... }
// void Game::drawState()
// { ... }



void Game::update()
{
	bool playedMusicLastFrame  = options.music;
	bool playedSFXLastFrame    = options.SFX;
	float volumeMusicLastFrame = options.volumeMusic;
	float volumeSFXLastFrame   = options.volumeSFX;

	updateFPS();
	if (transition.transitioning)
	{
		transition.update();
		if (transition.destination == ExitScreen)
		{
			static const float originalVolume = audio.titleTrack.getVolume();
			audio.titleTrack.setVolume(originalVolume * (1 - transition.opacity));
		}
	}
	if (window->hasFocus())
	{
		processKeyboardInput();
		processMouseInput();
	}
	updateMenu();
	updateState();
	updateCursor();
	updateViewport();

	if (playedMusicLastFrame != options.music || volumeMusicLastFrame != options.volumeMusic)
	{
		audio.updateMusicVolume(options.music, options.volumeMusic);
	}
	if (playedSFXLastFrame != options.SFX || volumeSFXLastFrame != options.volumeSFX)
	{
		audio.updateSFXVolume(options.SFX, options.volumeSFX);
	}

	if (state != lastState)
	{
		changedState = true;
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

		if (state == LevelEditor)
		{
			editor.updateSizeInputs();
		}

		if (state != LevelEditor && state != LevelPlay)
		{
			resetView();
		}

		switch (lastState)
		{
			case SplashScreen:
				audio.titleTrack.play();
				break;

			case LevelEditor:
				editor.playButton->active = false;
				sprites.resetScale();
				break;
		}
	}

	lastState = state;
}

void Game::draw()
{
	drawState();
	if (paused)
	{
		pauseRect.setPosition(relativeViewPosition(view, {0, 0}));
		window->draw(pauseRect);
		text.draw("Paused", Center, Center, relativeViewPosition(view, {viewWidth * 0.5, viewHeight * 0.25}), {2, 2});
	}
	drawMenu();
	if (state == MainMenu && bookIsOpen)
	{
		drawBookIsOpen();
	}
	if (transition.transitioning)
	{
		transition.draw();
	}
	if (options.FPS)
	{
		drawFPS();
	}
	displayClock.restart();
}
