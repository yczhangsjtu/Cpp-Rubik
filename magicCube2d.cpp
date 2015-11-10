#include "magicCube.h"

int main(int argc, char **argv)
{
	initialize();
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(ww,hh);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Magic Cube");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(handleKeyEvent);
	glutMouseFunc(handleMouseEvent);
	glutMotionFunc(handleMouseMoveEvent);
	glutIdleFunc(idle);
	init();
	int id = glutCreateMenu(menu);
	glutSetMenu(id);
	glutAddMenuEntry("Recover",0);
	glutAddMenuEntry("Exit",1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutMainLoop();
}
