#include<stdio.h>
#include<stdlib.h>
#include<GL/glut.h>
#include<math.h>
#include<stdbool.h>
#include <sys/types.h>
#include <utime.h>

#define PI 3.1415926535
#define TIMER_ID_0 0
#define TIMER_ID_1 1
#define TIMER_ID_2 2
#define TIMER_ID_3 3
#define TIMER_INTERVAL 20


static int window_width, window_height;
static int xFw=0,zDesno=0;
static int animation_ongoing=0;
static int animation_ongoing_W=0,animation_ongoing_D=0,animation_ongoing_A=0;
static int koef=1,j=0;
static int prepreke_x[10];
static int prepreke_y[10];



static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);
static void on_timer(int value);



int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Formula");
    /* inicijalizujemo random vrednosti x i z koordinata za prepreke */
    srand(time(NULL));
    for(j=0;j<10;j++){
        prepreke_x[j]=rand();
        if(prepreke_x[j] < 0 || prepreke_x[j] > 200)
            prepreke_x[j]=prepreke_x[j] % 200;
        prepreke_y[j]=rand();
        if(prepreke_y[j] < 0 || prepreke_y[j] > 200)
            prepreke_y[j]=prepreke_y[j] % 200;
    }
    
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);
    

    glClearColor(0.75, 0.75, 0.75, 0);
    glEnable(GL_DEPTH_TEST);

    glutMainLoop();

    return 0;
}

static void on_keyboard(unsigned char key, int x, int y)
{

	switch(key){
		case 27:
			exit(0);
			break;
		
		case 'w':
		case 'W':{
            if(!animation_ongoing_W)
            {
                animation_ongoing_W=1;
                animation_ongoing_A=0;
                animation_ongoing_D=0;
                glutTimerFunc(50,on_timer,0);
            }
			break;
		}
		
		case 'd':
		case 'D':{
            if(!animation_ongoing_D)
            {
                animation_ongoing_D=1;
                animation_ongoing_W=0;
                animation_ongoing_A=0;
                glutTimerFunc(50,on_timer,1);
            }
			break;
		}
		
		case 'a':
		case 'A':{
            if(!animation_ongoing_A)
            {
                animation_ongoing_A=1;
                animation_ongoing_W=0;
                animation_ongoing_D=0;
                glutTimerFunc(50,on_timer,2);
            }
			break;
		}
		
		case 's':
		case 'S':{
			animation_ongoing_W=0;
			animation_ongoing_A=0;
			animation_ongoing_D=0;
			break;
		}
		/* pritiskom na G ubrzava se kretanje*/
		case 'g':
		case 'G':{
			koef+=1;
            if(koef>3)
                koef=3;
			break;
		}
        /* pritiskom na F usporava se kretanje*/

		case 'f':
		case 'F':{
			koef-=1;
            if(koef<1)
                koef=1;
			break;
		}

	}
}

static void on_reshape(int width, int height)
{
    window_width = width;
    window_height = height;
}

static void on_display(void)
{
    /*praznimo bafere*/
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*postavljamo viewport*/
    glViewport(0, 0, window_width, window_height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(
            60,
            (float)window_width/(float)window_height,
            1, 1000);

    /*podesavamo kameru da prati korisnika*/
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
            -60 +xFw,40,0 + zDesno,
            0+xFw,1, 0+zDesno,
            1,1, 0
        );
    /*koordinantni sistem*/
    glBegin(GL_LINES);
		/* x */
		glColor3f(1, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(50, 0, 0);
		/* y */
		glColor3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 50, 0);
		/* z */
		glColor3f(0, 0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 50);

	glEnd();
    
    /* platforma*/
    glColor3f(0.8,0,0.2);
	glPushMatrix();
		glTranslatef(100,-3,100);
		glScalef(200, 5, 200);
		glutSolidCube(1);
	glPopMatrix();
    
    /*formula*/
    
	glColor3f(0,0.3,0.8);
	glPushMatrix();
		glTranslatef(0+xFw,0,10+zDesno);
		glutSolidCube(2);
	glPopMatrix();
    
    /* finish */
    glColor3f(0,1,0);
    glPushMatrix();
        glTranslatef(195,-3,195);
        glutSolidCube(10);
    glPopMatrix();
    
    /*prepreke*/
    for(j=0;j<10;j++){
        glColor3f(1,1,0);
        glPushMatrix();
            glTranslatef(prepreke_x[j],2,prepreke_y[j]);
            glutSolidCube(6);
        glPopMatrix();
    }
    
}
static void on_timer(int value){
    
    if((value != TIMER_ID_0) && (value != TIMER_ID_1) && (value != TIMER_ID_2))
        return;
    
    
    if(value == TIMER_ID_0 && animation_ongoing_W==1){
        xFw+=1*koef;
        glutPostRedisplay();
        glutTimerFunc(50,on_timer,0);
    }
    
    else if(value == TIMER_ID_1 && animation_ongoing_D==1){
        zDesno+=1*koef;
        glutPostRedisplay();
        glutTimerFunc(50,on_timer,1);
    }

    
    else if(value == TIMER_ID_2 && animation_ongoing_A==1){			
        zDesno-=1*koef;
        glutPostRedisplay();
        glutTimerFunc(50,on_timer,2);
    }
    
    
  
   
   
    if(zDesno < -10 || zDesno > 190 || xFw>200){
        animation_ongoing_W=0;
        animation_ongoing_D=0;
        animation_ongoing_A=0;
        glutInitWindowSize(250, 20);
        glutInitWindowPosition(560, 560);
        glutCreateWindow("Izgubili ste!");
    }
    for(j=0;j<10;j++){
        if(((zDesno +10) > (prepreke_y[j]-3)) && ((zDesno + 10) < (prepreke_y[j]+3)) 
            && (xFw > (prepreke_x[j]-3)) && (xFw < (prepreke_x[j]+3)))
        {
        animation_ongoing_W=0;
        animation_ongoing_D=0;
        animation_ongoing_A=0;
        glutInitWindowSize(250, 20);
        glutInitWindowPosition(560, 560);
        glutCreateWindow("Izgubili ste!");
        }
    }

    if(zDesno>180 && xFw>190){
        animation_ongoing_W=0;
        animation_ongoing_D=0;
        animation_ongoing_A=0;
        glutInitWindowSize(250, 20);
        glutInitWindowPosition(560, 560);
        glutCreateWindow("Pobedili ste!");
    }
    glutSwapBuffers();
}

