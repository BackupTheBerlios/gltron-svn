#include "gltron.h"

static char *speed_list[] = {  "boring", "normal", "fast", "crazy", NULL };
static char *arena_list[] = { "tiny", "medium", "big", "vast", "extreme", NULL };

static int coffset;

static char message[255] ="pregame";
static char chat[1024]   = "";

void mousePregame (int buttons, int state, int x, int y)
{
  //if ( state == SYSTEM_MOUSEPRESSED )
      //  switchCallbacks(&guiCallbacks);
}

void keyPregame(int k, int unicode, int x, int y)
{
  switch( k )
    {
    case 13:
      switchCallbacks(&keyboardreadingCallbacks);
      break;
    case SDLK_ESCAPE:
      fprintf(stderr, "exit network game\n");
      isConnected=0;
      isLogged=0;
      Net_disconnect();
      serverstate=preGameState; //We hope that next time server will be to preGameState
      changeCallback(&guiCallbacks);      
      //TODO: see how not to came back to this callBack when doing lot of esc in gui!
      break;
      
    }
}

void idlePregame() {
  SystemPostRedisplay();
  if( isConnected && Net_checksocks() )
    {
      handleServer();
    }
}

void
drawMessage(char *str)
{
  strcpy(message, str);
}

void
drawChat(char *str)
{
  //TODO: multiline chat...
  strcpy(chat, str);
}

void drawPregame() {
  int time;
  int x, y;
  int h;
  int i, len;
  char str[255];
  float colors[][3] = { { 1.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 }, { 0.1, 0.1, 0.5} , { 0.0, 0.5, 1.0 }};
  time = SDL_GetTicks() - coffset;
  
  //glClearColor(.0, .0, .0, .0);
  //glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //rasonly(game->screen);
  
  //Message
  h = game->screen->vp_h / (24 * 1.5);
  glColor3fv(colors[1]);
  x = 10;
  y = game->screen->vp_h - 1.5 * h * (0 + 1);
  drawText(gameFtx, x, y, h, message);

  //chat
/*   glColor3fv(colors[0]); */
/*   x = 10; */
/*   y = game->screen->vp_h - 1.5 * h * (5 + 1); */
/*   drawText(gameFtx, x, y, h, chat); */
  
  //Draw pregametext
  draw_wtext(pregametext);
  
  //calculate the max len of a name;
  len=5; //for empty;
  for(i=0; i<MAX_PLAYERS; ++i)
    {
      if( slots[i].active && strlen( slots[i].name)>len)
	{
	  len= strlen( slots[i].name);
	}
    }
  glColor3fv(colors[1]);
  x = game->screen->vp_w - 1.5 * (len+1)*( game->screen->vp_w / (50 * 1.5) );
  y = game->screen->vp_h - 1.5 * h * (5);
  drawText(gameFtx, x, y, h, "Users");

  //Users
  for(i=0; i<MAX_PLAYERS; ++i)
    {
      y = game->screen->vp_h - 1.5 * h * (i + 6);
      if( slots[i].active == 1 )
	{
	  if( slots[i].isMaster )
	    glColor3fv(colors[3]);
	    
	  drawText(gameFtx, x, y, h, slots[i].name);
	  if( slots[i].isMaster )
	    glColor3fv(colors[1]);
	  
	} else {
	  drawText(gameFtx, x, y, h, "Empty");
	}
    }

  //Inputs
  glColor3fv(colors[2]);
  x = 10;
  y = h-1;
  drawText(gameFtx, x, y, h, getInputEntry());

  //NetRules
  glColor3fv(colors[1]);
  x = game->screen->vp_w - 1.5 * 20*( game->screen->vp_w / (50 * 1.5) );
  y = game->screen->vp_h - 1.5 * h * 10;
  drawText(gameFtx, x, y, h, "Game Settings"); 
  sprintf(str, "Games: %d", netrulenbwins);
  y = game->screen->vp_h - 1.5 * h * 11; 
  drawText(gameFtx, x, y, h, str);
  sprintf(str, "Time: %d", netruletime);
  y = game->screen->vp_h - 1.5 * h * 12; 
  drawText(gameFtx, x, y, h, str);

  //GameRules 
  sprintf(str, "eraseCrashed: %d", game2->rules.eraseCrashed);
  y = game->screen->vp_h - 1.5 * h * 13; 
  drawText(gameFtx, x, y, h, str);
  sprintf(str, "Speed: %s", speed_list[game->settings->game_speed]);
  y = game->screen->vp_h - 1.5 * h * 14; 
  drawText(gameFtx, x, y, h, str);
  sprintf(str, "arena size: %s", arena_list[game->settings->arena_size]);
  y = game->screen->vp_h - 1.5 * h * 15; 
  drawText(gameFtx, x, y, h, str);

  
  
}

void displayPregame() {
  drawGuiBackground();
  if(!game->settings->softwareRendering)
    drawGuiLogo();
  drawPregame();
  SystemSwapBuffers();
}

void initPregame() {
  coffset = SDL_GetTicks();

  //Reinit scores
  netscores.winner = -1;

  pregametext = new_wtext(100, 10, 15, 100);
  insert_wtext(pregametext, "welcome to gltron server...\nlogged\n", 0);

  printf("entering netpregame\n");
}

void cleanPregame()
{
  free_wtext(pregametext);
  pregametext=NULL;
  //fprintf(stderr, "pregame: deconnecting...\n");
  //Net_deconnect();
}

callbacks netPregameCallbacks = {
  displayPregame, idlePregame, keyPregame, initPregame,
  cleanPregame, NULL, mousePregame, NULL
};

