#include "server_gltron.h"

int getCol(int x, int y) {
  if(x < 0 || x >= game->settings->grid_size -1 ||
     y < 0 || y >= game->settings->grid_size -1 ||
     colmap[ y * colwidth + x ] != 0)
    return 1;
  else return 0;
}

void initGameStructures() { /* called only once */
  int i;

  AI *ai;
  Player *p;

  game->winner = -1;
  game->players = PLAYERS;
  game->player = (Player *) malloc(MAX_PLAYERS * sizeof(Player));
  for(i = 0; i < game->players; i++) {
    p = (game->player + i);
    p->ai = (AI*) malloc(sizeof(AI));
    p->data = (Data*) malloc(sizeof(Data));
    p->data->trails = (line*) malloc(MAX_TRAIL * sizeof(line));

    ai = p->ai;
    switch(i) {
    case 0: ai->active = game->settings->ai_player1; break;
    case 1: ai->active = game->settings->ai_player2; break;
    case 2: ai->active = game->settings->ai_player3; break;
    case 3: ai->active = game->settings->ai_player4; break;
    default:
      fprintf(stderr, "player index #%d not caught!\n", i);
      ai->active = AI_NONE;
    }
    ai->tdiff = 0;
    ai->moves = 0;
    ai->danger = 0;
    ai->lastx = 0;
    ai->lasty = 0;
  }

  game2->events.next = NULL;
  game2->mode = GAME_SINGLE;
}

void initPlayerData() {
  int i;
  Data *data;
  AI *ai;
  int not_playing = 0;

  for(i = 0; i < game->players; i++) {
    float startpos[][2] = { 
	{ 0.5, 0.25 }, { 0.75, 0.5 }, { 0.5, 0.75 }, { 0.25, 0.5 }
    };
    /* float startdir[] = { 2, 1, 0, 3 }; */

    data = game->player[i].data;
    ai = game->player[i].ai;

    /* arrange players in circle around center */

    data->iposx = startpos[i][0] * game->settings->grid_size;
    data->iposy = startpos[i][1] * game->settings->grid_size;
    if(i == 0) data->iposy -= 1;
    /* randomize starting direction */
    data->dir = rand() & 3;
    /* data->dir = startdir[i]; */
    data->last_dir = data->dir;
    data->posx = data->iposx;
    data->posy = data->iposy;
    data->t = 0;
    data->turn = 0;
    data->turn_time = -TURN_LENGTH;

    /* if player is playing... */
    if(ai->active != 2) {
      data->speed = game->settings->current_speed;
      data->trail_height = TRAIL_HEIGHT;
      data->exp_radius = 0;
    } else {
      data->speed = SPEED_GONE;
      data->trail_height = 0;
      data->exp_radius = EXP_RADIUS_MAX;

      not_playing++;
    }
    data->trail = data->trails;

    data->trail->sx = data->trail->ex = data->iposx;
    data->trail->sy = data->trail->ey = data->iposy;

    ai->tdiff = 0;
    ai->moves = game->settings->grid_size / 10;
    ai->danger = 0;
    ai->lasttime = 0;
  }
  game->running = game->players - not_playing; /* not everyone is alive */
  printf("starting game with %d players\n", game->running);
  game->winner = -1;
}

void initData() {
  int i;

  /* colmap */

  /* TODO: check if grid_size/colwidth has changed and  
   *       reallocate colmap accordingly                */

  colwidth = game->settings->grid_size;
  if(colmap != NULL)
    free(colmap);
  colmap = (unsigned char*) malloc(colwidth * game->settings->grid_size);
  for(i = 0; i < colwidth * game->settings->grid_size; i++)
    *(colmap + i) = 0;

  /* lasttime = SystemGetElapsedTime(); */
  game->pauseflag = 0;

  game2->rules.speed = game->settings->current_speed;
  game2->rules.eraseCrashed = game->settings->erase_crashed;
  /* time management */
  game2->time.lastFrame = 0;
  game2->time.current = 0;
  game2->time.offset = SystemGetElapsedTime();
  /* TODO: fix that */
  game2->players = game->players;
  /* event management */
  game2->events.next = NULL;
  /* TODO: free any old events that might have gotten left */

  initPlayerData();
}

int updateTime() {
  game2->time.lastFrame = game2->time.current;
  game2->time.current = SystemGetElapsedTime() - game2->time.offset;
  game2->time.dt = game2->time.current - game2->time.lastFrame;
  /* fprintf(stderr, "dt: %d\n", game2->time.dt); */
  return game2->time.dt;
}

void addList(list **l, void* data) {
  list *p;
  if(*l == NULL) {
    *l = (list*) malloc(sizeof(list));
    (*l)->next = NULL;
  }
  for(p = *l; p->next != NULL; p = p->next);
  p->next = (list*) malloc(sizeof(list));
  p->next->next = NULL;
  p->data = data;
}

void resetScores() {
  int i;
  for(i = 0; i < game->players; i++)
    game->player[i].data->score = 0;
}

void moveStep(Data* data) {
  data->iposx += dirsX[data->dir];
  data->iposy += dirsY[data->dir];
}

void clearTrail(int player) {
  int i;

  for(i = 0; i < colwidth * game->settings->grid_size; i++)
    if(colmap[i] == player + 1)
      colmap[i] = 0;

}

void crashPlayer(int player) {
  int j;
  for(j = 0; j < game->players; j++) 
    if(j != player && game->player[j].data->speed > 0)
      game->player[j].data->score++;

  game->player[player].data->speed = SPEED_CRASHED;

  if(game2->rules.eraseCrashed == 1)
    clearTrail(player);
}

void writePosition(int player) {
  int x, y;

  x = game->player[player].data->iposx;
  y = game->player[player].data->iposy;

  /* collision detection */
  colmap[ y * colwidth + x ] = player + 1;

}

void newTrail(Data* data) {
  line *new;

  data->trail->ex = data->iposx;
  data->trail->ey = data->iposy;

  new = data->trail + 1; /* new line */

  new->sx = data->iposx;
  new->sy = data->iposy;

  data->trail = new;
}
      
void doTurn(Data *data, int time) {
  newTrail(data);
  data->last_dir = data->dir;
  data->dir = (data->dir + data->turn) % 4;
  data->turn = 0;
  data->turn_time = game2->time.current;
  data->posx = data->iposx + data->t * dirsX[data->dir];
  data->posy = data->iposy + data->t * dirsY[data->dir];
}

int applyGameInfo() {
  int i; 
  Data *data;
  //if(game2->players > game->players) {
  if(game2->players > MAX_PLAYERS) {
  
    fprintf(stderr, "more players in demo than allowed\n");
    return 1;
  }



  game->settings->grid_size = default_arena_sizes[game->settings->arena_size];

  /* choose speed */
  game->settings->current_speed = default_speeds[ game->settings->game_speed ];

  for(i = 0; i < game2->players; i++) {
    data = game->player[i].data;
    data->speed = game->settings->current_speed;

  }

  for(; i < MAX_PLAYERS; i++) {
    data = game->player[i].data;
    data->speed = SPEED_GONE;
  }


  return 0;
}
