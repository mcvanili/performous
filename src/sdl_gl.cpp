#include <sdl_gl.h>
#ifdef USE_OPENGL
void SDL_GL::draw_func (int _width,int _height, unsigned char* surfacedata, unsigned int textureid, GLenum format, int x, int y) {

        glMatrixMode (GL_MODELVIEW);
        glLoadIdentity ();
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glPushMatrix ();

        glBindTexture (GL_TEXTURE_RECTANGLE_ARB, textureid);
        glTexImage2D (GL_TEXTURE_RECTANGLE_ARB,
                                  0,
                                  GL_RGBA,
                                  _width,
                                  _height,
                                  0,
                                  format,
                                  GL_UNSIGNED_BYTE,
                                  surfacedata);

        glBegin (GL_QUADS);
        glTexCoord2f ((GLfloat)x, (GLfloat)y);
        glVertex2f (0, 0);
        glTexCoord2f ((GLfloat) _width, (GLfloat)y);
        glVertex2f (1.0f, 0.0f);
        glTexCoord2f ((GLfloat) _width, (GLfloat) _height);
        glVertex2f (1.0f, 1.0f);
        glTexCoord2f ((GLfloat)x, (GLfloat) _height);
        glVertex2f (0.0f, 1.0f);

        glEnd ();

        glPopMatrix ();
}

void SDL_GL::initTexture (int _width,int _height,unsigned int* textureid, GLenum format) {

	glGenTextures (1, textureid);
        glBindTexture (GL_TEXTURE_RECTANGLE_ARB, *textureid);
        glTexImage2D (GL_TEXTURE_RECTANGLE_ARB,
                                  0,
                                  GL_RGBA,
                                  _width,
                                  _height,
                                  0,
                                  format,
                                  GL_UNSIGNED_BYTE,
                                  NULL);
}

void SDL_GL::freeTexture (unsigned int textureid)
{
        glDeleteTextures (1, &textureid);
}
#endif