#include "headers/Editor.hpp"

#define regionSize 9 // 3 x 3

Region::Region(unsigned int xCrop, unsigned int yCrop, std::vector<int> _region)
{
	crop.x = xCrop;
	crop.y = yCrop;
	region = _region;
}

bool sameTile(int tile0, int tile1)
{
	if (tile0 == tile1)
	{
		return true;
	}

	return false;
}

bool equalRegion(std::vector<int> region0, std::vector<int> region1)
{
	for (unsigned int i = 0; i < regionSize; i++)
	{
		if (region1.at(i) != 2)
		{
			if (region0.at(i) != region1.at(i))
			{
				return false;
			}
		}
	}

	return true;
}

void Editor::declareRegions()
{
	regions.push_back(Region(0, 0, {2, 0, 2,
	                                0, 1, 1,
	                                2, 1, 1}));


	regions.push_back(Region(1, 0, {2, 0, 2,
	                                1, 1, 1,
	                                1, 1, 1}));

	regions.push_back(Region(2, 0, {2, 0, 2,
	                                1, 1, 0,
	                                1, 1, 2}));

	regions.push_back(Region(3, 0, {2, 0, 2,
	                                0, 1, 0,
	                                2, 1, 2}));

	regions.push_back(Region(4, 0, {2, 1, 1,
	                                0, 1, 1,
	                                2, 1, 0}));

	regions.push_back(Region(5, 0, {1, 1, 2,
	                                1, 1, 0,
	                                0, 1, 2}));

	regions.push_back(Region(6, 0, {1, 1, 1,
	                                1, 1, 1,
	                                1, 1, 0}));

	regions.push_back(Region(7, 0, {1, 1, 1,
	                                1, 1, 1,
	                                0, 1, 1}));

	regions.push_back(Region(0, 1, {2, 1, 1,
	                                0, 1, 1,
	                                2, 1, 1}));

	regions.push_back(Region(1, 1, {1, 1, 1,
	                                1, 1, 1,
	                                1, 1, 1}));

	regions.push_back(Region(2, 1, {1, 1, 2,
	                                1, 1, 0,
	                                1, 1, 2}));

	regions.push_back(Region(3, 1, {2, 1, 2,
	                                0, 1, 0,
	                                2, 1, 2}));

	regions.push_back(Region(4, 1, {2, 1, 0,
	                                0, 1, 1,
	                                2, 1, 0}));

	regions.push_back(Region(5, 1, {0, 1, 2,
	                                1, 1, 0,
	                                0, 1, 2}));

	regions.push_back(Region(6, 1, {1, 1, 0,
	                                1, 1, 1,
	                                1, 1, 1}));

	regions.push_back(Region(7, 1, {0, 1, 1,
	                                1, 1, 1,
	                                1, 1, 1}));

	regions.push_back(Region(0, 2, {2, 1, 1,
	                                0, 1, 1,
	                                2, 0, 2}));

	regions.push_back(Region(1, 2, {1, 1, 1,
	                                1, 1, 1,
	                                2, 0, 2}));

	regions.push_back(Region(2, 2, {1, 1, 2,
	                                1, 1, 0,
	                                2, 0, 2}));

	regions.push_back(Region(3, 2, {2, 1, 2,
	                                0, 1, 0,
	                                2, 0, 2}));

	regions.push_back(Region(4, 2, {2, 1, 0,
	                                0, 1, 1,
	                                2, 1, 1}));

	regions.push_back(Region(5, 2, {0, 1, 2,
	                                1, 1, 0,
	                                1, 1, 2}));

	regions.push_back(Region(6, 2, {0, 1, 0,
	                                1, 1, 1,
	                                0, 1, 1}));

	regions.push_back(Region(7, 2, {0, 1, 0,
	                                1, 1, 1,
	                                1, 1, 0}));

	regions.push_back(Region(0, 3, {2, 0, 2,
	                                0, 1, 1,
	                                2, 0, 2}));

	regions.push_back(Region(1, 3, {2, 0, 2,
	                                1, 1, 1,
	                                2, 0, 2}));

	regions.push_back(Region(2, 3, {2, 0, 2,
	                                1, 1, 0,
	                                2, 0, 2}));

	regions.push_back(Region(3, 3, {2, 0, 2,
	                                0, 1, 0,
	                                2, 0, 2}));

	regions.push_back(Region(4, 3, {1, 1, 0,
	                                1, 1, 1,
	                                1, 1, 0}));

	regions.push_back(Region(5, 3, {0, 1, 1,
	                                1, 1, 1,
	                                0, 1, 1}));

	regions.push_back(Region(6, 3, {0, 1, 1,
	                                1, 1, 1,
	                                0, 1, 0}));

	regions.push_back(Region(7, 3, {1, 1, 0,
	                                1, 1, 1,
	                                0, 1, 0}));

	regions.push_back(Region(0, 4, {2, 0, 2,
	                                1, 1, 1,
	                                1, 1, 0}));

	regions.push_back(Region(1, 4, {2, 0, 2,
	                                1, 1, 1,
	                                0, 1, 0}));

	regions.push_back(Region(2, 4, {2, 0, 2,
	                                1, 1, 1,
	                                0, 1, 1}));

	regions.push_back(Region(3, 4, {1, 1, 1,
	                                1, 1, 1,
	                                0, 1, 0}));

	regions.push_back(Region(4, 4, {2, 0, 2,
	                                0, 1, 1,
	                                2, 1, 0}));

	regions.push_back(Region(5, 4, {2, 0, 2,
	                                1, 1, 0,
	                                0, 1, 2}));

	regions.push_back(Region(6, 4, {0, 1, 1,
	                                1, 1, 1,
	                                1, 1, 0}));

	regions.push_back(Region(7, 4, {1, 1, 0,
	                                1, 1, 1,
	                                0, 1, 1}));

	regions.push_back(Region(0, 5, {1, 1, 0,
	                                1, 1, 1,
	                                2, 0, 2}));

	regions.push_back(Region(1, 5, {0, 1, 0,
	                                1, 1, 1,
	                                2, 0, 2}));

	regions.push_back(Region(2, 5, {0, 1, 1,
	                                1, 1, 1,
	                                2, 0, 2}));

	regions.push_back(Region(3, 5, {0, 1, 0,
	                                1, 1, 1,
	                                1, 1, 1}));

	regions.push_back(Region(4, 5, {2, 1, 0,
	                                0, 1, 1,
	                                2, 0, 2}));

	regions.push_back(Region(5, 5, {0, 1, 2,
	                                1, 1, 0,
	                                2, 0, 2}));

	regions.push_back(Region(6, 5, {0, 1, 0,
	                                1, 1, 1,
	                                0, 1, 0}));
}

sf::Vector3i Editor::determineTile(sf::Vector2i levelCoord, int tileset)
{
	sf::Vector2u crop;

	if (tileset < 4)
	{
		// T = top;  M = middle; B = bottom;
		// L = left; C = center; R = right;
		int TL = 1;
		int TC = 0;
		int TR = 1;
		int ML = 1;
		int MC = 1;
		int MR = 1;
		int BL = 1;
		int BC = 1;
		int BR = 1;

		if (levelCoord.y > 0)
		{
			if (levelCoord.x > 0)
			{
				TL = sameTile(getTileset(levelCoord.x - 1, levelCoord.y - 1), tileset);
			}
				TC = sameTile(getTileset(levelCoord.x + 0, levelCoord.y - 1), tileset);
			if (levelCoord.x < levelSize.x - 1)
			{
				TR = sameTile(getTileset(levelCoord.x + 1, levelCoord.y - 1), tileset);
			}
		}

			if (levelCoord.x > 0)
			{
				ML = sameTile(getTileset(levelCoord.x - 1, levelCoord.y + 0), tileset);
			}
				MC = sameTile(getTileset(levelCoord.x + 0, levelCoord.y + 0), tileset);
			if (levelCoord.x < levelSize.x - 1)
			{
				MR = sameTile(getTileset(levelCoord.x + 1, levelCoord.y + 0), tileset);
			}

		if (levelCoord.y < levelSize.y - 1)
		{
			if (levelCoord.x > 0)
			{
				BL = sameTile(getTileset(levelCoord.x - 1, levelCoord.y + 1), tileset);
			}
				BC = sameTile(getTileset(levelCoord.x + 0, levelCoord.y + 1), tileset);
			if (levelCoord.x < levelSize.x - 1)
			{
				BR = sameTile(getTileset(levelCoord.x + 1, levelCoord.y + 1), tileset);
			}
		}

		std::vector<int> tileRegion = {TL, TC, TR,
		                               ML, MC, MR,
		                               BL, BC, BR};
		
		bool null = true;

		for (Region &region : regions)
		{
			if (equalRegion(tileRegion, region.region))
			{
				crop = region.crop;
				null = false;
				break;
			}
		}

		if (null)
		{
			crop.x = 7;
			crop.y = 5;
		}
	}
	else
	{
		crop = selectionCoord;
	}

	return sf::Vector3i(tileset, crop.x, crop.y);
}
