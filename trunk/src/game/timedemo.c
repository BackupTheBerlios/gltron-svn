#include "game/gltron.h"

static int startTime = 0;
static int frames = 0;

void idleTimedemo(void) {
	int t;
	int i, j;

	List *p, *l;

	Sound_idle();
	
	game2->time.current += 20;
	
	for(j = 0; j < 2; j++) { // run display at a theoretical 50 Hz
		t = 10; // run game physics at 100 Hz

		game2->time.dt = 10;
		
		for(i = 0; i < game->players; i++) {
			if(game->player[i].ai != NULL) {
				if(game->player[i].ai->active == AI_COMPUTER &&
					 PLAYER_IS_ACTIVE(&game->player[i])) {
					doComputer(i, 0);
				}
			}
		}

		/* process any outstanding events generated by the AI (turns, etc) */
		for(p = &(game2->events); p->next != NULL; p = p->next) {
			if(processEvent((GameEvent*) p->data))
				return;
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
	}
	
	game2->time.dt = 20;
	doCameraMovement();
	doRecognizerMovement();
	scripting_RunGC();
	SystemPostRedisplay();
	frames++;
	game2->time.lastFrame += 20;
}

void keyTimedemo(int state, int key, int x, int y) {
	if(state == SYSTEM_KEYSTATE_UP)
		return;

	if(key == 27)
		SystemExitLoop(RETURN_TIMEDEMO_ABORT);
}

struct {
	float speed;
	int eraseCrashed, grid_size;
} saveRules;

extern int c_resetCamera();

void initTimedemo(void) {
	int i = 0;

	printf("-- initializing timedemo\n");
	
	frames = 0;
	startTime = SystemGetElapsedTime();
	
	tsrand(12313);

	resetRecognizer();
	
	updateSettingsCache();

	// overwrite AI skills & rules in settingsCache
	gSettingsCache.ai_level = 2;
	gSettingsCache.show_ai_status = 0;
	gSettingsCache.show_fps = 0;
	gSettingsCache.camType = CAM_CIRCLE;
	gSettingsCache.show_console = 0;
	
	saveRules.speed = getSettingf("speed");
	saveRules.eraseCrashed = getSettingi("erase_crashed");
	saveRules.grid_size = getSettingi("grid_size");

  setSettingf("speed", 12);
	setSettingi("erase_crashed", 1);
	setSettingi("grid_size", 200);
		
 	game2->mode = GAME_SINGLE;
  initData();
  changeDisplay(-1);

	for(i = 0; i < game->players; i++) {
		game->player[i].ai->active = AI_COMPUTER;
		// set all camera phi values to 0
		game->player[i].camera->movement[CAM_PHI] = PI / 18;
		game->player[i].camera->movement[CAM_CHI] = PI / 3;
	}
	
  SystemHidePointer();
  SystemWarpPointer(MOUSE_ORIG_X, MOUSE_ORIG_Y);
  game2->time.offset = SystemGetElapsedTime() - game2->time.current;
}

void exitTimedemo(void) {
	int dt = SystemGetElapsedTime() - startTime;
	if(dt) {
		displayMessage(TO_STDERR | TO_CONSOLE, 
									 "timedemo FPS: %.2f\n", 
									 (float) frames / dt * 1000.0f);
		// displayMessage(TO_STDERR | TO_CONSOLE, "timedemo FPS: %.2f (%d frames in %f seconds)\n", (float) frames / dt * 1000.0f, frames, dt / 1000.0f);
	}
	else {
		// displayMessage(TO_STDERR | TO_CONSOLE, "dt: %d, frames: %d\n", dt, frames);
		// actually, this would be a good reason to abort with a fatal error
	}

  setSettingf("speed", saveRules.speed);
	setSettingi("eraseCrashed", saveRules.eraseCrashed);
	setSettingi("grid_size", saveRules.grid_size);
}

Callbacks timedemoCallbacks = {
	displayGame, idleTimedemo, keyTimedemo, initTimedemo, exitTimedemo,
	NULL /* mouse button */, NULL /* mouse motion */, "timedemo"
};

