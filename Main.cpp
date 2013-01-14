#include <stdio.h>
#include <allegro5/allegro.h>
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>
#include <string>
enum KEYSUSED {KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_ENTER, KEY_R, KEY_1};
const float SCREEN_W=740;
const float SCREEN_H=480;
const float FPS=60;
const float WALKSPEED=20;
int MOVESPEED=2;
float FIREMOVESPEED=5;
int COINCOUNT=10;
int BGCOLORS[3]={0,0,0};

bool walkA = true;
bool walkB = false;
int random_x[100];
int random_y[100];
int coinAmount;

int randomFire_y[100];
int fireball_x[100];
int fireballAmount;

int powerUp_x;
int powerUp_y;

int Points=0;
int Potion=0;
int lifeCount=3;
int player_x=50 , player_y=50;
int curlevel=1;

///////////////////////////////////

int walk(ALLEGRO_BITMAP *a, ALLEGRO_BITMAP *b){
  if (walkA){
		al_draw_bitmap(a, player_x, player_y, 0);
	}
	else if(walkB) al_draw_bitmap(b, player_x, player_y, 0);
return 0;
}

int drawLives(ALLEGRO_BITMAP* livesBMP, int lives){
		--lives;
		int dist=17;
		for (int i=0; i<=lives; ++i){
			al_draw_bitmap(livesBMP, 170+(dist*i), 0, 0);
		}	
}

int setCoinAmount(int amount){
	 coinAmount=amount-1;
	 srand((unsigned)time(0));
	 for (int i=0; i<=amount; ++i){
		random_x[i] = (rand()%((int)SCREEN_W-18))+1;
		random_y[i] = (rand()%((int)SCREEN_H-18))+1;
	 }
}

int drawCoins(ALLEGRO_BITMAP *coinBMP){
	for (int i=0; i<=coinAmount; ++i){
		if (random_x[i]!=0&&random_y!=0) al_draw_bitmap(coinBMP, random_x[i], random_y[i], 0);
		}
}
	
int checkCoinPickup(int x, int y){
		for (int i=0; i<=coinAmount; ++i){
			for (int ranInc=0; ranInc<=18; ++ranInc){
				for (int yInc=0; yInc<=17; ++yInc){
					if (y+yInc==random_y[i]+ranInc){
						for (int xInc=0; xInc<=25; ++xInc){
							if (x+xInc==random_x[i]+ranInc){random_x[i]=0; random_y[i]=0; ++Points;}
						}	
					}			
				}
			}
		}
}

int setFireballCount(int fireballCount){
		fireballAmount=fireballCount;
		for (int i=0; i<=fireballCount; ++i){
			randomFire_y[i] = (rand()%((int)SCREEN_H-20))+1;
			fireball_x[i] = SCREEN_W-20;
			}
	}

int drawFireballs(ALLEGRO_BITMAP* fireballBMP){
		for (int i=0; i<=fireballAmount; ++i){
			if (fireball_x[i]>(-20)){
				al_draw_bitmap(fireballBMP, fireball_x[i], randomFire_y[i], 0);
				fireball_x[i]-=FIREMOVESPEED;
			}
			else setFireballCount(fireballAmount);
		}
}		

int checkFireHit(){
	for (int i=0; i<=fireballAmount; ++i){
		for (int pyInc=0; pyInc<=17; ++pyInc){
				for (int fyInc=0; fyInc<20; ++fyInc){
					if (randomFire_y[i]+fyInc==player_y+pyInc){
						for (int pxInc=0; pxInc<=25; ++pxInc){
							for (int fxInc; fxInc<=20; ++fxInc){
								if (fireball_x[i]+fxInc==player_x+pxInc){
									 al_rest(1);
									 --lifeCount;
									 player_x=50;
									 player_y=50;
									 setFireballCount(fireballAmount);	
								 }								 
							}
						}
					}
				}
			}
		}
}

int setPowerUpXY(){
			srand((unsigned)time(0));
			powerUp_x=(rand()%((int)SCREEN_W-18))+1;	
			powerUp_y=(rand()%((int)SCREEN_H-18))+1;
}

int drawPowerUp(ALLEGRO_BITMAP* powerupBMP){
	if (curlevel%3==0&&powerUp_x!=0&&powerUp_y!=0) al_draw_bitmap(powerupBMP, powerUp_x, powerUp_y, 0);
}

int checkPowerUp(){
	if (curlevel%3==0){
		for (int pyInc=0; pyInc<=17; ++pyInc){
				for (int potyInc=0; potyInc<20; ++potyInc){
					if (powerUp_y+potyInc==player_y+pyInc){
						for (int pxInc=0; pxInc<=25; ++pxInc){
							for (int potxInc; potxInc<=20; ++potxInc){
								if (powerUp_x+potxInc==player_x+pxInc){
									 powerUp_x=0;
									 powerUp_y=0;
									 al_rest(0.5);
									 ++MOVESPEED;
								 }								 
							}
						}
					}
				}
			}	
	}
}

int nextLevel(ALLEGRO_FONT* fontB, ALLEGRO_FONT* fontS, ALLEGRO_BITMAP* livesBMP, ALLEGRO_BITMAP* coinsBMP, ALLEGRO_BITMAP* powerupBMP){
		++curlevel;
		++COINCOUNT;
		++FIREMOVESPEED;
		++fireballAmount;
		if (curlevel%3==0) Potion=1;
		al_clear_to_color(al_map_rgb(0,0,0));
		al_draw_textf(fontB, al_map_rgb(255, 255, 255), (SCREEN_W/2), (SCREEN_H/2)-70, ALLEGRO_ALIGN_CENTRE, "Level %d", curlevel, ":");
		al_draw_bitmap(livesBMP, (SCREEN_W/2)-120, (SCREEN_H/2)+30, 0);
		al_draw_text(fontS, al_map_rgb(255, 255, 255), (SCREEN_W/2)-92, (SCREEN_H/2)+32, ALLEGRO_ALIGN_CENTRE, "x3");
		al_draw_bitmap(coinsBMP, (SCREEN_W/2)-20, (SCREEN_H/2)+30, 0);
		al_draw_textf(fontS, al_map_rgb(255, 255, 255), (SCREEN_W/2)+15, (SCREEN_H/2)+32, ALLEGRO_ALIGN_CENTRE, "x\ %d", COINCOUNT);
		al_draw_bitmap(powerupBMP, (SCREEN_W/2)+80, (SCREEN_H/2)+30, 0);
		al_draw_textf(fontS, al_map_rgb(255, 255, 255), (SCREEN_W/2)+114, (SCREEN_H/2)+32, ALLEGRO_ALIGN_CENTRE, "x\ %d", Potion);
		al_flip_display();
		al_rest(1.5);
		setCoinAmount(COINCOUNT);
		setFireballCount(fireballAmount);	
		Points=0;
		setPowerUpXY();
		Potion=0;
		lifeCount=3;
		player_x=50;
		player_y=50;
}
	
int main(int argc, char *argv[]){
	bool redraw=false;
	bool doexit=false;
	bool startEasy=false;
	bool startMedium=false;
	bool startHard=false;
	bool startGame=false;
	bool exitMenu = false;
	bool key[4] = {false, false, false, false};//keyboard keys
	bool player_d[4]={false, false, false, false}; //player direction
	bool player_still[4]={false, false, false, true}; //in which direction should the still bmp be loaded 
	
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_TIMER *walkt = NULL;
	ALLEGRO_EVENT_QUEUE *eventQueue = NULL;
	ALLEGRO_BITMAP *DOWNLEFT = NULL;
	ALLEGRO_BITMAP *DOWNRIGHT = NULL;
	ALLEGRO_BITMAP *DOWNSTILL = NULL;
	ALLEGRO_BITMAP *LEFTLEFT = NULL;
	ALLEGRO_BITMAP *LEFTRIGHT = NULL;
	ALLEGRO_BITMAP *LEFTSTILL = NULL;
	ALLEGRO_BITMAP *UPLEFT = NULL;
	ALLEGRO_BITMAP *UPRIGHT = NULL;
	ALLEGRO_BITMAP *UPSTILL = NULL;
	ALLEGRO_BITMAP *RIGHTLEFT = NULL;
	ALLEGRO_BITMAP *RIGHTRIGHT = NULL;
	ALLEGRO_BITMAP *RIGHTSTILL = NULL;
	ALLEGRO_BITMAP *Randcoin = NULL;
	ALLEGRO_BITMAP *Fireball = NULL;
	ALLEGRO_BITMAP *LivesBMP = NULL;
	ALLEGRO_BITMAP *Powerup = NULL;
	
    if(!al_init()) {
       fprintf(stderr, "failed to initialize allegro!\n");
       return -1;
    }
	
	al_init_font_addon();
	al_init_ttf_addon(); 
	
	if(!al_init_image_addon()) {
      al_show_native_message_box(display, "Error", "Error", "Failed to initialize al_init_image_addon!", 
                                 NULL, ALLEGRO_MESSAGEBOX_ERROR);
      return -1;
   }
	
	al_init_primitives_addon();
	
	if(!al_install_keyboard()) {
      fprintf(stderr, "failed to initialize the keyboard!\n");
      return -1;
   }

	display = al_create_display(SCREEN_W, SCREEN_H);	
    DOWNLEFT = al_load_bitmap("Images/SPRITES/DOWN-LEFT.bmp");
    DOWNRIGHT = al_load_bitmap("Images/SPRITES/DOWN-RIGHT.bmp");
    DOWNSTILL = al_load_bitmap("Images/SPRITES/DOWN-STILL.bmp");
    LEFTLEFT = al_load_bitmap("Images/SPRITES/LEFT-LEFT.bmp");
    LEFTRIGHT = al_load_bitmap("Images/SPRITES/LEFT-RIGHT.bmp");
    LEFTSTILL = al_load_bitmap("Images/SPRITES/LEFT-STILL.bmp");
    UPLEFT = al_load_bitmap("Images/SPRITES/UP-LEFT.bmp");
    UPRIGHT = al_load_bitmap("Images/SPRITES/UP-RIGHT.bmp");
    UPSTILL = al_load_bitmap("Images/SPRITES/UP-STILL.bmp");
    RIGHTLEFT = al_load_bitmap("Images/SPRITES/RIGHT-LEFT.bmp");
    RIGHTRIGHT = al_load_bitmap("Images/SPRITES/RIGHT-RIGHT.bmp");
    RIGHTSTILL = al_load_bitmap("Images/SPRITES/RIGHT-STILL.bmp");
    Randcoin = al_load_bitmap("Images/SPRITES/Coin.bmp");
    Fireball = al_load_bitmap("Images/SPRITES/FireBall.bmp");
    LivesBMP = al_load_bitmap("Images/SPRITES/Lives.bmp");
    Powerup = al_load_bitmap("Images/SPRITES/powerup.bmp");
    ALLEGRO_FONT *fontSmall = al_load_ttf_font("Images/Hero.otf",20,0 );
    ALLEGRO_FONT *fontBig = al_load_ttf_font("Images/Hero.otf",70,0 );
    ALLEGRO_FONT *fontMedium = al_load_ttf_font("Images/Hero.otf",40,0 );
    timer = al_create_timer(1.0/FPS);
    walkt = al_create_timer(1/WALKSPEED);
    eventQueue = al_create_event_queue();
	
	  
    
    al_register_event_source(eventQueue, al_get_keyboard_event_source());
    al_register_event_source(eventQueue, al_get_display_event_source(display));
    al_register_event_source(eventQueue, al_get_timer_event_source(timer));
    al_register_event_source(eventQueue, al_get_timer_event_source(walkt));
    al_clear_to_color(al_map_rgb(0,0,0)); 
    al_flip_display(); 
    
    al_draw_text(fontBig, al_map_rgb(255,255,255), 0, 0, 0, "Coin Collector.");
    for (int i=0; i<=90; ++i) al_draw_bitmap(Randcoin, 0+(i*18), 60, 0);
    al_draw_text(fontBig, al_map_rgb(255,255,255), 140, 100, 0, "1: Easy");
	al_draw_text(fontBig, al_map_rgb(255,255,255), 160, 200, 0, "2: Medium");
	al_draw_text(fontBig, al_map_rgb(255,255,255), 200, 300, 0, "3: Hard");
	al_flip_display();
	
	while (!exitMenu){
		ALLEGRO_EVENT ev;
		al_wait_for_event(eventQueue, &ev);
		
   if (ev.type==ALLEGRO_EVENT_DISPLAY_CLOSE){
			doexit=true;
			break;
		}
   else if(ev.type == ALLEGRO_EVENT_KEY_UP) {
         switch(ev.keyboard.keycode) {
            case ALLEGRO_KEY_1: 
				startEasy=true;
               startGame=true;
               exitMenu=true;      
               break;
               
            case ALLEGRO_KEY_2:
				startMedium=true;
				startGame=true;
				exitMenu=true;
				break;
				
			case ALLEGRO_KEY_3:
				startHard=true;
				startGame=true;
				exitMenu=true;
				break;
		   }
	   }
}

	if (startEasy) {COINCOUNT=10; FIREMOVESPEED=5; fireballAmount=10; BGCOLORS[0]=80; BGCOLORS[1]=224; BGCOLORS[2]=27;}
	else if (startMedium) {COINCOUNT=15; FIREMOVESPEED=7; fireballAmount=12; BGCOLORS[0]=87; BGCOLORS[1]=87; BGCOLORS[2]=87;}
	else if (startHard) {COINCOUNT=20; FIREMOVESPEED=9; fireballAmount=14; BGCOLORS[0]=117; BGCOLORS[1]=20; BGCOLORS[2]=0;}
    setCoinAmount(COINCOUNT);
    setFireballCount(fireballAmount);
    setPowerUpXY();
    
    if (startGame){
	al_clear_to_color(al_map_rgb(0,0,0));
	al_draw_textf(fontBig, al_map_rgb(255, 255, 255), (SCREEN_W/2), (SCREEN_H/2)-70, ALLEGRO_ALIGN_CENTRE, "Level %d", curlevel, ":");
	al_draw_bitmap(LivesBMP, (SCREEN_W/2)-120, (SCREEN_H/2)+30, 0);
	al_draw_text(fontSmall, al_map_rgb(255, 255, 255), (SCREEN_W/2)-92, (SCREEN_H/2)+32, ALLEGRO_ALIGN_CENTRE, "x3");
	al_draw_bitmap(Randcoin, (SCREEN_W/2)-20, (SCREEN_H/2)+30, 0);
	al_draw_textf(fontSmall, al_map_rgb(255, 255, 255), (SCREEN_W/2)+15, (SCREEN_H/2)+32, ALLEGRO_ALIGN_CENTRE, "x\ %d", COINCOUNT);
	al_draw_bitmap(Powerup, (SCREEN_W/2)+80, (SCREEN_H/2)+30, 0);
	al_draw_textf(fontSmall, al_map_rgb(255, 255, 255), (SCREEN_W/2)+114, (SCREEN_H/2)+32, ALLEGRO_ALIGN_CENTRE, "x\ %d", Potion);
	al_flip_display();
	al_rest(1.5);
	al_clear_to_color(al_map_rgb(BGCOLORS[0], BGCOLORS[1], BGCOLORS[2]));
    al_start_timer(timer);
    al_start_timer(walkt);
    while (1){
		ALLEGRO_EVENT ev;
		al_wait_for_event(eventQueue, &ev);
		if (!redraw&&ev.type==ALLEGRO_EVENT_TIMER&&ev.timer.source == timer){
			if (key[KEY_UP]&&player_y>=0){player_y+=-MOVESPEED; player_d[0]=true; player_d[1]=false; player_d[2]=false; player_d[3]=false;}
			else if (key[KEY_DOWN]&&player_y+25<=SCREEN_H){player_y+=MOVESPEED; player_d[1]=true; player_d[0]=false; player_d[2]=false; player_d[3]=false;}
			else if (key[KEY_LEFT]&&player_x>=0){player_x+=-MOVESPEED; player_d[2]=true; player_d[1]=false; player_d[0]=false; player_d[3]=false;}
			else if (key[KEY_RIGHT]&&player_x+17<=SCREEN_W){player_x+=MOVESPEED; player_d[3]=true; player_d[1]=false; player_d[2]=false; player_d[0]=false;}
			else if (player_d[0]){ player_d[0]=false; player_still[0]=true; player_still[1]=false; player_still[2]=false; player_still[3]=false;}
			else if (player_d[1]){ player_d[1]=false; player_still[1]=true; player_still[0]=false; player_still[2]=false; player_still[3]=false;}
			else if (player_d[2]){ player_d[2]=false; player_still[2]=true; player_still[1]=false; player_still[0]=false; player_still[3]=false;}
			else if (player_d[3]){ player_d[3]=false; player_still[3]=true; player_still[1]=false; player_still[2]=false; player_still[0]=false;}
			checkCoinPickup(player_x, player_y);
			checkFireHit();
			checkPowerUp();
			redraw=true;
		}
		else if (ev.type==ALLEGRO_EVENT_DISPLAY_CLOSE){
			doexit=true;
			break;
		}
	  else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
         switch(ev.keyboard.keycode) {
            case ALLEGRO_KEY_UP:
               key[KEY_UP] = true;               
               break;
 
            case ALLEGRO_KEY_DOWN:
               key[KEY_DOWN] = true;               
               break;
 
            case ALLEGRO_KEY_LEFT: 
               key[KEY_LEFT] = true;               
               break;
 
            case ALLEGRO_KEY_RIGHT:
               key[KEY_RIGHT] = true;               
               break;
            
			case ALLEGRO_KEY_R:
				key[KEY_R] = true;
				break;
			
         }
      }
      else if(ev.type == ALLEGRO_EVENT_KEY_UP) {
         switch(ev.keyboard.keycode) {
            case ALLEGRO_KEY_UP:
               key[KEY_UP] = false;
               
               break;
 
            case ALLEGRO_KEY_DOWN:
               key[KEY_DOWN] = false;
               break;
 
            case ALLEGRO_KEY_LEFT: 
               key[KEY_LEFT] = false;
               break;
 
            case ALLEGRO_KEY_RIGHT:
               key[KEY_RIGHT] = false;
               break;
            
			case ALLEGRO_KEY_R:
				key[KEY_R] = true;
				break;
			
          }
		
	   }
		else if (redraw&&al_is_event_queue_empty(eventQueue)){
			redraw=false;
			if (lifeCount==0){
				break;
			}
			if (Points==COINCOUNT) nextLevel(fontBig, fontSmall, LivesBMP, Randcoin, Powerup);
			al_clear_to_color(al_map_rgb(BGCOLORS[0], BGCOLORS[1], BGCOLORS[2]));
			drawCoins(Randcoin);
			drawPowerUp(Powerup);
			checkPowerUp();
			if (player_d[0])walk(UPLEFT, UPRIGHT);
			else if (player_d[1])walk(DOWNLEFT, DOWNRIGHT);
			else if (player_d[2])walk(LEFTLEFT, LEFTRIGHT);
			else if (player_d[3])walk(RIGHTLEFT, RIGHTRIGHT);
			drawFireballs(Fireball);
			checkFireHit();
			if (ev.type == ALLEGRO_EVENT_TIMER && ev.timer.source == walkt){
				if (walkA) {walkA = false; walkB = true;}
				else if (walkB) {walkB = false; walkA = true;}
			}
			
			
			if (!player_d[3]&&!player_d[1]&&!player_d[2]&&!player_d[0]){
					if (player_still[0])al_draw_bitmap(UPSTILL, player_x, player_y, 0);
					else if (player_still[1])al_draw_bitmap(DOWNSTILL, player_x, player_y, 0);
					else if (player_still[2])al_draw_bitmap(LEFTSTILL, player_x, player_y, 0);
					else if (player_still[3])al_draw_bitmap(RIGHTSTILL, player_x, player_y, 0);
				}
			
			al_draw_textf(fontSmall, al_map_rgb(255,255,255), 0, 0, 0, "Coins: %d", Points);
			al_draw_textf(fontSmall, al_map_rgb(255,255,255), 70, 0, 0, " / %d", COINCOUNT);
			al_draw_text(fontSmall, al_map_rgb(255,255,255), 115, 0, 0, "Lives: ");
			drawLives(LivesBMP, lifeCount);
			al_flip_display();
		}		
	}
}
	if (!doexit){
		al_clear_to_color(al_map_rgb(0,0,0));
		al_draw_text(fontBig, al_map_rgb(255, 255, 255), (SCREEN_W/2)-20, (SCREEN_H/2)-50, ALLEGRO_ALIGN_CENTRE, "You Died!");
		al_flip_display();
		al_rest(1);
	}
	al_destroy_bitmap(DOWNLEFT);
    al_destroy_bitmap(DOWNRIGHT);
    al_destroy_bitmap(DOWNSTILL);
    al_destroy_bitmap(LEFTLEFT);
    al_destroy_bitmap(LEFTRIGHT);
    al_destroy_bitmap(LEFTSTILL);
    al_destroy_bitmap(UPLEFT);
    al_destroy_bitmap(UPRIGHT);
    al_destroy_bitmap(UPSTILL);
    al_destroy_bitmap(RIGHTLEFT);
    al_destroy_bitmap(RIGHTRIGHT);
    al_destroy_bitmap(RIGHTSTILL);
    al_destroy_timer(timer);
	al_destroy_display(display);
    al_destroy_event_queue(eventQueue);
    
	return 0;
}
