/* Mastermind */

#include <stdlib.h>
#include <time.h>
#include "GL/glut.h"

int cross_X = 0;
int cross_Y = 0;
int VIEWPORT_W = 400;
int	VIEWPORT_H = 800;
int	SIZE_CEIL = 30;	
bool finished = false;
bool won = false;


static char code[4];
static char state[10][8]; 
static float colors[][3] = {
	{0.0, 0.0, 0.0},	//black
	{1.0, 1.0, 0.0},	//col1 (yellow)
	{0.0, 1.0, 0.0},	//col2 (green)
	{0.0, 0.0, 1.0},	//col3 (blue)
	{0.5, 0.0, 0.5},	//col4 (purple)
	{1.0, 0.5, 0.0},	//col5 (orange)
	{0.03, 0.91, 0.87},	//col6 (turquoise)
	{1.0, 0.0, 0.0},	//right position
	{1.0, 1.0, 1.0},	//right color
	{0.0, 0.0, 0.0},	//board color
	{0.29, 0.21, 0.13},	//background
	{0.7, 0.0, 0.5}		//cross_color
};
enum PresetValues
{
	COLOR_BOARD = 9,
	COLOR_BGROUND = 10,
	COLOR_POSITION = 7,
	COLOR_RIGHT = 8,
	COLOR_CROSS = 11,
	SIZE_SUB = 20,
	BOARD_HEIGHT = 10
};


void setColor(int colnum)
{
	glColor3f(colors[colnum][0], colors[colnum][1], colors[colnum][2]);
}

void putPawn(int color)
{
	setColor(color);
	glutSolidSphere(SIZE_CEIL-2, SIZE_SUB, SIZE_SUB);
}

void putSmallPawn(int color)
{	
	setColor(color);
	glutSolidSphere((SIZE_CEIL-2)/4, SIZE_SUB, SIZE_SUB);
}

void putCross()
{   
	glPushMatrix();
	
	setColor(COLOR_CROSS);
	glTranslatef(0, -SIZE_CEIL/3, 0);
	glRotatef(90, -0.5, 0, 0);
	glutWireCone(SIZE_CEIL/2, SIZE_CEIL, SIZE_SUB, SIZE_SUB);
	glRotatef(-90, -0.5, 0, 0);
	glTranslatef(0, SIZE_CEIL*0.75, 0);
	glRotatef(90, 0.5, 0, 0);
	glutWireCone(SIZE_CEIL/2, SIZE_CEIL, SIZE_SUB, SIZE_SUB);

	glPopMatrix();
}

void endgame()
{   
	int s=SIZE_CEIL*7;
	static float a = 0;
	a+=1;
	glRotatef(a, 0, 0, 1);
	glTranslatef(-s/2, -s/2, 0);

	GLfloat vert[][2] = 
	{
		{0,0},
		{s,0},
		{s, s},
		{0,s},

		{s/4,s-s/4},
		{s/2-s/10,s-s/4},
		{s/2-s/10,s-s/3},
		{s/4,s-s/3},

		{s-s/4,s-s/4},
		{s-s/2+s/10,s-s/4},
		{s-s/2+s/10,s-s/3},
		{s-s/4,s-s/3},

		{s/5,won?s/2:s/8},
		{s/4,won?s/3:s/4},
		{s/2,won?s/5:s/2-s/5},
		{s-s/4,won?s/3:s/4},
		{s-s/5,won?s/2:s/8},
	};

	glVertexPointer(2, GL_FLOAT, sizeof(GLfloat)*2, vert); 
	glEnableClientState( GL_VERTEX_ARRAY );
	setColor(5);
	glDrawArrays(GL_QUADS, 0, 4);
	glLineWidth(s/20);
	setColor(2);
	glDrawArrays(GL_QUADS, 4, 8);
	glDrawArrays(GL_LINE_STRIP, 12, 5);
	setColor(0);
	glDrawArrays(GL_LINE_LOOP, 0, 4);
	
}

void lineConfirmed()
{
	
	int s=0;
	bool mask_code[4];
	bool mask_curr[4];
	bool mask_colors[6];

	for (int i=0; i<4; i++) {
		if (state[cross_Y][i]==0) return;
		mask_code[i]=true;
		mask_curr[i]=true;
	}

	for (int i = 0; i < 6; i++)
		mask_colors[i] = true;
	
	for (int i=0; i<4; i++)
		if (code[i]==state[cross_Y][i]) 
		{
			mask_code[i] = mask_curr[i] = mask_colors[code[i]-1] = false;
			state[cross_Y][4+s] = COLOR_POSITION;
			s++;
		}

	for (int i=0; i<4; i++)
		for (int j = 0; j < 4; j++)
			if (mask_curr[i] && mask_code[j] && mask_colors[code[j]-1] && state[cross_Y][i]==code[j]) 
			{
				state[cross_Y][4+s] = COLOR_RIGHT;
				s++;
				mask_code[j] = mask_curr[i] = mask_colors[code[j]-1] = false;
			}
	
	won = true;
	for (int i = 0; i < 4; i++)
	{
		if (state[cross_Y][4+i] != COLOR_POSITION) won = false;
	}
	if (cross_Y==9 || won) finished = true;
	cross_Y++;	
}

void idle()
{
	glutPostRedisplay();
}

void initialize()
{
	int i,j;

	srand( time(NULL));
	
	for (i = 0; i < BOARD_HEIGHT; i++)
		for (j = 0; j < 4*2; j++)
			state[i][j]=0;
	for (i = 0; i < 4; i++)
		code[i] = (rand()%6) + 1;
}

void display()
{
	int i,j;

    glClear(GL_COLOR_BUFFER_BIT);	

	glLoadIdentity();
	glTranslatef(VIEWPORT_W/2-4*SIZE_CEIL, VIEWPORT_H/2-9*SIZE_CEIL, 0);
	
	for ( i = 0; i < BOARD_HEIGHT; i++)
	{
		glPushMatrix();
		for ( j = 0; j < 4; j++)
		{
			putPawn(state[i][j]);
			if (cross_X==j && cross_Y==i) putCross();
			glTranslatef(SIZE_CEIL*2, 0, 0);
		}
		glTranslatef(-SIZE_CEIL/2, -SIZE_CEIL/4, 0);
		for ( j = 0; j < 2; j++)
		{
			putSmallPawn(state[i][2*j+4]);
			glTranslatef(SIZE_CEIL/2, 0, 0);
			putSmallPawn(state[i][2*j+5]);
			glTranslatef(-SIZE_CEIL/2, SIZE_CEIL/2, 0);
		}
		glPopMatrix();
		glTranslatef(0, 2*SIZE_CEIL, 0);
	}
	/* display secret code:
	glLoadIdentity();

	for (i = 0; i < 4; i++)
	{
		putPawn(code[i]);
		glTranslatef(100, 0, 0);
	}*/

	if (finished)
	{
		glLoadIdentity();
		glTranslatef(VIEWPORT_W/2, VIEWPORT_H/2+(cross_Y<7?5*SIZE_CEIL:-SIZE_CEIL), 0);
		endgame();
	}

    glutSwapBuffers();
}

void reshape(int w, int h)
{
	VIEWPORT_W = w;
	VIEWPORT_H = h;
	SIZE_CEIL = (h>2*w?w/10:h/20)-1;
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glOrtho(0.0, (GLdouble) w, 0.0, (GLdouble) h, -100, 100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 27:
	case 'q':
	case 'Q':
        exit(0);
        break;
	case 13:
		lineConfirmed();
	}
	if (key>='1' && key <='6' && !finished) state[cross_Y][cross_X] = key-'1'+1;
}

void keyboard_spec(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_LEFT:
		cross_X = cross_X>0?cross_X-1:3;
		break;
    case GLUT_KEY_RIGHT:
		cross_X = (cross_X + 1) % 4;
		break;
	}	
}

int main(int argc,char *argv[])
{
	initialize();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(VIEWPORT_W, VIEWPORT_H);
    glutCreateWindow(argv[0]);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboard_spec);
	glutIdleFunc(idle);
	glClearColor(colors[COLOR_BGROUND][0], colors[COLOR_BGROUND][1], colors[COLOR_BGROUND][2], 0);
    glutMainLoop();
    return 0;
}