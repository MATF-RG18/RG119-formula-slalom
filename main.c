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
static int xFw=0,zDesno=0; /* koordinate za kretanje po x i z osi */
static int animation_ongoing_W=0,animation_ongoing_D=0,animation_ongoing_A=0;
static int koef=1,j=0;
static int prepreke_x[10];
static int prepreke_y[10];



static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display();
static void on_timer(int value);


/* Deo koda za osvetljenje i postavljanje ekrana uzet je sa vezbi */

int main(int argc, char **argv)
{
    /* Ambijentalna boja svetla. */
    GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1 };

    /* Difuzna boja svetla. */
    GLfloat light_diffuse[] = { 1, 1, 1, 1 };

    /* Spekularna boja svetla. */
    GLfloat light_specular[] = { 1, 1, 1, 1 };

    /* Ambijentalno osvetljenje scene. */
    GLfloat model_ambient[] = { 0.4, 0.4, 0.4, 1 };
    
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
    

    glClearColor(1, 1, 1, 0);
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);

    
    
    glutMainLoop();

    return 0;
}

static void on_keyboard(unsigned char key, int x, int y)
{

	switch(key){
        /*pritiskom na esc izlazimo napolje*/
		case 27:
			exit(0);
			break;
		/* pritiskom na W krecemo se napred */
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
		/* pritiskom na D krecemo se desno */
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
		/* pritiskom na A krecemo se levo */
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
		/* pritiskom na S krecemo se zaustavljamo */
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
    /*postavljamo viewport*/
     glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(
            60,
            (float)window_width/(float)window_height,
            1, 1000);
}

static void on_display(void)
{
    /* pozicija svetla */
    GLfloat light_position[] = { 100, 500, 0, 0 };

    /* koeficijenti refleksija materijala */
    GLfloat no_material[] = { 0, 0, 0, 1 };
    GLfloat material_ambient[] = { 0.2, 0.2, 0.2, 1 };
    GLfloat material_ambient_heterogeneous[] = { 0.8, 0.8, 0.2, 1 };
    GLfloat material_diffuse[] = { 0.9, 0.9, 0.9, 1 };
    GLfloat material_specular[] = { 1, 1, 1, 1 };
    GLfloat no_shininess[] = { 0 };
    GLfloat low_shininess[] = { 5 };
    GLfloat high_shininess[] = { 100 };
    GLfloat material_emission[] = { 0.3, 0.2, 0.2, 0 };

    /*praznimo bafere*/
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    /* podesavamo poziciju odakle dolazi svetlo */
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);


    /*podesavamo kameru da prati korisnika*/
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
            -60 + xFw,40,0 + zDesno,
            0+xFw,1, 0 + zDesno,
            0,1,0
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
	glPushMatrix();
        glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, no_material);
        glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
        glMaterialfv(GL_FRONT, GL_EMISSION, material_emission);
		glTranslatef(100,-3,100);
		glScalef(200, 5, 200);
		glutSolidCube(1);
	glPopMatrix();
    
    /*formula - kocka */
    
	glPushMatrix();
        glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient_heterogeneous);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
        glMaterialfv(GL_FRONT, GL_EMISSION, no_material);
		glTranslatef(0+xFw,0,10+zDesno);
		glutSolidCube(2);
	glPopMatrix();
    
    /* finish */
    glPushMatrix();
        glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient_heterogeneous);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
        glMaterialfv(GL_FRONT, GL_EMISSION, material_emission);
        glTranslatef(195,-3,195);
        glutSolidCube(10);
    glPopMatrix();
    
    /*prepreke*/
    for(j=0;j<10;j++){
        glPushMatrix();
            glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient_heterogeneous);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, no_material);
            glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
            glMaterialfv(GL_FRONT, GL_EMISSION, material_emission);
            glTranslatef(prepreke_x[j],2,prepreke_y[j]);
            glutSolidCube(6);
        glPopMatrix();
    }
        glutSwapBuffers();

    
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
    
    
  
   
   /* ukoliko korisnik sleti sa platforme , gubi */
    if(zDesno < -10 || zDesno > 190 || xFw>200){
        animation_ongoing_W=0;
        animation_ongoing_D=0;
        animation_ongoing_A=0;
        glutInitWindowSize(250, 20);
        glutInitWindowPosition(560, 560);
        glutCreateWindow("Izgubili ste!");
    }
    /* ukoliko korisnik udari u prepreku, gubi */
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
    /* ukoliko korisnik stigne do cilja u gornjem desnom uglu , pobedjuje */
    if(zDesno>180 && xFw>190){
        animation_ongoing_W=0;
        animation_ongoing_D=0;
        animation_ongoing_A=0;
        glutInitWindowSize(250, 20);
        glutInitWindowPosition(560, 560);
        glutCreateWindow("Pobedili ste!");
    }

}

