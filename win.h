#ifndef __WIN_H_
#define __WIN_H_

#include "GL/glut.h"
#include "frame.h"
#include "vec.h"
#include "rt.h"

namespace kart
{
  class win
  {
  private:
    static frame scene;
  public:
    win()
    {
      	int arg = 1;
	const char* arr[] = { "test" };

	glutInit(&arg, const_cast<char**>(arr));
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(2400, 1400);
	glutCreateWindow("LINUX TOP< WINDA GOVNO");
	glutDisplayFunc(Display);
	glutKeyboardFunc(Keyboard);
    glClearColor(0, 0.4, 0.9, 1);
    }
    static void Display()
    {
	glClearColor(0, 0.4, 0.9, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    MainScene.shot.draw();

	glFinish();
	glutSwapBuffers();
	glutPostRedisplay();
    }
    static void Keyboard( unsigned char key, int x, int y )
    {
      if (key == 27)
        exit(0);
    }
    static void Run()
    {
      glutMainLoop();
    }
  };
}

#endif

