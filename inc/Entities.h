#ifndef ENTITIES_H
#define ENTITIES_H

#include "World.h"
#include "/home/codeleaded/System/Static/Library/AudioPlayer.h"

#define ENTITY_FALSE		            0
#define ENTITY_TRUE			            1

typedef enum Entity_ID {
	ENTITY_ID_NONE = 0U,
	ENTITY_ID_BLACKHAIR = 1U,
	ENTITY_ID_TREE = 2U,
	ENTITY_ID_WOOD = 3U
} Entity_ID;

typedef enum Entity_LD {
	ENTITY_LD_NONE = 0U,
	ENTITY_LD_DOWN = 1U,
	ENTITY_LD_LEFT = 2U,
	ENTITY_LD_RIGHT = 3U,
	ENTITY_LD_UP = 4U
} Entity_LD;

#define PLAYER_DIM_X	               	0.5f
#define PLAYER_DIM_Y					1.2f
#define PLAYER_ACC		            	30.0f
#define PLAYER_ACC_MAX		            50.0f
#define PLAYER_VEL_CLP		            0.01f
#define PLAYER_VEL_MAX	            	5.0f
#define PLAYER_DOOR_EXIT_DISTANCE		1.5f
#define PLAYER_INVENTORY_WIDTH			(9)
#define PLAYER_INVENTORY_HEIGHT			(3)
#define PLAYER_SAVE_MAGIC				0x504C5952U

typedef struct Player {
	World_Entity e;
	Vec2 spawn;
	Timepoint start;
	Entity_LD lookdir;
	World_Item_Stack hand;
	World_Inventory inventory;
} Player;

typedef struct World_Item_Entity {
	World_Entity e;
	World_Item_Stack stack;
} World_Item_Entity;

void Player_Move(Player* m,const float dir_x,const float dir_y){
	// min for clean acc
	if(dir_x < -0.2f){
		m->e.a.x = -PLAYER_ACC * F32_Abs(dir_x);
	}else if(dir_x > 0.2f){
		m->e.a.x = PLAYER_ACC * F32_Abs(dir_x);
	}else
		m->e.a.x = 0.0f;

	if(dir_y < -0.2f){
		m->e.a.y = -PLAYER_ACC * F32_Abs(dir_y);
	}else if(dir_y > 0.2f){
		m->e.a.y = PLAYER_ACC * F32_Abs(dir_y);
	}else
		m->e.a.y = 0.0f;
}
void Player_Update(Player* m,World* w,float t){
    m->e.a.x = F32_Clamp(m->e.a.x,-PLAYER_ACC_MAX,PLAYER_ACC_MAX);
	m->e.v.x = F32_Clamp(m->e.v.x,-PLAYER_VEL_MAX,PLAYER_VEL_MAX);
    m->e.a.y = F32_Clamp(m->e.a.y,-PLAYER_ACC_MAX,PLAYER_ACC_MAX);
	m->e.v.y = F32_Clamp(m->e.v.y,-PLAYER_VEL_MAX,PLAYER_VEL_MAX);

	const char exiting_door = w && w->portal_exit_timer > 0.0f;
	if(exiting_door) w->portal_exit_timer = F32_Max(0.0f,w->portal_exit_timer - t);

	if(m->e.v.x != 0.0f || m->e.v.y != 0.0f){
		if(F32_Abs(m->e.v.x) >= F32_Abs(m->e.v.y))
			m->lookdir = m->e.v.x < 0.0f ? ENTITY_LD_LEFT : ENTITY_LD_RIGHT;
		else
			m->lookdir = m->e.v.y < 0.0f ? ENTITY_LD_UP : ENTITY_LD_DOWN;
	}

	if(F32_Abs(m->e.v.x) > PLAYER_VEL_CLP){
		const float add = -F32_Sign(m->e.v.x) * PLAYER_ACC * t;
		m->e.v.x = F32_Passes(m->e.v.x,add,0.0f) ? 0.0f : m->e.v.x + add;
	}else
		m->e.v.x = 0.0f;

	if(exiting_door){
		m->e.v.y = PLAYER_VEL_MAX;
	}else if(F32_Abs(m->e.v.y) > PLAYER_VEL_CLP){
		const float add = -F32_Sign(m->e.v.y) * PLAYER_ACC * t;
		m->e.v.y = F32_Passes(m->e.v.y,add,0.0f) ? 0.0f : m->e.v.y + add;
	}else
		m->e.v.y = 0.0f;

	m->e.v.x = m->e.v.x + F32_Krung(m->e.a.x,PLAYER_ACC_MAX) * t;
	if(!exiting_door) m->e.v.y = m->e.v.y + F32_Krung(m->e.a.y,PLAYER_ACC_MAX) * t;

	m->e.r.p = Vec2_Add(m->e.r.p,Vec2_Mulf(m->e.v,t));
}
void Player_WorldCollision(Player* m,World* w){
	//if(m->e.r.p.x < 0.0f) m->e.r.p.x = 0.0f;
	//if(m->e.r.p.y < 0.0f) m->e.r.p.y = 0.0f;
}
char Player_IsSolid(Player* m,World* w,int32_t x,int32_t y,Side s){
	const Block b = World_Get(w,x,y);
	const Block up = World_Get(w,x,y - 1);
	const Block down = World_Get(w,x,y + 1);
	const Block down2 = World_Get(w,x,y + 2);

	const char b_s = World_IsSolid(b);
	const char b_w = World_IsWalkable(b);
	const char b_b = b_s || !b_w;

	const char u_s = World_IsSolid(up);
	const char u_w = World_IsWalkable(up);
	const char u_b = u_s || !u_w;

	const char d_s = World_IsSolid(down);
	const char d_s2 = World_IsSolid(down2);
	const char d_w = World_IsWalkable(down);
	const char d_b = d_s || !d_w;

	return (
		(b_s && (
			(!u_s &&  d_s && !d_s2) ||
			( u_s &&  d_s) ||
			(!u_s && !d_s)
		)) || (!b_w && (
			(!u_s && u_w) ||
			d_b
		))
	);
}
void Player_Collision(Player* m,World* w,int32_t x,int32_t y,Side s){
	const Block b = World_Get(w,x,y);
	if(b == BLOCK_DOOR && s == SIDE_BOTTOM && World_Portal_Travel(w,x,y,&m->e.r.p)){
		m->e.r.p.x += 0.5f * (1.0f - PLAYER_DIM_X);
		m->e.r.p.y += 1.0f;
		m->e.v = (Vec2){ 0.0f,PLAYER_VEL_MAX };
		m->e.a = (Vec2){ 0.0f,0.0f };
		w->portal_exit_timer = PLAYER_DOOR_EXIT_DISTANCE / PLAYER_VEL_MAX;
		return;
	}

	if(s==SIDE_TOP && m->e.v.y>0.0f) 			m->e.v.y = 0.0f;
	else if(s==SIDE_BOTTOM && m->e.v.y<0.0f) 	m->e.v.y = 0.0f;
	else if(s==SIDE_LEFT && m->e.v.x>0.0f) 		m->e.v.x = 0.0f;
	else if(s==SIDE_RIGHT && m->e.v.x<0.0f) 	m->e.v.x = 0.0f;
}
void Player_EntityCollision(Player* m,World* w,World_Entity* other,int32_t x,int32_t y,Side s){
	switch (other->id){
		case ENTITY_ID_BLACKHAIR:{
			Resolve_Rect_Rect_Side(&m->e.r,other->r,s);
			break;
		}
		case ENTITY_ID_TREE:{
			Resolve_Rect_Rect_Side(&m->e.r,other->r,s);
			break;
		}
		case ENTITY_ID_WOOD:{
			World_Item_Entity* item = (World_Item_Entity*)other;
			World_Inventory_Add(&m->inventory,&item->stack);
			if(item->stack.id == ITEM_NONE)
				World_Remove(w,other);
			break;
		}
	}
}
Player* Player_Load(DataStream* ds){
	if(!ds || ds->size < (int32_t)sizeof(World_Entity)) return NULL;
	Player* p = (Player*)calloc(1,sizeof(Player));
	if(!p) return NULL;
	DataStream_ReadCount(ds,&p->e,0,sizeof(p->e));

	if(ds->size >= (int32_t)sizeof(uint32_t)){
		uint32_t magic = 0U;
		memcpy(&magic,ds->Memory,sizeof(magic));
		if(magic == PLAYER_SAVE_MAGIC){
			DataStream_ReadCount(ds,&magic,0,sizeof(magic));
			if(ds->size < (int32_t)(sizeof(p->spawn) + sizeof(p->start) + sizeof(p->lookdir) + sizeof(p->hand) + 2U * sizeof(uint32_t))){
				free(p);
				return NULL;
			}
			DataStream_ReadCount(ds,&p->spawn,0,sizeof(p->spawn));
			DataStream_ReadCount(ds,&p->start,0,sizeof(p->start));
			DataStream_ReadCount(ds,&p->lookdir,0,sizeof(p->lookdir));
			DataStream_ReadCount(ds,&p->hand,0,sizeof(p->hand));

			uint32_t size = 0U;
			uint32_t width = 0U;
			DataStream_ReadCount(ds,&size,0,sizeof(size));
			DataStream_ReadCount(ds,&width,0,sizeof(width));
			if(width == 0U || size > (uint32_t)(ds->size / (int32_t)sizeof(World_Item_Stack)) ||
			   !World_Inventory_Init(&p->inventory,size,width)){
				free(p);
				return NULL;
			}
			DataStream_ReadCount(ds,p->inventory.slots,0,(int)(sizeof(World_Item_Stack) * size));
			return p;
		}
	}

	if(ds->size < (int32_t)(sizeof(p->spawn) + sizeof(p->start) + sizeof(p->lookdir)) ||
	   !World_Inventory_Init(&p->inventory,PLAYER_INVENTORY_WIDTH * PLAYER_INVENTORY_HEIGHT,PLAYER_INVENTORY_WIDTH)){
		free(p);
		return NULL;
	}
	DataStream_ReadCount(ds,&p->spawn,0,sizeof(p->spawn));
	DataStream_ReadCount(ds,&p->start,0,sizeof(p->start));
	DataStream_ReadCount(ds,&p->lookdir,0,sizeof(p->lookdir));
	return p;
}
void Player_Save(Player* m,DataStream* ds){
	DataStream_PushCount(ds,&m->e,sizeof(m->e));
	DataStream_PushCount(ds,(uint32_t[]){ PLAYER_SAVE_MAGIC },sizeof(uint32_t));
	DataStream_PushCount(ds,&m->spawn,sizeof(m->spawn));
	DataStream_PushCount(ds,&m->start,sizeof(m->start));
	DataStream_PushCount(ds,&m->lookdir,sizeof(m->lookdir));
	DataStream_PushCount(ds,&m->hand,sizeof(m->hand));
	DataStream_PushCount(ds,&m->inventory.size,sizeof(m->inventory.size));
	DataStream_PushCount(ds,&m->inventory.width,sizeof(m->inventory.width));
	DataStream_PushCount(ds,m->inventory.slots,(int)(sizeof(World_Item_Stack) * m->inventory.size));
}
char Player_LoadInventoryFile(Player* m,World* w){
	if(!m || !w || !w->path) return 0;
	CStr path = CStr_Format("%s/Player.bin",w->path);
	FilesSize file_size = 0;
	char* data = Files_ReadB(path,&file_size);
	if(!data){
		if(Files_Exists(path))
			printf("[Player]: LoadInventory -> failed to read '%s'\n",path);
		CStr_Free(&path);
		return 0;
	}

	const FilesSize header_size = (FilesSize)(sizeof(World_Item_Stack) + sizeof(uint32_t));
	if(file_size < header_size || file_size > (FilesSize)INT32_MAX){
		printf("[Player]: LoadInventory -> invalid file size in '%s'\n",path);
		free(data);
		CStr_Free(&path);
		return 0;
	}

	DataStream ds = DataStream_By(data,(int)file_size);
	World_Item_Stack hand = { 0 };
	uint32_t size = 0U;
	DataStream_ReadCount(&ds,&hand,0,sizeof(hand));
	DataStream_ReadCount(&ds,&size,0,sizeof(size));

	const uint64_t expected_size = (uint64_t)header_size + (uint64_t)size * sizeof(World_Item_Stack);
	if(size > m->inventory.size || expected_size != (uint64_t)file_size ||
	   (size > 0U && !m->inventory.slots)){
		printf("[Player]: LoadInventory -> invalid inventory data in '%s'\n",path);
		DataStream_Free(&ds);
		CStr_Free(&path);
		return 0;
	}

	if(m->inventory.size > 0U)
		memset(m->inventory.slots,0,sizeof(World_Item_Stack) * m->inventory.size);
	DataStream_ReadCount(&ds,m->inventory.slots,0,(int)(sizeof(World_Item_Stack) * size));
	m->hand = hand;
	DataStream_Free(&ds);
	CStr_Free(&path);
	return 1;
}
char Player_SaveInventoryFile(Player* m,World* w){
	if(!m || !w || !w->path) return 0;
	if(m->inventory.size != PLAYER_INVENTORY_WIDTH * PLAYER_INVENTORY_HEIGHT ||
	   m->inventory.width != PLAYER_INVENTORY_WIDTH || !m->inventory.slots){
		printf("[Player]: SaveInventory -> invalid player inventory, not saving\n");
		return 0;
	}

	CStr path = CStr_Format("%s/Player.bin",w->path);
	DataStream ds = DataStream_New();
	DataStream_PushCount(&ds,&m->hand,sizeof(m->hand));
	DataStream_PushCount(&ds,&m->inventory.size,sizeof(m->inventory.size));
	DataStream_PushCount(&ds,m->inventory.slots,(int)(sizeof(World_Item_Stack) * m->inventory.size));
	Files_Write(path,ds.Memory,(FilesSize)ds.size);
	FilesSize written_size = 0;
	char* written_data = Files_ReadB(path,&written_size);
	const char success = written_data && written_size == (FilesSize)ds.size &&
		memcmp(written_data,ds.Memory,(size_t)ds.size) == 0;
	free(written_data);
	if(!success)
		printf("[Player]: SaveInventory -> failed to write '%s'\n",path);
	DataStream_Free(&ds);
	CStr_Free(&path);
	return success;
}
SubSprite Player_GetRender(Player* m,EntityAtlas* ea){
	unsigned int ox = 0U;
	unsigned int oy = m->lookdir - ENTITY_LD_DOWN;
	unsigned int dx = ea->atlas.w / ea->cx;
	unsigned int dy = ea->atlas.h / ea->cy;
	
	if(m->e.v.x == 0.0f && m->e.v.y == 0.0f){
		ox = 0U;
	}else{
		const FDuration f = F64_Max(2.0,0.1 * (FDuration)Vec2_Mag(m->e.v));
		const FDuration d = Animation_Phase(m->start,f);
		ox = (unsigned int)(4.0 * d);
	}

	return SubSprite_New(&ea->atlas,ox * dx,oy * dy,dx,dy);
}
void Player_Free(Player* m){
	if(!m) return;
	World_Inventory_Free(&m->inventory);
}
Player Player_By(Vec2 p,Entity_ID id){
	Player b;
	memset(&b,0,sizeof(b));
	b.e.id = id;
	b.e.r = (Rect){ { p.x - 0.5f * PLAYER_DIM_X,p.y - 0.5f * PLAYER_DIM_Y },{ PLAYER_DIM_X,PLAYER_DIM_Y } };
	b.e.v = (Vec2){ 0.0f,0.0f };
	b.e.a = (Vec2){ 0.0f,0.0f };
	b.spawn = p;
	b.start = 0UL;
	b.lookdir = ENTITY_LD_DOWN;
	if(!World_Inventory_Init(&b.inventory,PLAYER_INVENTORY_WIDTH * PLAYER_INVENTORY_HEIGHT,PLAYER_INVENTORY_WIDTH))
		printf("[Player]: By -> failed to allocate inventory slots\n");
	return b;
}
Player* Player_New(Vec2 p,Entity_ID id){
	Player b = Player_By(p,id);
	Player* hb = (Player*)malloc(sizeof(Player));
	if(!hb){
		Player_Free(&b);
		return NULL;
	}
	memcpy(hb,&b,sizeof(Player));
	return hb;
}
#define WOOD_ITEM_DIM_X				0.65f
#define WOOD_ITEM_DIM_Y				0.65f

World_Item_Entity* World_Item_Entity_Load(DataStream* ds){
	if(!ds || ds->size < (int32_t)(sizeof(World_Entity) + sizeof(World_Item_Stack))) return NULL;
	World_Item_Entity* item = (World_Item_Entity*)malloc(sizeof(World_Item_Entity));
	if(!item) return NULL;
	DataStream_ReadCount(ds,&item->e,0,sizeof(item->e));
	DataStream_ReadCount(ds,&item->stack,0,sizeof(item->stack));
	return item;
}
void World_Item_Entity_Save(World_Item_Entity* item,DataStream* ds){
	DataStream_PushCount(ds,&item->e,sizeof(item->e));
	DataStream_PushCount(ds,&item->stack,sizeof(item->stack));
}
SubSprite World_Item_Entity_GetRender(World_Item_Entity* item,EntityAtlas* ea){
	(void)item;
	return SubSprite_New(&ea->atlas,0U,0U,ea->atlas.w,ea->atlas.h);
}
World_Item_Entity* World_Item_Entity_By(Vec2 p,Entity_ID id){
	World_Item_Entity* item = (World_Item_Entity*)calloc(1,sizeof(World_Item_Entity));
	if(!item) return NULL;
	item->e.id = id;
	item->e.r = (Rect){ { p.x - 0.5f * WOOD_ITEM_DIM_X,p.y - 0.5f * WOOD_ITEM_DIM_Y },{ WOOD_ITEM_DIM_X,WOOD_ITEM_DIM_Y } };
	item->stack = (World_Item_Stack){ .id = ITEM_WOOD,.count = 1U };
	return item;
}

#define TREE_DIM_X	               	1.5f
#define TREE_DIM_Y					1.25f

typedef World_Entity Tree;

Tree* Tree_Load(DataStream* ds){
	if(!ds || ds->size < (int32_t)sizeof(Tree)) return NULL;
	Tree* p = (Tree*)malloc(sizeof(Tree));
	if(!p) return NULL;
	DataStream_ReadCount(ds,p,0,sizeof(*p));
	return p;
}
void Tree_Save(Tree* m,DataStream* ds){
	DataStream_PushCount(ds,m,sizeof(*m));
}
SubSprite Tree_GetRender(Tree* m,EntityAtlas* ea){
	unsigned int dx = ea->atlas.w / ea->cx;
	unsigned int dy = ea->atlas.h / ea->cy;
	return SubSprite_New(&ea->atlas,0.0f,0.0f,dx,dy);
}
Tree Tree_By(Vec2 p,Entity_ID id){
	Tree b;
	b.id = id;
	b.r = (Rect){ { p.x - 0.5f * TREE_DIM_X,p.y - 0.5f * TREE_DIM_Y },{ TREE_DIM_X,TREE_DIM_Y } };
	b.v = (Vec2){ 0.0f,0.0f };
	b.a = (Vec2){ 0.0f,0.0f };
	return b;
}
Tree* Tree_New(Vec2 p,Entity_ID id){
	Tree b = Tree_By(p,id);
	Tree* hb = malloc(sizeof(Tree));
	memcpy(hb,&b,sizeof(Tree));
	return hb;
}

void* World_Entity_New(Vec2 p,Entity_ID id){
	switch (id){
		case ENTITY_ID_BLACKHAIR:	return Player_New(p,id);
		case ENTITY_ID_TREE:		return Tree_New(p,id);
		case ENTITY_ID_WOOD:		return World_Item_Entity_By(p,id);
	}
	return NULL;
}

#endif