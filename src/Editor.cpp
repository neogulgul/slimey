#include <algorithm>
#include <cmath>
#include <fstream>

#include "headers/Editor.hpp"

#define levelOutlineThickness 4
#define selectionTilesetOutlineThickness 2

#define backspace 8

Input::Input() {}

Input::Input(sf::Vector2f _position, sf::Vector2f _size, unsigned int _maxLength, bool _numbersOnly)
{
	position = _position;
	size     = _size;
	shape.setSize(size);
	shape.setOutlineThickness(-1);

	maxLength      = _maxLength;
	numbersOnly = _numbersOnly;
}

int Input::getValue()
{
	return std::atoi(value.str().c_str());
}

std::string Input::getString()
{
	return value.str();
}

Editor::Editor() {}

Editor::Editor(sf::RenderWindow* _window, sf::View* _view, sf::FloatRect* _viewport, Audio* _audio, Level* _level, Sprites* _sprites, Text* _text,
               sf::Vector2f* _mousePosition, bool* _handyCursor, bool* _leftClick,
               bool* _pressingControl, bool* _pressingShift, bool* _pressingAlt, bool* _paused, bool* _transitioning)
{
	window          = _window;
	view            = _view;
	viewport        = _viewport;

	audio           = _audio;
	level           = _level;
	sprites         = _sprites;
	text            = _text;

	mousePosition   = _mousePosition;
	handyCursor     = _handyCursor;
	leftClick       = _leftClick;
	pressingControl = _pressingControl;
	pressingShift   = _pressingShift;
	pressingAlt     = _pressingAlt;
	paused          = _paused;
	transitioning   = _transitioning;
	
	declareRegions();

	levelSize = {initialLevelWidth, initialLevelHeight};
	clearLevel();

	sizeInputs = {
		new Input({viewWidth * 0.5 - 48, viewHeight - 24}, {96, 12}, 15),
		new Input({16, viewHeight - 32}, {24 ,12}, 3, true),
		new Input({16, viewHeight - 16}, {24 ,12}, 3, true)
	};
	levelNameInput   = sizeInputs.at(0);
	levelWidthInput  = sizeInputs.at(1);
	levelHeightInput = sizeInputs.at(2);
	levelWidthInput ->value << levelSize.x;
	levelHeightInput->value << levelSize.y;

	buttons = {
		Button("Play", {35, 20}, End, End, {viewWidth - 5, viewHeight - 55}),
		Button("Save", {35, 20}, End, End, {viewWidth - 5, viewHeight - 30}),
		Button("Load", {35, 20}, End, End, {viewWidth - 5, viewHeight - 5})
	};
	playButton = &buttons.at(0);
	saveButton = &buttons.at(1);
	loadButton = &buttons.at(2);

	tileDimension = {tilesize, tilesize};

	levelRect.setPosition(viewWidth * 0.5, viewHeight * 0.5);
	levelRect.setFillColor(sf::Color::Transparent);
	levelRect.setOutlineThickness(levelOutlineThickness);
	levelRect.setOutlineColor(levelOutlineColor);
	updateLevelRect();

	levelChecker.setSize(tileDimension);
	levelChecker.setFillColor(levelOutlineColor);
	levelRestrictedArea.setSize(tileDimension);
	levelRestrictedArea.setFillColor(levelRestrictedColor);
	levelGhostTile.setSize(tileDimension);
	levelCrosshair.setFillColor(levelCrosshairColor);

	selectionTilesetRect.setSize((sf::Vector2f)sprites->tilesetOtherTexture.getSize());
	selectionTilesetRect.setFillColor(selectionTilesetBackgroundColor);
	selectionTilesetRect.setOutlineColor(selectionTilesetOutlineColor);
	selectionTilesetRect.setOutlineThickness(selectionTilesetOutlineThickness);
	selectionTilesetSelection.setSize(tileDimension);
	selectionTilesetSelection.setFillColor(sf::Color::Transparent);
	selectionTilesetSelection.setOutlineColor(selectionTilesetSelectionOutlineColor);
	selectionTilesetSelection.setOutlineThickness(-selectionTilesetOutlineThickness);

	sawbladeAnimation = Animation(sawbladeFrameCount, sawbladeFrameDuration);
}



// this is in DetermineTile.cpp
// void Editor::declareRegions()
// {
// 	...
// }



void Editor::handleZoom(sf::Event event)
{
	if (*paused) { return; }

	if (event.mouseWheel.delta == MouseWheel::Forward)
	{
		zoom += zoomStep;
	}
	else if (event.mouseWheel.delta == MouseWheel::Backward)
	{
		zoom -= zoomStep;
	}
	zoom = std::clamp(zoom, minZoom, maxZoom);
	adjustZoom();
}

void Editor::adjustZoom()
{
	levelRect.setScale(zoom, zoom);
	levelChecker.setScale(zoom, zoom);
	levelGhostTile.setScale(zoom, zoom);
	levelCrosshair.setScale(zoom, zoom);
	updateLevelBounds();
}

void Editor::resetView()
{
	view->setCenter(viewWidth * 0.5, viewHeight * 0.5);
	zoom = 1.0;
	adjustZoom();
}



void Editor::processKeyboardInput()
{
	// brush
	handlePress(pressing(sf::Keyboard::B), brushPress, brushPressed);
	// fill
	handlePress(pressing(sf::Keyboard::F), fillPress, fillPressed);
	// erase
	handlePress(pressing(sf::Keyboard::E), erasePress, erasePressed);
	// crosshair
	handlePress(pressing(sf::Keyboard::C), crosshairPress, crosshairPressed);
	// reset view
	handlePress(pressing(sf::Keyboard::R), resetViewPress, resetViewPressed);
	// clear
	handlePress(*pressingControl && *pressingShift && pressing(sf::Keyboard::R), clearLevelPress, clearLevelPressed);
	// save
	handlePress(*pressingControl && pressing(sf::Keyboard::S), saveLevelPress, saveLevelPressed);
	// load
	handlePress(*pressingControl && pressing(sf::Keyboard::L), loadLevelPress, loadLevelPressed);

	handleKeyboardInput();
}

void Editor::handleKeyboardInput()
{
	if (!inputSelected)
	{
		if (brushPress)
		{
			method = Brush;
		}
		else if (fillPress)
		{
			method = Fill;
		}

		if (erasePress)
		{
			toggle(erase);
		}

		if (crosshairPress)
		{
			toggle(crosshair);
		}

		if (resetViewPress && !clearLevelPress)
		{
			resetView();
		}
	}

	if (saveLevelPress)
	{
		saveLevel();
	}
	else if (loadLevelPress)
	{
		loadLevel();
	}
	else if (clearLevelPress)
	{
		clearLevel();
	}
}

void Editor::processMouseInput()
{
	mouseOnLevel              = false;
	mouseOnSelectionTileset = false;

	if (getSelectionTilesetMouseBounds().contains(*mousePosition))
	{
		mouseOnSelectionTileset = true;
	}
	else if (getLevelMouseBounds().contains(*mousePosition))
	{
		mouseOnLevel = true;

		mouseLevelCoord.x = std::floor((mousePosition->x - levelBounds.left - levelOutlineThickness * zoom) / (tilesize * zoom));
		mouseLevelCoord.y = std::floor((mousePosition->y - levelBounds.top  - levelOutlineThickness * zoom) / (tilesize * zoom));
	}

	handleDragging();
}

void Editor::handleDragging()
{
	if (pressing(sf::Mouse::Middle) || *pressingShift)
	{
		if (!dragging)
		{
			dragging = true;
			draggingStartPosition = *mousePosition;
		}
		sf::Vector2f dragDelta = *mousePosition - draggingStartPosition;
		view->setCenter(view->getCenter() - dragDelta);
	}
	else
	{
		dragging = false;
	}
}



void Editor::updateLevelRect()
{
	levelRect.setSize({(float)levelSize.x * tilesize, (float)levelSize.y * tilesize});
	levelRect.setOrigin(levelRect.getSize().x * 0.5, levelRect.getSize().y * 0.5);
	updateLevelBounds();
}

void Editor::updateLevelBounds()
{
	levelBounds = levelRect.getGlobalBounds();
}

void Editor::updateSelectionTilesetBounds()
{
	selectionTilesetBounds = selectionTilesetRect.getGlobalBounds();
}

sf::Vector2f Editor::relativeLevelPosition(float x, float y)
{
	return {levelBounds.left + (levelOutlineThickness + x) * zoom, levelBounds.top + (levelOutlineThickness + y) * zoom};
}

sf::Vector2f Editor::relativeSelectionTilesetPosition(float x, float y)
{
	return {selectionTilesetBounds.left + selectionTilesetOutlineThickness + x, selectionTilesetBounds.top + selectionTilesetOutlineThickness + y};
}

sf::FloatRect Editor::getSelectionTilesetMouseBounds()
{
	return sf::FloatRect(
		{selectionTilesetBounds.left  + selectionTilesetOutlineThickness    , selectionTilesetBounds.top    + selectionTilesetOutlineThickness    },
		{selectionTilesetBounds.width - selectionTilesetOutlineThickness * 2, selectionTilesetBounds.height - selectionTilesetOutlineThickness * 2}
	);
}

sf::FloatRect Editor::getLevelMouseBounds()
{
	return sf::FloatRect(
		{levelBounds.left  + levelOutlineThickness * zoom    , levelBounds.top    + levelOutlineThickness * zoom    },
		{levelBounds.width - levelOutlineThickness * zoom * 2, levelBounds.height - levelOutlineThickness * zoom * 2}
	);
}



void Editor::clearLevel()
{
	levelVector.clear();
	for (unsigned int x = 0; x < levelSize.x; x++)
	{
		levelVector.push_back({});
		for (unsigned int y = 0; y < levelSize.y; y++)
		{
			levelVector.at(x).push_back(emptyTile);
		}
	}
}

void Editor::playLevel()
{
	level->loadLevel(levelVector, LevelEditor);
	level->transition->to(LevelPlay);
}

void Editor::saveLevel()
{
	std::stringstream levelStream;

	levelStream << "{\n\t{ ";
	for (unsigned int x = 0; x < levelSize.x; x++)
	{
		for (unsigned int y = 0; y < levelSize.y; y++)
		{
			sf::Vector3i tile = getTile(x, y);
			levelStream << "{" << tile.x << "," << tile.y << "," << tile.z << "}";

			if (y == levelSize.y - 1)
			{
				levelStream << " },\n";
				if (x < levelSize.x - 1)
				{
					levelStream << "\t{ ";
				}
			}
			else
			{
				levelStream << ", ";
			}
		}
	}
	levelStream << "};";

	if (!fs::exists("custom_levels"))
	{
		fs::create_directory("custom_levels");
	}

	std::ofstream output("custom_levels/" + levelNameInput->getString() + ".txt");
	output << levelStream.str();
	output.close();
}

void Editor::loadLevel()
{
	if (fs::exists("custom_levels/" + levelNameInput->getString() + ".txt"))
	{
		levelVector = getCustomLevelVector(levelNameInput->getString());

		levelSize.x = levelVector.size();
		levelSize.y = levelVector.at(0).size();

		levelWidthInput ->value.str("");
		levelHeightInput->value.str("");
		levelWidthInput ->value << levelSize.x;
		levelHeightInput->value << levelSize.y;

		for (unsigned int x = 0; x < levelSize.x; x++)
		{
			for (unsigned int y = 0; y < levelSize.y; y++)
			{
				if (getTile(x, y) == spawnTile)
				{
					spawnPosition.x = x;
					spawnPosition.y = y;
				}
				else if (getTile(x, y) == exitTile)
				{
					exitPosition.x = x;
					exitPosition.y = y;
				}
			}
		}
	}

	updateLevelRect();
}

void Editor::changeLevelSize(unsigned int newWidth, unsigned int newHeight)
{
	unsigned int oldWidth  = levelSize.x;
	unsigned int oldHeight = levelSize.y;

	levelSize.x = newWidth;
	levelSize.y = newHeight;
	updateLevelRect();

	// reseting spawn and exit positions if any of them are out of bounds
	if (spawnPosition.x > newWidth || spawnPosition.y > newHeight)
	{ spawnPosition = sf::Vector2u(); }
	if (exitPosition.x > newWidth || exitPosition.y > newHeight)
	{ exitPosition = sf::Vector2u(); }

	// creating new level
	std::vector<std::vector<sf::Vector3i>> newLevel;
	for (unsigned int x = 0; x < newWidth; x++)
	{
		newLevel.push_back({});
		for (unsigned int y = 0; y < newHeight; y++)
		{
			newLevel.at(x).push_back(emptyTile);
		}
	}

	// populating new level
	for (unsigned int x = 0; x < oldWidth; x++)
	{
		for (unsigned int y = 0; y < oldHeight; y++)
		{
			if (x < newWidth && y < newHeight)
			{
				newLevel.at(x).at(y) = levelVector.at(x).at(y);
			}
		}
	}

	levelVector = newLevel;

	// adjusting tiles that has been or is adjacent around the bottom and right sides
	if (newHeight > oldHeight)
	{
		for (unsigned int x = 0; x < newWidth; x++) {
			adjustTile(x, oldHeight - 1);
		}
	}
	else if (newHeight < oldHeight)
	{
		for (unsigned int x = 0; x < newWidth; x++) {
			adjustTile(x, newHeight - 1);
		}
	}
	if (newWidth > oldWidth)
	{
		for (unsigned int y = 0; y < newHeight; y++) {
			adjustTile(oldWidth - 1, y);
		}
	}
	else if (newWidth < oldWidth)
	{
		for (unsigned int y = 0; y < newHeight; y++) {
			adjustTile(newWidth - 1, y);
		}
	}
}



// this function got its own separate file because it got so big
// sf::Vector3i Editor::determineTile(sf::Vector2i levelCoord, int tileset)
// {
// 	...
// }

sf::Vector3i Editor::getTile(unsigned int x, unsigned int y)
{
	return levelVector.at(x).at(y);
}

sf::Vector2u Editor::getTileCrop(unsigned int x, unsigned int y)
{
	return sf::Vector2u(getTile(x, y).y, getTile(x, y).z);
}

int Editor::getTileset(unsigned int x, unsigned int y)
{
	return getTile(x, y).x;
}

int Editor::getSelectedTileset()
{
	if (erase)
	{
		return Tileset::Null;
	}
	else if (selectionCoord == sf::Vector2u(0, 0))
	{
		return Tileset::Normal;
	}
	else if (selectionCoord == sf::Vector2u(0, 1))
	{
		return Tileset::Ice;
	}
	else if (selectionCoord == sf::Vector2u(0, 2))
	{
		return Tileset::Bounce;
	}

	return Tileset::Other;
}

bool Editor::invalidTile(unsigned int x, unsigned int y)
{
	if (x < 0 || x >= levelSize.x
	    ||
	    y < 0 || y >= levelSize.y)
	{
		return true;
	}

	return false;
}

void Editor::eyedropper()
{
	int tileset = getTileset(mouseLevelCoord.x, mouseLevelCoord.y);
	if (tileset == Tileset::Null) { return; }

	if (tileset < Tileset::Other)
	{
		selectionCoord = sf::Vector2u(0, tileset - 1);
	}
	else
	{
		selectionCoord = getTileCrop(mouseLevelCoord.x, mouseLevelCoord.y);
	}
}

void Editor::placeTile(unsigned int x, unsigned int y, sf::Vector3i newTile)
{
	if (x == spawnPosition.x && y == spawnPosition.y - 1 && getTile(spawnPosition.x, spawnPosition.y) == spawnTile
	    ||
	    x ==  exitPosition.x && y ==  exitPosition.y - 1 && getTile( exitPosition.x,  exitPosition.y) ==  exitTile)
	{
		return;
	}

	if (newTile == spawnTile)
	{
		if (getTile(spawnPosition.x, spawnPosition.y) == spawnTile)
		{
			placeTile(spawnPosition.x, spawnPosition.y, emptyTile);
		}
		if (y != 0) // removing any tile placed above the spawn
		{
			placeTile(x, y - 1, emptyTile);
		}
		spawnPosition.x = x;
		spawnPosition.y = y;
	}
	else if (newTile == exitTile)
	{
		if (getTile(exitPosition.x, exitPosition.y) == exitTile)
		{
			placeTile(exitPosition.x, exitPosition.y, emptyTile);
		}
		if (y != 0) // removing any tile placed above the exit
		{
			placeTile(x, y - 1, emptyTile);
		}
		exitPosition.x = x;
		exitPosition.y = y;
	}

	levelVector.at(x).at(y) = newTile;
	adjustAdjacentTiles(x, y);
}

void Editor::adjustTile(unsigned int x, unsigned int y)
{
	if (invalidTile(x, y)) { return; }

	if (getTileset(x, y) != Tileset::Null && getTileset(x, y) != Tileset::Other)
	{
		levelVector.at(x).at(y) = determineTile(sf::Vector2i(x, y), getTileset(x, y));
	}
}

void Editor::adjustAdjacentTiles(unsigned int x, unsigned int y)
{
	adjustTile(x - 1, y - 1);
	adjustTile(x + 0, y - 1);
	adjustTile(x + 1, y - 1);

	adjustTile(x - 1, y + 0);
	adjustTile(x + 0, y + 0);
	adjustTile(x + 1, y + 0);

	adjustTile(x - 1, y + 1);
	adjustTile(x + 0, y + 1);
	adjustTile(x + 1, y + 1);
}

void Editor::fill(std::vector<std::vector<sf::Vector3i>> &localLevel, unsigned int x, unsigned int y, int oldTileset      , int newTileset)
{
	if (invalidTile(x, y) || localLevel.at(x).at(y).x != oldTileset) { return; }

	fillTiles.push_back(sf::Vector2u(x, y));
	localLevel.at(x).at(y).x = newTileset;

	// expand up
	fill(localLevel, x, y - 1, oldTileset, newTileset);
	// expand down
	fill(localLevel, x, y + 1, oldTileset, newTileset);
	// expand left
	fill(localLevel, x - 1, y, oldTileset, newTileset);
	// expand right
	fill(localLevel, x + 1, y, oldTileset, newTileset);
}

void Editor::fill(std::vector<std::vector<sf::Vector3i>> &localLevel, unsigned int x, unsigned int y, sf::Vector3i oldTile, int newTileset)
{
	if (invalidTile(x, y) || localLevel.at(x).at(y) != oldTile) { return; }

	fillTiles.push_back(sf::Vector2u(x, y));
	localLevel.at(x).at(y).x = newTileset;

	// expand up
	fill(localLevel, x, y - 1, oldTile, newTileset);
	// expand down
	fill(localLevel, x, y + 1, oldTile, newTileset);
	// expand left
	fill(localLevel, x - 1, y, oldTile, newTileset);
	// expand right
	fill(localLevel, x + 1, y, oldTile, newTileset);
}

void Editor::fill(std::vector<std::vector<sf::Vector3i>> &localLevel, unsigned int x, unsigned int y, int oldTileset      , sf::Vector3i newTile)
{
	if (invalidTile(x, y) || localLevel.at(x).at(y).x != oldTileset) { return; }

	fillTiles.push_back(sf::Vector2u(x, y));
	localLevel.at(x).at(y) = newTile;

	// expand up
	fill(localLevel, x, y - 1, oldTileset, newTile);
	// expand down
	fill(localLevel, x, y + 1, oldTileset, newTile);
	// expand left
	fill(localLevel, x - 1, y, oldTileset, newTile);
	// expand right
	fill(localLevel, x + 1, y, oldTileset, newTile);
}

void Editor::fill(std::vector<std::vector<sf::Vector3i>> &localLevel, unsigned int x, unsigned int y, sf::Vector3i oldTile, sf::Vector3i newTile)
{
	if (invalidTile(x, y) || localLevel.at(x).at(y) != oldTile) { return; }

	fillTiles.push_back(sf::Vector2u(x, y));
	localLevel.at(x).at(y) = newTile;

	// expand up
	fill(localLevel, x, y - 1, oldTile, newTile);
	// expand down
	fill(localLevel, x, y + 1, oldTile, newTile);
	// expand left
	fill(localLevel, x - 1, y, oldTile, newTile);
	// expand right
	fill(localLevel, x + 1, y, oldTile, newTile);
}

void Editor::fillArea()
{
	if (invalidTile(mouseLevelCoord.x, mouseLevelCoord.y)) { return; }

	fillTiles.clear();

	std::vector<std::vector<sf::Vector3i>> localLevel = levelVector;

	if (getSelectedTileset() < Tileset::Other)
	{
		// fill (int oldTileset, int newTileset)
		if (getTileset(mouseLevelCoord.x, mouseLevelCoord.y) < Tileset::Other)
		{
			if (getTileset(mouseLevelCoord.x, mouseLevelCoord.y) != getSelectedTileset())
			{
				fill(localLevel, mouseLevelCoord.x, mouseLevelCoord.y, getTileset(mouseLevelCoord.x, mouseLevelCoord.y), getSelectedTileset());
			}
		}
		// fill (vector3i oldTile, int newTileset)
		else
		{
			fill(localLevel, mouseLevelCoord.x, mouseLevelCoord.y, getTile(mouseLevelCoord.x, mouseLevelCoord.y), getSelectedTileset());
		}
	}
	else
	{
		// fill (int oldTileset, vector3i newTile)
		if (getTileset(mouseLevelCoord.x, mouseLevelCoord.y) < Tileset::Other)
		{
			fill(localLevel, mouseLevelCoord.x, mouseLevelCoord.y, getTileset(mouseLevelCoord.x, mouseLevelCoord.y), sf::Vector3i(getSelectedTileset(), selectionCoord.x, selectionCoord.y));
		}
		// fill (vector3i oldTile, vector3i newTile)
		else
		{
			if (getTile(mouseLevelCoord.x, mouseLevelCoord.y) != sf::Vector3i(getSelectedTileset(), selectionCoord.x, selectionCoord.y))
			{
				fill(localLevel, mouseLevelCoord.x, mouseLevelCoord.y, getTile(mouseLevelCoord.x, mouseLevelCoord.y), sf::Vector3i(getSelectedTileset(), selectionCoord.x, selectionCoord.y));
			}
		}
	}
}

void Editor::tiling()
{
	switch (method)
	{
		case Brush:
			if (erase)
			{
				placeTile(mouseLevelCoord.x, mouseLevelCoord.y, emptyTile);
			}
			else
			{
				placeTile(mouseLevelCoord.x, mouseLevelCoord.y, determineTile(mouseLevelCoord, getSelectedTileset()));
			}
			break;

		case Fill:
			sf::Vector3i tile(getSelectedTileset(), selectionCoord.x, selectionCoord.y);
			if (!erase && (tile == spawnTile || tile == exitTile))
			{
				placeTile(mouseLevelCoord.x, mouseLevelCoord.y, determineTile(mouseLevelCoord, getSelectedTileset()));
			}
			else
			{
				for (sf::Vector2u levelCoord : fillTiles)
				{
					if (erase)
					{
						placeTile(levelCoord.x, levelCoord.y, emptyTile);
					}
					else
					{
						placeTile(levelCoord.x, levelCoord.y, determineTile(mouseLevelCoord, getSelectedTileset()));
					}
				}
			}
			break;
	}
}



void Editor::drawLevelCheckers()
{
	for (unsigned int x = 0; x < levelSize.x; x++)
	{
		for (unsigned int y = 0; y < levelSize.y; y++)
		{
			if ((x + y) % 2 == 0)
			{
				levelChecker.setFillColor(levelCheckerEvenColor);
			}
			else
			{
				levelChecker.setFillColor(levelCheckerOddColor);
			}
			levelChecker.setPosition(relativeLevelPosition(x * tilesize, y * tilesize));
			window->draw(levelChecker);
		}
	}
}

void Editor::drawLevelTiles()
{
	for (unsigned int x = 0; x < levelSize.x; x++)
	{
		for (unsigned int y = 0; y < levelSize.y; y++)
		{
			sf::Vector3i tile = getTile(x, y);
			int tileset = tile.x;
			if (tileset != 0)
			{
				sf::IntRect tilesetCrop;
				tilesetCrop.width  = tilesize;
				tilesetCrop.height = tilesize;

				sf::Sprite* tilesetSprite;
				if (tile == sawbladeTile)
				{
					tilesetSprite = &sprites->sawblade;
					tilesetCrop.left = sawbladeAnimation.frame * tilesize;
					tilesetCrop.top  = 0;
				}
				else
				{
					tilesetSprite = sprites->tilesets.at(tileset - 1);
					tilesetCrop.left = tile.y * tilesize;
					tilesetCrop.top  = tile.z * tilesize;
				}
				tilesetSprite->setPosition(relativeLevelPosition(x * tilesize, y * tilesize));
				tilesetSprite->setTextureRect(tilesetCrop);
				tilesetSprite->setScale(zoom, zoom);
				window->draw(*tilesetSprite);
			}
		}
	}
}

void Editor::drawLevelRestrictedAreas()
{
	if (spawnPosition.y != 0 && getTile(spawnPosition.x, spawnPosition.y) == spawnTile)
	{
		levelRestrictedArea.setPosition(relativeLevelPosition(spawnPosition.x * tilesize, (spawnPosition.y - 1) * tilesize));
		levelRestrictedArea.setScale(zoom, zoom);
		sprites->slimey.setPosition(relativeLevelPosition(spawnPosition.x * tilesize + 1, (spawnPosition.y - 1) * tilesize + 4));
		sprites->slimey.setScale(zoom, zoom);
		window->draw(levelRestrictedArea);
		window->draw(sprites->slimey);
	}
	if (exitPosition.y != 0 && getTile(exitPosition.x, exitPosition.y) == exitTile)
	{
		sf::Vector2f position = relativeLevelPosition(exitPosition.x * tilesize, (exitPosition.y - 1) * tilesize);
		levelRestrictedArea.setPosition(position);
		levelRestrictedArea.setScale(zoom, zoom);
		sprites->exitSign.setPosition(position);
		sprites->exitSign.setScale(zoom, zoom);
		window->draw(levelRestrictedArea);
		window->draw(sprites->exitSign);
	}
}

void Editor::drawLevelGhostTiles()
{
	if (erase)
	{
		levelGhostTile.setFillColor(eraseColor);
	}
	else
	{
		levelGhostTile.setFillColor(placeColor);
	}

	switch (method)
	{
		case Brush:
			levelGhostTile.setPosition(relativeLevelPosition((float)mouseLevelCoord.x * tilesize, (float)mouseLevelCoord.y * tilesize));
			window->draw(levelGhostTile);
			break;

		case Fill:
			for (sf::Vector2u levelCoord : fillTiles)
			{
				levelGhostTile.setPosition(relativeLevelPosition((float)levelCoord.x * tilesize, (float)levelCoord.y * tilesize));
				window->draw(levelGhostTile);
			}
			break;
	}
}

void Editor::drawLevelCrosshair()
{
	// horizontal
	levelCrosshair.setSize({(float)levelSize.x * tilesize, levelOutlineThickness});
	levelCrosshair.setOrigin(0, levelOutlineThickness / 2);
	levelCrosshair.setPosition(relativeLevelPosition(0, (float)levelSize.y / 2 * tilesize));
	window->draw(levelCrosshair);
	// vertical
	levelCrosshair.setSize({levelOutlineThickness, (float)levelSize.y * tilesize});
	levelCrosshair.setOrigin(levelOutlineThickness / 2, 0);
	levelCrosshair.setPosition(relativeLevelPosition((float)levelSize.x / 2 * tilesize, 0));
	window->draw(levelCrosshair);
}

void Editor::drawLevel()
{
	window->draw(levelRect);
	drawLevelCheckers();
	drawLevelTiles();
	drawLevelRestrictedAreas();
	if (mouseOnLevel && !(inputHovering || buttonHovering))
	{
		drawLevelGhostTiles();
	}
	if (crosshair)
	{
		drawLevelCrosshair();
	}
}



void Editor::selectTile()
{
	selectionCoord.x = std::floor((mousePosition->x - getSelectionTilesetMouseBounds().left) / tilesize);
	selectionCoord.y = std::floor((mousePosition->y - getSelectionTilesetMouseBounds().top ) / tilesize);
}

void Editor::drawSelectionTileset()
{
	selectionTilesetRect.setPosition(relativeViewPosition(view, {(float)viewWidth - (float)sprites->tilesetOtherTexture.getSize().x - selectionTilesetOutlineThickness, selectionTilesetOutlineThickness}));
	updateSelectionTilesetBounds();
	window->draw(selectionTilesetRect);

	sprites->tilesetOther.setTextureRect(sf::IntRect({0, 0}, (sf::Vector2i)sprites->tilesetOtherTexture.getSize()));
	sprites->tilesetOther.setScale(1, 1);
	sprites->tilesetOther.setPosition(relativeSelectionTilesetPosition(0, 0));
	window->draw(sprites->tilesetOther);

	selectionTilesetSelection.setPosition(relativeSelectionTilesetPosition(selectionCoord.x * tilesize, selectionCoord.y * tilesize));
	window->draw(selectionTilesetSelection);
}



void removeLastCharFromStringstream(std::stringstream &stringstream)
{
	if (stringstream.str().empty()) { return; }
	std::string newString = stringstream.str();
	newString.pop_back();
	stringstream.str(newString);
	stringstream.seekp(stringstream.str().length());
}

char uppercaseToLowercase(char character)
{
	return character + 'a' - 'A';
}

void Editor::handleTextEntered(sf::Event event)
{
	if (*paused || !inputSelected) { return; }

	char character = static_cast<char>(event.text.unicode);

	if (character == backspace) // backspace to delete
	{
		removeLastCharFromStringstream(selectedInput->value);
	}
	else if (!selectedInput->numbersOnly && (
	         	character >= '0' && character <= '9'
	         	||
	         	character >= 'A' && character <= 'Z'
	         	||
	         	character >= 'a' && character <= 'z'
	         	||
	         	character == '_' || character == '-'
	         )
	         ||
	         selectedInput->numbersOnly && character >= '0' && character <= '9')
	{
		if (selectedInput->value.str().length() == selectedInput->maxLength)
		{
			removeLastCharFromStringstream(selectedInput->value);
		}
		if (character >= 'A' && character <= 'Z')
		{
			character = uppercaseToLowercase(character);
		}
		selectedInput->value << character;
	}
}

void Editor::clampSizeInputs()
{
	// clamp width
	int width = std::atoi(levelWidthInput->value.str().c_str());         // string to int
	width = std::clamp((unsigned int)width, minLevelSize, maxLevelSize);
	levelWidthInput->value.str(std::to_string(width));                   // int to string
	levelWidthInput->value.seekp(levelWidthInput->value.str().length());

	// clamp height
	int height = std::atoi(levelHeightInput->value.str().c_str());       // string to int
	height = std::clamp((unsigned int)height, minLevelSize, maxLevelSize);
	levelHeightInput->value.str(std::to_string(height));                 // int to string
	levelHeightInput->value.seekp(levelHeightInput->value.str().length());
}

void Editor::updateSizeInputs()
{
	inputHovering = false;

	for (Input* input : sizeInputs)
	{
		input->shape.setPosition(relativeViewPosition(view, input->position));
		input->bounds = input->shape.getGlobalBounds();

		input->shape.setFillColor(inactiveMenuboxBackground);
		input->shape.setOutlineColor(inactiveMenuboxForeground);
		input->textColor = inactiveMenuboxForeground;

		if (input->bounds.contains(*mousePosition))
		{
			inputHovering = true;

			input->shape.setFillColor(activeMenuboxBackground);
			input->shape.setOutlineColor(activeMenuboxForeground);
			input->textColor = activeMenuboxForeground;

			if (pressing(sf::Mouse::Left))
			{
				inputSelected = true;
				selectedInput = input;
			}
		}

		if (inputSelected && selectedInput == input)
		{
			input->shape.setFillColor(activeMenuboxForeground);
			input->shape.setOutlineColor(activeMenuboxBackground);
			input->textColor = activeMenuboxBackground;
		}
	}
}

void Editor::drawSizeInputs()
{
	for (Input* input : sizeInputs)
	{
		window->draw(input->shape);
		text->draw(input->value.str(), Start, Center, {input->bounds.left + 3.5f, input->bounds.top + input->bounds.height / 2}, input->textColor);
	}

	text->draw("Name of level", Center, Center, {levelNameInput->bounds.left + levelNameInput->bounds.width / 2, levelNameInput->bounds.top - 6});
	text->draw("Size", Center, Center, {levelWidthInput->bounds.left + levelWidthInput->bounds.width / 2, levelWidthInput->bounds.top - 6});
	text->draw("x", Center, Center, {levelWidthInput->bounds.left - 6, levelWidthInput->bounds.top + levelWidthInput->bounds.height / 2});
	text->draw("y", Center, Center, {levelHeightInput->bounds.left - 6, levelHeightInput->bounds.top + levelHeightInput->bounds.height / 2});
}



void Editor::updateButtons()
{
	buttonHovering = false;
	for (Button &button : buttons)
	{
		button.update(*mousePosition);
		if (button.active)
		{
			buttonHovering = true;
			activeButton = &button;
		}
	}
}

void Editor::drawButtons()
{
	for (Button &button : buttons)
	{
		button.draw(window, view, text);
	}
}



void Editor::update()
{
	if (*paused || *transitioning || !window->hasFocus()) { return; }

	processKeyboardInput();
	processMouseInput();

	updateSizeInputs();
	updateButtons();

	if (buttonHovering && *leftClick)
	{
		audio->click.play();
		if (activeButton == playButton)
		{
			playLevel();
		}
		else if (activeButton == saveButton)
		{
			saveLevel();
		}
		else if (activeButton == loadButton)
		{
			loadLevel();
		}
	}

	if (inputSelected && (pressing(sf::Keyboard::Enter) || (pressing(sf::Mouse::Left) && !inputHovering)))
	{
		inputSelected = false;
		clampSizeInputs();
		changeLevelSize(levelWidthInput->getValue(), levelHeightInput->getValue());
	}

	if (!inputHovering && !buttonHovering)
	{
		if (method == Fill && mouseOnLevel)
		{
			fillArea();
		}

		if (pressing(sf::Mouse::Left))
		{
			if (*pressingAlt)
			{
				eyedropper();
			}
			else
			{
				if (mouseOnSelectionTileset)
				{
					selectTile();
				}
				else if (mouseOnLevel)
				{
					tiling();
				}
			}
		}
	}

	sawbladeAnimation.update();

	if (inputHovering || buttonHovering || mouseOnSelectionTileset || mouseOnLevel)
	{
		*handyCursor = true;
	}
}

void Editor::draw()
{
	drawLevel();
	drawSelectionTileset();
	drawSizeInputs();
	drawButtons();
}
