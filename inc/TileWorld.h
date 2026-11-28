#ifndef TILEWORLD_H
#define TILEWORLD_H

#include "World.h"
#include "Entities.h"

#include "/home/codeleaded/System/Static/Library/AudioPlayer.h"

#define TILE_COUNT_X				8U
#define TILE_COUNT_Y				6U
#define DOCK_TILE_COUNT_X			12U
#define DOCK_TILE_COUNT_Y			5U
#define HOUSE_TILE_COUNT_X			4U
#define HOUSE_TILE_COUNT_Y			3U
#define CEIL_FLAT_TILE_COUNT_X		3U
#define CEIL_FLAT_TILE_COUNT_Y		2U
#define CEIL_NORMAL_TILE_COUNT_X	5U
#define CEIL_NORMAL_TILE_COUNT_Y	3U
#define TILEWORLD_INVENTORY_SLOT_SCREEN_RATIO	0.112f
#define TILEWORLD_INVENTORY_SLOT_GAP_RATIO	0.08f
#define TILEWORLD_INVENTORY_PANEL_PAD_RATIO	0.14f

// Neighbor bits are NW, N, NE, W, E, SW, S, SE; values are row-major atlas cell IDs.
static const uint8_t TileWorld_TileLookup[256] = {
    // 0x00 - 0x0F
    0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01,
    0x02, 0x02, 0x03, 0x04, 0x02, 0x02, 0x03, 0x04,

    // 0x10 - 0x1F
    0x05, 0x05, 0x06, 0x06, 0x05, 0x05, 0x07, 0x07,
    0x08, 0x08, 0x09, 0x0A, 0x08, 0x08, 0x0B, 0x0C,

    // 0x20 - 0x2F
    0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01,
    0x02, 0x02, 0x03, 0x04, 0x02, 0x02, 0x03, 0x04,

    // 0x30 - 0x3F
    0x05, 0x05, 0x06, 0x06, 0x05, 0x05, 0x07, 0x07,
    0x08, 0x08, 0x09, 0x0A, 0x08, 0x08, 0x0B, 0x0C,

    // 0x40 - 0x4F
    0x0D, 0x0D, 0x0E, 0x0E, 0x0D, 0x0D, 0x0E, 0x0E,
    0x0F, 0x0F, 0x10, 0x11, 0x0F, 0x0F, 0x10, 0x11,

    // 0x50 - 0x5F
    0x12, 0x12, 0x13, 0x13, 0x12, 0x12, 0x14, 0x14,
    0x15, 0x15, 0x16, 0x17, 0x15, 0x15, 0x18, 0x19,

    // 0x60 - 0x6F
    0x0D, 0x0D, 0x0E, 0x0E, 0x0D, 0x0D, 0x0E, 0x0E,
    0x1A, 0x1A, 0x1B, 0x1C, 0x1A, 0x1A, 0x1B, 0x1C,

    // 0x70 - 0x7F
    0x12, 0x12, 0x13, 0x13, 0x12, 0x12, 0x14, 0x14,
    0x1D, 0x1D, 0x1E, 0x1F, 0x1D, 0x1D, 0x20, 0x21,

    // 0x80 - 0x8F
    0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01,
    0x02, 0x02, 0x03, 0x04, 0x02, 0x02, 0x03, 0x04,

    // 0x90 - 0x9F
    0x05, 0x05, 0x06, 0x06, 0x05, 0x05, 0x07, 0x07,
    0x08, 0x08, 0x09, 0x0A, 0x08, 0x08, 0x0B, 0x0C,

    // 0xA0 - 0xAF
    0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01,
    0x02, 0x02, 0x03, 0x04, 0x02, 0x02, 0x03, 0x04,

    // 0xB0 - 0xBF
    0x05, 0x05, 0x06, 0x06, 0x05, 0x05, 0x07, 0x07,
    0x08, 0x08, 0x09, 0x0A, 0x08, 0x08, 0x0B, 0x0C,

    // 0xC0 - 0xCF
    0x0D, 0x0D, 0x0E, 0x0E, 0x0D, 0x0D, 0x0E, 0x0E,
    0x0F, 0x0F, 0x10, 0x11, 0x0F, 0x0F, 0x10, 0x11,

    // 0xD0 - 0xDF
    0x22, 0x22, 0x23, 0x23, 0x22, 0x22, 0x24, 0x24,
    0x25, 0x25, 0x26, 0x27, 0x25, 0x25, 0x28, 0x29,

    // 0xE0 - 0xEF
    0x0D, 0x0D, 0x0E, 0x0E, 0x0D, 0x0D, 0x0E, 0x0E,
    0x1A, 0x1A, 0x1B, 0x1C, 0x1A, 0x1A, 0x1B, 0x1C,

    // 0xF0 - 0xFF
    0x22, 0x22, 0x23, 0x23, 0x22, 0x22, 0x24, 0x24,
    0x2A, 0x2A, 0x2B, 0x2C, 0x2A, 0x2A, 0x2D, 0x2E
};

void World_Chunk_GenerateObjects(World_Chunk* ch){
	static const Entity_ID baum_ids[] = {
		ENTITY_ID_BAUM,
		ENTITY_ID_BAUM_DARK,
		ENTITY_ID_BAUM_LIGHT,
		ENTITY_ID_BAUM_OBST,
		ENTITY_ID_PALME,
		ENTITY_ID_TANNE
	};
	const uint32_t count = Random_u32_MinMax(0U,8U);

	for(uint32_t i = 0;i<count;i++){
		const float px = Random_f64_MinMax(0.0f,WORLD_CHUNK_WIDTH);
		const float py = Random_f64_MinMax(0.0f,WORLD_CHUNK_HEIGHT);
		const uint32_t tree_index = Random_u32_MinMax(0U,(uint32_t)(sizeof(baum_ids) / sizeof(baum_ids[0])));
		World_Entity* baum = (World_Entity*)World_Entity_New((Vec2){ ch->x * WORLD_CHUNK_WIDTH + px,ch->y * WORLD_CHUNK_HEIGHT + py },baum_ids[tree_index]);
		if(baum) PVector_PPush(&ch->entities,baum);
	}

	const uint32_t stein_count = Random_u32_MinMax(0U,3U);
	
	for(uint32_t i = 0;i<stein_count;i++){
		const float px = Random_f64_MinMax(0.0f,WORLD_CHUNK_WIDTH);
		const float py = Random_f64_MinMax(0.0f,WORLD_CHUNK_HEIGHT);
		World_Stein* stein = World_Stein_New((Vec2){ ch->x * WORLD_CHUNK_WIDTH + px,ch->y * WORLD_CHUNK_HEIGHT + py });
		if(stein) PVector_PPush(&ch->entities,stein);
	}
}
World_Chunk World_Chunk_Generate(int32_t x,int32_t y,Block background){
	World_Chunk ch;
	ch.x = x;
	ch.y = y;
	ch.tags = Vector_New(sizeof(World_Chunk_Tag));
	ch.entities = PVector_New();

	for(int i = 0;i<WORLD_CHUNK_WIDTH * WORLD_CHUNK_HEIGHT;i++)
		ch.data[i] = background;

	World_Chunk_GenerateObjects(&ch);
	return ch;
}

SubSprite TileWorld_Atlas_Cell_Get(Animation* a,unsigned int tile_x,unsigned int tile_y){
	if(!a->atlas_img.img || a->atlas_cx == 0U || a->atlas_cy == 0U)
		return SubSprite_Null();

	const unsigned int dx = a->atlas_img.w / a->atlas_cx;
	const unsigned int dy = a->atlas_img.h / a->atlas_cy;
	if(dx == 0U || dy == 0U || tile_x >= a->atlas_cx || tile_y >= a->atlas_cy)
		return SubSprite_Null();

	return SubSprite_New(&a->atlas_img,tile_x * dx,tile_y * dy,dx,dy);
}
SubSprite TileWorld_AutoTile_Get_Neighbours(Animation* a,uint8_t nbs){
	const uint8_t id = TileWorld_TileLookup[nbs];

	if(a->atlas_cx == 0U || id >= (unsigned int)a->atlas_cx * a->atlas_cy)
		return SubSprite_Null();

	return TileWorld_Atlas_Cell_Get(a,id % a->atlas_cx,id / a->atlas_cx);
}
SubSprite TileWorld_AutoTile_Get(Animation* a,World* w,int32_t x,int32_t y,Block tile){
	return TileWorld_AutoTile_Get_Neighbours(a,World_GetNeigbours(w,tile,x,y));
}
SubSprite TileWorld_Gras_Get(Animation* a,World* w,int32_t x,int32_t y){
	return TileWorld_AutoTile_Get(a,w,x,y,BLOCK_GRAS);
}
SubSprite TileWorld_Water_Get(Animation* a,World* w,int32_t x,int32_t y){
	const uint8_t water_neighbors = World_GetNeigbours(w,BLOCK_WATER,x,y);
	const uint8_t dock_neighbors = World_GetNeigbours(w,BLOCK_DOCK,x,y);
	return TileWorld_AutoTile_Get_Neighbours(a,water_neighbors | dock_neighbors);
}
SubSprite TileWorld_Stone_Get(Animation* a,World* w,int32_t x,int32_t y){
	return TileWorld_AutoTile_Get(a,w,x,y,BLOCK_STONE);
}
SubSprite TileWorld_Dock_Get(Animation* a,World* w,int32_t x,int32_t y){
	const uint8_t nbs = World_GetNeigbours(w,BLOCK_DOCK,x,y);
	const uint8_t west = (nbs >> 3U) & 1U;
	const uint8_t east = (nbs >> 4U) & 1U;
	const uint8_t north = (nbs >> 1U) & 1U;
	const uint8_t south = (nbs >> 6U) & 1U;

	unsigned int tile_y;
	if(!north)
		tile_y = 0U;
	else if(!south)
		tile_y = 4U;
	else{
		int32_t phase = y % 3;
		if(phase < 0) phase += 3;
		tile_y = 1U + (unsigned int)phase;
	}

	unsigned int tile_x;
	if(!west && !east)
		tile_x = 6U;
	else if(!west)
		tile_x = 0U;
	else if(!east)
		tile_x = 11U;
	else{
		int32_t phase = x % 10;
		if(phase < 0) phase += 10;
		tile_x = 1U + (unsigned int)phase;
	}

	return TileWorld_Atlas_Cell_Get(a,tile_x,tile_y);
}

Block TileWorld_House_Family_Get(World* w,int32_t x,int32_t y){
	static const int8_t offsets[][2] = {
		{ -1,0 },{ 1,0 },{ 0,-1 },{ 0,1 },
		{ -1,-1 },{ 1,-1 },{ -1,1 },{ 1,1 }
	};

	for(unsigned int i = 0U;i<sizeof(offsets) / sizeof(offsets[0]);i++){
		const Block b = World_Get(w,x + offsets[i][0],y + offsets[i][1]);
		if(b == BLOCK_HOUSE_GRAY || b == BLOCK_HOUSE_RED)
			return b;
	}
	return BLOCK_HOUSE_GRAY;
}
Animation* TileWorld_House_Animation_Get(World* w,Block family){
	if(family != BLOCK_HOUSE_GRAY && family != BLOCK_HOUSE_RED)
		return NULL;
	return (Animation*)Vector_Get(&w->animations,(int)family);
}
int TileWorld_House_Neighbour_Is(World* w,Block family,int32_t x,int32_t y){
	const Block b = World_Get(w,x,y);
	return b == family || (b == BLOCK_DOOR && TileWorld_House_Family_Get(w,x,y) == family);
}
uint8_t TileWorld_House_Neighbours_Get(World* w,Block family,int32_t x,int32_t y){
	const uint8_t north = (uint8_t)TileWorld_House_Neighbour_Is(w,family,x,y - 1);
	const uint8_t east = (uint8_t)TileWorld_House_Neighbour_Is(w,family,x + 1,y);
	const uint8_t south = (uint8_t)TileWorld_House_Neighbour_Is(w,family,x,y + 1);
	const uint8_t west = (uint8_t)TileWorld_House_Neighbour_Is(w,family,x - 1,y);
	return (uint8_t)(north | (east << 1U) | (south << 2U) | (west << 3U));
}

// Four-neighbour mask order is N, E, S, W; values are row-major 4x3 cells.
static const uint8_t TileWorld_House_TileLookup[16] = {
	1U, 9U, 0U, 8U, 1U, 4U, 0U, 4U,
	3U, 11U, 1U, 9U, 3U, 7U, 1U, 5U
};

uint32_t TileWorld_Parity(int32_t x,int32_t y){
	return (((uint32_t)x + (uint32_t)y) & 1U);
}
SubSprite TileWorld_House_Get(Animation* a,World* w,int32_t x,int32_t y,Block family){
	const uint8_t neighbours = TileWorld_House_Neighbours_Get(w,family,x,y);
	unsigned int tile = TileWorld_House_TileLookup[neighbours];
	if(neighbours == 0x0FU && TileWorld_Parity(x,y))
		tile = 2U;
	return TileWorld_Atlas_Cell_Get(a,tile % HOUSE_TILE_COUNT_X,tile / HOUSE_TILE_COUNT_X);
}
SubSprite TileWorld_House_Gray_Get(Animation* a,World* w,int32_t x,int32_t y){
	return TileWorld_House_Get(a,w,x,y,BLOCK_HOUSE_GRAY);
}
SubSprite TileWorld_House_Red_Get(Animation* a,World* w,int32_t x,int32_t y){
	return TileWorld_House_Get(a,w,x,y,BLOCK_HOUSE_RED);
}
SubSprite TileWorld_Door_Get(Animation* a,World* w,int32_t x,int32_t y){
	(void)a;
	const Block family = TileWorld_House_Family_Get(w,x,y);
	Animation* house = TileWorld_House_Animation_Get(w,family);
	if(!house)
		return SubSprite_Null();

	const int door_above = World_Get(w,x,y - 1) == BLOCK_DOOR;
	const int door_below = World_Get(w,x,y + 1) == BLOCK_DOOR;
	const unsigned int tile_y = door_below && !door_above ? 1U : 2U;
	return TileWorld_Atlas_Cell_Get(house,2U,tile_y);
}
uint8_t TileWorld_Ceil_Neighbours_Get(World* w,Block ceiling,int32_t x,int32_t y){
	const uint8_t north = (uint8_t)(World_Get(w,x,y - 1) == ceiling);
	const uint8_t east = (uint8_t)(World_Get(w,x + 1,y) == ceiling);
	const uint8_t south = (uint8_t)(World_Get(w,x,y + 1) == ceiling);
	const uint8_t west = (uint8_t)(World_Get(w,x - 1,y) == ceiling);
	return (uint8_t)(north | (east << 1U) | (south << 2U) | (west << 3U));
}
SubSprite TileWorld_Ceil_Flat_Get(Animation* a,World* w,int32_t x,int32_t y){
	const uint8_t neighbours = TileWorld_Ceil_Neighbours_Get(w,BLOCK_CEIL_FLAT,x,y);
	const uint8_t east = neighbours & 0x02U;
	const uint8_t west = neighbours & 0x08U;
	const unsigned int tile_x = !west ? 0U : (!east ? 2U : 1U);
	return TileWorld_Atlas_Cell_Get(a,tile_x,0U);
}
SubSprite TileWorld_Ceil_Normal_Get(Animation* a,World* w,int32_t x,int32_t y){
	const uint8_t neighbours = TileWorld_Ceil_Neighbours_Get(w,BLOCK_CEIL_NORMAL,x,y);
	const uint8_t north = neighbours & 0x01U;
	const uint8_t east = neighbours & 0x02U;
	const uint8_t south = neighbours & 0x04U;
	const uint8_t west = neighbours & 0x08U;
	const int west2 = World_Get(w,x - 2,y) == BLOCK_CEIL_NORMAL;
	const int east2 = World_Get(w,x + 2,y) == BLOCK_CEIL_NORMAL;
	const unsigned int tile_x = !west ? 0U :
		(!east ? 4U : (!west2 ? 1U : (!east2 ? 3U : 2U)));
	const unsigned int tile_y = !north && south ? 0U :
		(north && !south ? 2U : 1U);
	return TileWorld_Atlas_Cell_Get(a,tile_x,tile_y);
}
SubSprite TileWorld_Chest_Get(Animation* a,World* w,int32_t x,int32_t y){
	if(!a->atlas_img.img || a->atlas_cx == 0U || a->atlas_cy == 0U)
		return SubSprite_Null();

	const unsigned int dx = a->atlas_img.w / a->atlas_cx;
	const unsigned int dy = a->atlas_img.h / a->atlas_cy;
	if(dx == 0U || dy == 0U)
		return SubSprite_Null();
	return SubSprite_New(&a->atlas_img,0U,0U,dx,dy);
}
EntityAtlas TileWorld_Item_EntityAtlas_Make(char* path_items,Item item){
	return EntityAtlas_Make(
		path_items,
		WORLD_ITEM_FILES[item],
		1,1,
		WORLD_ITEM_DIM_X,WORLD_ITEM_DIM_Y,
		NULL,NULL,
		(SubSprite(*)(void*,EntityAtlas*))World_Item_Entity_GetRender,
		NULL,NULL,NULL,NULL,NULL,
		(void*(*)(DataStream*))World_Item_Entity_Load,
		(void(*)(void*,DataStream*))World_Item_Entity_Save
	);
}

TileWorld TileWorld_New(char* path_world,char* path_blocks,char* path_figures,char* path_objects,char* path_items){
    TileWorld tw;
	memset(&tw,0,sizeof(tw));
	tw.player = World_Player_By((Vec2){ 0.0f,0.0f },ENTITY_ID_BLACKHAIR);
	CStr item_frame_path = CStr_Concat(path_items,"Itemframe.png");
	tw.item_frame = Sprite_Load(item_frame_path);
	CStr_Free(&item_frame_path);

	tw.tv = TransformedView_New((Vec2){ GetWidth(),GetHeight() });
	TransformedView_Zoom(&tw.tv,(Vec2){ 0.05f,0.05f });
	//TransformedView_Offset(&tv,(Vec2){ -0.5f,0.0f });

	tw.ap = AudioPlayer_New();
	EntityAtlas entity_atlases[ENTITY_ID_COUNT] = { 0 };
	entity_atlases[ENTITY_ID_BLACKHAIR - 1U] = EntityAtlas_Make(
		path_figures,
		"Fox6.png",
		4,6,
		2.0f,1.6f,
		(void(*)(void*,void*,const float))World_Player_Update,
		(void(*)(void*,const float,const float))World_Player_Move,
		(SubSprite(*)(void*,EntityAtlas*))World_Player_GetRender,
		(void(*)(void*))World_Player_Free,
		(void(*)(void*,World*))World_Player_WorldCollision,
		(char(*)(void*,World*,int32_t,int32_t,Side))World_Player_IsSolid,
		(void(*)(void*,World*,int32_t,int32_t,Side))World_Player_Collision,
		(void(*)(void*,World*,void*,int32_t,int32_t,Side))World_Player_EntityCollision,
		(void*(*)(DataStream*))World_Player_Load,
		(void(*)(void*,DataStream*))World_Player_Save
	);
	entity_atlases[ENTITY_ID_BAUM - 1U] = EntityAtlas_Make(
		path_objects,
		"Baum.png",
		1,1,
		3.5f,4.0f,
		NULL,NULL,
		(SubSprite(*)(void*,EntityAtlas*))World_Baum_GetRender,
		NULL,NULL,NULL,NULL,NULL,
		(void*(*)(DataStream*))World_Baum_Load,
		(void(*)(void*,DataStream*))World_Baum_Save
	);
	entity_atlases[ENTITY_ID_BAUM_DARK - 1U] = EntityAtlas_Make(
		path_objects,
		"Baum_dark.png",
		1,1,
		3.5f,4.0f,
		NULL,NULL,
		(SubSprite(*)(void*,EntityAtlas*))World_Baum_Dark_GetRender,
		NULL,NULL,NULL,NULL,NULL,
		(void*(*)(DataStream*))World_Baum_Dark_Load,
		(void(*)(void*,DataStream*))World_Baum_Dark_Save
	);
	entity_atlases[ENTITY_ID_BAUM_LIGHT - 1U] = EntityAtlas_Make(
		path_objects,
		"Baum_light.png",
		1,1,
		3.5f,4.0f,
		NULL,NULL,
		(SubSprite(*)(void*,EntityAtlas*))World_Baum_Light_GetRender,
		NULL,NULL,NULL,NULL,NULL,
		(void*(*)(DataStream*))World_Baum_Light_Load,
		(void(*)(void*,DataStream*))World_Baum_Light_Save
	);
	entity_atlases[ENTITY_ID_BAUM_OBST - 1U] = EntityAtlas_Make(
		path_objects,
		"Baum_obst.png",
		1,1,
		3.5f,4.0f,
		NULL,NULL,
		(SubSprite(*)(void*,EntityAtlas*))World_Baum_Obst_GetRender,
		NULL,NULL,NULL,NULL,NULL,
		(void*(*)(DataStream*))World_Baum_Obst_Load,
		(void(*)(void*,DataStream*))World_Baum_Obst_Save
	);
	entity_atlases[ENTITY_ID_PALME - 1U] = EntityAtlas_Make(
		path_objects,
		"Palme.png",
		1,1,
		3.5f,4.0f,
		NULL,NULL,
		(SubSprite(*)(void*,EntityAtlas*))World_Palme_GetRender,
		NULL,NULL,NULL,NULL,NULL,
		(void*(*)(DataStream*))World_Palme_Load,
		(void(*)(void*,DataStream*))World_Palme_Save
	);
	entity_atlases[ENTITY_ID_STEIN - 1U] = EntityAtlas_Make(
		path_objects,
		"Stein.png",
		1,1,
		2.5f,2.4f,
		NULL,NULL,
		(SubSprite(*)(void*,EntityAtlas*))World_Stein_GetRender,
		NULL,NULL,NULL,NULL,NULL,
		(void*(*)(DataStream*))World_Stein_Load,
		(void(*)(void*,DataStream*))World_Stein_Save
	);
	entity_atlases[ENTITY_ID_TANNE - 1U] = EntityAtlas_Make(
		path_objects,
		"Tanne.png",
		1,1,
		3.5f,4.0f,
		NULL,NULL,
		(SubSprite(*)(void*,EntityAtlas*))World_Tanne_GetRender,
		NULL,NULL,NULL,NULL,NULL,
		(void*(*)(DataStream*))World_Tanne_Load,
		(void(*)(void*,DataStream*))World_Tanne_Save
	);
	for(Item item = ITEM_HOLZ;item < ITEM_COUNT;item++){
		const Entity_ID entity_id = World_Item_Entity_ID(item);
		entity_atlases[entity_id - 1U] = TileWorld_Item_EntityAtlas_Make(path_items,item);
	}
	tw.world = World_Make(path_world,
        (Animation[]){
		    Animation_Make_SpritePath(path_blocks,"Dirt.png"),
		    Animation_Make_AtlasPath(path_blocks,"Gras.png",TILE_COUNT_X,TILE_COUNT_Y,TileWorld_Gras_Get),
		    Animation_Make_AtlasPath(path_blocks,"Water.png",TILE_COUNT_X,TILE_COUNT_Y,TileWorld_Water_Get),
		    Animation_Make_AtlasPath(path_blocks,"Stone.png",TILE_COUNT_X,TILE_COUNT_Y,TileWorld_Stone_Get),
		    Animation_Make_AtlasPath(path_blocks,"Dock_Tiles.png",DOCK_TILE_COUNT_X,DOCK_TILE_COUNT_Y,TileWorld_Dock_Get),
		    Animation_Make_AtlasPath(path_blocks,"GreyHouse.png",HOUSE_TILE_COUNT_X,HOUSE_TILE_COUNT_Y,TileWorld_House_Gray_Get),
		    Animation_Make_AtlasPath(path_blocks,"RedHouse.png",HOUSE_TILE_COUNT_X,HOUSE_TILE_COUNT_Y,TileWorld_House_Red_Get),
		    Animation_Make_AtlasPath(path_blocks,"Chests.png",2U,3U,TileWorld_Chest_Get),
		    Animation_Make_AtlasPath(path_blocks,"GreyHouse.png",HOUSE_TILE_COUNT_X,HOUSE_TILE_COUNT_Y,TileWorld_Door_Get),
		    Animation_Make_AtlasPath(path_blocks,"CeilFlat.png",CEIL_FLAT_TILE_COUNT_X,CEIL_FLAT_TILE_COUNT_Y,TileWorld_Ceil_Flat_Get),
		    Animation_Make_AtlasPath(path_blocks,"CeilNormal.png",CEIL_NORMAL_TILE_COUNT_X,CEIL_NORMAL_TILE_COUNT_Y,TileWorld_Ceil_Normal_Get),
		    Animation_Make_SpritePath(path_blocks,"Plank_Shadow.png"),
		    Animation_Make_SpritePath(path_blocks,"Plank.png"),
		    Animation_Null()
	    },
		entity_atlases
    );

	tw.selected = BLOCK_NONE;
    tw.player.e.r.p = tw.world.spawn;
	World_Player_LoadInventoryFile(&tw.player,&tw.world);
    return tw;
}
void TileWorld_AudioPlayerStart(TileWorld* tw){
    AudioPlayer_Start(&tw->ap);
}
void TileWorld_RenderEntityPreview(Pixel* out,unsigned int width,unsigned int height,SubSprite ss,float x,float y,float box_width,float box_height){
	if(!out || !ss.sp || !ss.sp->img || ss.dx <= 0.0f || ss.dy <= 0.0f || box_width <= 0.0f || box_height <= 0.0f)
		return;

	Rect source;
	if(!Rect_Clip(&source,ss.ox,ss.oy,ss.dx,ss.dy,0.0f,0.0f,(float)ss.sp->w,(float)ss.sp->h))
		return;

	const int32_t source_x = (int32_t)floorf(source.p.x);
	const int32_t source_y = (int32_t)floorf(source.p.y);
	const int32_t source_width = (int32_t)ceilf(source.p.x + source.d.x) - source_x;
	const int32_t source_height = (int32_t)ceilf(source.p.y + source.d.y) - source_y;
	const int32_t target_width = (int32_t)floorf(box_width);
	const int32_t target_height = (int32_t)floorf(box_height);
	if(source_width <= 0 || source_height <= 0 || target_width <= 0 || target_height <= 0)
		return;

	const float scale = fminf((float)target_width / source_width,(float)target_height / source_height);
	const int32_t draw_width = (int32_t)floorf(source_width * scale);
	const int32_t draw_height = (int32_t)floorf(source_height * scale);
	if(draw_width <= 0 || draw_height <= 0)
		return;

	const int32_t draw_x = (int32_t)floorf(x + (target_width - draw_width) * 0.5f);
	const int32_t draw_y = (int32_t)floorf(y + (target_height - draw_height) * 0.5f);
	for(int32_t dy = 0;dy<draw_height;dy++){
		const int32_t target_y = draw_y + dy;
		if(target_y < 0 || target_y >= (int32_t)height) continue;
		const int32_t sample_y = source_y + (int32_t)(((float)dy + 0.5f) * source_height / draw_height);

		for(int32_t dx = 0;dx<draw_width;dx++){
			const int32_t target_x = draw_x + dx;
			if(target_x < 0 || target_x >= (int32_t)width) continue;
			const int32_t sample_x = source_x + (int32_t)(((float)dx + 0.5f) * source_width / draw_width);
			const Pixel pixel = ss.sp->img[sample_y * (int32_t)ss.sp->w + sample_x];
			const int alpha = Pixel_A(pixel);
			if(alpha == 0) continue;

			Pixel* target = out + target_y * width + target_x;
			*target = alpha == 255 ? pixel : Pixel_Alpha(pixel,*target);
		}
	}
}
void TileWorld_Inventory_RenderCount(Pixel* out,unsigned int width,unsigned int height,uint32_t count,float x,float y,float slot_size){
	AlxFont* font = GetAlxFont();
	if(!font || !font->Atlas.img || font->Columns <= 0 || font->Rows <= 0 ||
	   font->CharSizeX <= 0 || font->CharSizeY <= 0)
		return;

	char text[16];
	snprintf(text,sizeof(text),"%u",count);
	const float glyph_width = slot_size * 0.22f;
	const float glyph_height = slot_size * 0.30f;
	const float pad = slot_size * 0.04f;
	const float start_x = x + slot_size - pad - (float)strlen(text) * glyph_width;
	const float start_y = y + slot_size - glyph_height - pad;
	for(unsigned int i = 0U;text[i] != '\0';i++){
		const unsigned int character = (unsigned int)(unsigned char)text[i];
		const unsigned int column = character % (unsigned int)font->Columns;
		const unsigned int row = character / (unsigned int)font->Rows;
		SubSprite glyph = SubSprite_New(
			&font->Atlas,
			column * (unsigned int)font->CharSizeX,
			row * (unsigned int)font->CharSizeY,
			(unsigned int)font->CharSizeX,
			(unsigned int)font->CharSizeY
		);
		TileWorld_RenderEntityPreview(out,width,height,glyph,start_x + i * glyph_width,start_y,glyph_width,glyph_height);
	}
}
World_Inventory* TileWorld_Inventory_Chest_Get(TileWorld* tw){
	if(!tw->world.inventory_has_chest) return NULL;
	World_Inventory* inventory = World_Chest_Inventory_Get(&tw->world,tw->world.inventory_chest_x,tw->world.inventory_chest_y);
	const float dx = (float)tw->world.inventory_chest_x + 0.5f - tw->player.e.r.p.x;
	const float dy = (float)tw->world.inventory_chest_y + 0.5f - tw->player.e.r.p.y;
	if(!inventory || dx * dx + dy * dy > 2.25f){
		tw->world.inventory_has_chest = 0U;
		tw->world.inventory_scroll_chest = 0U;
		return NULL;
	}
	return inventory;
}
void TileWorld_Inventory_Toggle(TileWorld* tw){
	if(tw->world.inventory){
		tw->world.inventory = 0U;
		tw->world.inventory_has_chest = 0U;
		return;
	}

	tw->world.inventory = 1U;
	tw->world.inventory_scroll_world_player = 0U;
	tw->world.inventory_scroll_chest = 0U;
	tw->world.inventory_has_chest = 0U;

	const int32_t world_player_x = (int32_t)floorf(tw->player.e.r.p.x);
	const int32_t world_player_y = (int32_t)floorf(tw->player.e.r.p.y);
	float nearest_distance = 2.25f;
	for(int32_t y = world_player_y - 2;y<=world_player_y + 2;y++){
		for(int32_t x = world_player_x - 2;x<=world_player_x + 2;x++){
			if(!World_Chest_Inventory_Get(&tw->world,x,y)) continue;
			const float dx = (float)x + 0.5f - tw->player.e.r.p.x;
			const float dy = (float)y + 0.5f - tw->player.e.r.p.y;
			const float distance = dx * dx + dy * dy;
			if(distance <= nearest_distance){
				nearest_distance = distance;
				tw->world.inventory_chest_x = x;
				tw->world.inventory_chest_y = y;
				tw->world.inventory_has_chest = 1U;
			}
		}
	}
}
uint32_t TileWorld_Inventory_LayoutColumns(TileWorld* tw){
	uint32_t columns = tw->player.inventory.width;
	if(columns < WORLD_PLAYER_INVENTORY_WIDTH) columns = WORLD_PLAYER_INVENTORY_WIDTH;
	if(tw->world.inventory_has_chest){
		World_Inventory* chest = TileWorld_Inventory_Chest_Get(tw);
		if(chest && columns < chest->width) columns = chest->width;
	}
	return columns;
}
float TileWorld_Inventory_SlotSize(unsigned int width,unsigned int height,uint32_t columns){
	if(width == 0U || height == 0U) return 0.0f;
	if(columns == 0U) columns = WORLD_PLAYER_INVENTORY_WIDTH;

	const float desired_size = (float)(width < height ? width : height) * TILEWORLD_INVENTORY_SLOT_SCREEN_RATIO;
	const float margin = fminf((float)width * 0.02f,16.0f);
	const float available_width = F32_Max(1.0f,(float)width - 2.0f * margin);
	const float max_columns = (float)columns + ((float)columns - 1.0f) * TILEWORLD_INVENTORY_SLOT_GAP_RATIO;
	return F32_Max(1.0f,F32_Min(desired_size,available_width / max_columns));
}
float TileWorld_Inventory_PanelPad(float slot_size){
	return F32_Max(4.0f,slot_size * TILEWORLD_INVENTORY_PANEL_PAD_RATIO);
}
float TileWorld_Inventory_TitleHeight(void){
	AlxFont* font = GetAlxFont();
	return font && font->CharSizeY > 0 ? (float)font->CharSizeY : 0.0f;
}
uint32_t TileWorld_Inventory_VisibleRows(unsigned int width,unsigned int height,uint32_t columns){
	const float panel_height = (float)(height / 2U);
	const float slot_size = TileWorld_Inventory_SlotSize(width,height,columns);
	if(slot_size <= 0.0f) return 0U;
	const float gap = slot_size * TILEWORLD_INVENTORY_SLOT_GAP_RATIO;
	const float pad = TileWorld_Inventory_PanelPad(slot_size);
	const float reserved = 2.0f * pad + TileWorld_Inventory_TitleHeight();
	if(panel_height <= reserved) return 0U;
	return (uint32_t)floorf((panel_height - reserved + gap) / (slot_size + gap));
}
uint32_t TileWorld_Inventory_RowCount(World_Inventory* inventory){
	if(!inventory || inventory->width == 0U) return 0U;
	return inventory->size / inventory->width + (inventory->size % inventory->width != 0U);
}
uint32_t TileWorld_Inventory_MaxScroll(World_Inventory* inventory,unsigned int width,unsigned int height,uint32_t columns){
	const uint32_t rows = TileWorld_Inventory_RowCount(inventory);
	const uint32_t visible_rows = TileWorld_Inventory_VisibleRows(width,height,columns);
	return rows > visible_rows ? rows - visible_rows : 0U;
}
void TileWorld_Inventory_Scroll(TileWorld* tw,int direction,unsigned int mouse_y,unsigned int width,unsigned int height){
	if(!tw->world.inventory || direction == 0) return;
	if(!tw->world.inventory_has_chest && mouse_y >= height / 2U) return;
	World_Inventory* inventory = &tw->player.inventory;
	uint32_t* scroll = &tw->world.inventory_scroll_world_player;
	if(tw->world.inventory_has_chest && mouse_y >= height / 2U){
		inventory = TileWorld_Inventory_Chest_Get(tw);
		scroll = &tw->world.inventory_scroll_chest;
	}
	if(!inventory) return;

	const uint32_t columns = TileWorld_Inventory_LayoutColumns(tw);
	const uint32_t max_scroll = TileWorld_Inventory_MaxScroll(inventory,width,height,columns);
	if(direction < 0 && *scroll > 0U)
		(*scroll)--;
	else if(direction > 0 && *scroll < max_scroll)
		(*scroll)++;
}
void TileWorld_Inventory_SlotPosition(World_Inventory* inventory,unsigned int width,unsigned int height,uint32_t layout_columns,unsigned int panel,unsigned int row,unsigned int column,float* x,float* y,float* slot_size){
	const unsigned int panel_top = panel ? height / 2U : 0U;
	const float size = TileWorld_Inventory_SlotSize(width,height,layout_columns);
	const float gap = size * TILEWORLD_INVENTORY_SLOT_GAP_RATIO;
	const float pad = TileWorld_Inventory_PanelPad(size);
	const float pitch = size + gap;
	const float grid_width = inventory->width * size + (inventory->width - 1U) * gap;
	const float left = grid_width <= width ? ((float)width - grid_width) * 0.5f : 0.0f;
	if(x) *x = left + column * pitch;
	if(y) *y = (float)panel_top + 2.0f * pad + TileWorld_Inventory_TitleHeight() + row * pitch;
	if(slot_size) *slot_size = size;
}
void TileWorld_Inventory_RenderScrollbar(Pixel* out,unsigned int width,unsigned int height,World_Inventory* inventory,uint32_t scroll,unsigned int panel,uint32_t layout_columns,uint32_t visible_rows,float slot_size){
	const uint32_t rows = TileWorld_Inventory_RowCount(inventory);
	if(!out || visible_rows == 0U || rows <= visible_rows) return;

	float grid_x;
	float track_y;
	TileWorld_Inventory_SlotPosition(inventory,width,height,layout_columns,panel,0U,0U,&grid_x,&track_y,NULL);
	const float gap = slot_size * TILEWORLD_INVENTORY_SLOT_GAP_RATIO;
	const float pitch = slot_size + gap;
	const float grid_width = inventory->width * slot_size + (inventory->width - 1U) * gap;
	const float bar_width = F32_Max(5.0f,slot_size * 0.08f);
	const float bar_gap = F32_Max(3.0f,slot_size * 0.04f);
	const float track_x = grid_x + grid_width + bar_gap;
	const float track_height = visible_rows * pitch - gap;
	if(track_height <= 0.0f || track_x + bar_width > (float)width) return;

	const float thumb_height = F32_Min(track_height,F32_Max(bar_width * 2.0f,track_height * (float)visible_rows / (float)rows));
	const float thumb_travel = track_height - thumb_height;
	const uint32_t max_scroll = rows - visible_rows;
	const float scroll_ratio = max_scroll ? (float)scroll / (float)max_scroll : 0.0f;
	const float thumb_y = track_y + thumb_travel * scroll_ratio;

	Rect_RenderXXAlpha(out,(int)width,(int)height,track_x,track_y,bar_width,track_height,Pixel_RGBA(35U,35U,35U,210U));
	Rect_RenderXX(out,(int)width,(int)height,track_x,thumb_y,bar_width,thumb_height,Pixel_RGBA(230U,230U,230U,255U));
}
void TileWorld_Inventory_ClickPanel(TileWorld* tw,World_Inventory* inventory,uint32_t* scroll,unsigned int panel,unsigned int mx,unsigned int my,unsigned int width,unsigned int height,uint32_t layout_columns,char right_click){
	if(!inventory || !scroll || inventory->width == 0U) return;
	const uint32_t visible_rows = TileWorld_Inventory_VisibleRows(width,height,layout_columns);
	const uint32_t max_scroll = TileWorld_Inventory_MaxScroll(inventory,width,height,layout_columns);
	if(*scroll > max_scroll) *scroll = max_scroll;

	for(uint32_t row = 0U;row<visible_rows;row++){
		for(uint32_t column = 0U;column<inventory->width;column++){
			const uint32_t index = (*scroll + row) * inventory->width + column;
			if(index >= inventory->size) continue;

			float x;
			float y;
			float slot_size;
			TileWorld_Inventory_SlotPosition(inventory,width,height,layout_columns,panel,row,column,&x,&y,&slot_size);
			if(x < 0.0f || x + slot_size > width) continue;
			if((float)mx >= x && (float)mx < x + slot_size &&
			   (float)my >= y && (float)my < y + slot_size){
				if(right_click)
					World_Inventory_RightClick(inventory,index,&tw->player.hand);
				else
					World_Inventory_LeftClick(inventory,index,&tw->player.hand);
			}
		}
	}
}
void TileWorld_Inventory_ClickButton(TileWorld* tw,unsigned int mx,unsigned int my,unsigned int width,unsigned int height,char right_click){
	if(!tw->world.inventory) return;
	const uint32_t columns = TileWorld_Inventory_LayoutColumns(tw);
	if(my < height / 2U){
		TileWorld_Inventory_ClickPanel(tw,&tw->player.inventory,&tw->world.inventory_scroll_world_player,0U,mx,my,width,height,columns,right_click);
	}else if(tw->world.inventory_has_chest){
		World_Inventory* chest = TileWorld_Inventory_Chest_Get(tw);
		TileWorld_Inventory_ClickPanel(tw,chest,&tw->world.inventory_scroll_chest,1U,mx,my,width,height,columns,right_click);
	}
}
void TileWorld_Inventory_Click(TileWorld* tw,unsigned int mx,unsigned int my,unsigned int width,unsigned int height){
	TileWorld_Inventory_ClickButton(tw,mx,my,width,height,0);
}
void TileWorld_Inventory_ClickRight(TileWorld* tw,unsigned int mx,unsigned int my,unsigned int width,unsigned int height){
	TileWorld_Inventory_ClickButton(tw,mx,my,width,height,1);
}
void TileWorld_Inventory_RenderStack(TileWorld* tw,Pixel* out,unsigned int width,unsigned int height,World_Item_Stack* stack,float x,float y,float slot_size){
	if(!stack || stack->id == ITEM_NONE) return;
	if(stack->id < ITEM_COUNT){
		const World_Entity item_entity = { .id = World_Item_Entity_ID(stack->id) };
		EntityAtlas* ea = World_EntityAtlas_Get(&tw->world,&item_entity);
		if(ea && ea->atlas.img)
			TileWorld_RenderEntityPreview(out,width,height,SubSprite_New(&ea->atlas,0U,0U,ea->atlas.w,ea->atlas.h),x,y,slot_size,slot_size);
	}
	if(World_Item_MaxStack(stack->id) > 1U && stack->count > 1U)
		TileWorld_Inventory_RenderCount(out,width,height,stack->count,x,y,slot_size);
}
void TileWorld_Inventory_RenderPanel(TileWorld* tw,Pixel* out,unsigned int width,unsigned int height,World_Inventory* inventory,uint32_t* scroll,unsigned int panel,char* label,uint32_t layout_columns){
	if(!inventory || !scroll || inventory->width == 0U || !tw->item_frame.img) return;
	const uint32_t visible_rows = TileWorld_Inventory_VisibleRows(width,height,layout_columns);
	const uint32_t max_scroll = TileWorld_Inventory_MaxScroll(inventory,width,height,layout_columns);
	if(*scroll > max_scroll) *scroll = max_scroll;
	const unsigned int panel_top = panel ? height / 2U : 0U;
	const float size = TileWorld_Inventory_SlotSize(width,height,layout_columns);
	const float pad = TileWorld_Inventory_PanelPad(size);
	CStr_RenderAlxFontf(out,width,height,GetAlxFont(),pad,(float)panel_top + pad,WHITE,"%s",label);

	for(uint32_t row = 0U;row<visible_rows;row++){
		for(uint32_t column = 0U;column<inventory->width;column++){
			const uint32_t index = (*scroll + row) * inventory->width + column;
			if(index >= inventory->size) continue;

			float x;
			float y;
			float slot_size;
			TileWorld_Inventory_SlotPosition(inventory,width,height,layout_columns,panel,row,column,&x,&y,&slot_size);
			if(x < 0.0f || x + slot_size > width) continue;
			TileWorld_RenderEntityPreview(out,width,height,SubSprite_New(&tw->item_frame,0U,0U,tw->item_frame.w,tw->item_frame.h),x,y,slot_size,slot_size);
			TileWorld_Inventory_RenderStack(tw,out,width,height,&inventory->slots[index],x,y,slot_size);
		}
	}
	TileWorld_Inventory_RenderScrollbar(out,width,height,inventory,*scroll,panel,layout_columns,visible_rows,size);
}
void TileWorld_Inventory_Render(TileWorld* tw,Pixel* out,unsigned int width,unsigned int height,unsigned int mx,unsigned int my){
	if(!tw->world.inventory) return;
	Rect_RenderXXAlpha(out,(int)width,(int)height,0.0f,0.0f,(float)width,(float)height,Pixel_RGBA(0U,0U,0U,128U));
	const uint32_t columns = TileWorld_Inventory_LayoutColumns(tw);
	TileWorld_Inventory_RenderPanel(tw,out,width,height,&tw->player.inventory,&tw->world.inventory_scroll_world_player,0U,"Inventory (E to close)",columns);
	if(tw->world.inventory_has_chest){
		World_Inventory* chest = TileWorld_Inventory_Chest_Get(tw);
		TileWorld_Inventory_RenderPanel(tw,out,width,height,chest,&tw->world.inventory_scroll_chest,1U,"Chest",columns);
	}
	const float slot_size = TileWorld_Inventory_SlotSize(width,height,columns);
	TileWorld_Inventory_RenderStack(tw,out,width,height,&tw->player.hand,(float)mx - slot_size * 0.5f,(float)my - slot_size * 0.5f,slot_size);
}
void TileWorld_RenderDbg(TileWorld* tw,Pixel* out,unsigned int width,unsigned int height,unsigned int mx,unsigned int my){
	if(tw->world.mode == ANIMATIONBG_DG){
		const Vec2 wpos = TransformedView_ScreenWorldPos(&tw->tv,(Vec2){ mx,my });
		const Vec2 spos = TransformedView_WorldScreenPos(&tw->tv,(Vec2){ floorf(wpos.x),floorf(wpos.y) });
		const Vec2 slen = TransformedView_WorldScreenLength(&tw->tv,(Vec2){ 1.0f,1.0f });
		Rect_RenderXWire(out,width,height,spos,slen,RED,1.0f);

		const int preshow = 7;
		const Vec2 sd = TransformedView_WorldScreenLength(&tw->tv,(Vec2){ 1.0f,1.0f });
		const float x = 10.0f;
		Rect_RenderXX(out,width,height,(preshow >> 1) * (sd.x * 1.1f),0.0f,sd.x + 10.0f,sd.y + 10.0f,RED);

		for(int i = 0;i<preshow;i++){
			const int64_t selection = (int64_t)tw->selected + i - (preshow >> 1);
			if(selection < 0 || selection > UINT32_MAX) continue;
			const uint32_t b = (uint32_t)selection;

			if(!tw->world.settype){
				if(World_isBlock(&tw->world,b)){
					SubSprite ss = World_Img(&tw->world,b,0.0f,0.0f);
					if(ss.sp) Sprite_RenderSubAlpha(out,width,height,ss.sp,x + i * (ss.dx * 1.1f),5.0f,ss.ox,ss.oy,ss.dx,ss.dy);
				}
			}else{
				if(b == ENTITY_ID_NONE || b > tw->world.entityatlas.size) continue;

				World_Entity* preview = (World_Entity*)World_Entity_New((Vec2){ 0.0f,0.0f },(Entity_ID)b);
				if(!preview) continue;

				EntityAtlas* ea = World_EntityAtlas_Get(&tw->world,preview);
				if(ea && ea->GetRender){
					SubSprite ss = ea->GetRender(preview,ea);

					if(ss.sp && ss.dx > 0.0f && ss.dy > 0.0f)
						TileWorld_RenderEntityPreview(out,width,height,ss,x + i * (sd.x * 1.1f),5.0f,sd.x,sd.y);
				}
				if(ea && ea->Free) ea->Free(preview);
				free(preview);
			}
		}
	}
}
void TileWorld_Free(TileWorld* tw){
	World_Player_SaveInventoryFile(&tw->player,&tw->world);
    World_Free(&tw->world);
    World_Player_Free(&tw->player);
	Sprite_Free(&tw->item_frame);
    AudioPlayer_Free(&tw->ap);
}

#endif