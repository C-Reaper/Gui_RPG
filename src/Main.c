//#define IMAGE_PNG_ONLY

#if defined __linux__ || defined _WINE || defined _WEB
#include "/home/codeleaded/System/Static/Library/PS4_Controller.h"
#include "/home/codeleaded/System/Static/Library/WindowEngine.h"
#include "/home/codeleaded/System/Static/Library/Files.h"
#include "/home/codeleaded/System/Static/Library/TransformedView.h"
#include "/home/codeleaded/System/Static/Library/Geometry.h"
#include "/home/codeleaded/System/Static/Library/AudioPlayer.h"
#elif defined _WIN32
#include "F:/home/codeleaded/System/Static/Library/PS4_Controller.h"
#include "F:/home/codeleaded/System/Static/Library/WindowEngine.h"
#include "F:/home/codeleaded/System/Static/Library/Files.h"
#include "F:/home/codeleaded/System/Static/Library/TransformedView.h"
#include "F:/home/codeleaded/System/Static/Library/Geometry.h"
#include "F:/home/codeleaded/System/Static/Library/AudioPlayer.h"
#endif

#include "../inc/World.h"
#include "../inc/Entities.h"
#include "../inc/TileWorld.h"

TileWorld world;

void Setup(AlxWindow* w){
	AlxFont_Resize(&window.font,32,32);
	
	world = TileWorld_New("./assets/World/","./assets/Tiles/","./assets/Figures/","./assets/Objects/","./assets/Items/");
	TransformedView_Focus(&world.tv,&world.player.e.r.p,world.player.e.r.d);
	//TileWorld_AudioPlayerStart(&world);
}
void Update(AlxWindow* w){
	TransformedView_Output(&world.tv,(Vec2){ GetWidth(),GetHeight() });
	if(!world.world.inventory)
		TransformedView_HandlePanZoom(&world.tv,window.Strokes,GetMouse());
	TransformedView_Focus(&world.tv,&world.player.e.r.p,world.player.e.r.d);

	if(Stroke(ALX_KEY_E).PRESSED &&
	   (world.world.inventory || world.world.portal_exit_timer <= 0.0f))
		TileWorld_Inventory_Toggle(&world);
	if(!world.world.inventory){
		if(Stroke(ALX_KEY_Q).PRESSED){
			world.world.mode = world.world.mode == ANIMATIONBG_DG ? ANIMATIONBG_ALL : ANIMATIONBG_DG;
		}
		if(Stroke(ALX_KEY_TAB).PRESSED){
			world.world.settype = !world.world.settype;
		}
		if(Stroke(ALX_KEY_CAPSLOCK).PRESSED){
			world.world.fill = !world.world.fill;
		}

		if(Stroke(ALX_KEY_1).PRESSED){
			world.selected--;
		}
		if(Stroke(ALX_KEY_2).PRESSED){
			world.selected++;
		}
	}
	if(world.world.inventory){
		const int scroll_direction = Stroke(ALX_MOUSE_S_UP).PRESSED ? -1 : (Stroke(ALX_MOUSE_S_DOWN).PRESSED ? 1 : 0);
		TileWorld_Inventory_Scroll(&world,scroll_direction,(unsigned int)w->MouseY,(unsigned int)GetWidth(),(unsigned int)GetHeight());
		if(Stroke(ALX_MOUSE_L).PRESSED)
			TileWorld_Inventory_Click(&world,(unsigned int)w->MouseX,(unsigned int)w->MouseY,(unsigned int)GetWidth(),(unsigned int)GetHeight());
		else if(Stroke(ALX_MOUSE_R).PRESSED)
			TileWorld_Inventory_ClickRight(&world,(unsigned int)w->MouseX,(unsigned int)w->MouseY,(unsigned int)GetWidth(),(unsigned int)GetHeight());
	}

	if(world.world.inventory){
		world.player.e.v = (Vec2){ 0.0f,0.0f };
		world.player.e.a = (Vec2){ 0.0f,0.0f };
	}else if(world.world.mode != ANIMATIONBG_DG){
		const float dir_x = Stroke(ALX_KEY_A).DOWN ? -1.0f : (Stroke(ALX_KEY_D).DOWN ? 1.0f : 0.0f);
		const float dir_y = Stroke(ALX_KEY_W).DOWN ? -1.0f : (Stroke(ALX_KEY_S).DOWN ? 1.0f : 0.0f);
		Player_Move(&world.player,dir_x,dir_y);
	}else{
		if(Stroke(ALX_MOUSE_M).PRESSED){
			const Vec2 p = TransformedView_ScreenWorldPos(&world.tv,(Vec2){ w->MouseX,w->MouseY });
			world.player.e.r.p = p;
		}
		if((Stroke(ALX_MOUSE_L).PRESSED && !world.world.fill) || (Stroke(ALX_MOUSE_L).DOWN && world.world.fill)){
			const Vec2 p = TransformedView_ScreenWorldPos(&world.tv,(Vec2){ w->MouseX,w->MouseY });
			if(!world.world.settype)
				World_Set(&world.world,floorf(p.x),floorf(p.y),world.selected);
			else
				World_Spawn(&world.world,World_Entity_New(p,world.selected));
		}
		if((Stroke(ALX_MOUSE_R).PRESSED && !world.world.fill) || (Stroke(ALX_MOUSE_R).DOWN && world.world.fill)){
			const Vec2 p = TransformedView_ScreenWorldPos(&world.tv,(Vec2){ w->MouseX,w->MouseY });
			if(!world.world.settype)
				World_Set(&world.world,floorf(p.x),floorf(p.y),BLOCK_NONE);
			else
				World_RemoveAt(&world.world,p.x,p.y);
		}
		
		if(Stroke(ALX_KEY_A).DOWN){
			world.player.e.v.x = -PLAYER_VEL_MAX;
		}else if(Stroke(ALX_KEY_D).DOWN){
			world.player.e.v.x = PLAYER_VEL_MAX;
		}else{
			world.player.e.v.x = 0.0f;
			world.player.e.a.x = 0.0f;
		}

		if(Stroke(ALX_KEY_W).DOWN){
			world.player.e.v.y = -PLAYER_VEL_MAX;
		}else if(Stroke(ALX_KEY_S).DOWN){
			world.player.e.v.y = PLAYER_VEL_MAX;
		}else{
			world.player.e.v.y = 0.0f;
			world.player.e.a.y = 0.0f;
		}
	}

	world.world.t_origin_x = (int32_t)floorf(world.player.e.r.p.x / WORLD_CHUNK_WIDTH);
	world.world.t_origin_y = (int32_t)floorf(world.player.e.r.p.y / WORLD_CHUNK_HEIGHT);
	World_UpdateChunks(&world.world);

	World_Update(&world.world,&world.tv,w->ElapsedTime);
	World_Collisions(&world.world,&world.tv);

	Player_Update(&world.player,&world.world,w->ElapsedTime);
	World_CollisionsEntity(&world.world,&world.tv,&world.player.e);

	Clear(LIGHT_BLUE);

	World_RenderTiles(&world.world,&world.tv,WINDOW_STD_ARGS);
	World_RenderEntities(&world.world,&world.tv,WINDOW_STD_ARGS);
	World_RenderEntity(&world.world,&world.tv,WINDOW_STD_ARGS,&world.player.e);
	
	World_Render(&world.world,&world.tv,WINDOW_STD_ARGS);
	TileWorld_RenderDbg(&world,WINDOW_STD_ARGS,GetMouse().x,GetMouse().y);
	TileWorld_Inventory_Render(&world,WINDOW_STD_ARGS,(unsigned int)w->MouseX,(unsigned int)w->MouseY);

	CStr_RenderAlxFontf(WINDOW_STD_ARGS,&window.font,0.0f,GetHeight() - 1 * GetAlxFont()->CharSizeY,WHITE,"Q: DBG");
	CStr_RenderAlxFontf(WINDOW_STD_ARGS,&window.font,0.0f,GetHeight() - 2 * GetAlxFont()->CharSizeY,WHITE,"%s | %s",world.world.mode == ANIMATIONBG_DG ? "Debug" : "None",world.world.fill ? "Fill" : "Put");
}
void Delete(AlxWindow* w){
	TileWorld_Free(&world);
}

int main(){
    if(Create("Role Playing Game",1800,1000,1,1,Setup,Update,Delete))
        Start();
    return 0;
}