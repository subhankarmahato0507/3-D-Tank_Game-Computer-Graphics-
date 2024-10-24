//3D Tank Game
//Author: Subhankar Mahato && Rakesh Bhunia
// Sri Venkateshwara College Of Engineering, Bangalore

#include <cstdlib>
#include<time.h>
#include <cmath>
#include<cstdio>
#include <GL/glut.h>

float angle = 0.0f;
int hh,ww;
float lx=0.0f,lz=-1.0f;

float x=0.0f, z=5.0f;
float bx=0;
float bz;
float blx,blz;
int cnt=0;

float enemyx[100],enemyz[100];
float enemylx[100],enemylz[100];
float enemyvx[100],enemyvz[100];


bool bullet=false;
float deltaAngle = 0.0f;
float deltaMove = 0;
int xOrigin = -1;
int dead=0;
int enemyno=0;

GLint TIMER_DELAY = 10000;

void changeSize(int w, int h) {
	hh=h;
	ww=w;
	if (h == 0)
		h = 1;

	float ratio =  w * 1.0 / h;

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void drawSnowMan() {

	glColor3f(0.0f, 0.0f, 0.0f);

	glPushMatrix();

	glTranslatef(-0.25f ,0.25f, 0.0f);
	glutSolidCube(0.25);
	glTranslatef(0 ,0.25f, 0.0f);
	glutSolidCube(0.25);

	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.25f ,0.25f, 0.0f);
	glutSolidCube(0.25);
	glTranslatef(0 ,0.25f, 0.0f);
	glutSolidCube(0.25);

	glPopMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);

// Draw Body
	glColor3f(1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f ,1.0f, 0.0f);
	glutSolidCube(1.0f);

	glPushMatrix();
	glTranslatef(-0.75 ,0.5f, 0.0f);
	glPopMatrix();

	glColor3f(0.91f, 0.76f, 0.65f);
// Draw Head
	glTranslatef(0.0f, 0.75f, 0.0f);
	glutSolidCube(0.4);

// Draw Eyes
	glPushMatrix();
	glColor3f(0.0f,0.0f,0.0f);
	glTranslatef(0.05f, 0.10f, 0.18f);
	glutSolidSphere(0.05f,10,10);
	glTranslatef(-0.1f, 0.0f, 0.0f);
	glutSolidSphere(0.05f,10,10);
	glPopMatrix();

// Draw Nose
	glColor3f(1.0f, 0.5f , 0.5f);
	glRotatef(0.0f,1.0f, 0.0f, 0.0f);
	glutSolidCone(0.08f,0.5f,10,2);

}
int gameovertimer=0;
int tmr=0;
void computePos(float deltaMove) {

	x += deltaMove * lx * 0.1f;
	z += deltaMove * lz * 0.1f;

}

void computeDir(float deltaAngle) {

	angle += deltaAngle;
	lx = sin(angle);
	lz = -cos(angle);
}
void renderBitmapString(
		float x,
		float y,
		float z,
		void *font,
		char *string) {

	char *c;
	glRasterPos3f(x, y,z);
	for (c=string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}

void renderStrokeFontString(
		float x,
		float y,
		float z,
		void *font,
		char *string) {

	char *c;
	glPushMatrix();
	glTranslatef(x, y,z);
	glScalef(0.002f, 0.002f, 0.002f);
	for (c=string; *c != '\0'; c++) {
		glutStrokeCharacter(font, *c);
	}
	glPopMatrix();
}

float turn=0;
char s[100];
int w=0;
char girl[100];
void renderScene(void) {
	if (deltaMove)
		computePos(deltaMove);
	if (deltaAngle)
		computeDir(deltaAngle);
	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(	x, 1.0f, z,
			x+lx, 1.0f,  z+lz,
			0.0f, 1.0f,  0.0f);

	glPushMatrix();
	glColor3f(1.0f,1.0f,0.0f);
	glTranslatef(x+lx, 1, z+lz);
	glutSolidSphere(0.005f,20,20);
	glPopMatrix();
	void *font1 = GLUT_STROKE_ROMAN;
	if(dead==36){

		gameovertimer++;
		if(gameovertimer==1000){
			exit(0);
		}
		sprintf(girl,"%s","The End");
		glPushMatrix();
		glColor3f(0,0,0);

		renderStrokeFontString(x+lx*3, 1, z+lz*3, (void *)font1 ,girl);
		glPopMatrix();

	}

	glBegin(GL_QUADS);
		int x=-100;
		int y=0;
		for(int i=0;i<20;i++){
			y=-100;
			for(int j=0;j<20;j++){
				if((i+j)&1){
					glColor3f(0.52, 0.37, 0.26);
				}
				else{
					glColor3f(0.137255 , 0.556863 ,0.137255);
				}
				glVertex3f(x, 0.0f, y);
				glVertex3f(x, 0.0f, y+10);
				glVertex3f( x+10, 0.0f,  y+10);
				glVertex3f( x+10, 0.0f, y);
				y+=10;
			}
			x+=10;
		}


	glEnd();
	turn++;
	if(turn>20) turn=0;
	if(turn==0)
	for(int i=0;i<enemyno;i++){
		enemyx[i]+=enemyvx[i];
		enemyz[i]+=enemyvz[i];
		enemyvx[i]+=(rand()%10-5)*0.01;
		enemyvz[i]+=(rand()%10-5)*0.01;
		if(enemyx[i]<-100) enemyvx[i]=0 , enemyx[i]=-90;
		if(enemyx[i]>100) enemyvx[i]=0 , enemyx[i]=90;

		if(enemyz[i]<-100) enemyvz[i]=0 , enemyz[i]=-90;
		if(enemyz[i]>100) enemyvz[i]=0 , enemyz[i]=90;

	}
	w++;
	if(w>1000){
		w=0;
		for(int i=0;i<enemyno;i++){
			enemyvx[i]=0;
			enemyvz[i]=0;
		}
	}

	if(bullet){

		bx+=blx;
		bz+=blz;

		cnt++;
		if(cnt>100){
			bullet=false;
			cnt=0;
		}
		for(int i=0;i<enemyno;i++){
			if(sqrt((bx-enemyx[i])*(bx-enemyx[i])+0.25*0.25+(bz-enemyz[i] )*(bz-enemyz[i] ) )<0.5+0.75){
           		  for(int j=i;j<enemyno-1;j++){
            		  	enemyx[j]=enemyx[j+1];
		    			enemyz[j]=enemyz[j+1];
           		  }
				   dead++;
				   enemyno--;
				   break;
           	}
		}


//  glFlush();
		glPushMatrix();

		glTranslatef(bx,1,bz);
		glColor3f(1.0, 0.0, 0.0);

		glutSolidSphere(0.2f,20,20);


		glPopMatrix();
	}


		for(int i=0;i<enemyno;i++){

					 glPushMatrix();
                     glTranslatef(enemyx[i],0,enemyz[i]);
					 drawSnowMan();
					 			glColor3f(1.0, 0.0, 0.0);

					 sprintf(girl,"%s","takla Principal");
                     if(i%5==0)
						 renderStrokeFontString(0.0f, 0.5f, 0.0f, (void *)font1 ,girl);
					 sprintf(girl,"%s","Ravi");

					 if(i%5==1)
					 	renderStrokeFontString(0.0f, 0.5f, 0.0f, (void *)font1 ,girl);
					 sprintf(girl,"%s","Subh");

					 if(i%5==2)
					 	renderStrokeFontString(0.0f, 0.5f, 0.0f, (void *)font1 ,girl);
					 sprintf(girl,"%s","Parinita");

					 if(i%5==3)
					 	renderStrokeFontString(0.0f, 0.5f, 0.0f, (void *)font1 ,girl);
					 sprintf(girl,"%s","Sweta");

					 if(i%5==4)
					 	renderStrokeFontString(0.0f, 0.5f, 0.0f, (void *)font1 ,girl);
					 glPopMatrix();
					//printf("%f %f %d\n",enemyx[i],enemyz[i],i);
		}
		// Set projection
		glMatrixMode(GL_PROJECTION);

	// save previous matrix which contains the
	//settings for the perspective projection
	glPushMatrix();

	// reset matrix
	glLoadIdentity();

	// set a 2D orthographic projection
	gluOrtho2D(0, ww, 0, hh);

	// switch back to modelview mode
	glMatrixMode(GL_MODELVIEW);
	void *font= GLUT_BITMAP_8_BY_13;
	glPushMatrix();
	glLoadIdentity();
	sprintf(s,"Score: %d",dead);
	renderBitmapString(30,15,0,font,s);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);

    glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int xx, int yy) {

        if (key == 27)
              exit(0);
        if(key==' '){
        	bullet=true;bx=x;bz=z;blx=lx;blz=lz;
        }
}

void releaseKey(int key, int x, int y) {

	switch (key) {
		case GLUT_KEY_LEFT :
		case GLUT_KEY_RIGHT : deltaAngle = 0.0f;break;
		case GLUT_KEY_UP :
		case GLUT_KEY_DOWN : deltaMove = 0;break;
	}
}

void pressKey(int key, int xx, int yy) {

	switch (key) {
		case GLUT_KEY_LEFT : deltaAngle = -0.01f; break;
		case GLUT_KEY_RIGHT : deltaAngle = 0.01f; break;
		case GLUT_KEY_UP : deltaMove = 0.5f; break;
		case GLUT_KEY_DOWN : deltaMove = -0.5f; break;
		case GLUT_KEY_F6:
			// return to default window
			glutLeaveGameMode();
			break;
		case GLUT_KEY_F1:
			glutGameModeString("1024x768:32");
			if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) {
				glutEnterGameMode();
				glutDisplayFunc(renderScene);
				glutReshapeFunc(changeSize);
				glutIdleFunc(renderScene);

				glutIgnoreKeyRepeat(1);
				glutKeyboardFunc(processNormalKeys);
				glutSpecialFunc(pressKey);
				glutSpecialUpFunc(releaseKey);


				glEnable(GL_DEPTH_TEST);
				glEnable(GL_CULL_FACE);
			}
			break;
	}
}

float deltaAngle1;
void mouseMove(int x, int y) {

	// this will only be true when the left button is down
	if (xOrigin >= 0) {

		// update deltaAngle
		deltaAngle1 = (x - xOrigin) * 0.001f;

		// update camera's direction
		lx = sin(angle + deltaAngle1);
		lz = -cos(angle + deltaAngle1);
	}
}

void mouseButton(int button, int state, int x, int y) {

	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON) {

		// when the button is released
		if (state == GLUT_UP) {
			angle += deltaAngle1;
			xOrigin = -1;
		}
		else  {// state = GLUT_DOWN
			xOrigin = x;
		}
	}
}
void myTimer(int id) {
        // reset timer for 10 seconds
}

int main(int argc, char **argv) {
	srand(time(0));
	for(int i = -3; i < 3; i++){
		for(int j=-3; j < 3; j++) {
			enemyx[enemyno]=10.0f*i;
			enemyz[enemyno]=10.0f*j;
			enemyno++;
		}
	}
		//printf("%d",enemyno);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,600);
	glutCreateWindow("Lighthouse3D - GLUT Tutorial");
	glClearColor(0.196078f, 0.6f, 1.0f, 1.0f);
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glutTimerFunc(TIMER_DELAY, myTimer, 0);
	glutMainLoop();

	return 1;
}
