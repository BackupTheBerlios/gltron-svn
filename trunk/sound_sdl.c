#include "sound.h"
#include "gltron.h"

/* linux only, at the moment */

static int engine_channel = -1;
static Mix_Music *music;

#define NUM_GAME_FX 5
#define NUM_MENU_FX 2

static Mix_Chunk *game_fx[NUM_GAME_FX];
static Mix_Chunk *menu_fx[NUM_MENU_FX];

static char *game_fx_names[] = {
  "game_engine.wav",
  "game_start.wav",
  "game_crash.wav",
  "game_win.wav",
  "game_lose.wav"
};

static char *menu_fx_names[] = {
  "menu_action.wav",
  "menu_highlight.wav"
};

void loadFX() {
  int i;
  char *path;

  for(i = 0; i < NUM_GAME_FX; i++) {
    path = getFullPath(game_fx_names[i]);
    if(path) {
      game_fx[i] = Mix_LoadWAV(path);
      free(path);
    }
  }
  for(i = 0; i < NUM_MENU_FX; i++) {
    path = getFullPath(menu_fx_names[i]);
    if(path) {
      menu_fx[i] = Mix_LoadWAV(path);
      free(path);
    }
  }
}
 
int initSound() {
  /* open the audio device */
  if(Mix_OpenAudio(22050, AUDIO_U16, 1, 1024) < 0) {
    fprintf(stderr, "can't open audio: %s\n", SDL_GetError());
    exit(2);
  }
  loadFX();
  return 0;
}


void shutdownSound() {
  Mix_CloseAudio();
}
  

int loadSound(char *name) {
  music = Mix_LoadMUS(name);
  return 0;
}

int playSound() {
  if( ! Mix_PlayingMusic() )
    Mix_PlayMusic(music, -1);
  /* todo: remove the following once the bug in SDL_mixer is fixed */
  /* we don't want too many references to game objects here */
  setMusicVolume(game->settings->musicVolume);
  return 0;
}

int stopSound() {
  if( Mix_PlayingMusic() )
    Mix_HaltMusic();
  return 0;
}

void soundIdle() {
  /* sdl_mixer uses pthreads, so no work here */
  return;
}

void playGameFX(int fx) {
  /* Mix_PlayChannel(-1, game_fx[fx], 0); */
  fprintf(stderr, "fx on channel %d\n", Mix_PlayChannel(-1, game_fx[fx], 0));
}

void playMenuFX(int fx) {
  Mix_PlayChannel(-1, menu_fx[fx], 0);
}

void playEngine() {
  if(engine_channel == -1) {
    engine_channel = Mix_PlayChannel(-1, game_fx[fx_engine], -1);
    fprintf(stderr, "started engine on channel %d\n", engine_channel);
  }
}

void stopEngine() {
  if(engine_channel != -1) {
    fprintf(stderr, "stopping engine on channel %d\n", engine_channel);
    Mix_HaltChannel(engine_channel);
    engine_channel = -1;
  }
}

void setMusicVolume(float volume) {
  if(volume > 1) volume = 1;
  if(volume < 0) volume = 0;

  Mix_VolumeMusic((int)(volume * 128));
}

void setFxVolume(float volume) {
  if(volume > 1) volume = 1;
  if(volume < 0) volume = 0;

  Mix_Volume(-1, (int)(volume * 128));
}
