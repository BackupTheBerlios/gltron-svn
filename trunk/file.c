#include "gltron.h"

/* #ifdef UNIX */
static char *share1 = "/usr/share/games/gltron";
static char *share2 = "/usr/local/share/games/gltron";
/* #endif */

static char* subdir = "data";

char* getFullPath(char *filename) {
  char *path;
  FILE *fp = NULL;
  char *base;

  /* check a few directories for the files and */
  /* return the full path. */
  
  /* check: data sub-directory, GLTRON_HOME, and, for UNIX only: */
  /* /usr/share/games/gltron and /usr/local/share/games/gltron */

  path = malloc(strlen(subdir) + 1 + strlen(filename) + 1);
  sprintf(path, "%s%c%s", subdir, SEPERATOR, filename);

  printf("checking '%s'...", path);
  fp = fopen(path, "r");
  if(fp != NULL) {
    fclose(fp);
	printf("ok\n");
    return path;
  }
  free(path);
  printf("unsuccessful\n");

  base = getenv("GLTRON_HOME");
  if(base != NULL) {
    path = malloc(strlen(base) + 1 + strlen(filename) + 1);
    sprintf(path, "%s%c%s", base, SEPERATOR, filename);

    printf("checking '%s'...", path);
    fp = fopen(path, "r");
    if(fp != NULL) {
      fclose(fp);
	  printf("ok\n");
      return path;
    }
    free(path);
    printf("unsuccessful\n");
  }

  /* #ifdef UNIX */
  path = malloc(strlen(share1) + 1 + strlen(filename) + 1);
  sprintf(path, "%s%c%s", share1, SEPERATOR, filename);

  printf("checking '%s'", path);
  fp = fopen(path, "r");
  if(fp != NULL) {
	printf("ok\n");
    fclose(fp);
    return path;
  }
  free(path);
  printf("unsuccessful\n");

  path = malloc(strlen(share2) + 1 + strlen(filename) + 1);
  sprintf(path, "%s%c%s", share2, SEPERATOR, filename);
  
  printf("checking '%s'", path);
  fp = fopen(path, "r");
  if(fp != NULL) {
    fclose(fp);
	printf("ok\n");
    return path;
  }  
  free(path);
  printf("unsuccessful\n");
  /* #endif */

  return NULL;
}

  
  

  
