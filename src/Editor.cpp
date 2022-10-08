#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>
#include <fstream>

#include "headers/Editor.hpp"

#define mapOutlineThickness 4
#define selectionTilesetOutlineThickness 2

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

Editor::Editor(sf::RenderWindow *_window, sf::View *_view, sf::FloatRect *_viewport, Sprites *_sprites, Text *_text,
               sf::Vector2f *_mousePosition, bool *_handyCursor, bool *_paused)
{
	window        = _window;
	view          = _view;
	viewport      = _viewport;
	sprites       = _sprites;
	text          = _text;
	mousePosition = _mousePosition;
	handyCursor   = _handyCursor;
	paused        = _paused;
	
	declareRegions();

	mapSize = {initialMapWidth, initialMapHeight};
	clearMap();

	sizeInputs = {
		new Input({viewWidth / 2 - 96 / 2, viewHeight - 24}, {96, 12}, 15),
		new Input({16, viewHeight - 32}, {24 ,12}, 3, true),
		new Input({16, viewHeight - 16}, {24 ,12}, 3, true)
	};
	mapNameInput   = sizeInputs.at(0);
	mapWidthInput  = sizeInputs.at(1);
	mapHeightInput = sizeInputs.at(2);
	mapWidthInput ->value << mapSize.x;
	mapHeightInput->value << mapSize.y;

	buttons = {
		Button("Save", {35, 20}, End, End, {viewWidth - 5, viewHeight - 30}),
		Button("Load", {35, 20}, End, End, {viewWidth - 5, viewHeight - 5})
	};
	saveButton = &buttons.at(0);
	loadButton = &buttons.at(1);

	tileDimension = {tilesize, tilesize};

	mapRect.setSize({(float)mapSize.x * tilesize, (float)mapSize.y * tilesize});
	mapRect.setOrigin(mapRect.getSize().x / 2, mapRect.getSize().y / 2);
	mapRect.setPosition(viewWidth / 2, viewHeight / 2);
	mapRect.setFillColor(sf::Color::Transparent);
	mapRect.setOutlineThickness(mapOutlineThickness);
	mapRect.setOutlineColor(mapOutlineColor);

	mapChecker.setSize(tileDimension);
	mapChecker.setFillColor(mapOutlineColor);
	mapRestrictedArea.setSize(tileDimension);
	mapRestrictedArea.setFillColor(mapRestrictedColor);
	mapGhostTile.setSize(tileDimension);
	mapCrosshair.setFillColor(mapCrosshairColor);

	selectionTilesetRect.setSize((sf::Vector2f)sprites->tilesetOtherTexture.getSize());
	selectionTilesetRect.setFillColor(selectionTilesetBackgroundColor);
	selectionTilesetRect.setOutlineColor(selectionTilesetOutlineColor);
	selectionTilesetRect.setOutlineThickness(selectionTilesetOutlineThickness);
	selectionTilesetSelection.setSize(tileDimension);
	selectionTilesetSelection.setFillColor(sf::Color::Transparent);
	selectionTilesetSelection.setOutlineColor(selectionTilesetSelectionOutlineColor);
	selectionTilesetSelection.setOutlineThickness(-selectionTilesetOutlineThickness);

	updateMapBounds();

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
	mapRect.setScale(zoom, zoom);
	mapChecker.setScale(zoom, zoom);
	mapGhostTile.setScale(zoom, zoom);
	mapCrosshair.setScale(zoom, zoom);
	updateMapBounds();
}

void Editor::resetView()
{
	view->setCenter(viewWidth / 2, viewHeight / 2);
	zoom = 1.0;
	adjustZoom();
}



void Editor::processKeyboardInput()
{
	bool pressingControl = pressing(sf::Keyboard::LControl) || pressing(sf::Keyboard::RControl);
	bool pressingShift = pressing(sf::Keyboard::LShift) || pressing(sf::Keyboard::RShift);

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
	handlePress(pressingControl && pressingShift && pressing(sf::Keyboard::R), clearMapPress, clearMapPressed);
	// save
	handlePress(pressingControl && pressing(sf::Keyboard::S), saveMapPress, saveMapPressed);
	// load
	handlePress(pressingControl && pressing(sf::Keyboard::L), loadMapPress, loadMapPressed);

	if (!inputSelected)
	{
		handleKeyboardInput();
	}
}

void Editor::handleKeyboardInput()
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

	if (saveMapPress)
	{
		saveMap();
	}
	else if (loadMapPress)
	{
		loadMap();
	}
	else if (clearMapPress)
	{
		clearMap();
	}
	else if (resetViewPress)
	{
		resetView();
	}
}

void Editor::processMouseInput()
{
	mouseOnMap              = false;
	mouseOnSelectionTileset = false;

	if (getSelectionTilesetMouseBounds().contains(*mousePosition))
	{
		mouseOnSelectionTileset = true;
	}
	else if (getMapMouseBounds().contains(*mousePosition))
	{
		mouseOnMap = true;

		mouseMapCoord.x = std::floor((mousePosition->x - mapBounds.left - mapOutlineThickness * zoom) / (tilesize * zoom));
		mouseMapCoord.y = std::floor((mousePosition->y - mapBounds.top  - mapOutlineThickness * zoom) / (tilesize * zoom));
	}

	handleDragging();
}

void Editor::handleDragging()
{
	if (pressing(sf::Mouse::Middle))
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



void Editor::updateMapBounds()
{
	mapBounds = mapRect.getGlobalBounds();
}

void Editor::updateSelectionTilesetBounds()
{
	selectionTilesetBounds = selectionTilesetRect.getGlobalBounds();
}

sf::Vector2f Editor::relativeMapPosition(float x, float y)
{
	return {mapBounds.left + (mapOutlineThickness + x) * zoom, mapBounds.top + (mapOutlineThickness + y) * zoom};
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

sf::FloatRect Editor::getMapMouseBounds()
{
	return sf::FloatRect(
		{mapBounds.left  + mapOutlineThickness * zoom    , mapBounds.top    + mapOutlineThickness * zoom    },
		{mapBounds.width - mapOutlineThickness * zoom * 2, mapBounds.height - mapOutlineThickness * zoom * 2}
	);
}



void Editor::clearMap()
{
	map.clear();
	for (unsigned int x = 0; x < mapSize.x; x++)
	{
		map.push_back({});
		for (unsigned int y = 0; y < mapSize.y; y++)
		{
			map.at(x).push_back(emptyTile);
		}
	}
}

void Editor::saveMap()
{
	std::stringstream mapStream;

	mapStream << "{\n\t{ ";
	for (unsigned int x = 0; x < mapSize.x; x++)
	{
		for (unsigned int y = 0; y < mapSize.y; y++)
		{
			sf::Vector3i tile = getTile(x, y);
			mapStream << "{" << tile.x << "," << tile.y << "," << tile.z << "}";

			if (y == mapSize.y - 1)
			{
				mapStream << " },\n";
				if (x < mapSize.x - 1)
				{
					mapStream << "\t{ ";
				}
			}
			else
			{
				mapStream << ", ";
			}
		}
	}
	mapStream << "};";

	if (!fs::exists("custom_maps"))
	{
		fs::create_directory("custom_maps");
	}

	std::ofstream output("custom_maps/" + mapNameInput->getString() + ".txt");
	output << mapStream.str();
	output.close();
}

void Editor::loadMap()
{
	if (fs::exists("custom_maps/" + mapNameInput->getString() + ".txt"))
	{
		map = getCustomMapVector(mapNameInput->getString());

		mapSize.x = map.size();
		mapSize.y = map.at(0).size();

		mapWidthInput ->value.str("");
		mapHeightInput->value.str("");
		mapWidthInput ->value << mapSize.x;
		mapHeightInput->value << mapSize.y;
	}
}

void Editor::changeMapSize(unsigned int newWidth, unsigned int newHeight)
{
	unsigned int oldWidth  = mapSize.x;
	unsigned int oldHeight = mapSize.y;

	mapSize.x = newWidth;
	mapSize.y = newHeight;
	mapRect.setSize({(float)mapSize.x * tilesize, (float)mapSize.y * tilesize});
	mapRect.setOrigin(mapRect.getSize().x / 2, mapRect.getSize().y / 2);

	updateMapBounds();

	// reseting spawn and exit positions if any of them are out of bounds
	if (spawnPosition.x > newWidth || spawnPosition.y > newHeight)
	{ spawnPosition = sf::Vector2u(); }
	if (exitPosition.x > newWidth || exitPosition.y > newHeight)
	{ exitPosition = sf::Vector2u(); }

	// creating new map
	std::vector<std::vector<sf::Vector3i>> newMap;
	for (unsigned int x = 0; x < newWidth; x++)
	{
		newMap.push_back({});
		for (unsigned int y = 0; y < newHeight; y++)
		{
			newMap.at(x).push_back(emptyTile);
		}
	}

	// populating new map
	for (unsigned int x = 0; x < oldWidth; x++)
	{
		for (unsigned int y = 0; y < oldHeight; y++)
		{
			if (x < newWidth && y < newHeight)
			{
				newMap.at(x).at(y) = map.at(x).at(y);
			}
		}
	}

	map = newMap;

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



// this function got its own seperate file because it got so big
// sf::Vector3i Editor::determineTile(sf::Vector2i mapCoord, int tileset)
// {
// 	...
// }

sf::Vector3i Editor::getTile(unsigned int x, unsigned int y)
{
	return map.at(x).at(y);
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
	if (x < 0 || x >= mapSize.x
	    ||
	    y < 0 || y >= mapSize.y)
	{
		return true;
	}

	return false;
}

void Editor::eyedropper()
{
	int tileset = getTileset(mouseMapCoord.x, mouseMapCoord.y);
	if (tileset == Tileset::Null) { return; }

	if (tileset < Tileset::Other)
	{
		selectionCoord = sf::Vector2u(0, tileset - 1);
	}
	else
	{
		selectionCoord = getTileCrop(mouseMapCoord.x, mouseMapCoord.y);
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
		placeTile(x, y - 1, emptyTile);
		spawnPosition.x = x;
		spawnPosition.y = y;
	}
	else if (newTile == exitTile)
	{
		if (getTile(exitPosition.x, exitPosition.y) == exitTile)
		{
			placeTile(exitPosition.x, exitPosition.y, emptyTile);
		}
		placeTile(x, y - 1, emptyTile);
		exitPosition.x = x;
		exitPosition.y = y;
	}

	map.at(x).at(y) = newTile;
	adjustAdjacentTiles(x, y);
}

void Editor::adjustTile(unsigned int x, unsigned int y)
{
	if (invalidTile(x, y)) { return; }

	if (getTileset(x, y) != Tileset::Null && getTileset(x, y) != Tileset::Other)
	{
		map.at(x).at(y) = determineTile(sf::Vector2i(x, y), getTileset(x, y));
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

void Editor::fill(std::vector<std::vector<sf::Vector3i>> &localMap, unsigned int x, unsigned int y, int oldTileset      , int newTileset)
{
	if (invalidTile(x, y) || localMap.at(x).at(y).x != oldTileset) { return; }

	fillTiles.push_back(sf::Vector2u(x, y));
	localMap.at(x).at(y).x = newTileset;

	// expand up
	fill(localMap, x, y - 1, oldTileset, newTileset);
	// expand down
	fill(localMap, x, y + 1, oldTileset, newTileset);
	// expand left
	fill(localMap, x - 1, y, oldTileset, newTileset);
	// expand right
	fill(localMap, x + 1, y, oldTileset, newTileset);
}

void Editor::fill(std::vector<std::vector<sf::Vector3i>> &localMap, unsigned int x, unsigned int y, sf::Vector3i oldTile, int newTileset)
{
	if (invalidTile(x, y) || localMap.at(x).at(y) != oldTile) { return; }

	fillTiles.push_back(sf::Vector2u(x, y));
	localMap.at(x).at(y).x = newTileset;

	// expand up
	fill(localMap, x, y - 1, oldTile, newTileset);
	// expand down
	fill(localMap, x, y + 1, oldTile, newTileset);
	// expand left
	fill(localMap, x - 1, y, oldTile, newTileset);
	// expand right
	fill(localMap, x + 1, y, oldTile, newTileset);
}

void Editor::fill(std::vector<std::vector<sf::Vector3i>> &localMap, unsigned int x, unsigned int y, int oldTileset      , sf::Vector3i newTile)
{
	if (invalidTile(x, y) || localMap.at(x).at(y).x != oldTileset) { return; }

	fillTiles.push_back(sf::Vector2u(x, y));
	localMap.at(x).at(y) = newTile;

	// expand up
	fill(localMap, x, y - 1, oldTileset, newTile);
	// expand down
	fill(localMap, x, y + 1, oldTileset, newTile);
	// expand left
	fill(localMap, x - 1, y, oldTileset, newTile);
	// expand right
	fill(localMap, x + 1, y, oldTileset, newTile);
}

void Editor::fill(std::vector<std::vector<sf::Vector3i>> &localMap, unsigned int x, unsigned int y, sf::Vector3i oldTile, sf::Vector3i newTile)
{
	if (invalidTile(x, y) || localMap.at(x).at(y) != oldTile) { return; }

	fillTiles.push_back(sf::Vector2u(x, y));
	localMap.at(x).at(y) = newTile;

	// expand up
	fill(localMap, x, y - 1, oldTile, newTile);
	// expand down
	fill(localMap, x, y + 1, oldTile, newTile);
	// expand left
	fill(localMap, x - 1, y, oldTile, newTile);
	// expand right
	fill(localMap, x + 1, y, oldTile, newTile);
}

void Editor::fillArea()
{
	if (invalidTile(mouseMapCoord.x, mouseMapCoord.y)) { return; }

	fillTiles.clear();

	std::vector<std::vector<sf::Vector3i>> localMap = map;

	if (getSelectedTileset() < Tileset::Other)
	{
		// fill (int oldTileset, int newTileset)
		if (getTileset(mouseMapCoord.x, mouseMapCoord.y) < Tileset::Other)
		{
			if (getTileset(mouseMapCoord.x, mouseMapCoord.y) != getSelectedTileset())
			{
				fill(localMap, mouseMapCoord.x, mouseMapCoord.y, getTileset(mouseMapCoord.x, mouseMapCoord.y), getSelectedTileset());
			}
		}
		// fill (vector3i oldTile, int newTileset)
		else
		{
			fill(localMap, mouseMapCoord.x, mouseMapCoord.y, getTile(mouseMapCoord.x, mouseMapCoord.y), getSelectedTileset());
		}
	}
	else
	{
		// fill (int oldTileset, vector3i newTile)
		if (getTileset(mouseMapCoord.x, mouseMapCoord.y) < Tileset::Other)
		{
			fill(localMap, mouseMapCoord.x, mouseMapCoord.y, getTileset(mouseMapCoord.x, mouseMapCoord.y), sf::Vector3i(getSelectedTileset(), selectionCoord.x, selectionCoord.y));
		}
		// fill (vector3i oldTile, vector3i newTile)
		else
		{
			if (getTile(mouseMapCoord.x, mouseMapCoord.y) != sf::Vector3i(getSelectedTileset(), selectionCoord.x, selectionCoord.y))
			{
				fill(localMap, mouseMapCoord.x, mouseMapCoord.y, getTile(mouseMapCoord.x, mouseMapCoord.y), sf::Vector3i(getSelectedTileset(), selectionCoord.x, selectionCoord.y));
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
				placeTile(mouseMapCoord.x, mouseMapCoord.y, emptyTile);
			}
			else
			{
				placeTile(mouseMapCoord.x, mouseMapCoord.y, determineTile(mouseMapCoord, getSelectedTileset()));
			}
			break;

		case Fill:
			sf::Vector3i tile(getSelectedTileset(), selectionCoord.x, selectionCoord.y);
			if (!erase && (tile == spawnTile || tile == exitTile))
			{
				placeTile(mouseMapCoord.x, mouseMapCoord.y, determineTile(mouseMapCoord, getSelectedTileset()));
			}
			else
			{
				for (sf::Vector2u mapCoord : fillTiles)
				{
					if (erase)
					{
						placeTile(mapCoord.x, mapCoord.y, emptyTile);
					}
					else
					{
						placeTile(mapCoord.x, mapCoord.y, determineTile(mouseMapCoord, getSelectedTileset()));
					}
				}
			}
			break;
	}
}



void Editor::drawMapCheckers()
{
	for (unsigned int x = 0; x < mapSize.x; x++)
	{
		for (unsigned int y = 0; y < mapSize.y; y++)
		{
			if ((x + y) % 2 == 0)
			{
				mapChecker.setFillColor(mapCheckerEvenColor);
			}
			else
			{
				mapChecker.setFillColor(mapCheckerOddColor);
			}
			mapChecker.setPosition(relativeMapPosition(x * tilesize, y * tilesize));
			window->draw(mapChecker);
		}
	}
}

void Editor::drawMapTiles()
{
	for (unsigned int x = 0; x < mapSize.x; x++)
	{
		for (unsigned int y = 0; y < mapSize.y; y++)
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
				tilesetSprite->setPosition(relativeMapPosition(x * tilesize, y * tilesize));
				tilesetSprite->setTextureRect(tilesetCrop);
				tilesetSprite->setScale(zoom, zoom);
				window->draw(*tilesetSprite);
			}
		}
	}
}

void Editor::drawMapRestrictedAreas()
{
	if (spawnPosition.y != 0 && getTile(spawnPosition.x, spawnPosition.y) == spawnTile)
	{
		mapRestrictedArea.setPosition(relativeMapPosition(spawnPosition.x * tilesize, (spawnPosition.y - 1) * tilesize));
		mapRestrictedArea.setScale(zoom, zoom);
		sprites->slimey.setPosition(relativeMapPosition(spawnPosition.x * tilesize + 1, (spawnPosition.y - 1) * tilesize + 4));
		sprites->slimey.setScale(zoom, zoom);
		window->draw(mapRestrictedArea);
		window->draw(sprites->slimey);
	}
	if (exitPosition.y != 0 && getTile(exitPosition.x, exitPosition.y) == exitTile)
	{
		sf::Vector2f position = relativeMapPosition(exitPosition.x * tilesize, (exitPosition.y - 1) * tilesize);
		mapRestrictedArea.setPosition(position);
		mapRestrictedArea.setScale(zoom, zoom);
		sprites->exitSign.setPosition(position);
		sprites->exitSign.setScale(zoom, zoom);
		window->draw(mapRestrictedArea);
		window->draw(sprites->exitSign);
	}
}

void Editor::drawMapGhostTiles()
{
	if (erase)
	{
		mapGhostTile.setFillColor(eraseColor);
	}
	else
	{
		mapGhostTile.setFillColor(placeColor);
	}

	switch (method)
	{
		case Brush:
			mapGhostTile.setPosition(relativeMapPosition((float)mouseMapCoord.x * tilesize, (float)mouseMapCoord.y * tilesize));
			window->draw(mapGhostTile);
			break;

		case Fill:
			for (sf::Vector2u mapCoord : fillTiles)
			{
				mapGhostTile.setPosition(relativeMapPosition((float)mapCoord.x * tilesize, (float)mapCoord.y * tilesize));
				window->draw(mapGhostTile);
			}
			break;
	}
}

void Editor::drawMapCrosshair()
{
	// horizontal
	mapCrosshair.setSize({(float)mapSize.x * tilesize, mapOutlineThickness});
	mapCrosshair.setOrigin(0, mapOutlineThickness / 2);
	mapCrosshair.setPosition(relativeMapPosition(0, mapSize.y / 2 * tilesize));
	window->draw(mapCrosshair);
	// vertical
	mapCrosshair.setSize({mapOutlineThickness, (float)mapSize.y * tilesize});
	mapCrosshair.setOrigin(mapOutlineThickness / 2, 0);
	mapCrosshair.setPosition(relativeMapPosition(mapSize.x / 2 * tilesize, 0));
	window->draw(mapCrosshair);
}

void Editor::drawMap()
{
	window->draw(mapRect);
	drawMapCheckers();
	drawMapTiles();
	drawMapRestrictedAreas();
	if (mouseOnMap && !(inputHovering || buttonHovering))
	{
		drawMapGhostTiles();
	}
	if (crosshair)
	{
		drawMapCrosshair();
	}
}



void Editor::selectTile()
{
	selectionCoord.x = std::floor((mousePosition->x - getSelectionTilesetMouseBounds().left) / tilesize);
	selectionCoord.y = std::floor((mousePosition->y - getSelectionTilesetMouseBounds().top ) / tilesize);
}

void Editor::drawSelectionTileset()
{
	selectionTilesetRect.setPosition(relativeViewPosition(*view, {(float)viewWidth - (float)sprites->tilesetOtherTexture.getSize().x - selectionTilesetOutlineThickness, selectionTilesetOutlineThickness}));
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

void Editor::handleTextEntered(sf::Event event)
{
	if (*paused || !inputSelected) { return; }

	char character = static_cast<char>(event.text.unicode);

	if ((int)character == 8) // backspace to delete
	{
		removeLastCharFromStringstream(selectedInput->value);
	}
	else if (!selectedInput->numbersOnly && (
	         	(int)character >= (int)'0' && (int)character <= (int)'9'
	         	||
	         	(int)character >= (int)'A' && (int)character <= (int)'Z'
	         	||
	         	(int)character >= (int)'a' && (int)character <= (int)'z'
	         )
	         ||
	         selectedInput->numbersOnly && (int)character >= (int)'0' && (int)character <= (int)'9')
	{
		if (selectedInput->value.str().length() == selectedInput->maxLength)
		{
			removeLastCharFromStringstream(selectedInput->value);
		}
		selectedInput->value << character;
	}
}

void Editor::clampSizeInputs()
{
	// clamp width
	int width = std::atoi(mapWidthInput->value.str().c_str());         // string to int
	width = std::clamp((unsigned int)width, minMapSize, maxMapSize);
	mapWidthInput->value.str(std::to_string(width));                   // int to string
	mapWidthInput->value.seekp(mapWidthInput->value.str().length());

	// clamp height
	int height = std::atoi(mapHeightInput->value.str().c_str());       // string to int
	height = std::clamp((unsigned int)height, minMapSize, maxMapSize);
	mapHeightInput->value.str(std::to_string(height));                 // int to string
	mapHeightInput->value.seekp(mapHeightInput->value.str().length());
}

void Editor::updateSizeInputs()
{
	inputHovering = false;

	for (Input *input : sizeInputs)
	{
		input->shape.setPosition(relativeViewPosition(*view, input->position));
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
	for (Input *input : sizeInputs)
	{
		window->draw(input->shape);
		text->draw(input->value.str(), Start, Center, {input->bounds.left + 3.5f, input->bounds.top + input->bounds.height / 2}, input->textColor);
	}

	text->draw("Name of map", Center, Center, {mapNameInput->bounds.left + mapNameInput->bounds.width / 2, mapNameInput->bounds.top - 6});
	text->draw("Size", Center, Center, {mapWidthInput->bounds.left + mapWidthInput->bounds.width / 2, mapWidthInput->bounds.top - 6});
	text->draw("x", Center, Center, {mapWidthInput->bounds.left - 6, mapWidthInput->bounds.top + mapWidthInput->bounds.height / 2});
	text->draw("y", Center, Center, {mapHeightInput->bounds.left - 6, mapHeightInput->bounds.top + mapHeightInput->bounds.height / 2});
}



void Editor::updateButtons()
{
	buttonHovering = false;
	for (Button &button : buttons)
	{
		button.active = false;
		if (button.bounds.contains(*mousePosition))
		{
			button.active = true;
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
	if (*paused) { return; }

	if (window->hasFocus())
	{
		processKeyboardInput();
		processMouseInput();
	}

	updateSizeInputs();
	updateButtons();

	if (buttonHovering && pressing(sf::Mouse::Left))
	{
		if (activeButton == saveButton)
		{
			saveMap();
		}
		else if (activeButton == loadButton)
		{
			loadMap();
		}
	}

	if (inputSelected && (pressing(sf::Keyboard::Enter) || (pressing(sf::Mouse::Left) && !inputHovering)))
	{
		inputSelected = false;
		clampSizeInputs();
		changeMapSize(mapWidthInput->getValue(), mapHeightInput->getValue());
	}

	if (!inputHovering && !buttonHovering)
	{
		if (method == Fill && mouseOnMap)
		{
			fillArea();
		}

		if (pressing(sf::Mouse::Left))
		{
			if (pressing(sf::Keyboard::LAlt) || pressing(sf::Keyboard::RAlt))
			{
				eyedropper();
			}
			else
			{
				if (mouseOnSelectionTileset)
				{
					selectTile();
				}
				else if (mouseOnMap)
				{
					tiling();
				}
			}
		}
	}

	sawbladeAnimation.update();

	if (inputHovering || buttonHovering || mouseOnSelectionTileset || mouseOnMap)
	{
		*handyCursor = true;
	}
}

void Editor::draw()
{
	drawMap();
	drawSelectionTileset();
	drawSizeInputs();
	drawButtons();
}
