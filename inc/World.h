#ifndef WORLD_H
#define WORLD_H

#include "/home/codeleaded/System/Static/Library/WindowEngine.h"
#include "/home/codeleaded/System/Static/Library/Files.h"
#include "/home/codeleaded/System/Static/Library/TransformedView.h"
#include "/home/codeleaded/System/Static/Library/Geometry.h"
#include "/home/codeleaded/System/Static/Library/Random.h"

#include "/home/codeleaded/System/Static/Container/DataStream.h"
#include "/home/codeleaded/System/Static/Container/PVector.h"

#define ENTITY_COLLISION_RADIUS_X			1U
#define ENTITY_COLLISION_RADIUS_Y			1U

#define ENTITY_NONE							0U

#define BLOCK_NONE							0U
#define BLOCK_GRAS							1U
#define BLOCK_WATER							2U
#define BLOCK_STONE							3U
#define BLOCK_DOCK							4U
#define BLOCK_HOUSE_GRAY					5U
#define BLOCK_HOUSE_RED						6U
#define BLOCK_CHEST							7U
#define BLOCK_DOOR							8U
#define BLOCK_CEIL_FLAT						9U
#define BLOCK_CEIL_NORMAL					10U
#define BLOCK_BORDER						0xFFFFFFFFU
typedef uint32_t Block;

#define ITEM_NONE							0U
typedef uint32_t Item;

#define WORLD_SPRITE_DEPTH_BACKGROUND		0U
#define WORLD_SPRITE_DEPTH_UNDERLAY			1U
#define WORLD_SPRITE_DEPTH_GROUND			2U
#define WORLD_SPRITE_DEPTH_OBJECT			3U
#define WORLD_SPRITE_DEPTH_ENTITY			4U

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
			SubSprite (*atlas_get)(struct Animation*,World*,int32_t,int32_t);
		};
		struct{
			Vector sprites_img;
			SubSprite (*sprites_get)(struct Animation*,World*,int32_t,int32_t);
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
Animation Animation_Make_Atlas(char* Path,unsigned short cx,unsigned short cy,SubSprite (*atlas_get)(struct Animation*,World*,int32_t,int32_t)){
	Animation a;
	a.type = ANIMATIONTYPE_ATLAS;
	a.atlas_img = Sprite_Load(Path);
	a.atlas_cx = cx;
	a.atlas_cy = cy;
	a.atlas_get = atlas_get;
	return a;
}
Animation Animation_Make_Sprites(char** Paths,SubSprite (*sprites_get)(struct Animation*,World*,int32_t,int32_t)){
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
Animation Animation_Make_AtlasPath(char* path,char* file,unsigned short cx,unsigned short cy,SubSprite (*atlas_get)(struct Animation*,World*,int32_t,int32_t)){
	CStr fullpath = CStr_Concat(path,file);
	Animation a = Animation_Make_Atlas(fullpath,cx,cy,atlas_get);
	CStr_Free(&fullpath);
	return a;
}
Animation Animation_Make_SpritesPath(char* path,char** files,SubSprite (*sprites_get)(struct Animation*,World*,int32_t,int32_t)){
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
	void (*Update)(void*,void*,float);
	void (*Move)(void*,const float,const float);
	SubSprite (*GetRender)(void*,struct EntityAtlas*);
	void (*Free)(void*);
	void (*WorldCollision)(void*,World*);
	char (*IsSolid)(void*,World*,int32_t,int32_t,Side);
	void (*Collision)(void*,World*,int32_t,int32_t,Side);
	void (*EntityCollision)(void*,World*,void*,int32_t,int32_t,Side);
	void* (*Load)(DataStream*);
	void (*Save)(void*,DataStream*);
} EntityAtlas;

EntityAtlas EntityAtlas_New(
	char* path,
	char* file,
	unsigned int cx,
	unsigned int cy,
	void (*Update)(void*,void*,float),
	void (*Move)(void*,const float,const float),
	SubSprite (*GetRender)(void*,EntityAtlas*),
	void (*Free)(void*),
	void (*WorldCollision)(void*,World*),
	char (*IsSolid)(void*,World*,int32_t,int32_t,Side),
	void (*Collision)(void*,World*,int32_t,int32_t,Side),
	void (*EntityCollision)(void*,World*,void*,int32_t,int32_t,Side),
	void* (*Load)(DataStream*),
	void (*Save)(void*,DataStream*)
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
	ea.Move = Move;
	ea.GetRender = GetRender;
	ea.Free = Free;
	ea.WorldCollision = WorldCollision;
	ea.IsSolid = IsSolid;
	ea.Collision = Collision;
	ea.EntityCollision = EntityCollision;
	ea.Load = Load;
	ea.Save = Save;
	return ea;
}
EntityAtlas EntityAtlas_Make(
	char* path,
	char* file,
	unsigned int cx,
	unsigned int cy,
	float sx,
	float sy,
	void (*Update)(void*,void*,float),
	void (*Move)(void*,const float,const float),
	SubSprite (*GetRender)(void*,EntityAtlas*),
	void (*Free)(void*),
	void (*WorldCollision)(void*,World*),
	char (*IsSolid)(void*,World*,int32_t,int32_t,Side),
	void (*Collision)(void*,World*,int32_t,int32_t,Side),
	void (*EntityCollision)(void*,World*,void*,int32_t,int32_t,Side),
	void* (*Load)(DataStream*),
	void (*Save)(void*,DataStream*)
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
	ea.WorldCollision = WorldCollision;
	ea.IsSolid = IsSolid;
	ea.Collision = Collision;
	ea.EntityCollision = EntityCollision;
	ea.Load = Load;
	ea.Save = Save;
	return ea;
}
EntityAtlas EntityAtlas_Null(){
	EntityAtlas ea;
	memset(&ea,0,sizeof(ea));
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
	ea->Move = NULL;
	ea->GetRender = NULL;
	ea->Free = NULL;
	ea->WorldCollision = NULL;
	ea->IsSolid = NULL;
	ea->Collision = NULL;
	ea->EntityCollision = NULL;
	ea->Load = NULL;
	ea->Save = NULL;
}


// abstract => inherite id
typedef struct World_Entity {
	uint32_t id;
	Rect r;
	Vec2 v;
	Vec2 a;
} World_Entity;

EntityAtlas* World_EntityAtlas_Get(World* w,const World_Entity* e);


typedef struct World_Item_Stack {
	Item id;
	union {
		uint32_t count;
		uint32_t durability;
	};
} World_Item_Stack;

typedef struct World_Chunk_Chest {
	uint32_t count;
	uint32_t padding;
	World_Item_Stack* chest;
} World_Chunk_Chest;

typedef struct World_Chunk_Portal {
	uint32_t dimension;
	int32_t tx;
	int32_t ty;
} World_Chunk_Portal;


#define WORLD_CHUNK_TAG_TYPE_NONE		0U
#define WORLD_CHUNK_TAG_TYPE_CHEST		1U
#define WORLD_CHUNK_TAG_TYPE_PORTAL		2U

typedef struct World_Chunk_Tag {
	uint8_t cx;
	uint8_t cy;
	uint16_t type;
	union {
		World_Chunk_Chest chest;
		World_Chunk_Portal portal;
	};
} World_Chunk_Tag;

World_Chunk_Tag World_Chunk_Tag_New(uint8_t cx,uint8_t cy){
	World_Chunk_Tag ct;
	memset(&ct,0,sizeof(ct));
	ct.cx = cx;
	ct.cy = cy;
	ct.type = WORLD_CHUNK_TAG_TYPE_NONE;
	return ct;
}
World_Chunk_Tag World_Chunk_Tag_Chest_New(uint8_t cx,uint8_t cy,uint32_t count){
	World_Chunk_Tag ct;
	memset(&ct,0,sizeof(ct));
	ct.cx = cx;
	ct.cy = cy;
	ct.type = WORLD_CHUNK_TAG_TYPE_CHEST;
	ct.chest.count = count;
	ct.chest.chest = (World_Item_Stack*)malloc(sizeof(World_Item_Stack) * count);
	memset(ct.chest.chest,0,sizeof(World_Item_Stack) * count);
	return ct;
}
World_Chunk_Tag World_Chunk_Tag_Chest_Tressure(uint8_t cx,uint8_t cy,uint32_t count){
	World_Chunk_Tag ct = World_Chunk_Tag_Chest_New(cx,cy,count);
	
	for(uint32_t i = 0U;i<count;i++){
		const uint32_t chance = Random_u32_MinMax(0U,10U);
		
		if(chance == 0U){
			ct.chest.chest[i] = (World_Item_Stack){
				.id = ITEM_NONE,
				.count = Random_u32_MinMax(1U,5U)
			};
		}
	}

	return ct;
}
World_Chunk_Tag World_Chunk_Tag_Load(DataStream* ds){
	World_Chunk_Tag ct;
	
	DataStream_ReadCount(ds,&ct.cx,0,sizeof(ct.cx));
	DataStream_ReadCount(ds,&ct.cy,0,sizeof(ct.cy));
	DataStream_ReadCount(ds,&ct.type,0,sizeof(ct.type));

	switch (ct.type){
		case WORLD_CHUNK_TAG_TYPE_CHEST:{
			uint32_t count = 0U;
			DataStream_ReadCount(ds,&count,0,sizeof(count));

			ct.chest = (World_Chunk_Chest){
				.chest = (World_Item_Stack*)malloc(sizeof(World_Item_Stack) * count),
				.count = count
			};
			
			DataStream_ReadCount(ds,ct.chest.chest,0,sizeof(World_Item_Stack) * count);
			break;
		}
		case WORLD_CHUNK_TAG_TYPE_PORTAL:{
			DataStream_ReadCount(ds,&ct.portal.dimension,0,sizeof(ct.portal.dimension));
			DataStream_ReadCount(ds,&ct.portal.tx,0,sizeof(ct.portal.tx));
			DataStream_ReadCount(ds,&ct.portal.ty,0,sizeof(ct.portal.ty));
			break;
		}
	}
	return ct;
}
void World_Chunk_Tag_Save(World_Chunk_Tag* ct,DataStream* ds){
	DataStream_PushCount(ds,&ct->cx,sizeof(ct->cx));
	DataStream_PushCount(ds,&ct->cy,sizeof(ct->cy));
	DataStream_PushCount(ds,&ct->type,sizeof(ct->type));

	switch (ct->type){
		case WORLD_CHUNK_TAG_TYPE_CHEST:{
			DataStream_PushCount(ds,&ct->chest.count,sizeof(ct->chest.count));
			DataStream_PushCount(ds,ct->chest.chest,sizeof(World_Item_Stack) * ct->chest.count);
			break;
		}
		case WORLD_CHUNK_TAG_TYPE_PORTAL:{
			DataStream_PushCount(ds,&ct->portal.dimension,sizeof(ct->portal.dimension));
			DataStream_PushCount(ds,&ct->portal.tx,sizeof(ct->portal.tx));
			DataStream_PushCount(ds,&ct->portal.ty,sizeof(ct->portal.ty));
			break;
		}
	}
}
void World_Chunk_Tag_Free(World_Chunk_Tag* ct){
	switch (ct->type){
		case WORLD_CHUNK_TAG_TYPE_CHEST:{
			free(ct->chest.chest);
			break;
		}
	}
	memset(ct,0,sizeof(*ct));
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
	Vector tags;		// Vector<World_Chunk_Tag>
	PVector entities;   // PVector<Entity>
} World_Chunk;

World_Chunk World_Chunk_New(int32_t x,int32_t y){
	World_Chunk ch;
	ch.x = x;
	ch.y = y;
	ch.tags = Vector_New(sizeof(World_Chunk_Tag));
	ch.entities = PVector_New();
	memset(&ch.data,0,sizeof(ch.data));
	return ch;
}
World_Chunk World_Chunk_Generate(int32_t x,int32_t y){
	World_Chunk ch;
	ch.x = x;
	ch.y = y;
	ch.tags = Vector_New(sizeof(World_Chunk_Tag));
	ch.entities = PVector_New();

	for(int i = 0;i<WORLD_CHUNK_WIDTH * WORLD_CHUNK_HEIGHT;i++)
		ch.data[i] = BLOCK_GRAS;

	return ch;
}
World_Chunk World_Chunk_Null(){
	World_Chunk ch;
	memset(&ch,0,sizeof(ch));
	return ch;
}
World_Chunk World_Chunk_Load(World* w,char* path,int32_t x,int32_t y){
	CStr fp = CStr_Format("%s/Chunk_%d_%d",path,(Number)x,(Number)y);
	
	FilesSize size;
	char* data = Files_ReadB(fp,&size);
	
	if(!data){
		CStr_Free(&fp);
		return World_Chunk_Generate(x,y);
	}

	DataStream ds = DataStream_By(data,size);
	World_Chunk ch = World_Chunk_New(x,y);
	if(size >= sizeof(ch.data)){
		DataStream_ReadCount(&ds,ch.data,0,sizeof(ch.data));

		int32_t size = 0;
		DataStream_ReadCount(&ds,&size,0,sizeof(size));

		for(int i = 0;i<size;i++){
			World_Chunk_Tag ct = World_Chunk_Tag_Load(&ds);
			if(ct.type == WORLD_CHUNK_TAG_TYPE_PORTAL){
				// TODO: Load dimension into dims
			}

			Vector_Push(&ch.tags,&ct);
		}

		int32_t entity_count = 0;
		if(ds.size >= (int32_t)sizeof(entity_count)){
			DataStream_ReadCount(&ds,&entity_count,0,sizeof(entity_count));
			if(entity_count < 0 || entity_count > ds.size / (int32_t)sizeof(World_Entity)){
				printf("[World_Chunk]: Load -> invalid entity count in '%s'\n",fp);
				entity_count = 0;
			}

			for(int32_t i = 0;i<entity_count;i++){
				if(ds.size < (int32_t)sizeof(World_Entity)){
					printf("[World_Chunk]: Load -> truncated entity data in '%s'\n",fp);
					break;
				}

				World_Entity header;
				memcpy(&header,ds.Memory,sizeof(header));
				EntityAtlas* ea = World_EntityAtlas_Get(w,&header);
				if(!ea || !ea->Load){
					printf("[World_Chunk]: Load -> no loader for entity id %u in '%s'\n",header.id,fp);
					break;
				}

				void* e_data = ea->Load(&ds);
				if(!e_data){
					printf("[World_Chunk]: Load -> failed to load entity id %u in '%s'\n",header.id,fp);
					break;
				}
				PVector_PPush(&ch.entities,e_data);
			}
		}
	}else{
		printf("[World_Chunk]: Load -> size invalid '%s': %u < %u\n",fp,(uint32_t)size,(uint32_t)sizeof(World_Chunk));
		DataStream_Free(&ds);
		CStr_Free(&fp);
		return ch;
	}

	DataStream_Free(&ds);
	CStr_Free(&fp);
	return ch;
}
void World_Chunk_Save(World_Chunk* ch,World* w,char* path){
	CStr fp = CStr_Format("%s/Chunk_%d_%d",path,(Number)ch->x,(Number)ch->y);

	DataStream ds = DataStream_New();
	DataStream_PushCount(&ds,ch->data,sizeof(ch->data));
	DataStream_PushCount(&ds,&ch->tags.size,sizeof(ch->tags.size));
	for(int i = 0;i<ch->tags.size;i++){
		World_Chunk_Tag* ct = (World_Chunk_Tag*)Vector_Get(&ch->tags,i);
		World_Chunk_Tag_Save(ct,&ds);
	}

	int32_t entity_count = 0;
	for(int i = 0;i<ch->entities.size;i++){
		World_Entity* e = (World_Entity*)PVector_Get(&ch->entities,i);
		EntityAtlas* ea = World_EntityAtlas_Get(w,e);
		if(ea && ea->Load && ea->Save)
			entity_count++;
		else if(e)
			printf("[World_Chunk]: Save -> entity id %u has no load/save callbacks\n",e->id);
	}

	DataStream_PushCount(&ds,&entity_count,sizeof(entity_count));
	for(int i = 0;i<ch->entities.size;i++){
		World_Entity* e = (World_Entity*)PVector_Get(&ch->entities,i);
		EntityAtlas* ea = World_EntityAtlas_Get(w,e);
		
		if(ea && ea->Load && ea->Save)
			ea->Save(e,&ds);
	}

	Files_Write(fp,ds.Memory,ds.size);
	DataStream_Free(&ds);
	CStr_Free(&fp);
}
World_Chunk_Tag* World_Chunk_Find(World_Chunk* ch,uint8_t cx,uint8_t cy){
	for(int i = 0;i<ch->tags.size;i++){
		World_Chunk_Tag* ct = (World_Chunk_Tag*)Vector_Get(&ch->tags,i);
		if(ct->cx == cx && ct->cy == cy)
			return ct;
	}
	return NULL;
}
void World_Chunk_Remove(World_Chunk* ch,uint8_t cx,uint8_t cy){
	for(int i = ch->tags.size - 1;i >= 0;i--){
		World_Chunk_Tag* ct = (World_Chunk_Tag*)Vector_Get(&ch->tags,i);
		
		if(ct->cx == cx && ct->cy == cy){
			World_Chunk_Tag_Free(ct);
			Vector_Remove(&ch->tags,i);
		}
	}
}
void World_Chunk_Free(World* w,World_Chunk* ch);


typedef Vector World_Chunk_Row;

World_Chunk_Row World_Chunk_Row_New(){
	return Vector_New(sizeof(World_Chunk));
}
World_Chunk_Row World_Chunk_Row_Load(World* w,char* path,int32_t x,int32_t y){
	World_Chunk_Row row = World_Chunk_Row_New();
	
	for(int32_t i = 0;i<WORLD_CHUNK_COUNT_X;i++){
		World_Chunk ch = World_Chunk_Load(w,path,x + i,y);
		Vector_Push(&row,&ch);
	}

	return row;
}
void World_Chunk_Row_Save(World_Chunk_Row* row,World* w,char* path){
	for(uint32_t i = 0U;i<row->size;i++){
		World_Chunk* ch = (World_Chunk*)Vector_Get(row,i);
		World_Chunk_Save(ch,w,path);
	}
}
void World_Chunk_Row_Free(World* w,World_Chunk_Row* row){
	for(uint32_t i = 0U;i<row->size;i++){
		World_Chunk* ch = (World_Chunk*)Vector_Get(row,i);
		World_Chunk_Free(w,ch);
	}
	Vector_Free(row);
}


typedef struct World_Dimension {
	Vector chunks;
	Block bg_tile;
} World_Dimension;

World_Dimension World_Dimension_New(){
	World_Dimension wd;
	wd.chunks = Vector_New(sizeof(World_Chunk_Row));
	wd.bg_tile = BLOCK_NONE;
	return wd;
}
World_Dimension World_Dimension_Move(Vector chunks){
	World_Dimension wd;
	wd.chunks = chunks;
	wd.bg_tile = BLOCK_NONE;
	return wd;
}
World_Dimension World_Dimension_Load(World* w,char* path,int32_t x,int32_t y){
	World_Dimension wd = World_Dimension_New();
	
	for(int32_t i = 0;i<WORLD_CHUNK_COUNT_Y;i++){
		World_Chunk_Row row = World_Chunk_Row_Load(w,path,x,y);
		Vector_Push(&wd.chunks,&row);
	}

	return wd;
}
void World_Dimension_Save(World_Dimension* wd,World* w,char* path){
	for(uint32_t i = 0U;i<wd->chunks.size;i++){
		World_Chunk_Row* row = (World_Chunk_Row*)Vector_Get(&wd->chunks,i);
		World_Chunk_Row_Save(row,w,path);
	}
}
void World_Dimension_Free(World* w,World_Dimension* wd){
	for(uint32_t i = 0U;i<wd->chunks.size;i++){
		World_Chunk_Row* row = (World_Chunk_Row*)Vector_Get(&wd->chunks,i);
		World_Chunk_Row_Free(w,row);
	}
	Vector_Free(&wd->chunks);
}




typedef struct World_Sprite {
	SubSprite sp;
	float x;
	float y;
	uint32_t alpha;
	uint32_t depth;
} World_Sprite;

int World_Sprite_Compare(const void* e1,const void* e2) {
	const World_Sprite* ws1 = (const World_Sprite*)e1;
	const World_Sprite* ws2 = (const World_Sprite*)e2;

	if(ws1->depth < WORLD_SPRITE_DEPTH_OBJECT || ws2->depth < WORLD_SPRITE_DEPTH_OBJECT){
		if(ws1->depth < ws2->depth) return -1;
		if(ws1->depth > ws2->depth) return 1;
	}

	const float py1 = ws1->y + ws1->sp.dy;
	const float py2 = ws2->y + ws2->sp.dy;
	if(py1 < py2) return -1;
	if(py1 > py2) return 1;
	if(ws1->depth < ws2->depth) return -1;
	if(ws1->depth > ws2->depth) return 1;
	if(ws1->x < ws2->x) return -1;
	if(ws1->x > ws2->x) return 1;
	return 0;
}

typedef struct World{
	Vector animations; 		// Vector<Animation>
	Vector entityatlas;		// Vector<EntityAtlas>
	Vector sprite_out; 		// Vector<World_Sprite>
	Vector dims; 			// PVector<World_Dimension>
	uint32_t dim_in;
	int32_t origin_x;
	int32_t origin_y;
	int32_t t_origin_x;
	int32_t t_origin_y;
	Vec2 spawn;
	AnimationBg mode;
	AnimationBg settype;
	AnimationBg fill;
	char* path;
	void* (*Spawner)(Vec2,SpawnType,unsigned int*);
} World;

void World_ReloadChunks(World* w);
World World_Init(char* path){
	World w;
	w.animations = Vector_New(sizeof(Animation));
	w.entityatlas = Vector_New(sizeof(EntityAtlas));
	w.mode = ANIMATIONBG_ALL;
	w.settype = 0U;
	w.fill = 0U;

	w.path = CStr_Cpy(path);
	w.sprite_out = Vector_New(sizeof(World_Sprite));
	w.dims = Vector_New(sizeof(World_Dimension));
	w.dim_in = 0U;
	
	w.origin_x = 0;
	w.origin_y = 0;
	w.t_origin_x = 0;
	w.t_origin_y = 0;
	w.spawn = (Vec2){ 0.0f,0.0f };

	Vector_Push(&w.dims,(World_Dimension[]){ World_Dimension_New() });
	return w;
}
World World_New(char* path){
	World w = World_Init(path);
	World_ReloadChunks(&w);
	return w;
}
void World_Sprite_Push(World* w,SubSprite ss,float x,float y,uint32_t alpha,uint32_t depth){
	if(!ss.sp) return;

	World_Sprite sprite = {
		.sp = ss,
		.x = x,
		.y = y,
		.alpha = alpha,
		.depth = depth
	};
	Vector_Push(&w->sprite_out,&sprite);
}
uint32_t World_Block_RenderDepth(Block b){
	switch(b){
		case BLOCK_NONE: return WORLD_SPRITE_DEPTH_BACKGROUND;
		case BLOCK_DOOR: return WORLD_SPRITE_DEPTH_UNDERLAY;
		case BLOCK_WATER: return WORLD_SPRITE_DEPTH_GROUND;
		case BLOCK_GRAS:
		case BLOCK_STONE:
		case BLOCK_DOCK: return WORLD_SPRITE_DEPTH_GROUND;
		default: return WORLD_SPRITE_DEPTH_OBJECT;
	}
}
World World_Make(char* Path,Animation* a,EntityAtlas* ea){
	World w = World_Init(Path);

	for(int i = 0;a[i].type != ANIMATIONTYPE_NONE;i++){
		Vector_Push(&w.animations,a + i);
	}
	for(int i = 0;ea[i].atlas.img;i++){
		Vector_Push(&w.entityatlas,ea + i);
	}

	World_ReloadChunks(&w);
	return w;
}
char World_isBlock(World* w,uint32_t b){
	return b < w->animations.size;
}
EntityAtlas* World_EntityAtlas_Get(World* w,const World_Entity* e){
	if(!w || !e || e->id == ENTITY_NONE || e->id > (uint32_t)w->entityatlas.size)
		return NULL;
	return (EntityAtlas*)Vector_Get(&w->entityatlas,e->id - 1U);
}
void World_Chunk_Free(World* w,World_Chunk* ch){
	for(int i = 0;i<ch->tags.size;i++){
		World_Chunk_Tag* ct = (World_Chunk_Tag*)Vector_Get(&ch->tags,i);
		World_Chunk_Tag_Free(ct);
	}
	for(int i = 0;i<ch->entities.size;i++){
		World_Entity* e = (World_Entity*)PVector_Get(&ch->entities,i);
		EntityAtlas* ea = World_EntityAtlas_Get(w,e);
		
		if(ea && ea->Free)
			ea->Free(e);
	}
	
	Vector_Free(&ch->tags);
	PVector_Free(&ch->entities);
}

World_Chunk* World_FindChunk(World* w,int32_t x,int32_t y){
	World_Dimension* dim = (World_Dimension*)Vector_Get(&w->dims,w->dim_in);
	Vector* chunks = &dim->chunks;

	for(uint32_t row_index = 0;row_index < chunks->size;row_index++){
		World_Chunk_Row* row = (World_Chunk_Row*)Vector_Get(chunks,row_index);
		for(uint32_t chunk_index = 0;chunk_index < row->size;chunk_index++){
			World_Chunk* ch = (World_Chunk*)Vector_Get(row,chunk_index);
			if(ch->x == x && ch->y == y) return ch;
		}
	}
	return NULL;
}
char World_Chunks_AreCurrent(World* w,int32_t origin_x,int32_t origin_y){
	Vector* chunks = (Vector*)Vector_Get(&w->dims,w->dim_in);
	if(chunks->size != WORLD_CHUNK_COUNT_Y) return 0;

	for(int32_t row_index = 0;row_index < WORLD_CHUNK_COUNT_Y;row_index++){
		World_Chunk_Row* row = (World_Chunk_Row*)Vector_Get(chunks,row_index);
		if(row->size != WORLD_CHUNK_COUNT_X) return 0;

		for(int32_t chunk_index = 0;chunk_index < WORLD_CHUNK_COUNT_X;chunk_index++){
			World_Chunk* ch = (World_Chunk*)Vector_Get(row,chunk_index);
			if(ch->x != origin_x - WORLD_CHUNK_RADIUS_X + chunk_index ||
			   ch->y != origin_y - WORLD_CHUNK_RADIUS_Y + row_index)
				return 0;
		}
	}
	return 1;
}

void World_Block_Place(World* w,World_Chunk* ch,uint8_t cx,uint8_t cy,Block b){
	switch (b){
		case BLOCK_CHEST:{
			World_Chunk_Tag ct = World_Chunk_Tag_Chest_New(cx,cy,20);
			Vector_Push(&ch->tags,&ct);
			break;
		}
	}
}
void World_Block_Remove(World* w,World_Chunk* ch,uint8_t cx,uint8_t cy){
	if(cx >= WORLD_CHUNK_WIDTH || cy >= WORLD_CHUNK_HEIGHT){
		printf("[World_Block]: Remove -> (%u,%u) out of bounds!\n",cx,cy);
		return;
	}
	
	World_Chunk_Remove(ch,cx,cy);
}

Vic2 World_GetChunkCoord(World* w,int32_t x,int32_t y){
	const float foffset_x = floorf((float)x / (float)WORLD_CHUNK_WIDTH);
	const float foffset_y = floorf((float)y / (float)WORLD_CHUNK_HEIGHT);
	return (Vic2){ (int32_t)foffset_x,(int32_t)foffset_y };
}
Vic2 World_GetRelCoord(World* w,int32_t x,int32_t y){
	World_Dimension* dim = (World_Dimension*)Vector_Get(&w->dims,w->dim_in);
	Vector* chunks = &dim->chunks;

	const float foffset_x = floorf((float)x / (float)WORLD_CHUNK_WIDTH);
	const float foffset_y = floorf((float)y / (float)WORLD_CHUNK_HEIGHT);

	const int32_t offset_x = (int32_t)foffset_x - w->origin_x + WORLD_CHUNK_RADIUS_X;
	const int32_t offset_y = (int32_t)foffset_y - w->origin_y + WORLD_CHUNK_RADIUS_Y;
	
	if(offset_y < 0 || offset_y >= chunks->size) return (Vic2){ -1,-1 };
	World_Chunk_Row* row = (World_Chunk_Row*)Vector_Get(chunks,offset_y);
	
	if(offset_x < 0 || offset_x >= row->size) return (Vic2){ -1,-1 };
	World_Chunk* ch = (World_Chunk*)Vector_Get(row,offset_x);
	const int32_t ch_x = x - ch->x * WORLD_CHUNK_WIDTH;
	const int32_t ch_y = y - ch->y * WORLD_CHUNK_HEIGHT;
	return (Vic2){ ch_x,ch_y };
}

Block World_Get(World* w,int32_t x,int32_t y){
	World_Dimension* dim = (World_Dimension*)Vector_Get(&w->dims,w->dim_in);
	Vector* chunks = &dim->chunks;

	const float foffset_x = floorf((float)x / (float)WORLD_CHUNK_WIDTH);
	const float foffset_y = floorf((float)y / (float)WORLD_CHUNK_HEIGHT);

	const int32_t offset_x = (int32_t)foffset_x - w->origin_x + WORLD_CHUNK_RADIUS_X;
	const int32_t offset_y = (int32_t)foffset_y - w->origin_y + WORLD_CHUNK_RADIUS_Y;
	
	if(offset_y < 0 || offset_y >= chunks->size) return BLOCK_NONE;
	World_Chunk_Row* row = (World_Chunk_Row*)Vector_Get(chunks,offset_y);
	
	if(offset_x < 0 || offset_x >= row->size) return BLOCK_NONE;
	World_Chunk* ch = (World_Chunk*)Vector_Get(row,offset_x);
	const int32_t ch_x = x - ch->x * WORLD_CHUNK_WIDTH;
	const int32_t ch_y = y - ch->y * WORLD_CHUNK_HEIGHT;
	return ch->data[ch_y * WORLD_CHUNK_WIDTH + ch_x];
}
unsigned char World_GetNeigbours(World* w,Block tile,int32_t x,int32_t y){
	unsigned char nbs = 0U;
	int shift = 0;

	// Skipping the center assigns bits in NW, N, NE, W, E, SW, S, SE order.
	for(int i = 0;i<9;i++){
		if(i == 4) continue;

		const int px = i % 3;
		const int py = i / 3;
		const Block tile_found = World_Get(w,x - 1 + px,y - 1 + py);
		nbs |= ((uint32_t)tile_found == tile || tile_found == BLOCK_BORDER) << (shift++);
	}
	
	return nbs;
}
void World_Set(World* w,int32_t x,int32_t y,Block b){
	World_Dimension* dim = (World_Dimension*)Vector_Get(&w->dims,w->dim_in);
	Vector* chunks = &dim->chunks;

	const float foffset_x = floorf((float)x / (float)WORLD_CHUNK_WIDTH);
	const float foffset_y = floorf((float)y / (float)WORLD_CHUNK_HEIGHT);

	const int32_t offset_x = (int32_t)foffset_x - w->origin_x + WORLD_CHUNK_RADIUS_X;
	const int32_t offset_y = (int32_t)foffset_y - w->origin_y + WORLD_CHUNK_RADIUS_Y;
	
	if(offset_y < 0 || offset_y >= chunks->size) return;
	World_Chunk_Row* row = (World_Chunk_Row*)Vector_Get(chunks,offset_y);
	
	if(offset_x < 0 || offset_x >= row->size) return;
	World_Chunk* ch = (World_Chunk*)Vector_Get(row,offset_x);
	const int32_t ch_x = x - ch->x * WORLD_CHUNK_WIDTH;
	const int32_t ch_y = y - ch->y * WORLD_CHUNK_HEIGHT;
	
	ch->data[ch_y * WORLD_CHUNK_WIDTH + ch_x] = b;
	World_Block_Remove(w,ch,ch_x,ch_y);
	World_Block_Place(w,ch,ch_x,ch_y,b);
}

void* World_Spawn(World* w,void* data){
	World_Dimension* dim = (World_Dimension*)Vector_Get(&w->dims,w->dim_in);
	if(!dim || !data) return NULL;

	const World_Entity* e_data = (World_Entity*)data;
	if(!World_EntityAtlas_Get(w,e_data)) return NULL;
	const Vic2 ch_p = World_GetChunkCoord(w,(int32_t)floorf(e_data->r.p.x),(int32_t)floorf(e_data->r.p.y));

	const int32_t row_index = ch_p.y - w->origin_y + WORLD_CHUNK_RADIUS_Y;
	const int32_t column_index = ch_p.x - w->origin_x + WORLD_CHUNK_RADIUS_X;
	if(row_index < 0 || row_index >= dim->chunks.size) return NULL;
	
	World_Chunk_Row* row = (World_Chunk_Row*)Vector_Get(&dim->chunks,row_index);
	if(!row || column_index < 0 || column_index >= row->size) return NULL;
	
	World_Chunk* ch = (World_Chunk*)Vector_Get(row,column_index);
	if(!ch) return NULL;
	
	PVector_PPush(&ch->entities,data);
	return PVector_Get(&ch->entities,ch->entities.size - 1);
}
void World_Remove(World* w,World_Entity* e){
	World_Dimension* dim = (World_Dimension*)Vector_Get(&w->dims,w->dim_in);
	if(!dim) return;
	
	for(uint32_t row_index = 0;row_index < dim->chunks.size;row_index++){
		World_Chunk_Row* row = (World_Chunk_Row*)Vector_Get(&dim->chunks,row_index);
		
		for(uint32_t chunk_index = 0;chunk_index < row->size;chunk_index++){
			World_Chunk* ch = (World_Chunk*)Vector_Get(row,chunk_index);
			
			for(int k = 0;k<ch->entities.size;k++){
				World_Entity* te = (World_Entity*)PVector_Get(&ch->entities,k);
				if(!te) continue;

				if(te == e){
					EntityAtlas* ea = World_EntityAtlas_Get(w,te);
					if(ea && ea->Free) ea->Free(te);
					PVector_Remove(&ch->entities,k);
					return;
				}
			}
		}
	}
}
void World_RemoveAt(World* w,float x,float y){
	World_Dimension* dim = (World_Dimension*)Vector_Get(&w->dims,w->dim_in);
	if(!dim) return;

	for(int row_index = 0;row_index < dim->chunks.size;row_index++){
		World_Chunk_Row* row = (World_Chunk_Row*)Vector_Get(&dim->chunks,row_index);
		if(!row) continue;

		for(int column_index = 0;column_index < row->size;column_index++){
			World_Chunk* ch = (World_Chunk*)Vector_Get(row,column_index);
			if(!ch) continue;

			for(int k = 0;k<ch->entities.size;k++){
				World_Entity* e = (World_Entity*)PVector_Get(&ch->entities,k);
				if(!e || !Rect_Point_Overlap(e->r,(Vec2){ x,y })) continue;

				EntityAtlas* ea = World_EntityAtlas_Get(w,e);
				if(ea && ea->Free) ea->Free(e);
				PVector_Remove(&ch->entities,k);
				return;
			}
		}
	}
}

SubSprite World_Img(World* w,uint32_t b,int32_t x,int32_t y){
	if(World_isBlock(w,b)){
		Animation* a = (Animation*)Vector_Get(&w->animations,b);
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
		World_Dimension* dim = (World_Dimension*)Vector_Get(&w->dims,w->dim_in);
		if(!dim) return;

		for(uint32_t row_index = 0;row_index < dim->chunks.size;row_index++){
			World_Chunk_Row* row = (World_Chunk_Row*)Vector_Get(&dim->chunks,row_index);
			
			for(uint32_t chunk_index = 0;chunk_index < row->size;chunk_index++){
				World_Chunk* ch = (World_Chunk*)Vector_Get(row,chunk_index);
				
				for(int k = 0;k<ch->entities.size;k++){
					World_Entity* e = (World_Entity*)PVector_Get(&ch->entities,k);
					if(!e) continue;
					
					EntityAtlas* ea = World_EntityAtlas_Get(w,e);
					if(ea && ea->Update && TransformedView_inViewRect(tv,e->r))
						ea->Update(e,w,t);
				}
			}
		}
	}
}

void World_UpdateChunks(World* w);
char World_Chunks_Contain(Vector* chunks,int32_t x,int32_t y){
	for(uint32_t row_index = 0;row_index < chunks->size;row_index++){
		World_Chunk_Row* row = (World_Chunk_Row*)Vector_Get(chunks,row_index);
		for(uint32_t chunk_index = 0;chunk_index < row->size;chunk_index++){
			World_Chunk* ch = (World_Chunk*)Vector_Get(row,chunk_index);
			if(ch->x == x && ch->y == y) return 1;
		}
	}
	return 0;
}
void World_SaveChunks(World* w){
	World_Dimension* dim = (World_Dimension*)Vector_Get(&w->dims,w->dim_in);
	if(!dim) return;

	for(uint32_t row_index = 0U;row_index < dim->chunks.size;row_index++){
		World_Chunk_Row* row = (World_Chunk_Row*)Vector_Get(&dim->chunks,row_index);
		for(uint32_t chunk_index = 0U;chunk_index < row->size;chunk_index++)
			World_Chunk_Save((World_Chunk*)Vector_Get(row,chunk_index),w,w->path);
	}
}
void World_ReloadChunks(World* w){
	World_Dimension* dim = (World_Dimension*)Vector_Get(&w->dims,w->dim_in);
	Vector* chunks = &dim->chunks;
	World_SaveChunks(w);

	for(int32_t i = 0;i<chunks->size;i++){
		World_Chunk_Row* row = (World_Chunk_Row*)Vector_Get(chunks,i);
		World_Chunk_Row_Free(w,row);
	}
	Vector_Free(chunks);

	Vector_Set(&w->dims,(World_Dimension[]){ World_Dimension_New() },w->dim_in);
	World_UpdateChunks(w);
}
void World_UpdateChunks(World* w){
	if(World_Chunks_AreCurrent(w,w->t_origin_x,w->t_origin_y)){
		w->origin_x = w->t_origin_x;
		w->origin_y = w->t_origin_y;
		return;
	}

	Vector next_chunks = Vector_New(sizeof(World_Chunk_Row));
	
	for(int32_t row_index = 0;row_index < WORLD_CHUNK_COUNT_Y;row_index++){
		const int32_t chunk_y = w->t_origin_y - WORLD_CHUNK_RADIUS_Y + row_index;
		World_Chunk_Row row = World_Chunk_Row_New();

		for(int32_t chunk_index = 0;chunk_index < WORLD_CHUNK_COUNT_X;chunk_index++){
			const int32_t chunk_x = w->t_origin_x - WORLD_CHUNK_RADIUS_X + chunk_index;
			World_Chunk* existing = World_FindChunk(w,chunk_x,chunk_y);
			World_Chunk chunk;
			if(existing){
				chunk = *existing;
				existing->tags = Vector_Null();
				existing->entities = PVector_Null();
			}else{
				chunk = World_Chunk_Load(w,w->path,chunk_x,chunk_y);
			}
			Vector_Push(&row,&chunk);
		}
		Vector_Push(&next_chunks,&row);
	}

	World_Dimension* dim = (World_Dimension*)Vector_Get(&w->dims,w->dim_in);
	Vector* chunks = &dim->chunks;

	for(uint32_t row_index = 0;row_index < chunks->size;row_index++){
		World_Chunk_Row* row = (World_Chunk_Row*)Vector_Get(chunks,row_index);
		for(uint32_t chunk_index = 0;chunk_index < row->size;chunk_index++){
			World_Chunk* ch = (World_Chunk*)Vector_Get(row,chunk_index);
			if(!World_Chunks_Contain(&next_chunks,ch->x,ch->y))
				World_Chunk_Save(ch,w,w->path);
		}
		World_Chunk_Row_Free(w,row);
	}
	Vector_Free(chunks);

	Vector_Set(&w->dims,(World_Dimension[]){ World_Dimension_Move(next_chunks) },w->dim_in);
	w->origin_x = w->t_origin_x;
	w->origin_y = w->t_origin_y;
}

char World_Entity_IsInside(World* w,int32_t ri,int32_t ci,World_Entity* e){
	World_Dimension* dim = (World_Dimension*)Vector_Get(&w->dims,w->dim_in);
	if(!dim || !e) return 1;

	const int32_t r_index = w->origin_y - WORLD_CHUNK_RADIUS_Y + ri;
	const int32_t c_index = w->origin_x - WORLD_CHUNK_RADIUS_X + ci;
	const Vic2 ch_p = World_GetChunkCoord(w,(int32_t)floorf(e->r.p.x),(int32_t)floorf(e->r.p.y));
	return r_index == ch_p.y && c_index == ch_p.x;
}
void World_Entity_Move(World* w,uint32_t ei,int32_t ri,int32_t ci){
	World_Dimension* dim = (World_Dimension*)Vector_Get(&w->dims,w->dim_in);
	if(!dim) return;

	if(ri < 0 || ri >= dim->chunks.size) return;
	World_Chunk_Row* row = (World_Chunk_Row*)Vector_Get(&dim->chunks,ri);
	if(!row || ci < 0 || ci >= row->size) return;
	World_Chunk* ch = (World_Chunk*)Vector_Get(row,ci);
	if(!ch || ei >= (uint32_t)ch->entities.size) return;

	World_Entity* e = (World_Entity*)PVector_Get(&ch->entities,ei);
	if(!e) return;
	
	const Vic2 ch_p = World_GetChunkCoord(w,(int32_t)floorf(e->r.p.x),(int32_t)floorf(e->r.p.y));
	const int32_t new_ri = ch_p.y - w->origin_y + WORLD_CHUNK_RADIUS_Y;
	const int32_t new_ci = ch_p.x - w->origin_x + WORLD_CHUNK_RADIUS_X;
	if(new_ri < 0 || new_ri >= dim->chunks.size) return;
	World_Chunk_Row* new_row = (World_Chunk_Row*)Vector_Get(&dim->chunks,new_ri);
	if(!new_row) return;

	if(new_ci < 0 || new_ci >= new_row->size) return;
	World_Chunk* new_ch = (World_Chunk*)Vector_Get(new_row,new_ci);
	if(!new_ch || new_ch == ch) return;
	PVector_PPush(&new_ch->entities,e);

	PVector_PRemove(&ch->entities,ei);
}
void World_Collision(World* w,World_Entity* src){
	if(src){
		if(w->mode!=ANIMATIONBG_DG){
			EntityAtlas* ea = World_EntityAtlas_Get(w,src);
			Vector rects = Vector_New(sizeof(TargetRect));
			
			float searchX = F32_Max(2.0f,2.0f * src->r.d.x);
			float searchY = F32_Max(2.0f,2.0f * src->r.d.y);

			for(float x = -searchX;x<searchX;x+=1.0f) {
				for(float y = -searchY;y<searchY;y+=1.0f) {
					const int32_t sx = (int32_t)floorf(src->r.p.x + x);
					const int32_t sy = (int32_t)floorf(src->r.p.y + y);
					const Block b = World_Get(w,sx,sy);
					const Rect br = { (Vec2){ sx,sy },(Vec2){ 1.0f,1.0f } };

					if(b != BLOCK_NONE && Overlap_Rect_Rect(src->r,br))
						Vector_Push(&rects,(TargetRect[]){ br,&src->r.p });
				}
			}

			qsort(rects.Memory,rects.size,rects.ELEMENT_SIZE,TargetRect_Compare);

			for(int i = 0;i<rects.size;i++) {
    		    TargetRect* r = (TargetRect*)Vector_Get(&rects,i);

				Side s = Side_Rect_Rect(src->r,r->r);
				const char collision = ea->IsSolid ? ea->IsSolid(src,w,r->r.p.x,r->r.p.y,s) : 0;
				
				if(collision){
					Resolve_Rect_Rect_Side(&src->r,r->r,s);
					if(ea->Collision)
						ea->Collision(src,w,r->r.p.x,r->r.p.y,s);
				}
			}

			Vector_Free(&rects);

			if(ea->WorldCollision)
				ea->WorldCollision(src,w);
		}
	}
}
void World_EntityCollision(World* w,World_Entity* src,int32_t ri,int32_t ci){
	if(src && w->mode!=ANIMATIONBG_DG){
		World_Dimension* dim = (World_Dimension*)Vector_Get(&w->dims,w->dim_in);
		if(!dim) return;

		const int32_t radius_y = (int32_t)ENTITY_COLLISION_RADIUS_Y;
		const int32_t radius_x = (int32_t)ENTITY_COLLISION_RADIUS_X;

		for(int32_t i = -radius_y;i<=radius_y;i++) {
    	    const int32_t rindex = ri + i;
			if(rindex < 0 || rindex >= dim->chunks.size) continue;

			World_Chunk_Row* row = (World_Chunk_Row*)Vector_Get(&dim->chunks,rindex);

			for(int32_t j = -radius_x;j<=radius_x;j++) {
    		    const int32_t cindex = ci + j;
				if(cindex < 0 || cindex >= row->size) continue;

				World_Chunk* ch = (World_Chunk*)Vector_Get(row,cindex);
				for(int32_t k = 0;k<ch->entities.size;k++) {
    			    World_Entity* e = (World_Entity*)PVector_Get(&ch->entities,k);
					if(!e || src == e) continue;

					EntityAtlas* ea = World_EntityAtlas_Get(w,src);
					if(Overlap_Rect_Rect(src->r,e->r)){
						Side s = Side_Rect_Rect(src->r,e->r);

						if(ea->EntityCollision)
							ea->EntityCollision(src,w,e,e->r.p.x,e->r.p.y,s);
					}
				}
			}
		}
	}
}
void World_CollisionsEntity(World* w,TransformedView* tv,World_Entity* e){
	if(!e || !World_EntityAtlas_Get(w,e) || !TransformedView_inViewRect(tv,e->r))
		return;
	
	const Vic2 ch_p = World_GetChunkCoord(w,(int32_t)floorf(e->r.p.x),(int32_t)floorf(e->r.p.y));
	const int32_t row_index = ch_p.y - w->origin_y + WORLD_CHUNK_RADIUS_Y;
	const int32_t chunk_index = ch_p.x - w->origin_x + WORLD_CHUNK_RADIUS_X;
	World_EntityCollision(w,e,row_index,chunk_index);
	World_Collision(w,e);
}
void World_Collisions(World* w,TransformedView* tv){
	if(w->mode == ANIMATIONBG_DG) return;
	World_Dimension* dim = (World_Dimension*)Vector_Get(&w->dims,w->dim_in);
	if(!dim) return;

	for(uint32_t row_index = 0;row_index < dim->chunks.size;row_index++){
		World_Chunk_Row* row = (World_Chunk_Row*)Vector_Get(&dim->chunks,row_index);
		if(!row) continue;

		for(uint32_t chunk_index = 0;chunk_index < row->size;chunk_index++){
			World_Chunk* ch = (World_Chunk*)Vector_Get(row,chunk_index);
			if(!ch) continue;

			for(int entity_index = 0;entity_index < ch->entities.size;entity_index++){
				World_Entity* e = (World_Entity*)PVector_Get(&ch->entities,entity_index);
				
				if(!e || !World_EntityAtlas_Get(w,e) || !TransformedView_inViewRect(tv,e->r))
					continue;
				
				World_EntityCollision(w,e,row_index,chunk_index);
				World_Collision(w,e);
			}
		}
	}

	for(uint32_t row_index = 0;row_index < dim->chunks.size;row_index++){
		World_Chunk_Row* row = (World_Chunk_Row*)Vector_Get(&dim->chunks,row_index);
		if(!row) continue;
		for(uint32_t chunk_index = 0;chunk_index < row->size;chunk_index++){
			World_Chunk* ch = (World_Chunk*)Vector_Get(row,chunk_index);
			if(!ch) continue;
			for(int entity_index = 0;entity_index < ch->entities.size;){
				World_Entity* e = (World_Entity*)PVector_Get(&ch->entities,entity_index);

				if(e && !World_Entity_IsInside(w,row_index,chunk_index,e)){
					const int old_size = ch->entities.size;
					World_Entity_Move(w,(uint32_t)entity_index,row_index,chunk_index);
					if(ch->entities.size < old_size)
						continue;
				}
				entity_index++;
			}
		}
	}
}

void World_RenderEntity(World* w,TransformedView* tv,Pixel* out,unsigned int width,unsigned int height,World_Entity* e){
	if(!e) return;
	
	if(!TransformedView_inViewRect(tv,e->r))
		return;
	
	EntityAtlas* ea = World_EntityAtlas_Get(w,e);
	if(ea && ea->GetRender){
		SubSprite ss = ea->GetRender(e,ea);
		const Vec2 rect_pos = TransformedView_WorldScreenPos(tv,e->r.p);
		const Vec2 rect_size = TransformedView_WorldScreenLength(tv,e->r.d);
		const float x = rect_pos.x + (rect_size.x - ss.dx) * 0.5f;
		const float y = rect_pos.y + rect_size.y - ss.dy;
		World_Sprite_Push(w,ss,x,y,1U,WORLD_SPRITE_DEPTH_ENTITY);
	}
}
void World_RenderEntities(World* w,TransformedView* tv,Pixel* out,unsigned int width,unsigned int height){
	World_Dimension* dim = (World_Dimension*)Vector_Get(&w->dims,w->dim_in);
	if(!dim) return;

	const Vec2 tl = TransformedView_ScreenWorldPos(tv,(Vec2){ 0.0f,0.0f });
	const Vec2 br = TransformedView_ScreenWorldPos(tv,(Vec2){ width,height });

	const Vic2 ch_tl = World_GetChunkCoord(w,(int32_t)floorf(tl.x),(int32_t)floorf(tl.y));
	const Vic2 ch_br = World_GetChunkCoord(w,(int32_t)floorf(br.x),(int32_t)floorf(br.y));

	int32_t row_start = ch_tl.y - (w->origin_y - WORLD_CHUNK_RADIUS_Y);
	int32_t row_end = ch_br.y - (w->origin_y - WORLD_CHUNK_RADIUS_Y);
	int32_t column_start = ch_tl.x - (w->origin_x - WORLD_CHUNK_RADIUS_X);
	int32_t column_end = ch_br.x - (w->origin_x - WORLD_CHUNK_RADIUS_X);

	if(row_start < 0) row_start = 0;
	if(column_start < 0) column_start = 0;
	if(row_end >= dim->chunks.size) row_end = dim->chunks.size - 1;
	if(row_start > row_end) return;

	for(int32_t row_index = row_start;row_index <= row_end;row_index++){
		World_Chunk_Row* row = (World_Chunk_Row*)Vector_Get(&dim->chunks,row_index);
		if(!row) continue;

		const int32_t row_column_end = column_end < row->size ? column_end : row->size - 1;
		if(column_start > row_column_end) continue;

		for(int32_t column_index = column_start;column_index <= row_column_end;column_index++){
			World_Chunk* ch = (World_Chunk*)Vector_Get(row,column_index);
			if(!ch) continue;

			for(int entity_index = 0;entity_index < ch->entities.size;entity_index++){
				World_Entity* e = (World_Entity*)PVector_Get(&ch->entities,entity_index);
				World_RenderEntity(w,tv,out,width,height,e);
			}
		}
	}
}
void World_RenderTiles(World* w,TransformedView* tv,Pixel* out,unsigned int width,unsigned int height){
	const Vec2 tl = TransformedView_ScreenWorldPos(tv,(Vec2){ 0.0f,0.0f });
	const Vec2 br = TransformedView_ScreenWorldPos(tv,(Vec2){ width,height });
	const Vec2 sd = TransformedView_WorldScreenLength(tv,(Vec2){ 1.0f,1.0f });
	World_Reload(w,(unsigned int)F32_Ceil(sd.x),(unsigned int)F32_Ceil(sd.y));
	
	for(int y = tl.y - 1;y<br.y;y++){
		for(int x = tl.x - 1;x<br.x;x++){
			const Block b = World_Get(w,x,y);
			const Vec2 sc = TransformedView_WorldScreenPos(tv,(Vec2){ x,y });
			
			SubSprite bg_ss = World_Img(w,BLOCK_NONE,0,0);
			World_Sprite_Push(w,bg_ss,sc.x,sc.y,0U,0U);

			if(b != BLOCK_NONE){
				if(b == BLOCK_DOCK){
					SubSprite water_ss = World_Img(w,BLOCK_WATER,x,y);
					World_Sprite_Push(w,water_ss,sc.x,sc.y,1U,WORLD_SPRITE_DEPTH_UNDERLAY);
				}

				SubSprite ss = World_Get_Img(w,x,y);
				if(ss.sp){
					World_Sprite_Push(w,ss,sc.x,sc.y,1U,World_Block_RenderDepth(b));
				}
			}
		}
	}
}
void World_Render(World* w,TransformedView* tv,Pixel* out,unsigned int width,unsigned int height){
	if(w->sprite_out.size > 1U)
		qsort(w->sprite_out.Memory,w->sprite_out.size,w->sprite_out.ELEMENT_SIZE,World_Sprite_Compare);

	for(int32_t i = 0;i<w->sprite_out.size;i++){
		World_Sprite* ws = (World_Sprite*)Vector_Get(&w->sprite_out,i);
		if(ws->alpha)	Sprite_RenderSubAlpha(out,width,height,ws->sp.sp,ws->x,ws->y,ws->sp.ox,ws->sp.oy,ws->sp.dx,ws->sp.dy);
		else			Sprite_RenderSub(out,width,height,ws->sp.sp,ws->x,ws->y,ws->sp.ox,ws->sp.oy,ws->sp.dx,ws->sp.dy);
	}

	Vector_Clear(&w->sprite_out);
}
void World_Free(World* w){
	World_SaveChunks(w);
	
	for(int i = 0;i<w->animations.size;i++){
		Animation* s = (Animation*)Vector_Get(&w->animations,i);
		Animation_Free(s);
	}
	Vector_Free(&w->animations);

	for(int i = 0;i<w->dims.size;i++){
		World_Dimension* dim = (World_Dimension*)Vector_Get(&w->dims,i);
		World_Dimension_Free(w,dim);
	}
	Vector_Free(&w->dims);
	for(int i = 0;i<w->entityatlas.size;i++){
		EntityAtlas* ea = (EntityAtlas*)Vector_Get(&w->entityatlas,i);
		EntityAtlas_Free(ea);
	}
	Vector_Free(&w->entityatlas);

	Vector_Free(&w->sprite_out);
	CStr_Free(&w->path);
}

#endif