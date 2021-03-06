#include "video/nebu_renderer_gl.h"
#include "video/nebu_video_system.h"
#include "base/nebu_system.h"

#include "SDL.h"

#include <assert.h>

static SDL_Surface *screen;
static int width = 0;
static int height = 0;
static int flags = 0;
static int video_initialized = 0;
static int window_id = 0;

void nebu_Video_Init(void) {
  if(SDL_Init(SDL_INIT_VIDEO) < 0 ) {
    fprintf(stderr, "Couldn't initialize SDL video: %s\n", SDL_GetError());
    exit(1); /* OK: critical, no visual */
  } else
		video_initialized = 1;
}

void nebu_Video_SetWindowMode(int x, int y, int w, int h) {
  fprintf(stderr, "ignoring (%d,%d) initial window position - feature not implemented\n", x, y);
  width = w;
  height = h;
}

void nebu_Video_GetDimension(int *x, int *y)
{
	*x = width;
	*y = height;
}

void nebu_Video_SetDisplayMode(int f) {
  int bitdepth, zdepth;

  flags = f;
  if(!video_initialized) {
    if(SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
      fprintf(stderr, "[system] can't initialize Video: %s\n", SDL_GetError());
      exit(1); /* OK: critical, no visual */
    }
  }
  if(flags & SYSTEM_DOUBLE)
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1);

  if(flags & SYSTEM_32_BIT) {
    zdepth = 24;
    bitdepth = 32;
  } else {
    zdepth = 16;
    bitdepth = 16;
  }
  if(flags & SYSTEM_ALPHA)
	  SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );
  if(flags & SYSTEM_DEPTH)
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, zdepth);
  if(flags & SYSTEM_STENCIL)
     SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 8);
  else 
     SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 0);
  video_initialized = 1;
  /* FIXME: bitdepth value unused */
}

void SystemSetGamma(float red, float green, float blue) {
  SDL_SetGamma(red, green, blue);
}

int nebu_Video_Create(char *name) {
  assert (window_id == 0);  // only single window allowed for now
  assert (width != 0 && height != 0);

  if( (screen = SDL_SetVideoMode( width, height, 0, 
	  ((flags & SYSTEM_FULLSCREEN) ? SDL_FULLSCREEN : 0) | SDL_OPENGL)) == NULL ) {
    fprintf(stderr, "[system] Couldn't set GL mode: %s\n", SDL_GetError());
    exit(1); /* OK: critical, no visual */
  }
  window_id = 1;

  SDL_WM_SetCaption(name, NULL);
  glewInit();
  if(!GLEW_ARB_multitexture)
  {
	  fprintf(stderr, "multitexturing is not available\n");
	  exit(1);
  }
  fprintf(stderr, "GL vendor: %s\n", glGetString(GL_VENDOR));
  fprintf(stderr, "GL renderer: %s\n", glGetString(GL_RENDERER));
  fprintf(stderr, "GL version: %s\n", glGetString(GL_VERSION));

  {
	int value;
	fprintf(stderr, "Bitdepth:\n");
	SDL_GL_GetAttribute(SDL_GL_RED_SIZE, &value);
	fprintf(stderr, "Red: %d\n", value);
	SDL_GL_GetAttribute(SDL_GL_GREEN_SIZE, &value);
	fprintf(stderr, "Green: %d\n", value);
	SDL_GL_GetAttribute(SDL_GL_BLUE_SIZE, &value);
	fprintf(stderr, "Blue: %d\n", value);
	SDL_GL_GetAttribute(SDL_GL_ALPHA_SIZE, &value);
	fprintf(stderr, "Alpha: %d\n", value);
  }
	

  glClearColor(0,0,0,0);
  glClear(GL_COLOR_BUFFER_BIT);
  nebu_System_SwapBuffers();
  return window_id;
}

void nebu_Video_Destroy(int id) {
  /* quit the video subsytem
	 * otherwise SDL can't create a new context on win32, if the stencil
	 * bits change 
	 */
	/* there used to be some problems (memory leaks, unprober driver unloading)
	 * caused by this, but I can't remember what they where
	 */
  if(id == window_id)
	  SDL_QuitSubSystem(SDL_INIT_VIDEO);
  else
	  assert(0);
  video_initialized = 0;
  window_id = 0;
}

void SystemReshapeFunc(void(*reshape)(int w, int h)) {
	fprintf(stderr, "can't set reshape function (%p) - feature not supported\n", reshape);
}

void nebu_Video_WarpPointer(int x, int y) {
  SDL_WarpMouse( (Uint16)x, (Uint16)y);
}

void nebu_Video_CheckErrors(const char *where) {
  int error;
  error = glGetError();
  if(error != GL_NO_ERROR)
    printf("[glError: %s] - %d\n", where, error);
}
