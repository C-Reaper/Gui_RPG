#ifndef ENTITIES_H
#define ENTITIES_H

#include "World.h"
#include "/home/codeleaded/System/Static/Library/AudioPlayer.h"

#define ENTITY_FALSE		            0
#define ENTITY_TRUE			            1

typedef enum Entity_ID {
	ENTITY_ID_NONE = 0U,
	ENTITY_ID_BLACKHAIR = 1U,
	ENTITY_ID_TREE = 2U
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

typedef struct Player {
	World_Entity e;
	Vec2 spawn;
	Timepoint start;
	Entity_LD lookdir;
} Player;

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

	if(F32_Abs(m->e.v.y) > PLAYER_VEL_CLP){
		const float add = -F32_Sign(m->e.v.y) * PLAYER_ACC * t;
		m->e.v.y = F32_Passes(m->e.v.y,add,0.0f) ? 0.0f : m->e.v.y + add;
	}else
		m->e.v.y = 0.0f;

	m->e.v.x = m->e.v.x + F32_Krung(m->e.a.x,PLAYER_ACC_MAX) * t;
	m->e.v.y = m->e.v.y + F32_Krung(m->e.a.y,PLAYER_ACC_MAX) * t;
	m->e.r.p = Vec2_Add(m->e.r.p,Vec2_Mulf(m->e.v,t));
}
void Player_WorldCollision(Player* m,World* w){
	//if(m->e.r.p.x < 0.0f) m->e.r.p.x = 0.0f;
	//if(m->e.r.p.y < 0.0f) m->e.r.p.y = 0.0f;
}
char Player_IsSolid(Player* m,World* w,int32_t x,int32_t y,Side s){
	const Block b = World_Get(w,x,y);
	switch(b){
		case BLOCK_WATER:
		case BLOCK_HOUSE_GRAY:
		case BLOCK_HOUSE_RED:
		case BLOCK_CEIL_FLAT:
		case BLOCK_CEIL_NORMAL:
			return 1;
		default:
			return 0;
	}
}
void Player_Collision(Player* m,World* w,int32_t x,int32_t y,Side s){
	const Block b = World_Get(w,x,y);

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
	}
}
Player* Player_Load(DataStream* ds){
	if(!ds || ds->size < (int32_t)sizeof(Player)) return NULL;
	Player* p = (Player*)malloc(sizeof(Player));
	if(!p) return NULL;
	DataStream_ReadCount(ds,p,0,sizeof(*p));
	return p;
}
void Player_Save(Player* m,DataStream* ds){
	DataStream_PushCount(ds,m,sizeof(*m));
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
	
}
Player Player_By(Vec2 p,Entity_ID id){
	Player b;
	b.e.id = id;
	b.e.r = (Rect){ { p.x - 0.5f * PLAYER_DIM_X,p.y - 0.5f * PLAYER_DIM_Y },{ PLAYER_DIM_X,PLAYER_DIM_Y } };
	b.e.v = (Vec2){ 0.0f,0.0f };
	b.e.a = (Vec2){ 0.0f,0.0f };
	b.spawn = p;
	b.start = 0UL;
	b.lookdir = ENTITY_LD_DOWN;
	return b;
}
Player* Player_New(Vec2 p,Entity_ID id){
	Player b = Player_By(p,id);
	Player* hb = malloc(sizeof(Player));
	memcpy(hb,&b,sizeof(Player));
	return hb;
}





#define TREE_DIM_X	               	2.0f
#define TREE_DIM_Y					1.5f

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
	}
	return NULL;
}

#endif