#ifndef TILEWORLD_H
#define TILEWORLD_H

#include "World.h"
#include "Entities.h"

#include "/home/codeleaded/System/Static/Library/AudioPlayer.h"

#define TILE_COUNT_X	8U
#define TILE_COUNT_Y	6U
#define DOCK_TILE_COUNT_X	12U
#define DOCK_TILE_COUNT_Y	5U
#define HOUSE_TILE_COUNT_X	4U
#define HOUSE_TILE_COUNT_Y	3U
#define CEIL_FLAT_TILE_COUNT_X	3U
#define CEIL_FLAT_TILE_COUNT_Y	2U
#define CEIL_NORMAL_TILE_COUNT_X	5U
#define CEIL_NORMAL_TILE_COUNT_Y	3U

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

typedef struct TileWorld {
    World world;
    Player player;
    TransformedView tv;
    AudioPlayer ap;
    Block selected;
} TileWorld;

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

TileWorld TileWorld_New(char* path_world,char* path_blocks,char* path_figures,char* path_objects){
    TileWorld tw;
	tw.player = Player_By((Vec2){ 0.0f,0.0f },ENTITY_ID_BLACKHAIR);

	tw.tv = TransformedView_New((Vec2){ GetWidth(),GetHeight() });
	TransformedView_Zoom(&tw.tv,(Vec2){ 0.05f,0.05f });
	//TransformedView_Offset(&tv,(Vec2){ -0.5f,0.0f });
	TransformedView_Focus(&tw.tv,&tw.player.e.r.p,tw.player.e.r.d);

	tw.ap = AudioPlayer_New();
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
		    Animation_Null()
	    },
        (EntityAtlas[]){
		    EntityAtlas_Make(
				path_figures,
				"Blackhair.png",
				4,4,
				1.0f,1.6f,
				(void(*)(void*,void*,const float))Player_Update,
				(void(*)(void*,const float,const float))Player_Move,
				(SubSprite(*)(void*,EntityAtlas*))Player_GetRender,
				(void(*)(void*))Player_Free,
				(void(*)(void*,World*))Player_WorldCollision,
				(char(*)(void*,World*,int32_t,int32_t,Side))Player_IsSolid,
				(void(*)(void*,World*,int32_t,int32_t,Side))Player_Collision,
				(void(*)(void*,World*,void*,int32_t,int32_t,Side))Player_EntityCollision,
				(void*(*)(DataStream*))Player_Load,
				(void(*)(void*,DataStream*))Player_Save
			),
		    EntityAtlas_Make(
				path_objects,
				"Tree.png",
				1,1,
				3.5f,4.0f,
				NULL,
				NULL,
				(SubSprite(*)(void*,EntityAtlas*))Tree_GetRender,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				(void*(*)(DataStream*))Tree_Load,
				(void(*)(void*,DataStream*))Tree_Save
			),
		    EntityAtlas_Null()
	    }
    );

	tw.selected = BLOCK_NONE;
    tw.player.e.r.p = tw.world.spawn;
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
    World_Free(&tw->world);
    Player_Free(&tw->player);
    AudioPlayer_Free(&tw->ap);
}

#endif