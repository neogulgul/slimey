#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>

#include "headers/Editor.hpp"

#define mapOutlineThickness 4
#define selectionTilesetOutlineThickness 2

Editor::Editor() {}

Editor::Editor(sf::RenderWindow *_window, sf::View *_view, sf::FloatRect *_viewport, Sprites *_sprites, Text *_text,
               sf::Vector2f *_mousePosition, bool *_paused)
{
	window        = _window;
	view          = _view;
	viewport      = _viewport;
	sprites       = _sprites;
	text          = _text;
	mousePosition = _mousePosition;
	paused        = _paused;
	
	declareRegions();

	mapSize = {initialMapWidth, initialMapHeight};
	clearMap();

	tileDimension = {tilesize, tilesize};

	mapRect.setSize(sf::Vector2f(mapSize.x * tilesize, mapSize.y * tilesize));
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

	if (event.mouseWheel.delta == MouseWheel::Up)
	{
		zoom += zoomStep;
	}
	else if (event.mouseWheel.delta == MouseWheel::Down)
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

	handleKeyboardInput();
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
	return sf::Vector2f(mapBounds.left + (mapOutlineThickness + x) * zoom, mapBounds.top + (mapOutlineThickness + y) * zoom);
}

sf::Vector2f Editor::relativeSelectionTilesetPosition(float x, float y)
{
	return sf::Vector2f(selectionTilesetBounds.left + selectionTilesetOutlineThickness + x, selectionTilesetBounds.top + selectionTilesetOutlineThickness + y);
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
	// todo: this
}

void Editor::changeMapSize(unsigned int newWidth, unsigned int newHeight)
{
	unsigned int oldWidth  = mapSize.x;
	unsigned int oldHeight = mapSize.y;

	mapSize.x = newWidth;
	mapSize.y = newHeight;
	mapRect.setSize(sf::Vector2f(mapSize.x * tilesize, mapSize.y * tilesize));
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
	mapCrosshair.setSize(sf::Vector2f(mapSize.x * tilesize, mapOutlineThickness));
	mapCrosshair.setOrigin(0, mapOutlineThickness / 2);
	mapCrosshair.setPosition(relativeMapPosition(0, mapSize.y / 2 * tilesize));
	window->draw(mapCrosshair);
	// vertical
	mapCrosshair.setSize(sf::Vector2f(mapOutlineThickness, mapSize.y * tilesize));
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
	if (mouseOnMap)
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
	selectionTilesetRect.setPosition(relativeViewPosition(*view, {selectionTilesetOutlineThickness, selectionTilesetOutlineThickness}));
	updateSelectionTilesetBounds();
	window->draw(selectionTilesetRect);

	sprites->tilesetOther.setTextureRect(sf::IntRect({0, 0}, (sf::Vector2i)sprites->tilesetOtherTexture.getSize()));
	sprites->tilesetOther.setScale(1, 1);
	sprites->tilesetOther.setPosition(relativeSelectionTilesetPosition(0, 0));
	window->draw(sprites->tilesetOther);

	selectionTilesetSelection.setPosition(relativeSelectionTilesetPosition(selectionCoord.x * tilesize, selectionCoord.y * tilesize));
	window->draw(selectionTilesetSelection);
}



void Editor::handleTextEntered(sf::Event event)
{
	if (*paused) { return; }
}

void Editor::updateSizeInputs()
{
}

void Editor::clampSizeInputs()
{
}

void Editor::drawSizeInputs()
{
}



void Editor::update()
{
	if (*paused) { return; }

	processKeyboardInput();
	processMouseInput();

	updateSizeInputs();

	if (true/*!inputHovering*/)
	{
		if (method == Fill && mouseOnMap)
		{
			fillArea();
		}

		if (pressing(sf::Mouse::Left))
		{
			// if (inputSelected)
			// {
			// 	inputSelected = false;
			// 	clampSizeInputs();
			// 	changeMapSize(mapWidthInput.getValue(), mapHeightInput.getValue());
			// }

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
}

void Editor::draw()
{
	drawMap();
	drawSelectionTileset();
	drawSizeInputs();
}
