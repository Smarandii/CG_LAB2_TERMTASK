#include "Render.h"

#include <sstream>
#include <iostream>

#include <windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>

#include "MyOGL.h"

#include "Camera.h"
#include "Light.h"
#include "Primitives.h"

#include "GUItextRectangle.h"

//Настройка материала
GLfloat amb[] = { 0.2, 0.2, 0.1, 1. };
GLfloat dif[] = { 0.4, 0.65, 0.5, 1. };
GLfloat spec[] = { 0.9, 0.8, 0.3, 1. };
GLfloat sh = 0.1f * 256;


double A[] = { 0, 0, 0 };
double B[] = { -4, 4, 0 };
double C[] = { 5, 6, 0 };
double D[] = { 4, 1, 0 };
double E[] = { 9, -3, 0 };
double O[] = { 3, -1, 0 };
double J[] = { 1, -5, 0 };
double K[] = { -3, -4, 0 };

double A1[] = { 0, 0, 2 };
double B1[] = { -4, 4, 2 };
double C1[] = { 5, 6, 2 };
double D1[] = { 4, 1, 2 };
double E1[] = { 9, -3, 2 };
double O1[] = { 3, -1, 2 };
double J1[] = { 1, -5, 2 };
double K1[] = { -3, -4, 2 };

double N[3] = { 0, 0, -0.1 };
double N1[3] = { 0, 0, 0.1 };

double* getVertexNormale(double* vertexA, double* vertexB, double* vertexC, bool reverce = false) {

	double* vectorA = new double[] {
		vertexB[0] - vertexA[0],
			vertexB[1] - vertexA[1],
			vertexB[2] - vertexA[2]
	};
	double* vectorB = new double[] {
		vertexC[0] - vertexA[0],
			vertexC[1] - vertexA[1],
			vertexC[2] - vertexA[2]
	};

	return new double[] {
		vectorA[1] * vectorB[2] - vectorB[1] * vectorA[2],
			-vectorA[0] * vectorB[2] + vectorB[0] * vectorA[2],
			vectorA[0] * vectorB[1] - vectorB[0] * vectorA[1],
	};
}

double* CalculateNorma(double A[], double B[]) {
	double N[3];
	N[0] = A[1] * B[2] - B[1] * A[2]; // Nx
	N[1] = -(A[0] * B[2] - B[0] * A[2]); // Ny
	N[2] = A[0] * B[1] - B[0] * A[1]; // Nz
	double lenght = sqrt(N[0] * N[0] + N[1] * N[1] + N[2] * N[2]);
	N[0] = N[0] / lenght;
	N[1] = N[1] / lenght;
	N[2] = N[2] / lenght;
	return N;
}

double* CalculateVector(double A[], double B[]) { 
	double V[3];

	V[0] = A[0] - B[0];
	V[1] = A[1] - B[1];
	V[2] = A[2] - B[2];


	return V;
}

void Draw2DPrisma(double A[], double B[], double C[], double D[], double E[], double O[], double J[], double K[], double r, double g, double b, double* N)
{

	glColor3d(r, g, b);
	glNormal3dv(N);
	glVertex3dv(A);
	glNormal3dv(N);
	glVertex3dv(B);
	glNormal3dv(N);
	glVertex3dv(C);
	glNormal3dv(N);
	glVertex3dv(A);
	glNormal3dv(N);
	glVertex3dv(C);
	glNormal3dv(N);
	glVertex3dv(D);
	glNormal3dv(N);
	glVertex3dv(A);
	glNormal3dv(N);
	glVertex3dv(D);
	glNormal3dv(N);
	glVertex3dv(E);
	glNormal3dv(N);
	glVertex3dv(A);
	glNormal3dv(N);
	glVertex3dv(O);
	glNormal3dv(N);
	glVertex3dv(J);
	glNormal3dv(N);
	glVertex3dv(A);
	glNormal3dv(N);
	glVertex3dv(K);
	glNormal3dv(N);
	glVertex3dv(J);


}


void DrawWallTriangles(double* A, double* A1, double* B, double* B1, double r, double g, double b) {
	double* N;
	glColor3d(r, g, b);

	N = getVertexNormale(B, A, A1);
	glNormal3dv(N);
	glVertex3dv(A1);
	glNormal3dv(N);
	glVertex3dv(A);
	glNormal3dv(N);
	glVertex3dv(B);

	glColor3d(r - 0.2, g - 0.2, b - 0.2);
	N = getVertexNormale(B1, B, A1);
	glNormal3dv(N);
	glVertex3dv(B1);
	glNormal3dv(N);
	glVertex3dv(B);
	glNormal3dv(N);
	glVertex3dv(A1);
}

bool textureMode = true;
bool lightMode = true;

//класс для настройки камеры
class CustomCamera : public Camera
{
public:
	//дистанция камеры
	double camDist;
	//углы поворота камеры
	double fi1, fi2;

	
	//значния масеры по умолчанию
	CustomCamera()
	{
		camDist = 15;
		fi1 = 1;
		fi2 = 1;
	}

	
	//считает позицию камеры, исходя из углов поворота, вызывается движком
	void SetUpCamera()
	{
		//отвечает за поворот камеры мышкой
		lookPoint.setCoords(0, 0, 0);

		pos.setCoords(camDist*cos(fi2)*cos(fi1),
			camDist*cos(fi2)*sin(fi1),
			camDist*sin(fi2));

		if (cos(fi2) <= 0)
			normal.setCoords(0, 0, -1);
		else
			normal.setCoords(0, 0, 1);

		LookAt();
	}

	void CustomCamera::LookAt()
	{
		//функция настройки камеры
		gluLookAt(pos.X(), pos.Y(), pos.Z(), lookPoint.X(), lookPoint.Y(), lookPoint.Z(), normal.X(), normal.Y(), normal.Z());
	}



}  camera;   //создаем объект камеры


//Класс для настройки света
class CustomLight : public Light
{
public:
	CustomLight()
	{
		//начальная позиция света
		pos = Vector3(1, 1, 3);
	}

	
	//рисует сферу и линии под источником света, вызывается движком
	void  DrawLightGhismo()
	{
		glDisable(GL_LIGHTING);

		
		glColor3d(0.9, 0.8, 0);
		Sphere s;
		s.pos = pos;
		s.scale = s.scale*0.08;
		s.Show();
		
		if (OpenGL::isKeyPressed('G'))
		{
			glColor3d(0, 0, 0);
			//линия от источника света до окружности
			glBegin(GL_LINES);
			glVertex3d(pos.X(), pos.Y(), pos.Z());
			glVertex3d(pos.X(), pos.Y(), 0);
			glEnd();

			//рисуем окруность
			Circle c;
			c.pos.setCoords(pos.X(), pos.Y(), 0);
			c.scale = c.scale*1.5;
			c.Show();
		}

	}

	void SetUpLight()
	{
		GLfloat amb[] = { 0.2, 0.2, 0.2, 0 };
		GLfloat dif[] = { 1.0, 1.0, 1.0, 0 };
		GLfloat spec[] = { .7, .7, .7, 0 };
		GLfloat position[] = { pos.X(), pos.Y(), pos.Z(), 1. };

		// параметры источника света
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		// характеристики излучаемого света
		// фоновое освещение (рассеянный свет)
		glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
		// диффузная составляющая света
		glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
		// зеркально отражаемая составляющая света
		glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

		glEnable(GL_LIGHT0);
	}


} light;  //создаем источник света




//старые координаты мыши
int mouseX = 0, mouseY = 0;

void mouseEvent(OpenGL *ogl, int mX, int mY)
{
	int dx = mouseX - mX;
	int dy = mouseY - mY;
	mouseX = mX;
	mouseY = mY;

	//меняем углы камеры при нажатой левой кнопке мыши
	if (OpenGL::isKeyPressed(VK_RBUTTON))
	{
		camera.fi1 += 0.01*dx;
		camera.fi2 += -0.01*dy;
	}

	
	//двигаем свет по плоскости, в точку где мышь
	if (OpenGL::isKeyPressed('G') && !OpenGL::isKeyPressed(VK_LBUTTON))
	{
		LPPOINT POINT = new tagPOINT();
		GetCursorPos(POINT);
		ScreenToClient(ogl->getHwnd(), POINT);
		POINT->y = ogl->getHeight() - POINT->y;

		Ray r = camera.getLookRay(POINT->x, POINT->y);

		double z = light.pos.Z();

		double k = 0, x = 0, y = 0;
		if (r.direction.Z() == 0)
			k = 0;
		else
			k = (z - r.origin.Z()) / r.direction.Z();

		x = k*r.direction.X() + r.origin.X();
		y = k*r.direction.Y() + r.origin.Y();

		light.pos = Vector3(x, y, z);
	}

	if (OpenGL::isKeyPressed('G') && OpenGL::isKeyPressed(VK_LBUTTON))
	{
		light.pos = light.pos + Vector3(0, 0, 0.02*dy);
	}

	
}

void mouseWheelEvent(OpenGL *ogl, int delta)
{

	if (delta < 0 && camera.camDist <= 1)
		return;
	if (delta > 0 && camera.camDist >= 100)
		return;

	camera.camDist += 0.01*delta;

}

void keyDownEvent(OpenGL *ogl, int key)
{
	if (key == 'L')
	{
		lightMode = !lightMode;
	}

	if (key == 'T')
	{
		textureMode = !textureMode;
	}

	if (key == 'R')
	{
		camera.fi1 = 1;
		camera.fi2 = 1;
		camera.camDist = 15;

		light.pos = Vector3(1, 1, 3);
	}

	if (key == 'F')
	{
		light.pos = camera.pos;
	}
}

void keyUpEvent(OpenGL *ogl, int key)
{
	
}



GLuint texId;

//выполняется перед первым рендером
void initRender(OpenGL *ogl)
{
	//настройка текстур

	//4 байта на хранение пикселя
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	//настройка режима наложения текстур
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//включаем текстуры
	glEnable(GL_TEXTURE_2D);
	

	//массив трехбайтных элементов  (R G B)
	RGBTRIPLE *texarray;

	//массив символов, (высота*ширина*4      4, потомучто   выше, мы указали использовать по 4 байта на пиксель текстуры - R G B A)
	char *texCharArray;
	int texW, texH;
	OpenGL::LoadBMP("texture.bmp", &texW, &texH, &texarray);
	OpenGL::RGBtoChar(texarray, texW, texH, &texCharArray);

	
	
	//генерируем ИД для текстуры
	glGenTextures(1, &texId);
	//биндим айдишник, все что будет происходить с текстурой, будте происходить по этому ИД
	glBindTexture(GL_TEXTURE_2D, texId);

	//загружаем текстуру в видеопямять, в оперативке нам больше  она не нужна
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW, texH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texCharArray);

	//отчистка памяти
	free(texCharArray);
	free(texarray);

	//наводим шмон
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	//камеру и свет привязываем к "движку"
	ogl->mainCamera = &camera;
	ogl->mainLight = &light;

	// нормализация нормалей : их длины будет равна 1
	glEnable(GL_NORMALIZE);

	// устранение ступенчатости для линий
	glEnable(GL_LINE_SMOOTH); 


	//   задать параметры освещения
	//  параметр GL_LIGHT_MODEL_TWO_SIDE - 
	//                0 -  лицевые и изнаночные рисуются одинаково(по умолчанию), 
	//                1 - лицевые и изнаночные обрабатываются разными режимами       
	//                соответственно лицевым и изнаночным свойствам материалов.    
	//  параметр GL_LIGHT_MODEL_AMBIENT - задать фоновое освещение, 
	//                не зависящее от сточников
	// по умолчанию (0.2, 0.2, 0.2, 1.0)

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);

	camera.fi1 = -1.3;
	camera.fi2 = 0.8;
}


void Render(OpenGL *ogl)
{



	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	glEnable(GL_DEPTH_TEST);
	if (textureMode)
		glEnable(GL_TEXTURE_2D);

	if (lightMode)
		glEnable(GL_LIGHTING);


	//альфаналожение
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	

	//фоновая
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	//дифузная
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
	//зеркальная
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec); \
		//размер блика
		glMaterialf(GL_FRONT, GL_SHININESS, sh);

	//чтоб было красиво, без квадратиков (сглаживание освещения)
	glShadeModel(GL_SMOOTH);
	//===================================
	//Прогать тут  

	glBegin(GL_TRIANGLES);
	//bottom
	Draw2DPrisma(A, B, C, D, E, O, J, K, 0.3, 1, 0.9, N); // green
	//top
	Draw2DPrisma(A1, B1, C1, D1, E1, O1, J1, K1, 1, 0, 0, N1); // red

	DrawWallTriangles(A, A1, B, B1, 0.2, 1, 1);
	DrawWallTriangles(B, B1, C, C1, 1, 0.2, 1);
	DrawWallTriangles(C, C1, D, D1, 0.2, 0.2, 0.5);
	DrawWallTriangles(D, D1, E, E1, 0, 0.5, 0.5);
	DrawWallTriangles(E, E1, O, O1, 0.5, 0.5, 0.5);
	DrawWallTriangles(O, O1, J, J1, 0, 0.5, 0.5);
	DrawWallTriangles(J, J1, K, K1, 0, 1, 0);
	DrawWallTriangles(K, K1, A, A1, 1, 0.5, 0.5);
	glEnd();

	//конец рисования


   //Сообщение вверху экрана

	
	glMatrixMode(GL_PROJECTION);	//Делаем активной матрицу проекций. 
	                                //(всек матричные операции, будут ее видоизменять.)
	glPushMatrix();   //сохраняем текущую матрицу проецирования (которая описывает перспективную проекцию) в стек 				    
	glLoadIdentity();	  //Загружаем единичную матрицу
	glOrtho(0, ogl->getWidth(), 0, ogl->getHeight(), 0, 1);	 //врубаем режим ортогональной проекции

	glMatrixMode(GL_MODELVIEW);		//переключаемся на модел-вью матрицу
	glPushMatrix();			  //сохраняем текущую матрицу в стек (положение камеры, фактически)
	glLoadIdentity();		  //сбрасываем ее в дефолт

	glDisable(GL_LIGHTING);



	GuiTextRectangle rec;		   //классик моего авторства для удобной работы с рендером текста.
	rec.setSize(300, 150);
	rec.setPosition(10, ogl->getHeight() - 150 - 10);


	std::stringstream ss;
	ss << "T - вкл/выкл текстур" << std::endl;
	ss << "L - вкл/выкл освещение" << std::endl;
	ss << "F - Свет из камеры" << std::endl;
	ss << "G - двигать свет по горизонтали" << std::endl;
	ss << "G+ЛКМ двигать свет по вертекали" << std::endl;
	ss << "Коорд. света: (" << light.pos.X() << ", " << light.pos.Y() << ", " << light.pos.Z() << ")" << std::endl;
	ss << "Коорд. камеры: (" << camera.pos.X() << ", " << camera.pos.Y() << ", " << camera.pos.Z() << ")" << std::endl;
	ss << "Параметры камеры: R="  << camera.camDist << ", fi1=" << camera.fi1 << ", fi2=" << camera.fi2 << std::endl;
	
	rec.setText(ss.str().c_str());
	rec.Draw();

	glMatrixMode(GL_PROJECTION);	  //восстанавливаем матрицы проекции и модел-вью обратьно из стека.
	glPopMatrix();


	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////