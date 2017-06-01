


// computer graphics1.cpp : Defines the entry point for the application.
//2017.6.1

#include "stdafx.h"
#include "computer graphics1.h"
#include <math.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_COMPUTERGRAPHICS1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_COMPUTERGRAPHICS1));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_COMPUTERGRAPHICS1));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_COMPUTERGRAPHICS1);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

void DDA(HDC hdc, int x1, int y1, int x2, int y2)
{
	double xi = x1, yi = y1;
	int deltax = abs(x2 - x1);
	int deltay = abs(y2 - y1);

	int max = (deltax >= deltay ? deltax : deltay);

	double dx = (x2 - x1) / max;
	double dy = (y2 - y1) / max;

	for (int i = 0; i < max; i++)
	{
		SetPixel(hdc, floor(xi), floor(yi), RGB(0, 0, 0));
		xi += dx;
		yi += dy;
	}
}


// Purpose: draw a line with Bresenham Algorithm
// Comment: in this function, I implement the Bresenham Algorithm with all k(-5, -0.5, 0, 0.5, 5, ...)
//          since the basic algorithm deals with condition when 0<k<1
void Bresenham(HDC hdc, int x1, int y1, int x2, int y2)
{
	float deltax = (x2 - x1);
	float deltay = (y2 - y1);

	int x = min(x1, x2);
	int y = ((x == x1) ? y1 : y2);
	float k = deltay / deltax;

	if (((k <= 1) && (k >= 0)) || (k == 0))
	{
		deltax = abs(x2 - x1);
		deltay = abs(y2 - y1);
		int e = -deltax;
		for (int i = 0; i < deltax; i++)
		{
			SetPixel(hdc, x, y, RGB(0, 0, 0));
			if (e >= 0)
			{
				y++;
				e = e - 2 * deltax;
			}
			x++;
			e = e + 2 * deltay;
		}
	}

	else if (k > 1)
	{
		deltax = abs(x2 - x1);
		deltay = abs(y2 - y1);
		int e = -deltax;
		for (int i = 0; i < deltay; i++)
		{
			SetPixel(hdc, x, y, RGB(0, 0, 0));
			if (e >= 0)
			{
				x++;
				e = e - 2 * deltay;
			}
			y++;
			e = e + 2 * deltax;
		}
	}

	else if (k >= -1)
	{
		int e = -deltax;
		for (int i = 0; i < abs(deltax); i++)
		{
			SetPixel(hdc, x, y, RGB(0, 0, 0));
			if (e >= 0)
			{
				y--;
				e = e - 2 * deltax;
			}
			x++;
			e = e + 2 * abs(deltay);
		}
	}

	else if (k < -1)
	{
		float e = -deltax;
		for (int i = 0; i < abs(deltay); i++)
		{
			SetPixel(hdc, x, y, RGB(0, 0, 0));
			if (e >= 0)
			{
				x++;
				e = e - 2 * abs(deltay);
			}
			y--;
			e = e + 2 * abs(deltax);
		}
	}
}

// Purpose: draw a circle with sin,cos, given a angle deltat
// Comment: though this function may have plenty of calculations of sin and cos,
//          it can get a smoothy circle than the circle using Bresenham Algorithm
void tyarc(HDC hdc, int xc, int yc, int r)
{
	int x = xc;
	int y = r + yc;
	float deltat = 0.001, pi = 3.14159;

	while (deltat <= 2 * pi)
	{
		SetPixel(hdc, x, y, RGB(0, 0, 0));
		x = xc + r * sin(deltat);
		y = yc + r * cos(deltat);
		deltat += 0.001;
	}
}


//Purpose: draw a circle using Bresenham Algorithm
//Comment: it may get a circle that is not so smoothy
void arc(HDC hdc, int xc, int yc, int r)         // Not that smoothy
{
	int x = xc;
	int y = r + yc;
	int d = 3 - 2 * r;
	while (x < y)
	{
		SetPixel(hdc, x, y, RGB(0, 0, 0));
		SetPixel(hdc, y, x, RGB(0, 0, 0));
		SetPixel(hdc, x, 2 * yc - y, RGB(0, 0, 0));
		SetPixel(hdc, y, 2 * xc - x, RGB(0, 0, 0));
		SetPixel(hdc, 2 * xc - x, y, RGB(0, 0, 0));
		SetPixel(hdc, 2 * xc - y, x, RGB(0, 0, 0));
		SetPixel(hdc, 2 * xc - x, 2 * yc - y, RGB(0, 0, 0));
		SetPixel(hdc, 2 * xc - y, 2 * xc - x, RGB(0, 0, 0));

		if (d < 0)
			d += 4 * x + 6;

		else
		{
			d += 4 * (x - y) + 10;
			y--;
		}
		x++;
	}
	if (x == y)
	{
		SetPixel(hdc, x, y, RGB(0, 0, 0));
	}

}

//Purpose: draw a ellipse 
//Comment: it may get a ellipse that is not so smoothy
void ellipsearc(HDC hdc, int xc, int yc, int a, int b)
{
	double t = 0, dt = 0.00001, pi = 3.1415926;
	int x1 = xc + a, y1 = yc;
	int x2, y2;

	while (t < 2 * pi)
	{
		t += dt;
		x2 = xc + (int)(a * cos(t));
		y2 = yc + (int)(b * sin(t));
		//SetPixel(hdc, x2, y2, RGB(0, 0, 0));
		Bresenham(hdc, x1, y1, x2, y2);
		x1 = x2;
		y1 = y2;
	}
}

//Purpose: bubblesort the array of intersection and draw lines between outer vertexes and inner vertexes
//Comment: the solution is quite simple but I forget to bubblesort y-coordinate while dealing with x-coordinate at first,
//         which generate a confusing figure. (йд.йд||| 
void bubbleandgenerate(HDC hdc, float D[][15], int cnt, int pnt)
{
	float sflt, sflt2;

	for (int i = 0; i < pnt; i++)                //BubbleSort
	{
		for (int j = i + 1; j < pnt; j++)
		{
			if (D[1][i]>D[1][j])
			{
				sflt = D[1][i];               // exchange x
				D[1][i] = D[1][j];
				D[1][j] = sflt;

				sflt2 = D[2][i];                 // exchange y   !!!!
				D[2][i] = D[2][j];
				D[2][j] = sflt2;
			}
		}
	}

	for (int i = 0; i < pnt; i = i + 2)
	{
		Bresenham(hdc, D[1][i], D[2][i], D[1][i + 1], D[2][i + 1]);
	}
}

//Purpose: generate a shadow-fill figure with inner vertexes
//Comment: I wrote the following code according to the given pseudocode 
void shadowfill(HDC hdc, float k, float h)
{
	// e.g. rectangle with a triangle 
	int x[4] = { 200, 400, 400, 200 };
	int y[4] = { 200, 200, 400, 400 };

	int a[3] = { 250, 350, 300 };
	int b[3] = { 350, 350, 250 };

	for (int i = 0; i < 4; i++)
	{
		int tmp1x = x[i];
		int tmp1y = y[i];
		int tmp2x = x[(i + 1) % 4];
		int tmp2y = y[(i + 1) % 4];
		DDA(hdc, tmp1x, tmp1y, tmp2x, tmp2y);
	}

	for (int i = 0; i < 3; i++)
	{
		int tmp1x = a[i];
		int tmp1y = b[i];
		int tmp2x = a[(i + 1) % 3];
		int tmp2y = b[(i + 1) % 3];
		Bresenham(hdc, tmp1x, tmp1y, tmp2x, tmp2y);
	}

	float tmpb1, tmpb2;
	float B[3][10];
	for (int i = 0; i < 4; i++)                 // to find the bound for B[][]
	{
		tmpb1 = y[i] - k*x[i];
		tmpb2 = y[(i + 1) % 4] - k*x[(i + 1) % 4];
		B[1][i] = min(tmpb1, tmpb2);
		B[2][i] = max(tmpb1, tmpb2);
	}

	float Bmin, Bmax;
	float min = B[1][0];
	float max = B[2][0];
	for (int i = 0; i < 3; i++)
	{
		if (B[1][(i + 1)] < min)
			min = B[1][(i + 1) % 4];
		if (B[2][(i + 1)] > max)
			max = B[2][(i + 1) % 4];
	}

	Bmin = min;
	Bmax = max;

	float currentb = Bmin + h / abs(cos(atan(k)));       //currentb 
	float D[3][15];

	while (currentb <= Bmax)
	{
		int cnt = 0;     // number of intersection vertexes

		for (int i = 0; i < 4; i++)             // outer one 
		{
			float bp = y[i] - k*x[i];
			float bq = y[(i + 1) % 4] - k*x[(i + 1) % 4];

			if (currentb >= min(bp, bq) && currentb <= max(bp, bq))
			{
				float xl = (x[i] * y[(i + 1) % 4] - y[i] * x[(i + 1) % 4] + currentb*(x[(i + 1) % 4] - x[i])) / (y[(i + 1) % 4] - y[i] - k*(x[(i + 1) % 4] - x[i]));
				float yl = k*xl + currentb;
				D[1][cnt] = xl;
				D[2][cnt++] = yl;
			}
		}

		int pnt = cnt;              // counter for inner intersaction vertex
		for (int i = 0; i < 3; i++)              //inner one
		{
			float bp = b[i] - k*a[i];
			float bq = b[(i + 1) % 3] - k*a[(i + 1) % 3];

			if (currentb >= min(bp, bq) && currentb < max(bp, bq))
			{
				float xl = (a[i] * b[(i + 1) % 3] - b[i] * a[(i + 1) % 3] + currentb*(a[(i + 1) % 3] - a[i])) / (b[(i + 1) % 3] - b[i] - k*(a[(i + 1) % 3] - a[i]));
				float yl = k*xl + currentb;
				D[1][pnt] = xl;
				D[2][pnt++] = yl;
			}
		}

		bubbleandgenerate(hdc, D, cnt, pnt);             // BubbleSort and generate shadow line
		currentb += h / abs(cos(atan(k)));            //increase b
	}
}

//Purpose: generate a figure using boundaryfill algorithm
//Comment: it is quite weired that it causes two different result using debug and release, respectively.
void boundaryfill(HDC hdc, int x1, int y1, int boundarycolor, int newcolor)    // debug  release??
{
	COLORREF colorref = GetPixel(hdc, x1, y1);

	if (colorref != boundarycolor && colorref != newcolor)
	{
		SetPixel(hdc, x1, y1, newcolor);
		boundaryfill(hdc, x1 + 1, y1, boundarycolor, newcolor);
		boundaryfill(hdc, x1 - 1, y1, boundarycolor, newcolor);
		boundaryfill(hdc, x1, y1 + 1, boundarycolor, newcolor);
		boundaryfill(hdc, x1, y1 - 1, boundarycolor, newcolor);
	}
}



//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...

		Bresenham(hdc, 20, 20, 40, 20);              //draw the name, Tao Hou
		Bresenham(hdc, 30, 20, 30, 50);

		tyarc(hdc, 60, 40, 10);
		Bresenham(hdc, 69, 40, 70, 50);

		tyarc(hdc, 90, 40, 10);

		Bresenham(hdc, 120, 20, 120, 50);
		Bresenham(hdc, 120, 35, 140, 35);
		Bresenham(hdc, 140, 20, 140, 50);

		tyarc(hdc, 170, 40, 10);
		
		Bresenham(hdc, 190, 30, 190, 50);
		Bresenham(hdc, 190, 50, 205, 50);
		Bresenham(hdc, 205, 30, 205, 50);

		
		ellipsearc(hdc, 400, 50, 60, 20);         // draw a ellipse

		
		shadowfill(hdc, 9, 5);               //  a rectangle contains a triangle inside


		int x[4] = { 600, 800, 800, 600 };                        //boundaryfillcolor 
		int y[4] = { 200, 200, 400, 400 };                        // e.g. rectangle
		for (int i = 0; i < 4; i++)
		{
			int tmp1x = x[i];
			int tmp1y = y[i];
			int tmp2x = x[(i + 1) % 4];
			int tmp2y = y[(i + 1) % 4];
			DDA(hdc, tmp1x, tmp1y, tmp2x, tmp2y);
		}
		boundaryfill(hdc, (x[0] + x[1]) / 2, (y[2] + y[1]) / 2, RGB(0, 0, 0), RGB(0, 255, 0));
		
		EndPaint(hWnd, &ps);
		break;
	}

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
