#include "glCanvas.h"
#include "scene_parser.h"
#include "light.h"
#include "camera.h"
#include "group.h"

// Included files for OpenGL Rendering
#include <GL/freeglut.h>

// ========================================================

// STATIC VARIABLES

// A reference to the function that performs the raytracing
// This function will get called from the 'keyboard' routine
void(*GLCanvas::renderFunction)(SceneParser *);

// A pointer to the global SceneParser
SceneParser *GLCanvas::scene;

// State of the mouse cursor
int GLCanvas::mouseButton;
int GLCanvas::mouseX;
int GLCanvas::mouseY;

// ========================================================
// Draw the x-, y-, and z-axes with lighting disabled
// ========================================================

void GLCanvas::drawAxes(void)
{
	glDisable(GL_LIGHTING);
	// Draw the x-axis in red
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(1, 0, 0);
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(1.0, 0.0, 0.0);
	glVertex3f(0.8, 0.07, 0.0);
	glVertex3f(0.8, 0.0, 0.07);
	glVertex3f(0.8, -0.07, 0.0);
	glVertex3f(0.8, 0.0, -0.07);
	glVertex3f(0.8, 0.07, 0.0);
	glEnd();

	// Draw the y-axis in green
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1, 0);
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0, 1.0, 0.0);
	glVertex3f(0.07, 0.8, 0.0);
	glVertex3f(0.0, 0.8, 0.07);
	glVertex3f(-0.07, 0.8, 0.0);
	glVertex3f(0.0, 0.8, -0.07);
	glVertex3f(0.07, 0.8, 0.0);
	glEnd();

	// Draw the z-axis in blue
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1);
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0, 0.0, 1.0);
	glVertex3f(0.07, 0.0, 0.8);
	glVertex3f(0.0, 0.07, 0.8);
	glVertex3f(-0.07, 0.0, 0.8);
	glVertex3f(0.0, -0.07, 0.8);
	glVertex3f(0.07, 0.0, 0.8);
	glEnd();
}

// ========================================================
// Callback for the OpenGL display loop.  To request that
// this function get called, use glutPostRedisplay()
// ========================================================

void GLCanvas::display(void)
{
	// Clear the display buffer
	Vec3f bgColor = scene->getBackgroundColor();
	glClearColor(bgColor.x(), bgColor.y(), bgColor.z(), 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set the camera parameters
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	scene->getCamera()->glPlaceCamera();

	glEnable(GL_DEPTH_TEST);
	drawAxes();  // Remove this line to disable axis display
	glEnable(GL_LIGHTING);

	// Place each of the lights in the scene
	for (int i = 0; i<scene->getNumLights(); i++) {
		scene->getLight(i)->glInit(i);
	}

	// Draw the scene tree
	scene->getGroup()->paint();

	// Swap the back buffer with the front buffer to display
	// the scene
	glutSwapBuffers();
}

// ========================================================
// Callback function for window resize
// ========================================================

void GLCanvas::reshape(int w, int h) {
	// Set the OpenGL viewport to fill the entire window
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	// Set the camera parameters to reflect the changes
	scene->getCamera()->glInit(w, h);
}

// ========================================================
// Callback function for mouse click or release
// ========================================================

void GLCanvas::mouse(int button, int state, int x, int y) {
	// Save the current state of the mouse.  This will be
	// used by the 'motion' function
	mouseButton = button;
	mouseX = x;
	mouseY = y;
}

// ========================================================
// Callback function for mouse drag
// ========================================================

void GLCanvas::motion(int x, int y) {
	// Left button = rotation
	// (rotate camera around the up and horizontal vectors)
	if (mouseButton == GLUT_LEFT_BUTTON) {
		scene->getCamera()->rotateCamera(0.005*(mouseX - x), 0.005*(mouseY - y));
		mouseX = x;
		mouseY = y;
	}
	// Middle button = translation
	// (move camera perpendicular to the direction vector)
	else if (mouseButton == GLUT_MIDDLE_BUTTON) {
		scene->getCamera()->truckCamera((mouseX - x)*0.01, (y - mouseY)*0.01);
		mouseX = x;
		mouseY = y;
	}
	// Right button = zoom
	// (move camera along the direction vector)
	else if (mouseButton == GLUT_RIGHT_BUTTON) {
		scene->getCamera()->dollyCamera((x - mouseX)*0.01);
		mouseX = x;
		mouseY = y;
	}

	// Redraw the scene with the new camera parameters
	glutPostRedisplay();
}

// ========================================================
// Callback function for keyboard events
// ========================================================

void GLCanvas::keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'r':  // 'R' = call the render routine
	case 'R':
		printf("Beginning scene render...\n");
		if (renderFunction) renderFunction(scene);
		printf("   ...done.\n");
		break;
	}
}

// ========================================================
// Initialize all appropriate OpenGL variables, set
// callback functions, and start the main event loop.
// This function will not return but can be terminated
// by calling 'exit(0)'
// ========================================================

void GLCanvas::initialize(SceneParser *_scene, void(*_renderFunction)(SceneParser *)) {
	scene = _scene;
	renderFunction = _renderFunction;

	// Set global lighting parameters
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_NORMALIZE);

	// Ambient light
	Vec3f ambColor = scene->getAmbientLight();
	GLfloat ambArr[4];
	ambArr[0] = ambColor.x();
	ambArr[1] = ambColor.y();
	ambArr[2] = ambColor.z();
	ambArr[3] = 1.0;
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambArr);

	// Set window parameters
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
	glEnable(GL_DEPTH_TEST);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("OpenGL Viewer");

	// Initialize callback functions
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);

	// Enter the main rendering loop
	glutMainLoop();
}

// ========================================================
// ========================================================


