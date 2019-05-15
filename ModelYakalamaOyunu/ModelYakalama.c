#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <glut.h>
#include <windows.h>

#include "tavsan.c"
#include "ari.c"
#include "balik.c"
#include "dinazor.c"
#include "Kopek.c"
#include "Ordek.c"
#include "BITMAP.C"

#define MAX_TEXT_SIZE 233	
#define BUFSIZE 512
#define PUAN_HIZI 10
#define MODEL_HIZI		500		/* ms MODEL_HIZI between updates */

static int saniye = 1000;
static int window;
static int menu_id;
static int value = 0;
int windowWidth = 640;
int windowHeight = 480;
int windowPosX = 100;
int windowPosY = 100;
int fullScreenMode = 1;
int geciciPuan, geciciSayac;
static int puan = 0;
static int sayac = 0;
void *font = GLUT_BITMAP_TIMES_ROMAN_24;
boolean flagPuanVeSayac = 1;
boolean flagDurDevam = 1;
GLuint texture;
static boolean FlagTavsan = 0, FlagAri = 0, FlagBalik = 0, FlagDinazor = 0, FlagKopek = 0, FlagOrdek = 0;
static int tavsanKonum = -5, ariKonum = -5, balikKonum = -5, dinazorKonum = -5, kopekKonum = -5, ordekKonum = -5;
static int ModelNo;
static int R_sure = 0;

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glDepthRange(0.0, 1.0);


}

void RastgeleCikma() {

	ModelNo = (rand() % 6) + (1);

	switch (ModelNo)
	{
	case 1:

		FlagTavsan = 1;
		FlagAri = FlagBalik = FlagDinazor = FlagKopek = FlagOrdek = 0;

		tavsanKonum = -2;
		ariKonum = balikKonum = dinazorKonum = kopekKonum = ordekKonum = -5;

		break;
	case 2:

		FlagAri = 1;
		FlagTavsan = FlagBalik = FlagDinazor = FlagKopek = FlagOrdek = 0;

		ariKonum = -1;
		tavsanKonum = balikKonum = dinazorKonum = kopekKonum = ordekKonum = -5;

		break;
	case 3:

		FlagBalik = 1;
		FlagTavsan = FlagAri = FlagDinazor = FlagKopek = FlagOrdek = 0;

		balikKonum = -1;
		tavsanKonum = ariKonum = dinazorKonum = kopekKonum = ordekKonum = -5;

		break;
	case 4:

		FlagDinazor = 1;
		FlagTavsan = FlagBalik = FlagAri = FlagKopek = FlagOrdek = 0;

		dinazorKonum = -1;
		tavsanKonum = ariKonum = balikKonum = kopekKonum = ordekKonum = -5;

		break;
	case 5:


		FlagKopek = 1;
		FlagTavsan = FlagBalik = FlagDinazor = FlagAri = FlagOrdek = 0;

		kopekKonum = -2;
		tavsanKonum = ariKonum = balikKonum = dinazorKonum = ordekKonum = -5;

		break;
	case 6:

		FlagOrdek = 1;
		FlagTavsan = FlagBalik = FlagDinazor = FlagKopek = FlagAri = 0;

		ordekKonum = -2;
		tavsanKonum = ariKonum = balikKonum = dinazorKonum = kopekKonum = -5;

		break;
	default:
		break;
	}

}

void RastgeleSayac(int value) {
	if (flagPuanVeSayac) {
		R_sure = value + 1000;
		glutTimerFunc(MODEL_HIZI, RastgeleSayac, R_sure);
		RastgeleCikma();
		glutPostRedisplay();
	}
	else {
		glutTimerFunc(MODEL_HIZI, RastgeleSayac, R_sure);
	}
}

void surec(int value) {
	if (flagPuanVeSayac) {
		sayac = value + 1;
		glutTimerFunc(saniye, surec, sayac);
	}
	else {

		glutTimerFunc(saniye, surec, sayac);
	}
}
void puanSayaci(int value) {
	glutPostRedisplay();
	glutTimerFunc(PUAN_HIZI, puanSayaci, puan);
}
int puanArttirma(boolean deger) {
	if (deger) {
		return puan++;
	}
	else return puan;

}

void menu(int num) {
	switch (num) {
	case 0:
		glutDestroyWindow(window);
		exit(0);
		break;
	case 1:
		fullScreenMode = !fullScreenMode;
		if (fullScreenMode) {
			windowPosX = glutGet(GLUT_WINDOW_X);
			windowPosY = glutGet(GLUT_WINDOW_Y);
			windowWidth = glutGet(GLUT_WINDOW_WIDTH);
			windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
			glutFullScreen();
		}
		else {
			glutReshapeWindow(windowWidth, windowHeight);
			glutPositionWindow(windowPosX, windowPosX);
		}
		break;
	case 2:
		if (flagDurDevam) {
			flagPuanVeSayac = 0;
			geciciPuan = puan;
			geciciSayac = sayac;
			saniye = 0;
			glutTimerFunc(saniye, surec, 0);
			flagDurDevam = 0;
			break;
		}
		else {
			flagPuanVeSayac = 1;
			saniye = 1000;
			puan = geciciPuan;
			sayac = geciciSayac;
			glutTimerFunc(saniye, surec, sayac);
			flagDurDevam = 1;
			break;
		}
	}
}

void createMenu(void) {
	menu_id = glutCreateMenu(menu);
	glutAddMenuEntry("Durdur/Devam Et", 2);
	glutAddMenuEntry("Buyut/Kucult", 1);
	glutAddMenuEntry("Cikis", 0);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

}
GLuint textureYukle(const char * filename)
{

	GLuint texture;

	int width, height;

	unsigned char * data;

	FILE * file;

	file = fopen(filename, "rb");

	if (file == NULL) return 0;
	width = 1024;
	height = 512;
	data = (unsigned char *)malloc(width * height * 3);
	fread(data, width * height * 3, 1, file);
	fclose(file);

	for (int i = 0; i < width * height; ++i)
	{
		int index = i * 3;
		unsigned char B, R;
		B = data[index];
		R = data[index + 2];

		data[index] = R;
		data[index + 2] = B;

	}


	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_TEXTURE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);


	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
	free(data);

	return texture;
}

void resmiAyarla(GLuint file,
	float x,
	float y,
	float x1,
	float x2,
	float y1,
	float y2,
	float angle)
{
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glPushMatrix();
	glTranslatef(x, y, -2);
	glRotatef(angle, 0.0, 0.0, 1.0);

	glBindTexture(GL_TEXTURE_2D, file);
	glEnable(GL_TEXTURE_2D);


	glTexCoord2f(0.0, 0.0); glVertex3f(x1, y1, -2);
	glTexCoord2f(0.0, 2.0); glVertex3f(x1, y2, -2);
	glTexCoord2f(2.0, 2.0); glVertex3f(x2, y2, -2);
	glTexCoord2f(2.0, 0.0); glVertex3f(x2, y1, -2);
	glPopMatrix();
}
void render(float x1, float x2, float y1, float y2)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	const double w = glutGet(GLUT_WINDOW_WIDTH);
	const double h = glutGet(GLUT_WINDOW_HEIGHT);
	gluPerspective(45.0, w / h, 0.1, 0);
	glMatrixMode(GL_MODELVIEW);
	glTranslatef(0, 0, -0);
	resmiAyarla(texture, 0, 0, x1, x2, y1, y2, 0);
	glutSwapBuffers();
}
void drawRects(GLenum mode)
{

	if (mode == GL_SELECT)
		glLoadName(1);
	glBegin(GL_QUADS);
	render(1, 3, 4, 8);
	glColor3f(1.0, 1.0, 0.0);
	glVertex3i(1, 4, -2);
	glVertex3i(1, 8, -2);
	glVertex3i(3, 8, -2);
	glVertex3i(3, 4, -2);
	glEnd();
	if (mode == GL_SELECT)
		glLoadName(2);
	glBegin(GL_QUADS);
	render(3, 5, 4, 8);
	glColor3f(0.0, 1.0, 1.0);
	glVertex3i(3, 4, -2);
	glVertex3i(3, 8, -2);
	glVertex3i(5, 8, -2);
	glVertex3i(5, 4, -2);
	glEnd();
	if (mode == GL_SELECT)
		glLoadName(3);
	glBegin(GL_QUADS);
	render(5, 7, 4, 8);
	glColor3f(1.0, 0.0, 1.0);
	glVertex3i(5, 4, -2);
	glVertex3i(5, 8, -2);
	glVertex3i(7, 8, -2);
	glVertex3i(7, 4, -2);
	glEnd();
	if (mode == GL_SELECT)
		glLoadName(4);
	glBegin(GL_QUADS);
	render(1, 3, 0, 4);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3i(1, 0, -2);
	glVertex3i(1, 4, -2);
	glVertex3i(3, 4, -2);
	glVertex3i(3, 0, -2);
	glEnd();
	if (mode == GL_SELECT)
		glLoadName(5);
	glBegin(GL_QUADS);
	render(3, 5, 0, 4);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3i(3, 0, -2);
	glVertex3i(3, 4, -2);
	glVertex3i(5, 4, -2);
	glVertex3i(5, 0, -2);
	glEnd();
	if (mode == GL_SELECT)
		glLoadName(6);
	glBegin(GL_QUADS);
	render(5, 7, 0, 4);
	glColor3f(1.0, 1.0, 1.0);
	glVertex3i(5, 0, -2);
	glVertex3i(5, 4, -2);
	glVertex3i(7, 4, -2);
	glVertex3i(7, 0, -2);
	glEnd();
}

void processHits(GLint hits, GLuint buffer[]) {
	unsigned int i, j;
	GLuint names, *ptr;
	ptr = (GLuint *)buffer;
	for (i = 0; i < hits; i++) {
		names = *ptr;
		ptr += 3;
		for (j = 0; j < names; j++) {
			if (*ptr == 1 && FlagTavsan == 1) {
				puanArttirma(flagPuanVeSayac);
				printf("Tavsan modeline tikladiniz. \n");
			}
			else if (*ptr == 2 && FlagBalik == 1) {
				puanArttirma(flagPuanVeSayac);
				printf("Balik modeline tikladiniz. \n");
			}
			else if (*ptr == 3 && FlagAri == 1) {
				puanArttirma(flagPuanVeSayac);
				printf("Ari modeline tikladiniz. \n");
			}
			else if (*ptr == 4 && FlagDinazor == 1) {
				puanArttirma(flagPuanVeSayac);
				printf("Dinazor modeline tikladiniz. \n");
			}
			else if (*ptr == 5 && FlagKopek == 1) {
				puanArttirma(flagPuanVeSayac);
				printf("Kopek modeline tikladiniz. \n");
			}
			else if (*ptr == 6 && FlagOrdek == 1) {
				puanArttirma(flagPuanVeSayac);
				printf("Ordek modeline tikladiniz. \n");
			}
		}
	}
}

void pickRects(int button, int state, int x, int y)
{
	GLuint selectBuf[BUFSIZE];
	GLint hits;
	GLint viewport[4];
	if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
		return;
	glGetIntegerv(GL_VIEWPORT, viewport);
	glSelectBuffer(BUFSIZE, selectBuf);
	(void)glRenderMode(GL_SELECT);
	glInitNames();
	glPushName(0);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	gluPickMatrix((GLdouble)x, (GLdouble)(viewport[3] - y),
		5.0, 5.0, viewport);
	glOrtho(0.0, 8.0, 0.0, 8.0, -0.5, 2.5);
	drawRects(GL_SELECT);
	glPopMatrix();
	glFlush();
	hits = glRenderMode(GL_RENDER);
	processHits(hits, selectBuf);
}
void yaziYazdir(const char * message, GLfloat x, GLfloat y)
{

	glRasterPos2f(x, y);

	while (*message) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *message++);
	}
}
void Isik() {

	glShadeModel(GL_SMOOTH);

	GLfloat glfLightAmbient[] = { 0, 0, 0, 0 };
	GLfloat glfLightDiffuse[] = { 5, 5, 5, 5 };
	GLfloat glfLightSpecular[] = { 0, 0, 0, 0};

	glLightfv(GL_LIGHT0, GL_AMBIENT, glfLightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, glfLightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, glfLightSpecular);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}


void ModelOlusturma() {


	glPushMatrix();	//1. kisim icin push
	Isik();
	glTranslatef(1.0, 6.0, tavsanKonum);
	tavsan();
	glPopMatrix();

	glPushMatrix();	//2. kisim icin push
	Isik();
	glTranslatef(3.0, 5.0, balikKonum);
	glRotatef(270, 1.0f, 0.0f, 0.0f);
	balik();
	glPopMatrix();

	glPushMatrix();	//3. kisim icin push
	Isik();
	glTranslatef(5.0, 5.0, ariKonum);
	glRotatef(270, 1.0f, 0.0f, 0.0f);
	ari();
	glPopMatrix();

	glPushMatrix();	//4. kisim icin push
	Isik();
	glTranslatef(1.0, 1.0, dinazorKonum);
	glRotatef(270, 1.0f, 0.0f, 0.0f);
	dinazor();
	glPopMatrix();

	glPushMatrix();	//5. kisim icin push
	Isik();
	glTranslatef(3.0, 2.0, kopekKonum);
	kopek();
	glPopMatrix();

	glPushMatrix();	//6. kisim icin push
	Isik();
	glTranslatef(5.0, 1.0, ordekKonum);
	glRotatef(270, 1.0f, 0.0f, 0.0f);
	ordek();
	glPopMatrix();
}




void display(void)
{

	char text[MAX_TEXT_SIZE];
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ModelOlusturma();
	drawRects(GL_RENDER);
	sprintf(text, "Puan : %d ", puan);
	yaziYazdir(text, 7.1, 6.5);
	sprintf(text, "Saniye : %d ", sayac);
	yaziYazdir(text, 7.1, 6);
	yaziYazdir("ESC : Cikis", 7.1, 5.5);
	yaziYazdir("F : Boyut", 7.1, 5);
	yaziYazdir("P : Durdur", 7.1, 4.5);

	glFlush();

}

void reshape(int w, int h)
{

	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 8.0, 0.0, 8.0, -0.5, 2.5);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}



void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		exit(0);
		break;

	case 'p':
	case 'P':
		if (flagDurDevam) {
			flagPuanVeSayac = 0;
			geciciPuan = puan;
			geciciSayac = sayac;
			saniye = 0;
			glutTimerFunc(saniye, surec, 0);
			flagDurDevam = 0;
			break;
		}
		else {
			flagPuanVeSayac = 1;
			saniye = 1000;
			puan = geciciPuan;
			sayac = geciciSayac;
			glutTimerFunc(saniye, surec, sayac);
			flagDurDevam = 1;
			break;
		}

	case 'f':
	case 'F':
		fullScreenMode = !fullScreenMode;
		if (fullScreenMode) {
			windowPosX = glutGet(GLUT_WINDOW_X);
			windowPosY = glutGet(GLUT_WINDOW_Y);
			windowWidth = glutGet(GLUT_WINDOW_WIDTH);
			windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
			glutFullScreen();
		}
		else {
			glutReshapeWindow(windowWidth, windowHeight);
			glutPositionWindow(windowPosX, windowPosX);
		}
		break;
	}
}

int main(int argc, char **argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(windowPosX, windowPosY);
	window = glutCreateWindow("Model Yakalama Oyunu");
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	texture = textureYukle("kutu.bmp");
	glutTimerFunc(MODEL_HIZI, RastgeleSayac, 0);
	glutTimerFunc(saniye, surec, 0);
	glutTimerFunc(PUAN_HIZI, puanSayaci, 0);
	glutFullScreen();
	createMenu();
	init();
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutMouseFunc(pickRects);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}

