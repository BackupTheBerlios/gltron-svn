#include "video/video.h"
#include <math.h>

static float alpha = 0;

const static float rec_scale_factor = 0.25f;

static float xv[] = { 0.5f, 0.3245f, 0.6f, 0.5f, 0.68f, -0.3f };
static float yv[] = { 0.8f, 1.0f, 0.0f, 0.2f, 0.2f, 0.0f };

static float x(void) { return xv[0] * sinf(xv[1] * alpha + xv[2]) - xv[3] * sinf(xv[4] * alpha + xv[5]); }
static float y(void) { return yv[0] * cosf(yv[1] * alpha + yv[2]) - yv[3] * sinf(yv[4] * alpha + yv[5]); }

static float dx(void) { return xv[1] * xv[0] * cosf(xv[1] * alpha + xv[2]) - xv[4] * xv[3] * cosf(xv[4] * alpha + xv[5]); }
static float dy(void) { return - yv[1] * yv[0] * sinf(yv[1] * alpha + yv[2]) - yv[4] * yv[3] * sinf(yv[4] * alpha + yv[5]); }

float getRecognizerAngle(Point *velocity)
{
  float dxval = velocity->x;
  float dyval = velocity->y;
  
  float phi = acosf ( dxval / sqrtf( dxval * dxval + dyval * dyval ) );
  if (dyval < 0) {
    phi = 2 * PI - phi;
  }
  return (phi + PI / 2) * 180 / PI;
}
  
void getRecognizerPositionVelocity(Point *p, Point *v) {
  float max = recognizer->BBox.vSize.v[0] * rec_scale_factor;
  float rec_boundry = game2->rules.grid_size - max;
  p->x = (max + (x() + 1.0f) * rec_boundry) / 2.0f;
  p->y = (max + (y() + 1.0f) * rec_boundry) / 2.0f;
  v->x = dx() * game2->rules.grid_size / 100.f;
  v->y = dy() * game2->rules.grid_size / 100.f;
}

void drawRecognizerShadow(void) {
  float dirx;
  Point p, v;
  /* states */

  glEnable(GL_CULL_FACE);
  if(game2->settingsCache.use_stencil) {
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
    glStencilFunc(GL_GREATER, 1, 1);
    glEnable(GL_BLEND);
    glColor4fv(shadow_color);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  } else {
    glColor3f(0, 0, 0);
    glDisable(GL_BLEND);
  }
  
  /* transformations */
  getRecognizerPositionVelocity(&p, &v);
  dirx = getRecognizerAngle(&v);

  glPushMatrix();
  glMultMatrixf(shadow_matrix);
  glTranslatef( p.x, p.y, RECOGNIZER_HEIGHT);
  glRotatef(dirx, 0, 0, 1); /* up */
  glScalef(rec_scale_factor, rec_scale_factor, rec_scale_factor);
  glEnable(GL_NORMALIZE);

  /* render */

  drawModel(recognizer, TRI_MESH);

  /* restore */

  if(game2->settingsCache.use_stencil)
    glDisable(GL_STENCIL_TEST);

  glDisable(GL_BLEND);
  glDisable(GL_CULL_FACE);
  glPopMatrix();
}

void drawRecognizer(void) {
  float dirx;
  Point p, v;

  glPushMatrix();

  /* transformations */
  getRecognizerPositionVelocity(&p, &v);
  dirx = getRecognizerAngle(&v);

  glTranslatef( p.x, p.y, RECOGNIZER_HEIGHT);
  glRotatef(dirx, 0, 0, 1); /* up */
  
  glScalef(rec_scale_factor, rec_scale_factor, rec_scale_factor); 
  
  glDisable(GL_LIGHT0);
  glDisable(GL_LIGHT1);
  glLightfv(GL_LIGHT2, GL_SPECULAR, rec_spec_color); 
  glEnable(GL_LIGHT2);

  glDisable(GL_BLEND);

  glEnable(GL_CULL_FACE);
  
  if (game2->settingsCache.light_cycles) {
    glEnable(GL_LIGHTING);
  }
  
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(1,1);

  glEnable(GL_NORMALIZE);
  glColor3f(0.0, 0.0, 0.0);
  drawModel(recognizer, TRI_MESH);

  glDisable(GL_POLYGON_OFFSET_FILL);

  glDisable(GL_LIGHT2);
  glEnable(GL_LIGHT1);
  glDisable(GL_LIGHTING);

  glColor3fv(rec_outline_color);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glEnable(GL_BLEND);
  glEnable(GL_LINE_SMOOTH);
  drawModel(recognizer_quad, QUAD_MESH);
  glDisable(GL_LINE_SMOOTH);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glDisable(GL_CULL_FACE);
  
  glPopMatrix();
}  

void doRecognizerMovement(void) {
  alpha += game2->time.dt / 2000.0f;
}

void resetRecognizer(void) {
	alpha = 0;
}




