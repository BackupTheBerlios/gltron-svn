#include "gltron.h"
#include "event.h"
#include "engine.h"
#include "console.h"
#include "recognizer.h"

int processEvent(GameEvent* e) {
  int value = 0;
  Data *data;

  if(game2->mode == GAME_SINGLE_RECORD) {
    writeEvent(e);
  }
  switch(e->type) {
  case EVENT_TURN_LEFT:
    doLeftTurn(e);
    break;
  case EVENT_TURN_RIGHT:
    doRightTurn(e);
    break;
  case EVENT_CRASH: 
    data = game->player[e->player].data;
    data->posx = data->iposx = e->x;
    data->posy = data->iposy = e->y;
    displayMessage(TO_CONSOLE | TO_STDERR, "player %d crashed", e->player + 1);
    doCrashPlayer(e);
    break;
  case EVENT_STOP:
    displayMessage(TO_STDERR, "game stopped");
    if(game2->mode == GAME_SINGLE_RECORD) {
      stopRecording();
      game2->mode = GAME_SINGLE;
    } else if(game2->mode == GAME_PLAY) {
      stopPlaying();
      game2->mode = GAME_SINGLE;
    }
    if(e->player<PLAYERS && game->player[e->player].ai->active != AI_NONE) {
      game->winner = e->player;
      displayMessage(TO_CONSOLE | TO_STDOUT, "winner: %d", game->winner + 1);
    } else {
      game->winner = -2;
      displayMessage(TO_CONSOLE | TO_STDOUT, "everyone died! no one wins!");
    }
    switchCallbacks(&pauseCallbacks);
    /* screenSaverCheck(0); */
    stoptime = SystemGetElapsedTime();
    game->pauseflag = PAUSE_GAME_FINISHED;
    value = 1;
    break;
  }
  free(e);
  return value;
}

static list* doMovement(int mode, int dt) {
  int i;
  float fs;
  Data *data;
  list *l = NULL;

  for(i = 0; i < game->players; i++) {
    data = game->player[i].data;
    if(data->speed > 0) { /* still alive */

#define FREQ 1200
#define FACTOR 0.09
      fs = 1.0 - FACTOR + FACTOR * 
	cos(i * M_PI / 4.0 + 
	    (float)(game2->time.current % FREQ) * 2.0 * M_PI / (float)FREQ);
#undef FREQ
#undef FACTOR

      data->t += dt / 100.0 * data->speed * fs;
      while(data->t >= 1) {
	moveStep(data);
	data->t--;
	if(getCol(data->iposx, data->iposy) && mode) {
	  createEvent(i, EVENT_CRASH);
	  break;
	} else {
	  writePosition(i);
	}
      }
      data->posx = data->iposx + data->t * dirsX[data->dir];
      data->posy = data->iposy + data->t * dirsY[data->dir];
    } else { /* already crashed */
      if(game2->rules.eraseCrashed == 1 && data->trail_height > 0)
	data->trail_height -= (float)(dt * TRAIL_HEIGHT) / 1000;
      if(data->exp_radius < EXP_RADIUS_MAX)
	data->exp_radius += (float)dt * EXP_RADIUS_DELTA;
      else if (data->speed == SPEED_CRASHED) {
	int winner = -1;

	data->speed = SPEED_GONE;
	game->running--;
	if(game->running <= 1) { /* all dead, find survivor */
	  int i, maxSpeed = SPEED_GONE;
	  /* create winner event */
	  for(i = 0; i < game->players; i++) {
	    if(game->player[i].data->speed >= maxSpeed) {
	      winner = i;
	      maxSpeed = game->player[i].data->speed;
	    }
	  }
	  if(mode) {
            createEvent(winner, EVENT_STOP);
	    /* a stop event is the last event that happens */
	    return l;
	  }
	}
      }
    }      
  }
  return l;
}
 
void idleGame( void ) {
  list *l;
  list *p;
  int i;
  int dt;
  int t;

#ifdef LUA_PROFILE
  printf("%d lua calls since last idle call\n", lua_profile); */
#endif
  lua_profile = 0;

#ifdef SOUND
  soundIdle();
#endif

  if(updateTime() == 0) return;

  switch(game2->mode) {
  case GAME_NETWORK_RECORD:
#ifdef NETWORK
    updateNet();
#endif
    /* fall through */
  case GAME_SINGLE:
  case GAME_SINGLE_RECORD:
    /* check for fast finish */
    
    if (game2->settingsCache.fast_finish == 1) {
      int factor = 4;
      for (i = 0; i < game->players; i++) {
	      if (game->player[i].ai->active != AI_COMPUTER &&
	          game->player[i].data->exp_radius < EXP_RADIUS_MAX) {
	        factor = 1;
        }
      }
      dt = game2->time.dt * factor;
    } else { 
      dt = game2->time.dt;
    }

    while(dt > 0) {
      if(dt > PHYSICS_RATE) t = PHYSICS_RATE;
      else t = dt;

      /* run AI */
      for(i = 0; i < game->players; i++)
	if(game->player[i].ai != NULL)
	  if(game->player[i].ai->active == AI_COMPUTER &&
	     PLAYER_IS_ACTIVE(&game->player[i])) {

	    ai_function(i, 0);
	  }

      /* process any outstanding events (turns, etc) */
      for(p = &(game2->events); p->next != NULL; p = p->next) {
	if(processEvent((GameEvent*) p->data)) return;
      }

      /* free events */
      p = game2->events.next;
      while(p != NULL) {
	l = p;
	p = p->next;
	free(l);
      }
      game2->events.next = NULL;

      l = doMovement(1, t); /* this can generate new events */
      if(l != NULL) {
	for(p = l; p->next != NULL; p = p->next) {
	  if(processEvent((GameEvent*) p->data));
	}

      }
      /* free list  */
      p = l;
      while(p != NULL) {
	l = p;
	p = p->next;
	free(l);
      }
      dt -= PHYSICS_RATE;
    }
    break;
  case GAME_PLAY_NETWORK:
#ifdef NETWORK
    updateNet();
    /* broadCast any outstanding events (turns, etc) */
    for(p = &(game2->events); p->next != NULL; p = p->next) {
      sendNetEvent((GameEvent*) p->data);
    }
#endif
    /* fall through to GAME_PLAY */
  case GAME_PLAY:
    getEvents(); 
    l = doMovement(0, game2->time.dt); /* this won't generate new events */
    if(l != NULL) {
      fprintf(stderr, "something is seriously wrong - ignoring events\n");
    }
    break;
  }
    
  doCameraMovement();
  recognizerMovement();

  scripting_RunGC();
  SystemPostRedisplay();

#ifdef LUA_PROFILE
  printf("%d lua calls after idle processing\n", lua_profile);
#endif
  /* fprintf(stderr, "game time: %.3f\n", game2->time.current / 1000.0); */
}

/* create an event and put it into the global event queue */
void createEvent(int player, event_type_e eventType) {
  GameEvent *e;
  list *p = &(game2->events);

  /* move to the end of the event list */
  while (p->next)
    p = p->next;

  /* TODO: handle failed malloc */
  e = (GameEvent*) malloc(sizeof(GameEvent));
  p->data = e;
  p->next = (list*) malloc(sizeof(list));
  p->next->next = NULL;
  e->type = eventType;
  e->x = game->player[player].data->iposx;
  e->y = game->player[player].data->iposy;
  e->player = player;
  e->timestamp = game2->time.current;
}
