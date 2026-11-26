#ifndef WORLD_H
#define WORLD_H

#include "/home/codeleaded/System/Static/Library/WindowEngine.h"
#include "/home/codeleaded/System/Static/Library/Files.h"
#include "/home/codeleaded/System/Static/Library/TransformedView.h"
#include "/home/codeleaded/System/Static/Library/Geometry.h"
#include "/home/codeleaded/System/Static/Library/Integer.h"

#include "/home/codeleaded/System/Static/Container/DataStream.h"
#include "/home/codeleaded/System/Static/Container/PVector.h"

#define ENTITY_NONE							0U

#define BLOCK_BOUNDS					255U
#define BLOCK_NONE						0U
#define BLOCK_DIRT						1U
#define BLOCK_GRAS						2U
#define BLOCK_STONE						3U
typedef unsigned char Block;

typedef unsigned char AnimationType;
typedef unsigned char AnimationBg;
typedef unsigned short SpawnType;

#define ANIMATIONTYPE_NONE 					0
#define ANIMATIONTYPE_SPRITE 				1
#define ANIMATIONTYPE_ATLAS   				2
#define ANIMATIONTYPE_SPRITES   			3
#define ANIMATIONTYPE_ANIMATIONATLAS   		4
#define ANIMATIONTYPE_ANIMATION   			5

#define ANIMATIONBG_NONE 					0
#define ANIMATIONBG_BG 						1
#define ANIMATIONBG_FG 						2
#define ANIMATIONBG_ALL 					3
#define ANIMATIONBG_DG 						4

#define WORLD_NEIGHBOUR_NONE 				0
#define WORLD_NEIGHBOUR_TOP_LEFT 			1
#define WORLD_NEIGHBOUR_TOP_MID 			2
#define WORLD_NEIGHBOUR_TOP_RIGHT 			3
#define WORLD_NEIGHBOUR_MID_LEFT 			4
#define WORLD_NEIGHBOUR_MID_RIGHT 			5
#define WORLD_NEIGHBOUR_BOTTOM_LEFT 		6
#define WORLD_NEIGHBOUR_BOTTOM_MID 			7
#define WORLD_NEIGHBOUR_BOTTOM_RIGHT 		8

unsigned char World_Neighbour_Mask(unsigned char* bits){
	unsigned char mask = 0U;
	for(int i = 0;bits[i] != WORLD_NEIGHBOUR_NONE && bits[i] < 9;i++){
		mask |= 1 << (bits[i] - 1);
	}
	return mask;
}

typedef struct TargetRect {
	Rect r;
	Vec2* t;
} TargetRect;

int TargetRect_Compare(const void* e1,const void* e2) {
	TargetRect* r1 = (TargetRect*)e1;
	TargetRect* r2 = (TargetRect*)e2;
    float d1 = Vec2_Mag(Vec2_Sub(r1->r.p,*r1->t));
    float d2 = Vec2_Mag(Vec2_Sub(r2->r.p,*r2->t));
    return d1 == d2 ? 0 : (d1 < d2 ? -1 : 1);
}



typedef struct World World;

typedef struct Animation {
	AnimationType type;
	SpawnType spawner;
	union {
		struct{
			Sprite sprite_img;
		};
		struct{
			unsigned short atlas_cx;
			unsigned short atlas_cy;
			Sprite atlas_img;
			SubSprite (*atlas_get)(struct Animation*,World*,unsigned int,unsigned int);
		};
		struct{
			Vector sprites_img;
			SubSprite (*sprites_get)(struct Animation*,World*,unsigned int,unsigned int);
		};
		struct{
			unsigned short aatlas_cx;
			unsigned short aatlas_cy;
			Sprite aatlas_img;
			Timepoint aatlas_start;
			FDuration aatlas_duration;
		};
		struct{
			Vector animation_img;
			Timepoint animation_start;
			FDuration animation_duration;
		};
	};
} Animation;

FDuration Animation_Phase(Timepoint start,FDuration frequency){
	if(frequency <= 0.0 || !isfinite(frequency)) return 0.0;

	const Timepoint now = Time_Nano();
	const FDuration elapsed = (FDuration)(now - start) / (FDuration)TIME_NANOTOSEC;
	FDuration phase = fmod(elapsed * frequency,1.0);
	if(phase < 0.0) phase += 1.0;
	return phase;
}

Animation Animation_New(AnimationType type){
	Animation a;
	memset(&a,0,sizeof(a));
	a.type = type;
	return a;
}
Animation Animation_Make_Sprite(char* Path){
	Animation a;
	a.type = ANIMATIONTYPE_SPRITE;
	a.sprite_img = Sprite_Load(Path);
	return a;
}
Animation Animation_Make_Atlas(char* Path,unsigned short cx,unsigned short cy,SubSprite (*atlas_get)(struct Animation*,World*,unsigned int,unsigned int)){
	Animation a;
	a.type = ANIMATIONTYPE_ATLAS;
	a.atlas_img = Sprite_Load(Path);
	a.atlas_cx = cx;
	a.atlas_cy = cy;
	a.atlas_get = atlas_get;
	return a;
}
Animation Animation_Make_Sprites(char** Paths,SubSprite (*sprites_get)(struct Animation*,World*,unsigned int,unsigned int)){
	Animation a;
	a.type = ANIMATIONTYPE_SPRITES;
	a.sprites_img = Vector_New(sizeof(Sprite));
	a.sprites_get = sprites_get;

	for(int i = 0;Paths[i];i++){
		Vector_Push(&a.sprites_img,(Sprite[]){ Sprite_Load(Paths[i]) });
	}

	return a;
}
Animation Animation_Make_AnimationAtlas(char* Path,unsigned short cx,unsigned short cy,FDuration aatlas_duration){
	Animation a;
	a.type = ANIMATIONTYPE_ANIMATIONATLAS;
	a.aatlas_img = Sprite_Load(Path);
	a.aatlas_cx = cx;
	a.aatlas_cy = cy;
	a.aatlas_start = Time_Nano();
	a.aatlas_duration = aatlas_duration;
	return a;
}
Animation Animation_Make_Animation(char** Paths,FDuration animation_duration){
	Animation a;
	a.type = ANIMATIONTYPE_ANIMATION;
	a.animation_img = Vector_New(sizeof(Sprite));
	a.animation_start = Time_Nano();
	a.animation_duration = animation_duration;

	for(int i = 0;Paths[i];i++){
		Vector_Push(&a.animation_img,(Sprite[]){ Sprite_Load(Paths[i]) });
	}

	return a;
}

Animation Animation_Make_SpritePath(char* path,char* file){
	CStr fullpath = CStr_Concat(path,file);
	Animation a = Animation_Make_Sprite(fullpath);
	CStr_Free(&fullpath);
	return a;
}
Animation Animation_Make_AtlasPath(char* path,char* file,unsigned short cx,unsigned short cy,SubSprite (*atlas_get)(struct Animation*,World*,unsigned int,unsigned int)){
	CStr fullpath = CStr_Concat(path,file);
	Animation a = Animation_Make_Atlas(fullpath,cx,cy,atlas_get);
	CStr_Free(&fullpath);
	return a;
}
Animation Animation_Make_SpritesPath(char* path,char** files,SubSprite (*sprites_get)(struct Animation*,World*,unsigned int,unsigned int)){
	Vector fullpaths = Vector_New(sizeof(CStr));
	for(int i = 0;files[i];i++)
		Vector_Push(&fullpaths,(CStr[]){ CStr_Concat(path,files[i]) });
	Vector_Push(&fullpaths,(CStr){ NULL });
	
	Animation a = Animation_Make_Sprites((CStr*)fullpaths.Memory,sprites_get);
	
	for(int i = 0;i<fullpaths.size - 1;i++)
		CStr_Free((CStr*)Vector_Get(&fullpaths,i));
	Vector_Free(&fullpaths);
	
	return a;
}
Animation Animation_Make_AnimationAtlasPath(char* path,char* file,unsigned short cx,unsigned short cy,FDuration aatlas_duration){
	CStr fullpath = CStr_Concat(path,file);
	Animation a = Animation_Make_AnimationAtlas(fullpath,cx,cy,aatlas_duration);
	CStr_Free(&fullpath);
	return a;
}
Animation Animation_Make_AnimationPath(char* path,char** files,FDuration animation_duration){
	Vector fullpaths = Vector_New(sizeof(CStr));
	for(int i = 0;files[i];i++)
		Vector_Push(&fullpaths,(CStr[]){ CStr_Concat(path,files[i]) });
	Vector_Push(&fullpaths,(CStr){ NULL });
	
	Animation a = Animation_Make_Animation((CStr*)fullpaths.Memory,animation_duration);
	
	for(int i = 0;i<fullpaths.size - 1;i++)
		CStr_Free((CStr*)Vector_Get(&fullpaths,i));
	Vector_Free(&fullpaths);
	return a;
}

Animation Animation_Null(){
	Animation a;
	memset(&a,0,sizeof(a));
	return a;
}
void Animation_Resize(Animation* a,unsigned int width,unsigned int height){
	switch(a->type){
		case ANIMATIONTYPE_NONE:
			break;
		case ANIMATIONTYPE_SPRITE:{
			if(a->sprite_img.w != width || a->sprite_img.h != height){
				Sprite_Reload(&a->sprite_img,width,height);
			}
			break;
		}
		case ANIMATIONTYPE_ATLAS:{
			const unsigned int w = a->atlas_img.w / a->atlas_cx;
			const unsigned int h = a->atlas_img.h / a->atlas_cy;
			if(w != width || h != height){
				Sprite_Reload(&a->atlas_img,width * a->atlas_cx,height * a->atlas_cy);
			}
			break;
		}
		case ANIMATIONTYPE_SPRITES:{
			for(int i = 0;i<a->sprites_img.size;i++){
				Sprite* s = (Sprite*)Vector_Get(&a->sprites_img,i);

				if(s->w != width || s->h != height){
					Sprite_Reload(s,width,height);
				}
			}
			break;
		}
		case ANIMATIONTYPE_ANIMATIONATLAS:{
			const unsigned int w = a->aatlas_img.w / a->aatlas_cx;
			const unsigned int h = a->aatlas_img.h / a->aatlas_cy;
			if(w != width || h != height){
				Sprite_Reload(&a->aatlas_img,width * a->aatlas_cx,height * a->aatlas_cy);
			}
			break;
		}
		case ANIMATIONTYPE_ANIMATION:{
			for(int i = 0;i<a->animation_img.size;i++){
				Sprite* s = (Sprite*)Vector_Get(&a->animation_img,i);

				if(s->w != width || s->h != height){
					Sprite_Reload(s,width,height);
				}
			}
			break;
		}
		default:
			break;
	}
}
void Animation_Free(Animation* a){
	switch(a->type){
		case ANIMATIONTYPE_NONE:
			break;
		case ANIMATIONTYPE_SPRITE:{
			Sprite_Free(&a->sprite_img);
			break;
		}
		case ANIMATIONTYPE_ATLAS:{
			Sprite_Free(&a->atlas_img);
			a->atlas_get = NULL;
			break;
		}
		case ANIMATIONTYPE_SPRITES:{
			for(int i = 0;i<a->sprites_img.size;i++){
				Sprite* s = (Sprite*)Vector_Get(&a->sprites_img,i);
				Sprite_Free(s);
			}
			Vector_Free(&a->sprites_img);
			a->sprites_get = NULL;
			break;
		}
		case ANIMATIONTYPE_ANIMATIONATLAS:{
			Sprite_Free(&a->aatlas_img);
			break;
		}
		case ANIMATIONTYPE_ANIMATION:{
			for(int i = 0;i<a->animation_img.size;i++){
				Sprite* s = (Sprite*)Vector_Get(&a->animation_img,i);
				Sprite_Free(s);
			}
			Vector_Free(&a->animation_img);
			break;
		}
		default:
			break;
	}
	a->spawner = 0U;
	a->type = ANIMATIONTYPE_NONE;
}


typedef struct EntityAtlas {
	Sprite atlas;
	unsigned int cx;
	unsigned int cy;
	float sx;
	float sy;
	void (*Update)(void*,float);
	SubSprite (*GetRender)(void*,struct EntityAtlas*);
	void (*Free)(void*);
} EntityAtlas;

EntityAtlas EntityAtlas_New(
	char* path,
	char* file,
	unsigned int cx,
	unsigned int cy,
	void (*Update)(void*,float),
	SubSprite (*GetRender)(void*,EntityAtlas*),
	void (*Free)(void*)
){
	EntityAtlas ea;
	
	CStr fullpath = CStr_Concat(path,file);
	ea.atlas = Sprite_Load(fullpath);
	CStr_Free(&fullpath);

	ea.cx = cx;
	ea.cy = cy;
	ea.sx = 1.0f;
	ea.sy = 1.0f;
	ea.Update = Update;
	ea.GetRender = GetRender;
	ea.Free = Free;
	return ea;
}
EntityAtlas EntityAtlas_Make(
	char* path,
	char* file,
	unsigned int cx,
	unsigned int cy,
	float sx,
	float sy,
	void (*Update)(void*,float),
	SubSprite (*GetRender)(void*,EntityAtlas*),
	void (*Free)(void*)
){
	EntityAtlas ea;
	
	CStr fullpath = CStr_Concat(path,file);
	ea.atlas = Sprite_Load(fullpath);
	CStr_Free(&fullpath);

	ea.cx = cx;
	ea.cy = cy;
	ea.sx = sx;
	ea.sy = sy;
	ea.Update = Update;
	ea.GetRender = GetRender;
	ea.Free = Free;
	return ea;
}
EntityAtlas EntityAtlas_Null(){
	EntityAtlas ea;
	ea.atlas = Sprite_Null();
	ea.cx = 0U;
	ea.cy = 0U;
	ea.Update = NULL;
	ea.GetRender = NULL;
	ea.Free = NULL;
	return ea;
}
void EntityAtlas_Resize(EntityAtlas* ea,int width,int height){
	unsigned int dx = (unsigned int)((float)(ea->cx * width) * ea->sx);
	unsigned int dy = (unsigned int)((float)(ea->cy * height) * ea->sy);
	Sprite_Reload(&ea->atlas,dx,dy);
}
void EntityAtlas_Free(EntityAtlas* ea){
	Sprite_Free(&ea->atlas);
	ea->Update = NULL;
	ea->GetRender = NULL;
	ea->Free = NULL;
}


// abstract => inherite id and override methods
typedef struct Entity {
	unsigned int id;
	unsigned int padd;
	Rect r;
	Vec2 v;
	Vec2 a;
	void (*WorldCollision)(struct Entity*,World*);
	char (*IsSolid)(struct Entity*,World*,unsigned int,unsigned int,Side);
	void (*Collision)(struct Entity*,World*,unsigned int,unsigned int,Side);
	void (*EntityCollision)(struct Entity*,World*,struct Entity*,unsigned int,unsigned int,Side);
} Entity;

void Entity_Free(Entity* e){
	e->WorldCollision = NULL;
	e->IsSolid = NULL;
	e->Collision = NULL;
	e->EntityCollision = NULL;
}



#define WORLD_CHUNK_RADIUS_X	3
#define WORLD_CHUNK_RADIUS_Y	3
#define WORLD_CHUNK_COUNT_X		(2 * WORLD_CHUNK_RADIUS_X + 1)
#define WORLD_CHUNK_COUNT_Y		(2 * WORLD_CHUNK_RADIUS_Y + 1)

#define WORLD_CHUNK_WIDTH		8
#define WORLD_CHUNK_HEIGHT		8

typedef struct World_Chunk {
	int32_t x;
	int32_t y;
	Block data[WORLD_CHUNK_WIDTH * WORLD_CHUNK_HEIGHT];
} World_Chunk;

World_Chunk World_Chunk_New(int32_t x,int32_t y){
	World_Chunk ch;
	ch.x = x;
	ch.y = y;
	memset(&ch.data,0,sizeof(ch.data));
	return ch;
}
World_Chunk World_Chunk_Generate(int32_t x,int32_t y){
	World_Chunk ch;
	ch.x = x;
	ch.y = y;
	memset(&ch.data,1U,sizeof(ch.data));
	return ch;
}
World_Chunk World_Chunk_Null(){
	World_Chunk ch;
	memset(&ch,0,sizeof(ch));
	return ch;
}
World_Chunk World_Chunk_Load(char* path,int32_t x,int32_t y){
	CStr fp = CStr_Format("%s/Chunk_%d_%d",path,x,y);
	
	FilesSize size;
	char* data = Files_ReadB(fp,&size);
	
	if(!data){
		CStr_Free(&fp);
		return World_Chunk_Generate(x,y);
	}else if(size != sizeof(World_Chunk)){
		printf("[World_Chunk]: Load -> size invalid '%s': %u vs %u\n",fp,(uint32_t)size,(uint32_t)sizeof(World_Chunk));
		free(data);
		CStr_Free(&fp);
		return World_Chunk_Generate(x,y);
	}

	World_Chunk ch = *(World_Chunk*)data;
	ch.x = x;
	ch.y = y;

	free(data);
	CStr_Free(&fp);
	return ch;
}
void World_Chunk_Save(World_Chunk* ch,char* path){
	CStr fp = CStr_Format("%s/Chunk_%d_%d",path,(Number)ch->x,(Number)ch->y);
	//Files_Write(fp,ch,sizeof(*ch));
	CStr_Free(&fp);
}
void World_Chunk_Free(World_Chunk* ch){
	
}



typedef Vector World_Chunk_Row;

World_Chunk_Row World_Chunk_Row_New(){
	return Vector_New(sizeof(World_Chunk));
}
World_Chunk_Row World_Chunk_Row_Load(char* path,int32_t x,int32_t y){
	World_Chunk_Row row = World_Chunk_Row_New();
	
	for(int32_t i = 0;i<WORLD_CHUNK_COUNT_X;i++){
		World_Chunk ch = World_Chunk_Load(path,x + i,y);
		Vector_Push(&row,&ch);
	}

	return row;
}
void World_Chunk_Row_Save(World_Chunk_Row* row,char* path){
	for(uint32_t i = 0U;i<row->size;i++){
		World_Chunk* ch = (World_Chunk*)Vector_Get(row,i);
		World_Chunk_Save(ch,path);
	}
}
void World_Chunk_Row_Shift(World_Chunk_Row* row,char* path,int32_t shift_x){
	if(row->size > 0){
		World_Chunk* first = (World_Chunk*)Vector_Get(row,0);
		const int32_t start_x = first->x;
		const int32_t start_y = first->y;

		if(shift_x < 0){
			const int32_t shifted = I32_Max(0,row->size + shift_x);
			const int32_t shifted_count = row->size - shifted;
			
			for(int32_t i = row->size - 1;i>=shifted;i--){
				World_Chunk* ch = (World_Chunk*)Vector_Get(row,i);
				World_Chunk_Save(ch,path);
				World_Chunk_Free(ch);
				Vector_Remove(row,i);
			}

			for(int32_t i = 0;i<shifted_count;i++){
				World_Chunk ch = World_Chunk_Load(path,start_x + shift_x + i,start_y);
				Vector_Add(row,&ch,i);
			}
		}else if(shift_x > 0){
			const int32_t shifted = I32_Min(row->size,shift_x);

			for(int32_t i = shifted - 1;i>=0;i--){
				World_Chunk* ch = (World_Chunk*)Vector_Get(row,i);
				World_Chunk_Save(ch,path);
				World_Chunk_Free(ch);
				Vector_Remove(row,i);
			}

			for(int32_t i = 0;i<shifted;i++){
				World_Chunk ch = World_Chunk_Load(path,start_x + shift_x + i,start_y);
				Vector_Push(row,&ch);
			}
		}
	}
}
void World_Chunk_Row_Free(World_Chunk_Row* row){
	for(uint32_t i = 0U;i<row->size;i++){
		World_Chunk* ch = (World_Chunk*)Vector_Get(row,i);
		World_Chunk_Free(ch);
	}
	Vector_Free(row);
}


typedef struct World{
	Vector animations; 	//Vector<Animation>
	Vector entityatlas;	//Vector<EntityAtlas>
	PVector entities;  	//PVector<Entity>
	Vector chunks; 	    //Vector<World_Chunk_Row>
	int32_t origin_x;
	int32_t origin_y;
	int32_t t_origin_x;
	int32_t t_origin_y;
	Vec2 spawn;
	AnimationBg mode;
	char* path;
	void* (*Spawner)(Vec2,SpawnType,unsigned int*);
} World;

World World_New(char* path){
	World w;
	w.animations = Vector_New(sizeof(Animation));
	w.entityatlas = Vector_New(sizeof(EntityAtlas));
	w.entities = PVector_New();
	w.mode = ANIMATIONBG_ALL;
	w.path = CStr_Cpy(path);
	w.chunks = Vector_New(sizeof(World_Chunk_Row));
	
	w.origin_x = 0;
	w.origin_y = 0;
	w.t_origin_x = 0;
	w.t_origin_y = 0;
	w.spawn = (Vec2){ 0.0f,0.0f };
	return w;
}
World World_Make(char* Path,Animation* a,EntityAtlas* ea){
	World w = World_New(Path);
	
	for(int i = 0;a[i].type != ANIMATIONTYPE_NONE;i++){
		Vector_Push(&w.animations,a + i);
	}
	for(int i = 0;ea[i].atlas.img;i++){
		Vector_Push(&w.entityatlas,ea + i);
	}

	return w;
}
char World_isBlock(World* w,Block b){
	return b!=BLOCK_NONE && b - 1 < w->animations.size;
}

Block World_Get(World* w,int32_t x,int32_t y){
	const float foffset_x = floorf((float)x / (float)WORLD_CHUNK_WIDTH);
	const float foffset_y = floorf((float)y / (float)WORLD_CHUNK_HEIGHT);

	const int32_t offset_x = (int32_t)foffset_x - w->origin_x + WORLD_CHUNK_RADIUS_X;
	const int32_t offset_y = (int32_t)foffset_y - w->origin_y + WORLD_CHUNK_RADIUS_Y;
	
	if(offset_y < 0 || offset_y >= w->chunks.size) return BLOCK_NONE;
	World_Chunk_Row* row = (World_Chunk_Row*)Vector_Get(&w->chunks,offset_y);
	
	if(offset_x < 0 || offset_x >= row->size) return BLOCK_NONE;
	World_Chunk* ch = (World_Chunk*)Vector_Get(row,offset_x);
	const int32_t ch_x = (x - offset_x * WORLD_CHUNK_WIDTH) % WORLD_CHUNK_WIDTH;
	const int32_t ch_y = (y - offset_y * WORLD_CHUNK_WIDTH) % WORLD_CHUNK_HEIGHT;
	return ch->data[ch_y * WORLD_CHUNK_WIDTH + ch_x];
}
unsigned char World_GetNeigbours(World* w,Block b,int32_t x,int32_t y){
	unsigned char nbs = 0U;
	int shift = 0;
	
	for(int i = 0;i<9;i++){
		if(i == 4) continue;
		const int px = i % 3;
		const int py = i / 3;
		Block got = World_Get(w,x - 1 + px,y - 1 + py);
		nbs |= (got == b || got == BLOCK_BOUNDS) << (shift++);
	}
	
	return nbs;
}
void World_Set(World* w,int32_t x,int32_t y,Block b){
	const float foffset_x = floorf((float)x / (float)WORLD_CHUNK_WIDTH);
	const float foffset_y = floorf((float)y / (float)WORLD_CHUNK_HEIGHT);

	const int32_t offset_x = (int32_t)foffset_x - w->origin_x + WORLD_CHUNK_RADIUS_X;
	const int32_t offset_y = (int32_t)foffset_y - w->origin_y + WORLD_CHUNK_RADIUS_Y;
	
	if(offset_y < 0 || offset_y >= w->chunks.size) return;
	World_Chunk_Row* row = (World_Chunk_Row*)Vector_Get(&w->chunks,offset_y);
	
	if(offset_x < 0 || offset_x >= row->size) return;
	World_Chunk* ch = (World_Chunk*)Vector_Get(row,offset_x);
	const int32_t ch_x = (x - offset_x * WORLD_CHUNK_WIDTH) % WORLD_CHUNK_WIDTH;
	const int32_t ch_y = (y - offset_y * WORLD_CHUNK_WIDTH) % WORLD_CHUNK_HEIGHT;
	ch->data[ch_y * WORLD_CHUNK_WIDTH + ch_x] = b;
}

void* World_Spawn(World* w,SpawnType st,Vec2 p){
	unsigned int size = 0U;
	void* e = w->Spawner(p,st,&size);
	if(e){
		PVector_PPush(&w->entities,e);
		//PVector_Push(&w->entities,e,size);
		//free(e);
		//void* ret = PVector_Get(&w->entities,w->entities.size - 1);	
		//return ret;
		return e;
	}
	return NULL;
}
void World_Remove(World* w,Entity* e){
	for(int i = 0;i<w->entities.size;i++){
		Entity* te = (Entity*)PVector_Get(&w->entities,i);
		EntityAtlas* ea = (EntityAtlas*)Vector_Get(&w->entityatlas,e->id - 1);
		
		if(te == e){
			if(ea->Free) ea->Free(e);
			PVector_Remove(&w->entities,i);
			return;
		}
	}
}

SubSprite World_Img(World* w,Block b,int32_t x,int32_t y){
	if(World_isBlock(w,b)){
		Animation* a = (Animation*)Vector_Get(&w->animations,b - 1);
		switch(a->type){
			case ANIMATIONTYPE_NONE: 	return SubSprite_Null();
			case ANIMATIONTYPE_SPRITE:	return SubSprite_New(&a->sprite_img,0.0f,0.0f,a->sprite_img.w,a->sprite_img.h);
			case ANIMATIONTYPE_ATLAS:	return a->atlas_get(a,w,x,y);
			case ANIMATIONTYPE_SPRITES:	return a->sprites_get(a,w,x,y);
			case ANIMATIONTYPE_ANIMATIONATLAS:{
				if(!a->aatlas_img.img || a->aatlas_cx == 0U || a->aatlas_cy == 0U || a->aatlas_duration <= 0.0)
					return SubSprite_Null();

				const FDuration phase = Animation_Phase(a->aatlas_start,1.0 / a->aatlas_duration);
				unsigned int img = (unsigned int)(phase * a->aatlas_cx);
				if(img >= a->aatlas_cx) img = a->aatlas_cx - 1U;

				const unsigned int dx = a->aatlas_img.w / a->aatlas_cx;
				const unsigned int dy = a->aatlas_img.h / a->aatlas_cy;
				if(dx == 0U || dy == 0U) return SubSprite_Null();

				return SubSprite_New(&a->aatlas_img,img * dx,0.0f,dx,dy);
			}
			case ANIMATIONTYPE_ANIMATION:{
				if(a->animation_img.size <= 0 || a->animation_duration <= 0.0)
					return SubSprite_Null();

				const FDuration phase = Animation_Phase(a->animation_start,1.0 / a->animation_duration);
				unsigned int img = (unsigned int)(phase * a->animation_img.size);
				if(img >= (unsigned int)a->animation_img.size) img = (unsigned int)a->animation_img.size - 1U;

				Sprite* s = (Sprite*)Vector_Get(&a->animation_img,(int)img);
				if(!s || !s->img) return SubSprite_Null();

				return SubSprite_New(s,0.0f,0.0f,s->w,s->h);
			}
			default:					return SubSprite_Null();
		}
	}
	return SubSprite_Null();
}
SubSprite World_Get_Img(World* w,int32_t x,int32_t y){
	const Block b = World_Get(w,x,y);
	return World_Img(w,b,x,y);
}
void World_Reload(World* w,unsigned int width,unsigned int height){
	for(int i = 0;i<w->animations.size;i++){
		Animation* a = (Animation*)Vector_Get(&w->animations,i);
		Animation_Resize(a,width,height);
	}
	for(int i = 0;i<w->entityatlas.size;i++){
		EntityAtlas* ea = (EntityAtlas*)Vector_Get(&w->entityatlas,i);
		EntityAtlas_Resize(ea,width,height);
	}
}
void World_Update(World* w,TransformedView* tv,float t){
	if(w->mode != ANIMATIONBG_DG){
		for(int i = 0;i<w->entities.size;i++){
			Entity* e = (Entity*)PVector_Get(&w->entities,i);
			EntityAtlas* ea = (EntityAtlas*)Vector_Get(&w->entityatlas,e->id - 1);
			
			if(ea && TransformedView_inViewRect(tv,e->r))
				ea->Update(e,t);
		}
	}
}
void World_ReloadChunks(World* w){
	for(int32_t i = 0;i<w->chunks.size;i++){
		World_Chunk_Row* row = (World_Chunk_Row*)Vector_Get(&w->chunks,i);
		World_Chunk_Row_Save(row,w->path);
		World_Chunk_Row_Free(row);
	}

	for(int32_t i = 0;i<WORLD_CHUNK_COUNT_Y;i++){
		World_Chunk_Row row = World_Chunk_Row_Load(w->path,w->origin_x - WORLD_CHUNK_RADIUS_Y,w->origin_y - WORLD_CHUNK_RADIUS_Y + i);
		Vector_Push(&w->chunks,&row);
	}
}
void World_UpdateChunks(World* w){
	const int32_t dir_x = w->t_origin_x - w->origin_x;
	const int32_t dir_y = w->t_origin_y - w->origin_y;
	
	if(dir_y < 0){
		const int32_t shifted = I32_Max(0,w->chunks.size + dir_y);
		const int32_t shifted_count = w->chunks.size - shifted;
		
		for(int32_t i = w->chunks.size - 1;i>=0;i--){
			World_Chunk_Row* row = (World_Chunk_Row*)Vector_Get(&w->chunks,i);
			World_Chunk_Row_Save(row,w->path);
			World_Chunk_Row_Free(row);
			Vector_Remove(&w->chunks,i);
		}

		for(int32_t i = 0;i < shifted_count || i < w->chunks.size;i++){
			if(i < shifted_count){
				World_Chunk_Row row = World_Chunk_Row_Load(w->path,w->origin_x + dir_x,w->origin_y + dir_y + i);
				Vector_Add(&w->chunks,&row,i);
			}else{
				World_Chunk_Row* row = (World_Chunk_Row*)Vector_Get(&w->chunks,i);
				World_Chunk_Row_Shift(row,w->path,dir_x);
			}
		}
	}else if(dir_y > 0){
		const int32_t shifted = I32_Min(w->chunks.size,dir_y);
		
		for(int32_t i = shifted - 1;i>=0;i--){
			World_Chunk_Row* row = (World_Chunk_Row*)Vector_Get(&w->chunks,i);
			World_Chunk_Row_Save(row,w->path);
			World_Chunk_Row_Free(row);
			Vector_Remove(&w->chunks,i);
		}

		for(int32_t i = 0;i < shifted || i < w->chunks.size;i++){
			if(i < shifted){
				World_Chunk_Row row = World_Chunk_Row_Load(w->path,w->origin_x + dir_x,w->origin_y + dir_y + i);
				Vector_Push(&w->chunks,&row);
			}else{
				World_Chunk_Row* row = (World_Chunk_Row*)Vector_Get(&w->chunks,i);
				World_Chunk_Row_Shift(row,w->path,dir_x);
			}
		}
	}else{
		for(int32_t i = 0;i < w->chunks.size;i++){
			World_Chunk_Row* row = (World_Chunk_Row*)Vector_Get(&w->chunks,i);
			World_Chunk_Row_Shift(row,w->path,dir_x);
		}
	}
	
	w->origin_x = w->t_origin_x;
	w->origin_y = w->t_origin_y;
}

void World_Collision(World* w,Entity* src){
	if(src){
		if(w->mode!=ANIMATIONBG_DG){
			Vector rects = Vector_New(sizeof(TargetRect));
			
			float searchX = F32_Max(2.0f,2.0f * src->r.d.x);
			float searchY = F32_Max(2.0f,2.0f * src->r.d.y);

			for(float x = -searchX;x<searchX;x+=1.0f) {
				for(float y = -searchY;y<searchY;y+=1.0f) {
					int sx = (int)(src->r.p.x + x);
					int sy = (int)(src->r.p.y + y);

    		        if(sy>=0) {
    		            Block b = World_Get(w,sx,sy);
    		            Rect br = { (Vec2){ sx,sy },(Vec2){ 1.0f,1.0f } };

						if(Overlap_Rect_Rect(src->r,br)){
							if(b!=BLOCK_NONE){
								Vector_Push(&rects,(TargetRect[]){ br,&src->r.p });
							}
						}
					}
				}
			}

			qsort(rects.Memory,rects.size,rects.ELEMENT_SIZE,TargetRect_Compare);

			for(int i = 0;i<rects.size;i++) {
    		    TargetRect* r = (TargetRect*)Vector_Get(&rects,i);

				Side s = Side_Rect_Rect(src->r,r->r);
				
				char collision = 1;
				if(src->IsSolid) collision = src->IsSolid(src,w,r->r.p.x,r->r.p.y,s);

				if(collision){
					Resolve_Rect_Rect_Side(&src->r,r->r,s);
					if(src->Collision) src->Collision(src,w,r->r.p.x,r->r.p.y,s);
				}
			}

			Vector_Free(&rects);

			if(src->WorldCollision) src->WorldCollision(src,w);
		}
	}
}
void World_EntityCollision(World* w,Entity* src){
	if(w->mode!=ANIMATIONBG_DG){
		for(int i = 0;i<w->entities.size;i++) {
    	    Entity* e = (Entity*)PVector_Get(&w->entities,i);
			if(src == e) continue;
			
			if(Overlap_Rect_Rect(src->r,e->r)){
				Side s = Side_Rect_Rect(src->r,e->r);
				
				if(src->EntityCollision)
					src->EntityCollision(src,w,e,e->r.p.x,e->r.p.y,s);
			}
		}
	}
}

void World_Collisions(World* w,TransformedView* tv){
	for(int i = 0;i<w->entities.size;i++){
		Entity* e = (Entity*)PVector_Get(&w->entities,i);
		EntityAtlas* ea = (EntityAtlas*)Vector_Get(&w->entityatlas,e->id - 1U);
		
		if(e->id < (unsigned int)w->entityatlas.size + 1U && TransformedView_inViewRect(tv,e->r)){
			World_EntityCollision(w,e);
			World_Collision(w,e);
		}
	}
}
void World_RenderEntities(World* w,TransformedView* tv,Pixel* out,unsigned int width,unsigned int height){
	for(int i = 0;i<w->entities.size;i++){
		Entity* e = (Entity*)PVector_Get(&w->entities,i);
		
		if(!TransformedView_inViewRect(tv,e->r))
			continue;
		
		EntityAtlas* ea = (EntityAtlas*)Vector_Get(&w->entityatlas,e->id - 1);
		if(ea){
			const Vec2 sc = TransformedView_WorldScreenPos(tv,(Vec2){ e->r.p.x,e->r.p.y });
			SubSprite ss = ea->GetRender(e,ea);
			if(ss.sp)
				Sprite_RenderSubAlpha(out,width,height,ss.sp,sc.x,sc.y,ss.ox,ss.oy,ss.dx,ss.dy);
		}
	}
}
void World_Render(World* w,TransformedView* tv,Pixel* out,unsigned int width,unsigned int height){
	const Vec2 tl = TransformedView_ScreenWorldPos(tv,(Vec2){ 0.0f,0.0f });
	const Vec2 br = TransformedView_ScreenWorldPos(tv,(Vec2){ width,height });
	const Vec2 sd = TransformedView_WorldScreenLength(tv,(Vec2){ 1.0f,1.0f });
	World_Reload(w,(unsigned int)F32_Ceil(sd.x),(unsigned int)F32_Ceil(sd.y));
	
	for(int y = tl.y - 1;y<br.y;y++){
		for(int x = tl.x - 1;x<br.x;x++){
			const Block b = World_Get(w,x,y);
			const Vec2 sc = TransformedView_WorldScreenPos(tv,(Vec2){ x,y });
			
			SubSprite bg_ss = World_Img(w,BLOCK_DIRT,0,0);
			Sprite_RenderSub(out,width,height,bg_ss.sp,sc.x,sc.y,bg_ss.ox,bg_ss.oy,bg_ss.dx,bg_ss.dy);

			if(b != BLOCK_NONE){
				SubSprite ss = World_Get_Img(w,x,y);
				if(ss.sp)
					Sprite_RenderSubAlpha(out,width,height,ss.sp,sc.x,sc.y,ss.ox,ss.oy,ss.dx,ss.dy);
			}
		}
	}
}
void World_Free(World* w){
	for(int i = 0;i<w->entities.size;i++){
		Entity* e = (Entity*)PVector_Get(&w->entities,i);
		EntityAtlas* ea = (EntityAtlas*)Vector_Get(&w->entityatlas,e->id - 1);
		if(ea) ea->Free(e);
	}
	PVector_Free(&w->entities);
	
	for(int i = 0;i<w->animations.size;i++){
		Animation* s = (Animation*)Vector_Get(&w->animations,i);
		Animation_Free(s);
	}
	Vector_Free(&w->animations);

	for(int i = 0;i<w->chunks.size;i++){
		World_Chunk_Row* row = (World_Chunk_Row*)Vector_Get(&w->chunks,i);
		World_Chunk_Row_Free(row);
	}
	Vector_Free(&w->chunks);
}

#endif