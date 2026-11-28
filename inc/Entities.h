#ifndef ENTITIES_H
#define ENTITIES_H

#include "World.h"
#include "/home/codeleaded/System/Static/Library/AudioPlayer.h"

#define ENTITY_FALSE		            0
#define ENTITY_TRUE			            1

typedef enum Entity_ID {
	ENTITY_ID_NONE = 0U,
	ENTITY_ID_BLACKHAIR = 1U,
	ENTITY_ID_BAUM = 2U,
	ENTITY_ID_ITEM_HOLZ = 3U,
	ENTITY_ID_ITEM_AERE = 4U,
	ENTITY_ID_ITEM_ANANAS = 5U,
	ENTITY_ID_ITEM_BESTECK = 6U,
	ENTITY_ID_ITEM_BEUTEL = 7U,
	ENTITY_ID_ITEM_BIENE = 8U,
	ENTITY_ID_ITEM_BIERKRUG = 9U,
	ENTITY_ID_ITEM_BIRNE = 10U,
	ENTITY_ID_ITEM_BLAETTER = 11U,
	ENTITY_ID_ITEM_BLATT = 12U,
	ENTITY_ID_ITEM_BLUETE = 13U,
	ENTITY_ID_ITEM_BOHNEN_BLAU = 14U,
	ENTITY_ID_ITEM_BOHNEN_ROT = 15U,
	ENTITY_ID_ITEM_BONBON = 16U,
	ENTITY_ID_ITEM_BREZE = 17U,
	ENTITY_ID_ITEM_BRIEF = 18U,
	ENTITY_ID_ITEM_BRILLE = 19U,
	ENTITY_ID_ITEM_BROT = 20U,
	ENTITY_ID_ITEM_BUERSTE = 21U,
	ENTITY_ID_ITEM_CHILI = 22U,
	ENTITY_ID_ITEM_EI = 23U,
	ENTITY_ID_ITEM_EIMER = 24U,
	ENTITY_ID_ITEM_ERDBEERE = 25U,
	ENTITY_ID_ITEM_FADEN = 26U,
	ENTITY_ID_ITEM_FALLE = 27U,
	ENTITY_ID_ITEM_FARBE = 28U,
	ENTITY_ID_ITEM_FARN = 29U,
	ENTITY_ID_ITEM_FASS = 30U,
	ENTITY_ID_ITEM_FISCH = 31U,
	ENTITY_ID_ITEM_FLASCHE = 32U,
	ENTITY_ID_ITEM_FLASCHE_LEER = 33U,
	ENTITY_ID_ITEM_FLEISCHKEULE = 34U,
	ENTITY_ID_ITEM_FLEISCHKNOCHEN = 35U,
	ENTITY_ID_ITEM_FLOETE = 36U,
	ENTITY_ID_ITEM_GABEL = 37U,
	ENTITY_ID_ITEM_GEMAELDE = 38U,
	ENTITY_ID_ITEM_GLAS = 39U,
	ENTITY_ID_ITEM_GLOCKE = 40U,
	ENTITY_ID_ITEM_GOCKEL = 41U,
	ENTITY_ID_ITEM_GOLDENWEIZEN = 42U,
	ENTITY_ID_ITEM_HAMSTER = 43U,
	ENTITY_ID_ITEM_HANTEL = 44U,
	ENTITY_ID_ITEM_HUFEISEN = 45U,
	ENTITY_ID_ITEM_HUHN = 46U,
	ENTITY_ID_ITEM_INSTRUMENT = 47U,
	ENTITY_ID_ITEM_KAESE = 48U,
	ENTITY_ID_ITEM_KAROTTE = 49U,
	ENTITY_ID_ITEM_KARTEN = 50U,
	ENTITY_ID_ITEM_KELLE = 51U,
	ENTITY_ID_ITEM_KERZE = 52U,
	ENTITY_ID_ITEM_KIRSCHE = 53U,
	ENTITY_ID_ITEM_KLEE = 54U,
	ENTITY_ID_ITEM_KLEIDERBUEGEL = 55U,
	ENTITY_ID_ITEM_KLOPAPIER = 56U,
	ENTITY_ID_ITEM_KNOBLAUCH = 57U,
	ENTITY_ID_ITEM_KOKUSNUSS = 58U,
	ENTITY_ID_ITEM_KREUZ = 59U,
	ENTITY_ID_ITEM_KRUG = 60U,
	ENTITY_ID_ITEM_KUCHEN = 61U,
	ENTITY_ID_ITEM_KUERBIS = 62U,
	ENTITY_ID_ITEM_LOLLI = 63U,
	ENTITY_ID_ITEM_LUPE = 64U,
	ENTITY_ID_ITEM_MAIS = 65U,
	ENTITY_ID_ITEM_MARMELADE = 66U,
	ENTITY_ID_ITEM_MAUS = 67U,
	ENTITY_ID_ITEM_MEHL = 68U,
	ENTITY_ID_ITEM_MESSER = 69U,
	ENTITY_ID_ITEM_OBERSCHIENE = 70U,
	ENTITY_ID_ITEM_PFANNE = 71U,
	ENTITY_ID_ITEM_PINSEL = 72U,
	ENTITY_ID_ITEM_ROSE = 73U,
	ENTITY_ID_ITEM_RUEBE = 74U,
	ENTITY_ID_ITEM_SACK = 75U,
	ENTITY_ID_ITEM_SAEGE = 76U,
	ENTITY_ID_ITEM_SALAT = 77U,
	ENTITY_ID_ITEM_SAMEN = 78U,
	ENTITY_ID_ITEM_SCHRIFTROLLE = 79U,
	ENTITY_ID_ITEM_SCHWAMM = 80U,
	ENTITY_ID_ITEM_SEIL = 81U,
	ENTITY_ID_ITEM_SENSE = 82U,
	ENTITY_ID_ITEM_SICHEL = 83U,
	ENTITY_ID_ITEM_STATUE = 84U,
	ENTITY_ID_ITEM_STEAK = 85U,
	ENTITY_ID_ITEM_STEIN = 86U,
	ENTITY_ID_ITEM_TASCHENUHR = 87U,
	ENTITY_ID_ITEM_TOMATE = 88U,
	ENTITY_ID_ITEM_TOPF = 89U,
	ENTITY_ID_ITEM_TRAUBE = 90U,
	ENTITY_ID_ITEM_TROMMEL = 91U,
	ENTITY_ID_ITEM_TROMPETE = 92U,
	ENTITY_ID_ITEM_TUCH = 93U,
	ENTITY_ID_ITEM_VIOLINE = 94U,
	ENTITY_ID_ITEM_VOGELKAEFIG = 95U,
	ENTITY_ID_ITEM_WAABE = 96U,
	ENTITY_ID_ITEM_WEIN = 97U,
	ENTITY_ID_ITEM_WEIZEN = 98U,
	ENTITY_ID_ITEM_WERKZEUG = 99U,
	ENTITY_ID_ITEM_WUERSTE = 100U,
	ENTITY_ID_ITEM_WURST = 101U,
	ENTITY_ID_ITEM_ZWIEBEL = 102U,
	ENTITY_ID_ITEM_LAST = 102U,
	ENTITY_ID_BAUM_DARK = 103U,
	ENTITY_ID_BAUM_LIGHT = 104U,
	ENTITY_ID_BAUM_OBST = 105U,
	ENTITY_ID_PALME = 106U,
	ENTITY_ID_STEIN = 107U,
	ENTITY_ID_TANNE = 108U,
	ENTITY_ID_COUNT = 109U
} Entity_ID;

Entity_ID World_Item_Entity_ID(Item item){
	if(item < ITEM_HOLZ || item >= ITEM_COUNT) return ENTITY_ID_NONE;
	return (Entity_ID)(ENTITY_ID_ITEM_HOLZ + item - ITEM_HOLZ);
}
Item World_Item_Entity_Item_ID(Entity_ID id){
	if(id < ENTITY_ID_ITEM_HOLZ || id > ENTITY_ID_ITEM_LAST) return ITEM_NONE;
	return (Item)(ITEM_HOLZ + id - ENTITY_ID_ITEM_HOLZ);
}

#define ENTITY_LD_NONE		0U
#define ENTITY_LD_DOWN		1U
#define ENTITY_LD_LEFT		2U
#define ENTITY_LD_RIGHT		3U
#define ENTITY_LD_UP		4U
typedef uint32_t Entity_LD;

#define WORLD_PLAYER_DIM_X	               	0.5f
#define WORLD_PLAYER_DIM_Y					1.2f
#define WORLD_PLAYER_ACC		            	30.0f
#define WORLD_PLAYER_ACC_MAX		            50.0f
#define WORLD_PLAYER_VEL_CLP		            0.01f
#define WORLD_PLAYER_VEL_MAX	            	5.0f
#define WORLD_PLAYER_DOOR_EXIT_DISTANCE		1.5f
#define WORLD_PLAYER_INVENTORY_WIDTH			(9)
#define WORLD_PLAYER_INVENTORY_HEIGHT			(3)
#define WORLD_PLAYER_DESTROY_TIME				1.0f
#define WORLD_PLAYER_SAVE_MAGIC				0x504C5952U

typedef struct World_Player {
	World_Entity e;
	Vec2 spawn;
	Timepoint start;
	union {
		Entity_LD look_data;
		struct {
			Entity_LD lr : 4;
			Entity_LD lookdir : 4;
			Entity_LD padding0 : 24;
		};
	};
	union {
		Entity_LD flags;
		struct {
			uint32_t attacking : 1;
			uint32_t padding1 : 31;
		};
	};
	struct {
		
	};
	World_Item_Stack hand;
	World_Inventory inventory;
} World_Player;

typedef struct World_Item_Entity {
	World_Entity e;
	World_Item_Stack stack;
} World_Item_Entity;

typedef struct TileWorld {
    World world;
    World_Player player;
    TransformedView tv;
    AudioPlayer ap;
    Block selected;
    Sprite item_frame;
} TileWorld;

void* World_Entity_New(Vec2 p,Entity_ID id);
void World_Entity_Spawn(World* w,Vec2 p,Entity_ID id){
	const float angle = Random_f64_MinMax(-F64_PI,F64_PI);
	const float len = Random_f64_MinMax(0.0f,2.0f);
	const Vec2 pos = Vec2_Add(p,Vec2_Mulf(Vec2_OfAngle(angle),len));
	World_Spawn(w,World_Entity_New(pos,id));
}

void World_Player_Move(World_Player* m,const float dir_x,const float dir_y){
	// min for clean acc
	if(dir_x < -0.2f){
		m->e.a.x = -WORLD_PLAYER_ACC * F32_Abs(dir_x);
	}else if(dir_x > 0.2f){
		m->e.a.x = WORLD_PLAYER_ACC * F32_Abs(dir_x);
	}else
		m->e.a.x = 0.0f;

	if(dir_y < -0.2f){
		m->e.a.y = -WORLD_PLAYER_ACC * F32_Abs(dir_y);
	}else if(dir_y > 0.2f){
		m->e.a.y = WORLD_PLAYER_ACC * F32_Abs(dir_y);
	}else
		m->e.a.y = 0.0f;
}
void World_Player_Update(World_Player* m,World* w,float t){
    m->e.a.x = F32_Clamp(m->e.a.x,-WORLD_PLAYER_ACC_MAX,WORLD_PLAYER_ACC_MAX);
	m->e.v.x = F32_Clamp(m->e.v.x,-WORLD_PLAYER_VEL_MAX,WORLD_PLAYER_VEL_MAX);
    m->e.a.y = F32_Clamp(m->e.a.y,-WORLD_PLAYER_ACC_MAX,WORLD_PLAYER_ACC_MAX);
	m->e.v.y = F32_Clamp(m->e.v.y,-WORLD_PLAYER_VEL_MAX,WORLD_PLAYER_VEL_MAX);

	const char exiting_door = w && w->portal_exit_timer > 0.0f;
	if(exiting_door) w->portal_exit_timer = F32_Max(0.0f,w->portal_exit_timer - t);

	if(m->e.v.x != 0.0f || m->e.v.y != 0.0f){
		m->lr = m->e.v.x < 0.0f ? ENTITY_LD_LEFT : ENTITY_LD_RIGHT;

		if(F32_Abs(m->e.v.x) >= F32_Abs(m->e.v.y))
			m->lookdir = m->lr;
		else
			m->lookdir = m->e.v.y < 0.0f ? ENTITY_LD_UP : ENTITY_LD_DOWN;
	}

	if(F32_Abs(m->e.v.x) > WORLD_PLAYER_VEL_CLP){
		const float add = -F32_Sign(m->e.v.x) * WORLD_PLAYER_ACC * t;
		m->e.v.x = F32_Passes(m->e.v.x,add,0.0f) ? 0.0f : m->e.v.x + add;
	}else
		m->e.v.x = 0.0f;

	if(exiting_door){
		m->e.v.y = WORLD_PLAYER_VEL_MAX;
	}else if(F32_Abs(m->e.v.y) > WORLD_PLAYER_VEL_CLP){
		const float add = -F32_Sign(m->e.v.y) * WORLD_PLAYER_ACC * t;
		m->e.v.y = F32_Passes(m->e.v.y,add,0.0f) ? 0.0f : m->e.v.y + add;
	}else
		m->e.v.y = 0.0f;

	m->e.v.x = m->e.v.x + F32_Krung(m->e.a.x,WORLD_PLAYER_ACC_MAX) * t;
	if(!exiting_door) m->e.v.y = m->e.v.y + F32_Krung(m->e.a.y,WORLD_PLAYER_ACC_MAX) * t;

	m->e.r.p = Vec2_Add(m->e.r.p,Vec2_Mulf(m->e.v,t));
}
void World_Player_WorldCollision(World_Player* m,World* w){
	//if(m->e.r.p.x < 0.0f) m->e.r.p.x = 0.0f;
	//if(m->e.r.p.y < 0.0f) m->e.r.p.y = 0.0f;
}
char World_Player_IsSolid(World_Player* m,World* w,int32_t x,int32_t y,Side s){
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
void World_Player_Collision(World_Player* m,World* w,int32_t x,int32_t y,Side s){
	const Block b = World_Get(w,x,y);
	if(b == BLOCK_DOOR && s == SIDE_BOTTOM && World_Portal_Travel(w,x,y,&m->e.r.p)){
		m->e.r.p.x += 0.5f * (1.0f - WORLD_PLAYER_DIM_X);
		m->e.r.p.y += 1.0f;
		m->e.v = (Vec2){ 0.0f,WORLD_PLAYER_VEL_MAX };
		m->e.a = (Vec2){ 0.0f,0.0f };
		w->portal_exit_timer = WORLD_PLAYER_DOOR_EXIT_DISTANCE / WORLD_PLAYER_VEL_MAX;
		return;
	}

	if(s==SIDE_TOP && m->e.v.y>0.0f) 			m->e.v.y = 0.0f;
	else if(s==SIDE_BOTTOM && m->e.v.y<0.0f) 	m->e.v.y = 0.0f;
	else if(s==SIDE_LEFT && m->e.v.x>0.0f) 		m->e.v.x = 0.0f;
	else if(s==SIDE_RIGHT && m->e.v.x<0.0f) 	m->e.v.x = 0.0f;
}
void World_Player_EntityCollision(World_Player* m,World* w,World_Entity* other,int32_t x,int32_t y,Side s){
	const Entity_ID other_id = (Entity_ID)other->id;
	switch (other_id){
		case ENTITY_ID_BLACKHAIR:{
			Resolve_Rect_Rect_Side(&m->e.r,other->r,s);
			break;
		}
		case ENTITY_ID_BAUM:{
			Resolve_Rect_Rect_Side(&m->e.r,other->r,s);
			if(m->attacking){
				const Timepoint now = Time_Nano();
				FDuration elap = Time_Elapsed(m->start,now);
				
				if(elap >= WORLD_PLAYER_DESTROY_TIME){
					m->start = now;
					const Vec2 o_p = other->r.p;
					World_Remove(w,other);

					const uint32_t holz_count = Random_u32_MinMax(1U,10U);
					const uint32_t blaetter_count = Random_u32_MinMax(1U,5U);
					const uint32_t ananas_count = Random_u32_MinMax(1U,3U);
					const uint32_t birne_count = Random_u32_MinMax(1U,3U);
					const uint32_t kirsche_count = Random_u32_MinMax(1U,4U);

					for(uint32_t i = 0U;i<holz_count;i++)
						World_Entity_Spawn(w,o_p,ENTITY_ID_ITEM_HOLZ);
					
					if(Random_u32_MinMax(0U,2U) == 0U)
						for(uint32_t i = 0U;i<blaetter_count;i++)
							World_Entity_Spawn(w,o_p,ENTITY_ID_ITEM_BLAETTER);

					if(Random_u32_MinMax(0U,5U) == 0U)
						for(uint32_t i = 0U;i<ananas_count;i++)
							World_Entity_Spawn(w,o_p,ENTITY_ID_ITEM_ANANAS);

					if(Random_u32_MinMax(0U,6U) == 0U)
						for(uint32_t i = 0U;i<birne_count;i++)
							World_Entity_Spawn(w,o_p,ENTITY_ID_ITEM_BIRNE);
					
					if(Random_u32_MinMax(0U,8U) == 0U)
						for(uint32_t i = 0U;i<kirsche_count;i++)
							World_Entity_Spawn(w,o_p,ENTITY_ID_ITEM_KIRSCHE);
				}
			}
			break;
		}
		case ENTITY_ID_BAUM_DARK:{
			Resolve_Rect_Rect_Side(&m->e.r,other->r,s);
			if(m->attacking){
				const Timepoint now = Time_Nano();
				if(Time_Elapsed(m->start,now) >= WORLD_PLAYER_DESTROY_TIME){
					m->start = now;
					const Vec2 p = other->r.p;
					World_Remove(w,other);
					const uint32_t wood_count = Random_u32_MinMax(1U,10U);
					const uint32_t leaves_count = Random_u32_MinMax(1U,5U);
					for(uint32_t i = 0U;i<wood_count;i++)
						World_Entity_Spawn(w,p,ENTITY_ID_ITEM_HOLZ);
					if(Random_u32_MinMax(0U,2U) == 0U)
						for(uint32_t i = 0U;i<leaves_count;i++)
							World_Entity_Spawn(w,p,ENTITY_ID_ITEM_BLAETTER);
				}
			}
			break;
		}
		case ENTITY_ID_BAUM_LIGHT:{
			Resolve_Rect_Rect_Side(&m->e.r,other->r,s);
			if(m->attacking){
				const Timepoint now = Time_Nano();
				if(Time_Elapsed(m->start,now) >= WORLD_PLAYER_DESTROY_TIME){
					m->start = now;
					const Vec2 p = other->r.p;
					World_Remove(w,other);
					const uint32_t wood_count = Random_u32_MinMax(1U,8U);
					const uint32_t leaves_count = Random_u32_MinMax(1U,5U);
					for(uint32_t i = 0U;i<wood_count;i++)
						World_Entity_Spawn(w,p,ENTITY_ID_ITEM_HOLZ);
					if(Random_u32_MinMax(0U,1U) == 0U)
						for(uint32_t i = 0U;i<leaves_count;i++)
							World_Entity_Spawn(w,p,ENTITY_ID_ITEM_BLAETTER);
				}
			}
			break;
		}
		case ENTITY_ID_BAUM_OBST:{
			Resolve_Rect_Rect_Side(&m->e.r,other->r,s);
			if(m->attacking){
				const Timepoint now = Time_Nano();
				if(Time_Elapsed(m->start,now) >= WORLD_PLAYER_DESTROY_TIME){
					m->start = now;
					const Vec2 p = other->r.p;
					World_Remove(w,other);
					const uint32_t wood_count = Random_u32_MinMax(1U,8U);
					const uint32_t fruit_count = Random_u32_MinMax(1U,4U);
					for(uint32_t i = 0U;i<wood_count;i++)
						World_Entity_Spawn(w,p,ENTITY_ID_ITEM_HOLZ);
					if(Random_u32_MinMax(0U,2U) == 0U)
						for(uint32_t i = 0U;i<fruit_count;i++)
							World_Entity_Spawn(w,p,ENTITY_ID_ITEM_BIRNE);
					if(Random_u32_MinMax(0U,3U) == 0U)
						for(uint32_t i = 0U;i<fruit_count;i++)
							World_Entity_Spawn(w,p,ENTITY_ID_ITEM_KIRSCHE);
				}
			}
			break;
		}
		case ENTITY_ID_PALME:{
			Resolve_Rect_Rect_Side(&m->e.r,other->r,s);
			if(m->attacking){
				const Timepoint now = Time_Nano();
				if(Time_Elapsed(m->start,now) >= WORLD_PLAYER_DESTROY_TIME){
					m->start = now;
					const Vec2 p = other->r.p;
					World_Remove(w,other);
					const uint32_t wood_count = Random_u32_MinMax(1U,8U);
					const uint32_t coconut_count = Random_u32_MinMax(1U,3U);
					for(uint32_t i = 0U;i<wood_count;i++)
						World_Entity_Spawn(w,p,ENTITY_ID_ITEM_HOLZ);
					if(Random_u32_MinMax(0U,3U) == 0U)
						for(uint32_t i = 0U;i<coconut_count;i++)
							World_Entity_Spawn(w,p,ENTITY_ID_ITEM_KOKUSNUSS);
				}
			}
			break;
		}
		case ENTITY_ID_TANNE:{
			Resolve_Rect_Rect_Side(&m->e.r,other->r,s);
			if(m->attacking){
				const Timepoint now = Time_Nano();
				if(Time_Elapsed(m->start,now) >= WORLD_PLAYER_DESTROY_TIME){
					m->start = now;
					const Vec2 p = other->r.p;
					World_Remove(w,other);
					const uint32_t wood_count = Random_u32_MinMax(1U,10U);
					const uint32_t leaves_count = Random_u32_MinMax(1U,6U);
					for(uint32_t i = 0U;i<wood_count;i++)
						World_Entity_Spawn(w,p,ENTITY_ID_ITEM_HOLZ);
					if(Random_u32_MinMax(0U,2U) == 0U)
						for(uint32_t i = 0U;i<leaves_count;i++)
							World_Entity_Spawn(w,p,ENTITY_ID_ITEM_BLAETTER);
				}
			}
			break;
		}
		case ENTITY_ID_STEIN:{
			Resolve_Rect_Rect_Side(&m->e.r,other->r,s);
			if(m->attacking){
				const Timepoint now = Time_Nano();
				if(Time_Elapsed(m->start,now) >= WORLD_PLAYER_DESTROY_TIME){
					m->start = now;
					const Vec2 p = other->r.p;
					World_Remove(w,other);
					const uint32_t stone_count = Random_u32_MinMax(1U,4U);
					for(uint32_t i = 0U;i<stone_count;i++)
						World_Entity_Spawn(w,p,ENTITY_ID_ITEM_STEIN);
				}
			}
			break;
		}
	}
	if(other_id >= ENTITY_ID_ITEM_HOLZ && other_id <= ENTITY_ID_ITEM_LAST){
		World_Item_Entity* item = (World_Item_Entity*)other;
		World_Inventory_Add(&m->inventory,&item->stack);
		if(item->stack.id == ITEM_NONE)
			World_Remove(w,other);
			
		//TileWorld* tw = (TileWorld*)w;
		//AudioPlayer_Add(&tw->ap,"./assets/Sound/OneUp.wav");
	}
}
World_Player* World_Player_Load(DataStream* ds){
	if(!ds || ds->size < (int32_t)sizeof(World_Entity)) return NULL;
	World_Player* p = (World_Player*)calloc(1,sizeof(World_Player));
	if(!p) return NULL;
	DataStream_ReadCount(ds,&p->e,0,sizeof(p->e));

	if(ds->size >= (int32_t)sizeof(uint32_t)){
		uint32_t magic = 0U;
		memcpy(&magic,ds->Memory,sizeof(magic));
		if(magic == WORLD_PLAYER_SAVE_MAGIC){
			DataStream_ReadCount(ds,&magic,0,sizeof(magic));
			
			if(ds->size < (int32_t)(sizeof(p->spawn) + sizeof(p->start) + sizeof(p->look_data) + sizeof(p->hand) + 2U * sizeof(uint32_t))){
				free(p);
				return NULL;
			}

			DataStream_ReadCount(ds,&p->spawn,0,sizeof(p->spawn));
			DataStream_ReadCount(ds,&p->start,0,sizeof(p->start));
			DataStream_ReadCount(ds,&p->look_data,0,sizeof(p->look_data));
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

	if(ds->size < (int32_t)(sizeof(p->spawn) + sizeof(p->start) + sizeof(p->look_data)) ||
	   !World_Inventory_Init(&p->inventory,WORLD_PLAYER_INVENTORY_WIDTH * WORLD_PLAYER_INVENTORY_HEIGHT,WORLD_PLAYER_INVENTORY_WIDTH)){
		free(p);
		return NULL;
	}

	DataStream_ReadCount(ds,&p->spawn,0,sizeof(p->spawn));
	DataStream_ReadCount(ds,&p->start,0,sizeof(p->start));
	DataStream_ReadCount(ds,&p->look_data,0,sizeof(p->look_data));
	return p;
}
void World_Player_Save(World_Player* m,DataStream* ds){
	DataStream_PushCount(ds,&m->e,sizeof(m->e));
	DataStream_PushCount(ds,(uint32_t[]){ WORLD_PLAYER_SAVE_MAGIC },sizeof(uint32_t));
	DataStream_PushCount(ds,&m->spawn,sizeof(m->spawn));
	DataStream_PushCount(ds,&m->start,sizeof(m->start));
	DataStream_PushCount(ds,&m->look_data,sizeof(m->look_data));
	DataStream_PushCount(ds,&m->hand,sizeof(m->hand));
	DataStream_PushCount(ds,&m->inventory.size,sizeof(m->inventory.size));
	DataStream_PushCount(ds,&m->inventory.width,sizeof(m->inventory.width));
	DataStream_PushCount(ds,m->inventory.slots,(int)(sizeof(World_Item_Stack) * m->inventory.size));
}
char World_Player_LoadInventoryFile(World_Player* m,World* w){
	if(!m || !w || !w->path) return 0;
	CStr path = CStr_Format("%s/Player.bin",w->path);

	FilesSize file_size = 0;
	char* data = Files_ReadB(path,&file_size);
	if(!data){
		if(Files_Exists(path))
			printf("[World_Player]: LoadInventory -> failed to read '%s'\n",path);
		
		CStr_Free(&path);
		return 0;
	}

	const FilesSize header_size = (FilesSize)(sizeof(World_Item_Stack) + sizeof(uint32_t));
	if(file_size < header_size || file_size > (FilesSize)INT32_MAX){
		printf("[World_Player]: LoadInventory -> invalid file size in '%s'\n",path);
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
		printf("[World_Player]: LoadInventory -> invalid inventory data in '%s'\n",path);
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
char World_Player_SaveInventoryFile(World_Player* m,World* w){
	if(!m || !w || !w->path) return 0;
	if(m->inventory.size != WORLD_PLAYER_INVENTORY_WIDTH * WORLD_PLAYER_INVENTORY_HEIGHT ||
	   m->inventory.width != WORLD_PLAYER_INVENTORY_WIDTH || !m->inventory.slots){
		printf("[World_Player]: SaveInventory -> invalid player inventory, not saving\n");
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
	const char success = written_data && written_size == (FilesSize)ds.size && memcmp(written_data,ds.Memory,(size_t)ds.size) == 0;
	free(written_data);
	
	if(!success)
		printf("[World_Player]: SaveInventory -> failed to write '%s'\n",path);
	
	DataStream_Free(&ds);
	CStr_Free(&path);
	return success;
}
SubSprite World_Player_GetRender(World_Player* m,EntityAtlas* ea){
	uint32_t ox = 0U;
	uint32_t oy = 0U;
	
	if(m->attacking){
		const FDuration d = Animation_Phase(m->start,5.0);
		ox = (unsigned int)(4.0 * d);
		oy = m->lr == ENTITY_LD_RIGHT ? 4U : 5U;
	}else{
		if(m->e.v.x == 0.0f && m->e.v.y == 0.0f){
			ox = 0U;
		}else{
			const FDuration f = F64_Max(2.0,0.1 * (FDuration)Vec2_Mag(m->e.v));
			const FDuration d = Animation_Phase(m->start,f);
			ox = (unsigned int)(4.0 * d);
		}

		oy = (uint32_t)m->lookdir - ENTITY_LD_DOWN;
	}

	const uint32_t dx = ea->atlas.w / ea->cx;
	const uint32_t dy = ea->atlas.h / ea->cy;
	return SubSprite_New(&ea->atlas,ox * dx,oy * dy,dx,dy);
}
void World_Player_Free(World_Player* m){
	if(!m) return;
	World_Inventory_Free(&m->inventory);
}
World_Player World_Player_By(Vec2 p,Entity_ID id){
	World_Player b;
	memset(&b,0,sizeof(b));
	b.e.id = id;
	b.e.r = (Rect){ { p.x - 0.5f * WORLD_PLAYER_DIM_X,p.y - 0.5f * WORLD_PLAYER_DIM_Y },{ WORLD_PLAYER_DIM_X,WORLD_PLAYER_DIM_Y } };
	b.e.v = (Vec2){ 0.0f,0.0f };
	b.e.a = (Vec2){ 0.0f,0.0f };
	b.spawn = p;
	b.start = 0UL;
	b.lookdir = ENTITY_LD_DOWN;
	b.attacking = 0U;

	if(!World_Inventory_Init(&b.inventory,WORLD_PLAYER_INVENTORY_WIDTH * WORLD_PLAYER_INVENTORY_HEIGHT,WORLD_PLAYER_INVENTORY_WIDTH))
		printf("[World_Player]: By -> failed to allocate inventory slots\n");
	
	return b;
}
World_Player* World_Player_New(Vec2 p,Entity_ID id){
	World_Player b = World_Player_By(p,id);
	World_Player* hb = (World_Player*)malloc(sizeof(World_Player));
	if(!hb){
		World_Player_Free(&b);
		return NULL;
	}
	memcpy(hb,&b,sizeof(World_Player));
	return hb;
}


#define WORLD_ITEM_DIM_X				0.65f
#define WORLD_ITEM_DIM_Y				0.65f

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
	const Item item_id = World_Item_Entity_Item_ID(id);
	if(item_id == ITEM_NONE) return NULL;
	World_Item_Entity* item = (World_Item_Entity*)calloc(1,sizeof(World_Item_Entity));
	if(!item) return NULL;
	item->e.id = id;
	item->e.r = (Rect){ { p.x - 0.5f * WORLD_ITEM_DIM_X,p.y - 0.5f * WORLD_ITEM_DIM_Y },{ WORLD_ITEM_DIM_X,WORLD_ITEM_DIM_Y } };
	item->stack = (World_Item_Stack){ .id = item_id,.count = 1U };
	return item;
}


#define WORLD_BAUM_DIM_X 1.5f
#define WORLD_BAUM_DIM_Y 1.25f

typedef World_Entity World_Baum;
World_Baum* World_Baum_Load(DataStream* ds){
	if(!ds || ds->size < (int32_t)sizeof(World_Baum)) return NULL;
	World_Baum* baum = (World_Baum*)malloc(sizeof(World_Baum));
	if(!baum) return NULL;
	DataStream_ReadCount(ds,baum,0,sizeof(*baum));
	return baum;
}
void World_Baum_Save(World_Baum* baum,DataStream* ds){
	DataStream_PushCount(ds,baum,sizeof(*baum));
}
SubSprite World_Baum_GetRender(World_Baum* baum,EntityAtlas* ea){
	(void)baum;
	const unsigned int dx = ea->atlas.w / ea->cx;
	const unsigned int dy = ea->atlas.h / ea->cy;
	return SubSprite_New(&ea->atlas,0U,0U,dx,dy);
}
World_Baum World_Baum_By(Vec2 p){
	World_Baum baum = { 0 };
	baum.id = ENTITY_ID_BAUM;
	baum.r = (Rect){ { p.x - 0.5f * WORLD_BAUM_DIM_X,p.y - 0.5f * WORLD_BAUM_DIM_Y },{ WORLD_BAUM_DIM_X,WORLD_BAUM_DIM_Y } };
	return baum;
}
World_Baum* World_Baum_New(Vec2 p){
	World_Baum baum = World_Baum_By(p);
	World_Baum* new_baum = (World_Baum*)malloc(sizeof(World_Baum));
	if(!new_baum) return NULL;
	memcpy(new_baum,&baum,sizeof(baum));
	return new_baum;
}

#define WORLD_BAUM_DARK_DIM_X 1.5f
#define WORLD_BAUM_DARK_DIM_Y 1.25f

typedef World_Entity World_Baum_Dark;
World_Baum_Dark* World_Baum_Dark_Load(DataStream* ds){
	if(!ds || ds->size < (int32_t)sizeof(World_Baum_Dark)) return NULL;
	World_Baum_Dark* baum = (World_Baum_Dark*)malloc(sizeof(World_Baum_Dark));
	if(!baum) return NULL;
	DataStream_ReadCount(ds,baum,0,sizeof(*baum));
	return baum;
}
void World_Baum_Dark_Save(World_Baum_Dark* baum,DataStream* ds){
	DataStream_PushCount(ds,baum,sizeof(*baum));
}
SubSprite World_Baum_Dark_GetRender(World_Baum_Dark* baum,EntityAtlas* ea){
	(void)baum;
	const unsigned int dx = ea->atlas.w / ea->cx;
	const unsigned int dy = ea->atlas.h / ea->cy;
	return SubSprite_New(&ea->atlas,0U,0U,dx,dy);
}
World_Baum_Dark World_Baum_Dark_By(Vec2 p){
	World_Baum_Dark baum = { 0 };
	baum.id = ENTITY_ID_BAUM_DARK;
	baum.r = (Rect){ { p.x - 0.5f * WORLD_BAUM_DARK_DIM_X,p.y - 0.5f * WORLD_BAUM_DARK_DIM_Y },{ WORLD_BAUM_DARK_DIM_X,WORLD_BAUM_DARK_DIM_Y } };
	return baum;
}
World_Baum_Dark* World_Baum_Dark_New(Vec2 p){
	World_Baum_Dark baum = World_Baum_Dark_By(p);
	World_Baum_Dark* new_baum = (World_Baum_Dark*)malloc(sizeof(World_Baum_Dark));
	if(!new_baum) return NULL;
	memcpy(new_baum,&baum,sizeof(baum));
	return new_baum;
}

#define WORLD_BAUM_LIGHT_DIM_X 1.5f
#define WORLD_BAUM_LIGHT_DIM_Y 1.25f

typedef World_Entity World_Baum_Light;
World_Baum_Light* World_Baum_Light_Load(DataStream* ds){
	if(!ds || ds->size < (int32_t)sizeof(World_Baum_Light)) return NULL;
	World_Baum_Light* baum = (World_Baum_Light*)malloc(sizeof(World_Baum_Light));
	if(!baum) return NULL;
	DataStream_ReadCount(ds,baum,0,sizeof(*baum));
	return baum;
}
void World_Baum_Light_Save(World_Baum_Light* baum,DataStream* ds){
	DataStream_PushCount(ds,baum,sizeof(*baum));
}
SubSprite World_Baum_Light_GetRender(World_Baum_Light* baum,EntityAtlas* ea){
	(void)baum;
	const unsigned int dx = ea->atlas.w / ea->cx;
	const unsigned int dy = ea->atlas.h / ea->cy;
	return SubSprite_New(&ea->atlas,0U,0U,dx,dy);
}
World_Baum_Light World_Baum_Light_By(Vec2 p){
	World_Baum_Light baum = { 0 };
	baum.id = ENTITY_ID_BAUM_LIGHT;
	baum.r = (Rect){ { p.x - 0.5f * WORLD_BAUM_LIGHT_DIM_X,p.y - 0.5f * WORLD_BAUM_LIGHT_DIM_Y },{ WORLD_BAUM_LIGHT_DIM_X,WORLD_BAUM_LIGHT_DIM_Y } };
	return baum;
}
World_Baum_Light* World_Baum_Light_New(Vec2 p){
	World_Baum_Light baum = World_Baum_Light_By(p);
	World_Baum_Light* new_baum = (World_Baum_Light*)malloc(sizeof(World_Baum_Light));
	if(!new_baum) return NULL;
	memcpy(new_baum,&baum,sizeof(baum));
	return new_baum;
}

#define WORLD_BAUM_OBST_DIM_X 1.5f
#define WORLD_BAUM_OBST_DIM_Y 1.25f

typedef World_Entity World_Baum_Obst;
World_Baum_Obst* World_Baum_Obst_Load(DataStream* ds){
	if(!ds || ds->size < (int32_t)sizeof(World_Baum_Obst)) return NULL;
	World_Baum_Obst* baum = (World_Baum_Obst*)malloc(sizeof(World_Baum_Obst));
	if(!baum) return NULL;
	DataStream_ReadCount(ds,baum,0,sizeof(*baum));
	return baum;
}
void World_Baum_Obst_Save(World_Baum_Obst* baum,DataStream* ds){
	DataStream_PushCount(ds,baum,sizeof(*baum));
}
SubSprite World_Baum_Obst_GetRender(World_Baum_Obst* baum,EntityAtlas* ea){
	(void)baum;
	const unsigned int dx = ea->atlas.w / ea->cx;
	const unsigned int dy = ea->atlas.h / ea->cy;
	return SubSprite_New(&ea->atlas,0U,0U,dx,dy);
}
World_Baum_Obst World_Baum_Obst_By(Vec2 p){
	World_Baum_Obst baum = { 0 };
	baum.id = ENTITY_ID_BAUM_OBST;
	baum.r = (Rect){ { p.x - 0.5f * WORLD_BAUM_OBST_DIM_X,p.y - 0.5f * WORLD_BAUM_OBST_DIM_Y },{ WORLD_BAUM_OBST_DIM_X,WORLD_BAUM_OBST_DIM_Y } };
	return baum;
}
World_Baum_Obst* World_Baum_Obst_New(Vec2 p){
	World_Baum_Obst baum = World_Baum_Obst_By(p);
	World_Baum_Obst* new_baum = (World_Baum_Obst*)malloc(sizeof(World_Baum_Obst));
	if(!new_baum) return NULL;
	memcpy(new_baum,&baum,sizeof(baum));
	return new_baum;
}

#define WORLD_PALME_DIM_X 1.5f
#define WORLD_PALME_DIM_Y 1.6f

typedef World_Entity World_Palme;
World_Palme* World_Palme_Load(DataStream* ds){
	if(!ds || ds->size < (int32_t)sizeof(World_Palme)) return NULL;
	World_Palme* palme = (World_Palme*)malloc(sizeof(World_Palme));
	if(!palme) return NULL;
	DataStream_ReadCount(ds,palme,0,sizeof(*palme));
	return palme;
}
void World_Palme_Save(World_Palme* palme,DataStream* ds){
	DataStream_PushCount(ds,palme,sizeof(*palme));
}
SubSprite World_Palme_GetRender(World_Palme* palme,EntityAtlas* ea){
	(void)palme;
	const unsigned int dx = ea->atlas.w / ea->cx;
	const unsigned int dy = ea->atlas.h / ea->cy;
	return SubSprite_New(&ea->atlas,0U,0U,dx,dy);
}
World_Palme World_Palme_By(Vec2 p){
	World_Palme palme = { 0 };
	palme.id = ENTITY_ID_PALME;
	palme.r = (Rect){ { p.x - 0.5f * WORLD_PALME_DIM_X,p.y - 0.5f * WORLD_PALME_DIM_Y },{ WORLD_PALME_DIM_X,WORLD_PALME_DIM_Y } };
	return palme;
}
World_Palme* World_Palme_New(Vec2 p){
	World_Palme palme = World_Palme_By(p);
	World_Palme* new_palme = (World_Palme*)malloc(sizeof(World_Palme));
	if(!new_palme) return NULL;
	memcpy(new_palme,&palme,sizeof(palme));
	return new_palme;
}

#define WORLD_STEIN_DIM_X 3.0f
#define WORLD_STEIN_DIM_Y 1.25f

typedef World_Entity World_Stein;
World_Stein* World_Stein_Load(DataStream* ds){
	if(!ds || ds->size < (int32_t)sizeof(World_Stein)) return NULL;
	World_Stein* stein = (World_Stein*)malloc(sizeof(World_Stein));
	if(!stein) return NULL;
	DataStream_ReadCount(ds,stein,0,sizeof(*stein));
	return stein;
}
void World_Stein_Save(World_Stein* stein,DataStream* ds){
	DataStream_PushCount(ds,stein,sizeof(*stein));
}
SubSprite World_Stein_GetRender(World_Stein* stein,EntityAtlas* ea){
	(void)stein;
	const unsigned int dx = ea->atlas.w / ea->cx;
	const unsigned int dy = ea->atlas.h / ea->cy;
	return SubSprite_New(&ea->atlas,0U,0U,dx,dy);
}
World_Stein World_Stein_By(Vec2 p){
	World_Stein stein = { 0 };
	stein.id = ENTITY_ID_STEIN;
	stein.r = (Rect){ { p.x - 0.5f * WORLD_STEIN_DIM_X,p.y - 0.5f * WORLD_STEIN_DIM_Y },{ WORLD_STEIN_DIM_X,WORLD_STEIN_DIM_Y } };
	return stein;
}
World_Stein* World_Stein_New(Vec2 p){
	World_Stein stein = World_Stein_By(p);
	World_Stein* new_stein = (World_Stein*)malloc(sizeof(World_Stein));
	if(!new_stein) return NULL;
	memcpy(new_stein,&stein,sizeof(stein));
	return new_stein;
}

#define WORLD_TANNE_DIM_X 1.5f
#define WORLD_TANNE_DIM_Y 1.25f

typedef World_Entity World_Tanne;
World_Tanne* World_Tanne_Load(DataStream* ds){
	if(!ds || ds->size < (int32_t)sizeof(World_Tanne)) return NULL;
	World_Tanne* tanne = (World_Tanne*)malloc(sizeof(World_Tanne));
	if(!tanne) return NULL;
	DataStream_ReadCount(ds,tanne,0,sizeof(*tanne));
	return tanne;
}
void World_Tanne_Save(World_Tanne* tanne,DataStream* ds){
	DataStream_PushCount(ds,tanne,sizeof(*tanne));
}
SubSprite World_Tanne_GetRender(World_Tanne* tanne,EntityAtlas* ea){
	(void)tanne;
	const unsigned int dx = ea->atlas.w / ea->cx;
	const unsigned int dy = ea->atlas.h / ea->cy;
	return SubSprite_New(&ea->atlas,0U,0U,dx,dy);
}
World_Tanne World_Tanne_By(Vec2 p){
	World_Tanne tanne = { 0 };
	tanne.id = ENTITY_ID_TANNE;
	tanne.r = (Rect){ { p.x - 0.5f * WORLD_TANNE_DIM_X,p.y - 0.5f * WORLD_TANNE_DIM_Y },{ WORLD_TANNE_DIM_X,WORLD_TANNE_DIM_Y } };
	return tanne;
}
World_Tanne* World_Tanne_New(Vec2 p){
	World_Tanne tanne = World_Tanne_By(p);
	World_Tanne* new_tanne = (World_Tanne*)malloc(sizeof(World_Tanne));
	if(!new_tanne) return NULL;
	memcpy(new_tanne,&tanne,sizeof(tanne));
	return new_tanne;
}

void* World_Entity_New(Vec2 p,Entity_ID id){
	switch (id){
		case ENTITY_ID_BLACKHAIR:	return World_Player_New(p,id);
		case ENTITY_ID_BAUM:		return World_Baum_New(p);
		case ENTITY_ID_BAUM_DARK:	return World_Baum_Dark_New(p);
		case ENTITY_ID_BAUM_LIGHT:	return World_Baum_Light_New(p);
		case ENTITY_ID_BAUM_OBST:	return World_Baum_Obst_New(p);
		case ENTITY_ID_PALME:		return World_Palme_New(p);
		case ENTITY_ID_STEIN:		return World_Stein_New(p);
		case ENTITY_ID_TANNE:		return World_Tanne_New(p);
		default:					break;
	}
	if(id >= ENTITY_ID_ITEM_HOLZ && id <= ENTITY_ID_ITEM_LAST)
		return World_Item_Entity_By(p,id);

	return NULL;
}

#endif