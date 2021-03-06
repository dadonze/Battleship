// assignment1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <glut.h>
#include <math.h>
#include <time.h>       
int win_W = 1024;
int win_H = 1024;
float win_WF = 1024.0;
float win_HF = 1024.0;

time_t timer;
struct tm curr_time;
void computer_Turn();
void place_Ships_RandomAI();
void place_Ships_RandomHuman();
void checkIfGameEnd(int);

float square_dims = 80.0;
float square_spacing = 82.0;
float ocean_Board_xoffset = -.375;
float ocean_Board_yoffset = -.85;
float target_Board_xoffset = -.375;
float target_Board_yoffset = .1;

short p1_Ocean_Board[10][10];
short p2_Ocean_Board[10][10];
short p1_Target_Board[10][10];
short p2_Target_Board[10][10];

int p1_hit_count = 0;
int p2_hit_count = 0;
int game_Phase = -2;
/*game Phase Description
	-2: Initial/Pior to game
	-1: Setting a Particular Ship
	0: Set-up phase
	1: Player 1 Turn
	2: Player 2(AI) Turn
	99: Game End
*/

void init() {

	if (game_Phase == -2)
	{
		srand(time(NULL));
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				/* For Ocean Boards:
				   0: Empty/Ocean
				   1: Healthy Ship
				   2: Damaged/Sunk Ship
				   3: Where Oppenent Fired Last Turn
				*/
				p1_Ocean_Board[i][j] = 0;
				p2_Ocean_Board[i][j] = 0;
				/* For Target Boards
					0: Not targeted/unkown
					1: Missed target
					2: Hit target
				
				*/
				p1_Target_Board[i][j] = 0;
				p2_Target_Board[i][j] = 0;
			}
		}/*
		p1_Ocean_Board[3][4] = 1;
		p1_Ocean_Board[3][5] = 1;
		p1_Ocean_Board[3][6] = 1;
		p1_Ocean_Board[3][7] = 1;
		p1_Ocean_Board[9][7] = 1;
		p1_Ocean_Board[9][6] = 1;
		p1_Ocean_Board[9][5] = 1;
		p1_Ocean_Board[6][5] = 1;
		p1_Ocean_Board[5][5] = 1;
		p1_Ocean_Board[1][1] = 1;
		p1_Ocean_Board[1][2] = 1;
		p1_Ocean_Board[1][3] = 1;
		p1_Ocean_Board[1][4] = 1;
		p1_Ocean_Board[1][5] = 1;
		p1_Ocean_Board[2][10] = 1;
		p1_Ocean_Board[3][10] = 1;
		p1_Ocean_Board[4][10] = 1;
		p1_Ocean_Board[5][10] = 1;
		*/
		place_Ships_RandomHuman();
		place_Ships_RandomAI();
		game_Phase = 1;
	}
}

void drawBitmapText(char *string, float x, float y)
{
	char *c;
	glRasterPos2f(x, y);

	for (c = string; *c != ';'; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	}
}

void
reshape(int w, int h)
{

	glViewport(0, 0, w, h);       /* Establish viewing area to cover entire window. */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, 0, h, -1, 1);
	glScalef(1, -1, 1);
	glTranslatef(0, -h, 0);
}

void
display(void)
{

	glClear(GL_COLOR_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	/* Create the display for the ocean board*/
	for (float i = 0.0; i < 10.0; i+= 1.0)
	{
		float xCord = ((square_spacing * i) / win_WF) + ocean_Board_xoffset;
		for (float j = 0.0; j < 10.0; j+= 1.0) {
			switch (p1_Ocean_Board[(int)i][(int)j]) {
			case 3:
				glColor3f(.9, .675, 0.0);
				break;
			case 2:
				glColor3f(.8, .2, .2);
				break;
			case 1:
				glColor3f(.4, .4, .4);
				break;
			case 0:
				glColor3f(.4, .4, 1.0);
				break;
			default:
				glColor3f(0.0, 1.0, 0.0); // green in the event of an error


			}

			glBegin(GL_POLYGON);
			
			float yCord = ((square_spacing * j) / win_HF) + ocean_Board_yoffset;

			glVertex3f(xCord, yCord, 0.0);
			glVertex3f(xCord + (square_dims / win_WF), yCord, 0.0);
			glVertex3f(xCord + (square_dims / win_WF), yCord + (square_dims / win_HF), 0.0);
			glVertex3f(xCord, yCord + (square_dims / win_HF), 0.0);
			
			glEnd();
			

		}
	}
	/*Create the display for the target board*/
	for (float i = 0.0; i < 10.0; i += 1.0)
	{
		float xCord = ((square_spacing * i) / win_WF) + target_Board_xoffset;
		for (float j = 0.0; j < 10.0; j += 1.0) {
			switch (p1_Target_Board[(int)i][(int)j]) {
			case 2:
				glColor3f(.8, .2, .2);
				break;
			case 1:
				glColor3f(.6, .6, .8);
				break;
			case 0:
				glColor3f(.7, .7, .7);
				break;
			default:
				glColor3f(0.0, 1.0, 0.0); // green in the event of an error


			}
			glBegin(GL_POLYGON);
			
			float yCord = ((square_spacing * j) / win_HF) + target_Board_yoffset;

			glVertex3f(xCord, yCord, 0.0);
			glVertex3f(xCord + (square_dims / win_WF), yCord, 0.0);
			glVertex3f(xCord + (square_dims / win_WF), yCord + (square_dims / win_HF), 0.0);
			glVertex3f(xCord, yCord + (square_dims / win_HF), 0.0);

			glEnd();


		}
	}
	
	if (game_Phase == 99) {
		if (p2_hit_count >= 16) {
			glColor3f(.4, .4, .9);
			glScalef(2.0, 2.0, 1.0);
			drawBitmapText("Player Wins!;", -.1, 0);
		}
		else{
			glColor3f(.9, .4, .4);
			glScalef(2.0, 2.0, 1.0);
			drawBitmapText("Computer Wins!;", -.1, 0);}
			
	}




	

	glutSwapBuffers(); // swap the back buffer to front


}

void fireMissle(int xloc, int yloc, int player) {

	if (player == 1)
	{
		if (p2_Ocean_Board[xloc][yloc] == 1) {
			p1_Target_Board[xloc][yloc] = 2;
			p2_Ocean_Board[xloc][yloc] = 2;
			p2_hit_count++;
			if (p2_hit_count >= 15)
			{

				checkIfGameEnd(2);
			}
			
		}
		else
		{
			p1_Target_Board[xloc][yloc] = 1;
		}
	}
	if (player == 2)
	{
		if (p1_Ocean_Board[xloc][yloc] == 1) {
			p2_Target_Board[xloc][yloc] = 2;
			p1_Ocean_Board[xloc][yloc] = 2;
			p1_hit_count++;
			if (p1_hit_count >= 15)
			{

				checkIfGameEnd(1);
			}
			

		}
		else
		{
			p2_Target_Board[xloc][yloc] = 1;
			p1_Ocean_Board[xloc][yloc] = 3;
		}
	}
}
// ~320 is X left bounds
// ~689 is X right bounds (39 incr + 2 space
// ~51 is Ytarg upper bounds (41 inc)
// ~320 is X left bounds
void mouseButton(int button, int state, int x, int y)
{

	if (game_Phase == 0 && button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{

	}
	if (game_Phase == 1 && button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		int xpos = -1, ypos = -1;
		if (game_Phase != 99) {
			if (x >= 320 && x <= 728)
			{
				xpos = ((x - 320) / 41);
			}
			if (y >= 51 && y <= 461)
			{
				ypos = 9 - (((y - 51) / 41));
			}
			if (xpos != -1 && ypos != -1) {
				if (p1_Target_Board[xpos][ypos] == 0)
				{
					game_Phase = 2;
					fireMissle(xpos, ypos, 1);




				}

			}

		}

	}
	

	if (game_Phase== 2 && button == GLUT_LEFT_BUTTON && state == GLUT_UP) {

		
		if (game_Phase!=99)
		{	game_Phase = 1;

			computer_Turn();
			
		}
		
	
	}

	
	

}
void place_Ships_RandomAI() {
	int ships_Remaining = 5;
	int xseed, yseed, dir,x1,x2,y1,y2;
	while (ships_Remaining > 0) {
		int ship_size = ships_Remaining;
		if (ships_Remaining < 4)
		{
			ship_size++;
		}
		xseed = rand() % 10;
		yseed = rand() % 10;
		dir = rand() % 2;

		x1 = xseed;
		x2 = xseed;
		y1 = yseed;
		y2 = yseed;

		if (p2_Ocean_Board[xseed][yseed] == 0)
		{
			p2_Ocean_Board[xseed][yseed] = 1;
			ship_size--;


			while (ship_size > 0)
			{
				switch (dir)
				{
				case 1:
					if ((y1 + 1) < 10)
					{
						y1++;
						if (p2_Ocean_Board[xseed][y1] == 0)
						{
							
							p2_Ocean_Board[xseed][y1] = 1;
							
							ship_size--;
						}


					}
					if ((y2-1) >= 0 && ship_size > 0)
					{
						y2--;
						if (p2_Ocean_Board[xseed][y2] == 0)
						{	

							
							p2_Ocean_Board[xseed][y2] = 1;
							
							ship_size--;

						}

					}
					break;
				case 0:
					if (x1+1 < 10)
					{
						x1++;
						if (p2_Ocean_Board[x1][yseed] == 0)
						{
							p2_Ocean_Board[x1][yseed] = 1;
							ship_size--;
						}

					}
					if (x2-1 >= 0 && ship_size > 0)
					{	
						x2--;
						if (p2_Ocean_Board[x2][yseed] == 0)
						{
							p2_Ocean_Board[x2][yseed] = 1;
							ship_size--;
						}

					}
					break;
				default:
					break;
				}

			}
			ships_Remaining--;
		}
	}
	

}
void place_Ships_RandomHuman() {
	int ships_Remaining = 5;
	int xseed, yseed, dir, x1, x2, y1, y2;
	while (ships_Remaining > 0) {
		int ship_size = ships_Remaining;
		if (ships_Remaining < 4)
		{
			ship_size++;
		}
		xseed = rand() % 10;
		yseed = rand() % 10;
		dir = rand() % 2;

		x1 = xseed;
		x2 = xseed;
		y1 = yseed;
		y2 = yseed;

		if (p1_Ocean_Board[xseed][yseed] == 0)
		{
			p1_Ocean_Board[xseed][yseed] = 1;
			ship_size--;


			while (ship_size > 0)
			{
				switch (dir)
				{
				case 1:
					if ((y1 + 1) < 10)
					{
						y1++;
						if (p1_Ocean_Board[xseed][y1] == 0)
						{

							p1_Ocean_Board[xseed][y1] = 1;

							ship_size--;
						}


					}
					if ((y2 - 1) >= 0 && ship_size > 0)
					{
						y2--;
						if (p1_Ocean_Board[xseed][y2] == 0)
						{


							p1_Ocean_Board[xseed][y2] = 1;

							ship_size--;

						}

					}
					break;
				case 0:
					if (x1 + 1 < 10)
					{
						x1++;
						if (p1_Ocean_Board[x1][yseed] == 0)
						{
							p1_Ocean_Board[x1][yseed] = 1;
							ship_size--;
						}

					}
					if (x2 - 1 >= 0 && ship_size > 0)
					{
						x2--;
						if (p1_Ocean_Board[x2][yseed] == 0)
						{
							p1_Ocean_Board[x2][yseed] = 1;
							ship_size--;
						}

					}
					break;
				default:
					break;
				}

			}
			ships_Remaining--;
		}
	}


}

void TimeEvent(int time_val)
{
	time(&timer); // get the current date and time from system
	localtime_s(&curr_time, &timer);
	glutPostRedisplay();
	glutTimerFunc(30, TimeEvent, 1);// By using a timed event, your application should run at the same speed on any machine.
}

int
main(int argc, char **argv)
{
	GLenum type;

	glutInit(&argc, argv);

	type = GLUT_DEPTH;
	type |= GLUT_RGB;
	type |= GLUT_DOUBLE;
	glutInitDisplayMode(type);

	time(&timer); // get current date and time
	localtime_s(&curr_time, &timer);

	// set window size and create a window for rendering
	
	glutInitWindowSize(win_H, win_W);
	glutInitWindowPosition(300, 0);
	glutCreateWindow("Battle Ship");

	init();
	glutDisplayFunc(display);
	glutMouseFunc(mouseButton);
	
	//glutReshapeFunc(reshape);

	glutTimerFunc(30, TimeEvent, 1);
	glutMainLoop();
	return 0;
}

void computer_Turn() {
	bool finished = false;
	while (!finished)
	{
		int xtarg = rand() % 10;
		int ytarg = rand() % 10;
		if (p2_Target_Board[xtarg][ytarg] == 0)
		{
			fireMissle(xtarg, ytarg, 2);
			finished = true;
		}
	}
}

void checkIfGameEnd(int player) {

	switch (player)
	{
	case 2:
		for (int i = 0; i < 10; i++) 
		{
				for (int j = 0; j < 10; j++)
			{
				if (p2_Ocean_Board[i][j] == 1)
				{
					return;
				}
			}
		}
		break;
	case 1:
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				if (p1_Ocean_Board[i][j] == 1)
				{
					return;
				}
			}
		}
		break;
	default:
		break;
	}

	game_Phase = 99;

	return;

	

}
