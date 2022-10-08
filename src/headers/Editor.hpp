#pragma once

#include "Animation.hpp"
#include "Global.hpp"
#include "Sprites.hpp"
#include "Text.hpp"

struct Region
{
	std::vector<int> region;
	sf::Vector2u crop;

	Region(unsigned int xCrop, unsigned int yCrop, std::vector<int> _region);
};

struct Editor
{
	sf::RenderWindow *window;
	sf::View *view;
	sf::FloatRect *viewport;

	Sprites *sprites;
	Text *text;

	bool *paused;

	mapVector map;
	/*
	one 3D vector has 3 values (x, y, z) which in this case represents one tile
	the x value represents which tileset to draw
	the y value represents the horizontal (x) crop of the respective tileset
	the z value represents the vertical (y) crop of the respective tileset

	could change it so that the z represents the tileset
	because that would make more sense
	but i am to lazy to do so
	*/
	unsigned int minMapSize = 16;
	unsigned int maxMapSize = 64;
	unsigned int initialMapWidth  = minMapSize;
	unsigned int initialMapHeight = minMapSize;
	sf::Vector2u mapSize;

	std::vector<Region> regions;

	sf::Vector2u spawnPosition;
	sf::Vector2u exitPosition;

	sf::Vector2f tileDimension;

	sf::RectangleShape mapRect;
	sf::RectangleShape mapChecker;
	sf::RectangleShape mapRestrictedArea;
	sf::RectangleShape mapGhostTile;
	sf::RectangleShape mapCrosshair;
	sf::FloatRect mapBounds;

	sf::RectangleShape selectionTilesetRect;
	sf::RectangleShape selectionTilesetSelection;
	sf::FloatRect selectionTilesetBounds;

	sf::Vector2f *mousePosition;
	sf::Vector2i mouseMapCoord;
	sf::Vector2i mouseSelectionCoord;
	sf::Vector2f draggingStartPosition;
	bool dragging = false;

	bool mouseOnMap              = false;
	bool mouseOnSelectionTileset = false;

	sf::Vector2u selectionCoord;
	std::vector<sf::Vector2u> fillTiles;
	enum Method { Brush, Fill };
	Method method = Brush;
	bool erase = false;

	bool crosshair = false;

	enum MouseWheel { Up = 1, Down = -1 };
	float zoom     = 1.0;
	float zoomStep = 0.1;
	float minZoom  = 0.1;
	float maxZoom  = 2.0;

	// B
	bool brushPress   = false;
	bool brushPressed = false;
	// F
	bool fillPress   = false;
	bool fillPressed = false;
	// E
	bool erasePress   = false;
	bool erasePressed = false;
	// C
	bool crosshairPress   = false;
	bool crosshairPressed = false;
	// R
	bool resetViewPress   = false;
	bool resetViewPressed = false;
	// Control + Shift + R
	bool clearMapPress   = false;
	bool clearMapPressed = false;
	// Control + S
	bool saveMapPress   = false;
	bool saveMapPressed = false;

	Animation sawbladeAnimation;

	Editor();
	Editor(sf::RenderWindow *_window, sf::View *_view, sf::FloatRect *_viewport, Sprites *_sprites, Text *_text,
	       sf::Vector2f *_mousePosition, bool *_paused);

	void declareRegions();

	void handleZoom(sf::Event event);
	void adjustZoom();
	void resetView();

	void processKeyboardInput();
	void handleKeyboardInput();
	void processMouseInput();
	void handleDragging();

	void updateMapBounds();
	void updateSelectionTilesetBounds();
	sf::Vector2f relativeMapPosition(float x, float y);
	sf::Vector2f relativeSelectionTilesetPosition(float x, float y);
	sf::FloatRect getSelectionTilesetMouseBounds();
	sf::FloatRect getMapMouseBounds();

	void clearMap(); // also works for initializing the map
	void saveMap();
	void changeMapSize(unsigned int newWidth, unsigned int newHeight);

	sf::Vector3i determineTile(sf::Vector2i mapCoord, int tileset);
	sf::Vector3i getTile(unsigned int x, unsigned int y);
	sf::Vector2u getTileCrop(unsigned int x, unsigned int y);
	int getTileset(unsigned int x, unsigned int y);
	int getSelectedTileset();
	bool invalidTile(unsigned int x, unsigned int y);
	void eyedropper();
	void placeTile(unsigned int x, unsigned int y, sf::Vector3i newTile);
	void adjustTile(unsigned int x, unsigned int y);
	void adjustAdjacentTiles(unsigned int x, unsigned int y);
	void fill(std::vector<std::vector<sf::Vector3i>> &localMap, unsigned int x, unsigned int y, int oldTileset      , int newTileset);
	void fill(std::vector<std::vector<sf::Vector3i>> &localMap, unsigned int x, unsigned int y, sf::Vector3i oldTile, int newTileset);
	void fill(std::vector<std::vector<sf::Vector3i>> &localMap, unsigned int x, unsigned int y, int oldTileset      , sf::Vector3i newTile);
	void fill(std::vector<std::vector<sf::Vector3i>> &localMap, unsigned int x, unsigned int y, sf::Vector3i oldTile, sf::Vector3i newTile);
	void fillArea();
	void tiling();

	void drawMapCheckers();
	void drawMapTiles();
	void drawMapRestrictedAreas();
	void drawMapGhostTiles();
	void drawMapCrosshair();
	void drawMap();

	void selectTile();
	void drawSelectionTileset();

	void handleTextEntered(sf::Event event);
	void updateSizeInputs();
	void clampSizeInputs();
	void drawSizeInputs();

	void update();
	void draw();
};
