#include <glut.h>
#include <windows.h>

VOID Display( VOID )
{
  glClear(GL_COLOR_BUFFER_BIT);
  glutWireTeapot(0.30);
  glFinish();
}

VOID main( VOID )
{
  glutCreateWindow("CGSG30");
  glutDisplayFunc(Display);
  glutMainLoop();
}