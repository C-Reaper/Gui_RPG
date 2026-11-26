#ifndef TILEWORLD_H
#define TILEWORLD_H

#include "World.h"
#include "Figure.h"
#include "Entities.h"

#include "/home/codeleaded/System/Static/Library/AudioPlayer.h"

typedef struct TileWorld {
    World world;
    Figure player;
    TransformedView tv;
    AudioPlayer ap;
    Block selected;
} TileWorld;

SubSprite TileWorld_Gras_Get(Animation* a,World* w,unsigned int x,unsigned int y){
	unsigned int ox = 1U;
	unsigned int oy = 1U;
	unsigned int dx = a->atlas_img.w / a->atlas_cx;
	unsigned int dy = a->atlas_img.h / a->atlas_cy;
	
	if(World_Get(w,x - 1,y) != BLOCK_GRAS)
		ox--;
	else if(World_Get(w,x + 1,y) != BLOCK_GRAS)
		ox++;

	if(World_Get(w,x,y - 1) != BLOCK_GRAS)
		oy--;
	else if(World_Get(w,x,y + 1) != BLOCK_GRAS)
		oy++;
	
	return SubSprite_New(&a->atlas_img,ox * dx,oy * dy,dx,dy);
}
SubSprite TileWorld_Stone_Get(Animation* a,World* w,unsigned int x,unsigned int y){
	unsigned int ox = 1U;
	unsigned int oy = 1U;
	unsigned int dx = a->atlas_img.w / a->atlas_cx;
	unsigned int dy = a->atlas_img.h / a->atlas_cy;
	
	if(World_Get(w,x - 1,y) != BLOCK_STONE)
		ox--;
	else if(World_Get(w,x + 1,y) != BLOCK_STONE)
		ox++;

	if(World_Get(w,x,y - 1) != BLOCK_STONE)
		oy--;
	else if(World_Get(w,x,y + 1) != BLOCK_STONE)
		oy++;
	
	return SubSprite_New(&a->atlas_img,ox * dx,oy * dy,dx,dy);
}

TileWorld TileWorld_New(char* path_world,char* path_blocks,char* path_figures,char* path_objects){
    TileWorld tw;
    
	tw.player = Figure_New(
		Player_New((Vec2){ 0.0f,0.0f },ENTITY_ID_BLACKHAIR),
		(void(*)(Entity*,World*,const float))Player_Update,
		(void(*)(Entity*,const float,const float))Player_Move,
		(void(*)(Entity*))Player_Free
	);

	tw.tv = TransformedView_New((Vec2){ GetWidth(),GetHeight() });
	TransformedView_Zoom(&tw.tv,(Vec2){ 0.05f,0.05f });
	//TransformedView_Offset(&tv,(Vec2){ -0.5f,0.0f });
	TransformedView_Focus(&tw.tv,&tw.player.e->r.p,tw.player.e->r.d);

	tw.ap = AudioPlayer_New();
	tw.world = World_Make(path_world,
        (Animation[]){
		    Animation_Make_SpritePath(path_blocks,"Dirt.png"),
		    Animation_Make_AtlasPath(path_blocks,"Gras.png",3,3,TileWorld_Gras_Get),
		    Animation_Make_AtlasPath(path_blocks,"Stone.png",3,3,TileWorld_Stone_Get),
		    Animation_Null()
	    },
        (EntityAtlas[]){
		    EntityAtlas_New(path_figures,"Blackhair.png",4,4,(void(*)(void*,const float))Player_Update,(SubSprite(*)(void*,EntityAtlas*))Player_GetRender,(void(*)(void*))Player_Free),
		    //EntityAtlas_New(path_objects,"Player.png",16,12,(void(*)(void*,const float))Player_Update,(SubSprite(*)(void*,EntityAtlas*))Player_GetRender,(void(*)(void*))Player_Free),
		    EntityAtlas_Null()
	    }
    );

	tw.selected = BLOCK_NONE;
    tw.player.e->r.p = tw.world.spawn;
    return tw;
}
void TileWorld_AudioPlayerStart(TileWorld* tw){
    AudioPlayer_Start(&tw->ap);
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
			const Block b = tw->selected + i - (preshow >> 1);

			if(World_isBlock(&tw->world,b)){
				SubSprite ss = World_Img(&tw->world,b,0.0f,0.0f);
				if(ss.sp) Sprite_RenderSubAlpha(out,width,height,ss.sp,x + i * (ss.dx * 1.1f),5.0f,ss.ox,ss.oy,ss.dx,ss.dy);
			}
		}
	}
}
void TileWorld_Free(TileWorld* tw){
    World_Free(&tw->world);
    Figure_Free(&tw->player);
    AudioPlayer_Free(&tw->ap);
}

#endif