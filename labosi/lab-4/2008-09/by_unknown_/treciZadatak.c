#include <stdlib.h> 
#include <stdio.h>

#include <GL/glut.h>

#define YELLOWMAT 1
#define BLUEMAT 2
#define WHITEMAT 3

static GLenum spinMode = GL_TRUE;   //"sklopka" za pokretanje ili zaustavljanje animacije

//ove tri varijable kontroliraju stanje i brzinu animacije
static float HourOfDay = 0.0;
static float DayOfYear = 0.0;
static float AnimateIncrement = 24.0/8;  // Oznaka vremena za animaciju(sati), podijelio sam sa 8 jer se ina�e Zemlja
										//	toliko brzo vrti da se ne vidi njena vrtnja.

//prototipi funkcije
static void init(void);
static void Animate(void );
static void Key_up(void );
static void Key_down(void );
static void ResizeWindow(int w, int h);
static void Arrows( int Key, int x, int y );



static void init(void) 
{
// definiranje komponenti lokalnog osvjetljenja pojedinih materijala 
	GLfloat yellowAmbientComp[] = {0.1, 0.1, 0.1, 1.0};
	GLfloat yellowDiffuseComp[] = {0.7, 0.7, 0.0, 1.0};
	GLfloat yellowSpecularComp[] = {1.0, 1.0, 1.0, 1.0};

	GLfloat blueAmbientComp[] = {0.2, 0.2, 0.6, 1.0};
	GLfloat blueDiffuseComp[] = {0.1, 0.1, 0.7, 1.0};
	GLfloat blueSpecularComp[] = {1.0, 1.0, 1.0, 1.0};

	GLfloat whiteAmbientComp[] = {0.7, 0.7, 0.7, 1.0};
	GLfloat whiteDiffuseComp[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat whiteSpecularComp[] = {1.0, 1.0, 1.0, 1.0};

// definiranje karakteristika izvora svjetlosti 
	GLfloat lightSourcePosition[] = {1.0, 0.5, 2.0, 0.0};
	GLfloat lightSourceDirection[] = {0.0, 0.0, 0.0, 0.0};

/* pridjeljivanje svojstava materijalima (koristenjem imena YELLOWMAT
 * moze se pojedinim objektima pridjeliti definirani materijal) 
 */
	

	glNewList(BLUEMAT, GL_COMPILE);
	glMaterialfv(GL_FRONT, GL_AMBIENT, blueAmbientComp);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blueDiffuseComp);
	glMaterialfv(GL_FRONT, GL_SPECULAR, blueSpecularComp);
	glMaterialf(GL_FRONT, GL_SHININESS, 90.0);
	glEndList();

	glNewList(WHITEMAT, GL_COMPILE);
	glMaterialfv(GL_FRONT, GL_AMBIENT, whiteAmbientComp);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, whiteDiffuseComp);
	glMaterialfv(GL_FRONT, GL_SPECULAR, whiteSpecularComp);
	glMaterialf(GL_FRONT, GL_SHININESS, 80.0);
	glEndList();

	glNewList(YELLOWMAT, GL_COMPILE);
	glMaterialfv(GL_FRONT, GL_AMBIENT, yellowAmbientComp);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, yellowDiffuseComp);
	glMaterialfv(GL_FRONT, GL_SPECULAR, yellowSpecularComp);
	glMaterialf(GL_FRONT, GL_SHININESS, 100.0);
	glEndList();

// pridjeljivanje karakteristika izvoru svjetlosti 
	glLightfv(GL_LIGHT0, GL_POSITION, lightSourcePosition);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightSourceDirection);

	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
}




//U funkciji Arrows se hvataju tipke gore i dolje, te se pri hvatanju tipki
//pozivaju funkcije Key_up() i Key_down() koje ubrzavaju ili usporavaju animaciju.
static void Arrows( int Key, int x, int y )
{
	switch ( Key ) {
	case GLUT_KEY_UP:		
		Key_up();
		break;
	case GLUT_KEY_DOWN:
		Key_down();
		break;
	}
}


//Funkcija mouse radi isto �to i funkcija Arrows, samo �ta to radi sa tipkama mi�a
void mouse(int button, int state, int x, int y) 
{
	switch (button) {
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_DOWN)
				spinMode = GL_TRUE;		//postavlja se "sklopka" (spinMode) na ON, pokre�e se animacija (ako je bila OFF)
			break;
		case GLUT_RIGHT_BUTTON:
			if (state == GLUT_DOWN)
				spinMode = GL_FALSE;	//postavlja se "sklopka" (spinMode) na OFF, zaustavlja se animacija, ako je bila ON)
			break;
		default:
			break;
   }
}

static void Key_up(void)
{
    AnimateIncrement *= 2.0;			// udvostru�avam brzinu animacije
}

static void Key_down(void)
{
    AnimateIncrement /= 2.0;			// usporavam brzinu animacije za pola
	
}



//funkcija Animate() zamjenjuje funkcije drawSphere() i display(), jer mi je ovako bolje :)
static void Animate(void)
{
	// �istim prozor
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (spinMode) {
		// ako je uklju�ena animacija, pove�avam HourOfDay, DayOfYear i time mi se animacija okre�e
        HourOfDay += AnimateIncrement;
        DayOfYear += AnimateIncrement/24.0;
        HourOfDay = HourOfDay - ((int)(HourOfDay/24))*24;
        DayOfYear = DayOfYear - ((int)(DayOfYear/365))*365;
	}
	
	// �istim modelview matricu
    glLoadIdentity();

	//odmi�em se za 24 jedinice da vidim cijelu scenu
    glTranslatef ( 0.0, 0.0, -24.0 );

	// rotiram svoj pogled prema gore da vidim scenu iz ljep�e perspektive :)
	// 
	glRotatef( 15.0, 1.0, 0.0, 0.0 );

    // Crtam sunce: prvo dodjeljujem boju, pa crtam, radijusa 6.96
	glCallList (YELLOWMAT);
    glutSolidSphere( 6.96, 115, 115 );

    // Crtam Zemlju
	// Prvo je pozicioniram na odre�enu udaljenost od sunca
	//		Korisim DayOfYear da dinami�ki odre�jujem njenu poziciju
    glRotatef( 360.0*DayOfYear/365.0, 0.0, 1.0, 0.0 );
    glTranslatef( 14.96, 0.0, 0.0 );
    glPushMatrix();						// sa�uvam stanje matrice

	// Drugo: rotiram zemlju oko svoje osi
	//		Koristim HourOfDay za odre�ivanje njegove rotacije
	glRotatef( 360.0*HourOfDay/24.0, 0.0, 1.0, 0.0 );

	// Tre�e: Crtam Zemlju, uzimam BLUEMAT boju, radijus 0.63
	glCallList(BLUEMAT);
	glutSolidSphere( 0.63, 10, 10);
    glPopMatrix();						// skidam stanje matrice sa stoga

	// Crtam Mjesec
	//	Koristim DayOfYear za rotaciju
	//		Translatiram ga u odnosu na Zemlju
   	glRotatef( 360.0*12.0*DayOfYear/365.0, 0.0, 1.0, 0.0 );
    glTranslatef( 1.0, 0.0, 0.0 );
    glCallList (WHITEMAT);
    glutSolidSphere( 0.17, 115, 115 );

	// Iscrtavam, i zamjenjujem spremnike
    glFlush();
    glutSwapBuffers();


	glutPostRedisplay();		// Zahtjevam ponovno crtanje da bi se dobila animacija

}



// ResizeWindow se koristi kad se mijenja veli�ina prozora
static void ResizeWindow(int w, int h)
{
    float aspectRatio;
	h = (h == 0) ? 1 : h;
	w = (w == 0) ? 1 : w;
	glViewport( 0, 0, w, h );	// Viewport koristi cijeli prozor
	aspectRatio = (float)w/(float)h;

	// Matrix mode - projekcija
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 60.0, aspectRatio, 1.0, 30.0 );

	// Izabirem modelView matricu
    glMatrixMode( GL_MODELVIEW );
}


// Main funkcija
// Pozivam sve potrebne funkcije
int main( int argc, char** argv )
{
	fprintf(stdout,"Naputak:\n" );
	fprintf(stdout,"Koristi strelice gore - dolje za upravljanje brzine animacije.\n" );
	fprintf(stdout,"Lijeva tipka misa pokrece animaciju.\n" );
	fprintf(stdout,"Desna tipka misa zaustavlja animaciju.\n" );

	// Koristim dvostruki spremnik za animaciju
	glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH ); //dvostruki spremnik, rgb, z-spremnik

	// Create and position the graphics window
    glutInitWindowPosition( 200, 300 );
    glutInitWindowSize( 900, 540 );
    glutCreateWindow( "Sunce, Zemlja, Mjesec" );

	// inicijaliziram OpenGL;
	init();

	// Zovem funkcije za hvatanje tipki mi�a i strelica
	glutSpecialFunc( Arrows );
	glutMouseFunc(mouse);

	// Zovem funkciju za pode�avanje veli�ine prozora
    glutReshapeFunc( ResizeWindow );

	// Zovem funkciju za crtanje
    glutDisplayFunc( Animate );
	
	// Pokre�em MainLoop. On nikad ne zavr�ava.
	glutMainLoop(  );

    return(0);			// Iako nikad ne�e do�i do ovog koraka, kompajler mi se �ali, pa sam stavio return :)
}
