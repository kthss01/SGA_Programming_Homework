#pragma once

#define CELL_WIDTH 100
#define CELL_HEIGHT 50

#define RADIUS_WIDTH (CELL_WIDTH / 2)
#define RADIUS_HEIGHT (CELL_HEIGHT / 2)

// ÃÊ±â ÁÂÇ¥
#define INIT_X 400
#define INIT_Y 20

#define TILE_COUNT_X 20
#define TILE_COUNT_Y 20

#define TILE_SIZE_X 8
#define TILE_SIZE_Y 7

enum TERRAIN {
	TR_GROUND,
	TR_NONE
};

enum OBJECT {
	OBJ_BLOCK,
	OBJ_NONE
};

struct tagTile {
	TERRAIN terrain;
	OBJECT obj;

	int terrainFrameX;
	int terrainFrameY;
	int objFrameX;
	int objFrameY;
};