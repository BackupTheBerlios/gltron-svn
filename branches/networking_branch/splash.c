#include "gltron.h"

static void drawbg_splash(Splash *splash );

unsigned int splash_textures[1];

Splash*
new_splash( int width, int height,  char *background, int options )
{
  Splash      *splash;
  int         x, y, w;

  //Allocate the splash structure
  splash = ( Splash *) malloc(sizeof(Splash));
  if( splash == NULL )
    {
      fprintf(stderr, "could not allocate memory for splash\n");
      exit(1);
    }

  //init size
  splash->width   = width;
  splash->height  = height;

  //options
  splash->options = options;

  //load background texture
  glGenTextures(1, splash_textures);
  glBindTexture(GL_TEXTURE_2D, splash_textures[0]);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  loadTexture(background, GL_RGB);

  //Create progress things.
  //Finding position of progress bar
  w = (int)((splash->width-20)/100);
  w *=100;
  x = (game->screen->vp_w/2-w/2)+10;
  printf("width:%d, w:%d, x: %d\n", splash->width, w, x);
  if( splash->options & PROGRESS_TOP )
    {
      y = game->screen->vp_h/2+splash->height/2-10;
    } else {
      
      y = 10 + game->screen->vp_h/2-splash->height/2;
    }
  //Creating the progress bar
  splash->wprogress = new_wprogressbar(x, y, w, 3);

  //finding position of progress status
  if( splash->options & PROGRESS_TOP )
    {
      y = game->screen->vp_h/2+splash->height/2-30;
    } else {
      
      y = 30 + game->screen->vp_h/2-splash->height/2;
    }
  //Creating the progress status
   x = (game->screen->vp_w/2-splash->width/2)+10;
  splash->wstatus = new_wprogressstatus(x, y, (splash->width-10)*1.5/12);

  return splash;
}

void
draw_splash( Splash *splash )
{
  drawbg_splash( splash );
  draw_wprogressbar(splash->wprogress);
  draw_wprogressstatus(splash->wstatus);
  SystemSwapBuffers();  
}


static void drawbg_splash(Splash *splash ) {

  checkGLError("splash background start");

  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  rasonly(game->screen);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, splash_textures[0]);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_QUADS);
  
/*   glVertex2f(game->screen->vp_w, 0); */
  
/*   glTexCoord2f(1.0, .75); */
/*   glVertex2f(game->screen->vp_w, game->screen->vp_h); */
  
/*   glTexCoord2f(0.0, .75); */
/*   glVertex2f(0, game->screen->vp_h); */
  glTexCoord2f(0.0, 0.0);
  glVertex2f(game->screen->vp_w/2-splash->width/2, game->screen->vp_h/2-splash->height/2);
  
  glTexCoord2f(1.0, 0.0);
  glVertex2f(game->screen->vp_w/2+splash->width/2, game->screen->vp_h/2-splash->height/2);

  glTexCoord2f(1.0, .75);
  glVertex2f(game->screen->vp_w/2+splash->width/2, game->screen->vp_h/2+splash->height/2);

  glTexCoord2f(0.0, .75);
  glVertex2f(game->screen->vp_w/2-splash->width/2, game->screen->vp_h/2+splash->height/2);
  
  glEnd();

  glDisable(GL_TEXTURE_2D);

}

void
update_splash(Splash *splash, float prog, char *status)
{
  if( prog < 0.0 && prog > 1.0 )
    return;
  update_wprogressbar(splash->wprogress, prog);
  update_wprogressstatus(splash->wstatus, status);
  draw_splash( splash );
}