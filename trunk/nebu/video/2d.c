#include "video/nebu_2d.h"
#include "video/nebu_renderer_gl.h"
#include "video/nebu_video_system.h"
#include <stdlib.h>

nebu_2d* nebu_2d_Create(nebu_Surface* pSurface, int flags) {
	nebu_2d *p2d;
	int source_format, target_format;
	int bpp, y;

	switch(pSurface->format) {
	case NEBU_SURFACE_RGB:
		source_format = GL_RGB;
		target_format = GL_RGB;
		bpp = 24;
		break;
	case NEBU_SURFACE_RGBA:
		source_format = GL_RGBA;
		target_format = GL_RGBA;
		bpp = 32;
		break;
	case NEBU_SURFACE_ALPHA:
		source_format = GL_LUMINANCE;
		target_format = GL_LUMINANCE;
		bpp = 8;
		break;
	default:
		fprintf(stderr, "[nebu_2d] can't handle format %d\n", pSurface->format);
		return NULL;
	}
			
	p2d = (nebu_2d*) malloc(sizeof(nebu_2d));
	p2d->w = pSurface->w;
	p2d->h = pSurface->h;
	p2d->tex_w = 1;
	p2d->tex_h = 1;
	while(p2d->tex_w < p2d->w) p2d->tex_w *= 2;
	while(p2d->tex_h < p2d->h) p2d->tex_h *= 2;

	unsigned char *pixels = 
		(unsigned char*) malloc(p2d->tex_w * p2d->tex_h * bpp / 8);
	
	memset(pixels, 0, p2d->tex_w * p2d->tex_h * bpp / 8);
	for(y = 0; y < p2d->h; y++) {
		memcpy(
					 pixels + y * bpp / 8 * p2d->tex_w,
					 pSurface->data + y * bpp / 8 * p2d->w,
					 bpp / 8 * p2d->w);
	}

	glGenTextures(1, &p2d->tex_id);
	glBindTexture(GL_TEXTURE_2D, p2d->tex_id);
	glTexImage2D(GL_TEXTURE_2D, 0, target_format, p2d->tex_w, p2d->tex_h,
							 0, source_format, GL_UNSIGNED_BYTE, pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	free(pixels);

	return p2d;
}

void nebu_2d_Free(nebu_2d *p2d) {
	glDeleteTextures(1, &p2d->tex_id);
	free(p2d);
}

void nebu_2d_Draw(const nebu_2d *p2d) {
	// draw quad with approritate texcoords
	float vertices[] = { 
		0, 0, 0,
		1, 0, 0,
		1, 1, 0,
		0, 1, 0
	};
	unsigned int indices[] = { 0, 1, 2, 0, 2, 3 };
	float uv[] = { 
		0, 0,
		1, 0,
		1, 1,
		0, 1
	};
	int i;

	for(i = 0; i < 4; i++) {
		vertices[ 3 * i + 0 ] *= p2d->w;
		vertices[ 3 * i + 1 ] *= p2d->h;
		uv[ 2 * i + 0 ] *= (float) p2d->w / (float) p2d->tex_w;
		uv[ 2 * i + 1 ] *= (float) p2d->h / (float) p2d->tex_h;
	}

	nebu_Video_CheckErrors("before 2d");
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, uv);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindTexture(GL_TEXTURE_2D, p2d->tex_id);
	glEnable(GL_TEXTURE_2D);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glColor3f(1,1,1);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);
	glDisable(GL_TEXTURE_2D);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	nebu_Video_CheckErrors("after 2d");
}
