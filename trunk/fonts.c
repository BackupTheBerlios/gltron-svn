#include "gltron.h"

char *gameDefault;
void initFonts() {
  char *path;
  FILE *f;
  char buf[100];
  char gamefont[100];
  char guifont[100];
  char *game = NULL, *gui = NULL;

  if(gameFtx != NULL) ftxUnloadFont(gameFtx);
  if(guiFtx != NULL) ftxUnloadFont(guiFtx);

  path = getPath(PATH_DATA, "fonts.txt");
  if(path != NULL) {
    f = fopen(path, "r");
    while(fgets(buf, sizeof(buf), f) != NULL) {
      if(sscanf(buf, "game: %s ", gamefont) == 1)
	game = gamefont;
      else if(sscanf(buf, "menu: %s ", guifont) == 1)
	gui = guifont;
    }
    fclose(f);
    free(path);
  } else {
    fprintf(stderr, "can't load fonts.txt\n");
    exit(1); /* OK: critical, installation corrupt */
  }

  if(game == NULL || gui == NULL) {
    fprintf(stderr, "incomplete font definition in fonts.txt\n");
    exit(1); /* OK: critical, installation corrupt */
  }

  gameFtx = ftxLoadFont(game);
  guiFtx = ftxLoadFont(gui);

  if(gameFtx == NULL) {
    fprintf(stderr, "can't load font %s\n", game);
    exit(1); /* OK: critical, installation corrupt */
  }

  if(guiFtx == NULL) {
    fprintf(stderr, "can't load font %s\n", gui);
    exit(1); /* OK: critical, installation corrupt */
  }
}

void deleteFonts() {
  if(gameFtx != NULL)
    ftxUnloadFont(gameFtx);
  gameFtx = NULL;
  if(guiFtx != NULL)
    ftxUnloadFont(guiFtx);
  guiFtx = NULL;
}
