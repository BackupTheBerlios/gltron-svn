#include "gltron.h"

#include "lua.h"
#include "lualib.h"

int c_quitGame(lua_State *L) {
  saveSettings();
  switchCallbacks(&creditsCallbacks);
  return 0;
}

int c_resetGame(lua_State *L) {
  initData();
  return 0;
}

int c_resetScores(lua_State *L) {
  resetScores();
  return 0;
}

int c_resetCamera(lua_State *L) {
  int i;
  int camType;
  Camera *cam;
  Data *data;

  for(i = 0; i < game->players; i++) {
    cam = game->player[i].camera;
    data = game->player[i].data;

    camType = (game->player[i].ai->active == AI_COMPUTER) ? 
      0 : getSettingi("camType");
    initCamera(cam, data, camType);
  }
  return 0;
}

int c_video_restart(lua_State *L) {
  initGameScreen();
  shutdownDisplay( game->screen );
  setupDisplay( game->screen );
  updateCallbacks();
  changeDisplay();
  return 0;
}

int c_update_settings_cache(lua_State *L) {
  updateSettingsCache();
  return 0;
}

int c_update_audio_volume(lua_State *L) { 
#ifdef SOUND
 setMusicVolume(getSettingf("musicVolume"));
 setFxVolume(getSettingf("fxVolume"));
#endif
 return 0;
}

int c_startGame(lua_State *L) { 
  game2->mode = GAME_SINGLE;
  initData();
  changeDisplay();
  switchCallbacks(&pauseCallbacks);
  return 0;
}

int c_reloadTrack(lua_State *L) {
#ifdef SOUND
  reloadTrack();
#endif
  return 0;
}

int c_reloadArtpack(lua_State *L) {
  reloadArt();
  return 0;
}
  
int c_restoreDefaults(lua_State *L) {
  initDefaultSettings();
  c_video_restart(L);
  fprintf(stderr, "loaded default settings\n");
  return 0;
}

int c_configureKeyboard(lua_State *L) {
  switchCallbacks(&configureCallbacks);
  return 0;
}

int c_getKeyName(lua_State *L) {
  int top = lua_gettop(L);
  if(lua_isnumber(L, top)) {
    lua_pushstring(L, SystemGetKeyName( lua_tonumber(L, top) ));
  } else {
    lua_pushstring(L, "error");
  }
  return 1;
}

void init_c_interface(lua_State *L) {
  lua_register(L, "c_quitGame", c_quitGame);
  lua_register(L, "c_resetGame", c_resetGame);
  lua_register(L, "c_resetScores", c_resetScores);
  lua_register(L, "c_resetCamera", c_resetCamera);
  lua_register(L, "c_video_restart", c_video_restart);
  lua_register(L, "c_update_settings_cache", c_update_settings_cache);
  lua_register(L, "c_update_audio_volume", c_update_audio_volume);
  lua_register(L, "c_startGame", c_startGame);
  lua_register(L, "c_reloadTrack", c_reloadTrack);
  lua_register(L, "c_reloadArtpack", c_reloadArtpack);
  lua_register(L, "c_restoreDefaults", c_restoreDefaults);
  lua_register(L, "c_configureKeyboard", c_configureKeyboard);
  lua_register(L, "c_getKeyName", c_getKeyName);
}






