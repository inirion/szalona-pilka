#include <windows.h>     //po nawiasie nie ma spacji
#include <vector>
TCHAR AppName[] = TEXT("Szalona pi³ka");
int random(int min, int max)
{
	return min + (rand() / (RAND_MAX / (max - min)));
}
class ball
{
private:
	int _x;
	int _y;
	int _size;
	int _xv;
	int _yv;
	COLORREF _kolor;
public:
	ball() {
		_x = 0;
		_y = 0;
		_size = 0;
		_xv = 0;
		_yv = 0;
		_kolor = RGB(255, 255, 255);
	};
	//Settery
	void Set_x(int x) {
		_x = x;
	};
	void Set_y(int y) {
		_y = y;
	};
	void Set_size(int size) {
		_size = size;
	};
	void Set_xv(int xv) {
		_xv = xv;
	};
	void Set_yv(int yv) {
		_yv = yv;
	};
	void Set_kolor(COLORREF kolor)
	{
		_kolor = kolor;
	}
	//Gettery
	int Get_x() {
		return _x;
	};
	int Get_y() const {
		return _y;
	};
	int Get_size() const {
		return _size;
	};
	int Get_xv() const {
		return _xv;
	};
	int Get_yv()const {
		return _yv;
	};
	COLORREF Get_kolor() {
		return _kolor;
	};

	BOOLEAN InRect(RECT r)
	{
		if (_x >= r.left && _y >= r.top && (_x + _size) < r.right && (_y + _size) < r.bottom) return TRUE;
		else return FALSE;
	};
	BOOLEAN TouchTop(RECT r) {
		if (_y <= r.top)
			return TRUE;
		else return FALSE;
	};
	BOOLEAN TouchBot(RECT r) {
		if ((_y + _size) >= r.bottom)
			return TRUE;
		else return FALSE;
	};
	BOOLEAN TouchLeft(RECT r) {
		if (_x <= r.left)
			return TRUE;
		else return FALSE;
	};
	BOOLEAN TouchRight(RECT r) {
		if ((_x + _size) >= r.right)
			return TRUE;
		else return FALSE;
	};

};

void odbicia(ball &pilka, RECT ClientRect, HWND &hwnd) // sprawdza czy pilka znajduje sie w obszarze klienckim oraz czy koliduje z sciankami;
{
	if (pilka.InRect(ClientRect))
	{
		pilka.Set_x(pilka.Get_x() + pilka.Get_xv());
		pilka.Set_y(pilka.Get_y() + pilka.Get_yv());
	}
	else
	{
		pilka.Set_x(pilka.Get_x() + pilka.Get_xv());
		pilka.Set_y(pilka.Get_y() + pilka.Get_yv());
	}
	if (pilka.TouchBot(ClientRect) == TRUE) {
		pilka.Set_xv(random(-10, 10));
		pilka.Set_yv(random(-10, -1));
	}
	if (pilka.TouchRight(ClientRect) == TRUE) {
		pilka.Set_xv(random(-1, -10));
		pilka.Set_yv(random(-10, 10));
	}
	if (pilka.TouchLeft(ClientRect) == TRUE) {
		pilka.Set_xv(random(1, 10));
		pilka.Set_yv(random(-10, 10));
	}
	if (pilka.TouchTop(ClientRect) == TRUE) {
		pilka.Set_xv(random(-10, 10));
		pilka.Set_yv(random(1, 10));
	}
};
void Pilki_start(int ilosc, std::vector<ball> &pilka)// dodaje pilki do tablicy
{
	for (int i = 0; i < ilosc; i++) {
		pilka.push_back(ball());
		pilka[i].Set_x(random(0, 700));
		pilka[i].Set_y(random(0, 500));
		pilka[i].Set_size(random(10, 100));
		pilka[i].Set_xv(random(-5, 5));
		pilka[i].Set_yv(random(-5, 5));
		pilka[i].Set_kolor(RGB(random(0, 255), random(0, 255), random(0, 255)));
	}
}
void Pilki_dodaj(std::vector<ball> &pilka, int numer)// dodaje pilki do tablicy
{
	pilka.push_back(ball());
	pilka[numer].Set_x(random(0, 700));
	pilka[numer].Set_y(random(0, 500));
	pilka[numer].Set_size(random(1, 100));
	pilka[numer].Set_xv(random(-5, 5));
	pilka[numer].Set_yv(random(-5, 5));
	pilka[numer].Set_kolor(RGB(random(0, 255), random(0, 255), random(0, 255)));
}
BOOLEAN klikniecie(ball &pilka, int x, int y, HDC &hdc) {
	if (GetPixel(hdc, x, y) == pilka.Get_kolor())
	{
		if (x >= pilka.Get_x() && x <= (pilka.Get_x() + pilka.Get_size()) && y >= pilka.Get_y() && y <= (pilka.Get_y() + pilka.Get_size()))
		{
			return TRUE;
		}
	}
	else return FALSE;
};


LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int x = LOWORD(lParam);
	int y = HIWORD(lParam);
	static RECT ClientRect;
	static std::vector <ball> pilka;// tablica pilek
	HBRUSH brush = (HBRUSH)GetStockObject(DC_BRUSH); // brush stockowy
	switch (message)
	{
	case WM_CREATE: {
		Pilki_start(9999, pilka);// dodanie 10 pilek
		SetWindowTextA(hwnd, "LPM - Dodanie kola              PPM - Usuniecie kola");
		SetTimer(hwnd, 1, 10, NULL); // timer co 10 ms
	}break;
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdcMem;
		HBITMAP hbmMem;
		HANDLE hbmOld;
		GetClientRect(hwnd, &ClientRect);
		BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &ClientRect);
		hdcMem = CreateCompatibleDC(ps.hdc);
		hbmMem = CreateCompatibleBitmap(ps.hdc, ClientRect.right, ClientRect.bottom);
		hbmOld = SelectObject(hdcMem, hbmMem);
		FillRect(hdcMem, &ClientRect, (HBRUSH)GetStockObject(WHITE_BRUSH));

		for (int i = 0;i < pilka.size();i++)
		{
			SetDCBrushColor(hdcMem, pilka[i].Get_kolor());
			SelectObject(hdcMem, brush);
			//Ellipse(hdcMem, pilka[i].Get_x(), pilka[i].Get_y(), pilka[i].Get_x() + pilka[i].Get_size(), pilka[i].Get_y() + pilka[i].Get_size());
			Rectangle(hdcMem, pilka[i].Get_x(), pilka[i].Get_y(), pilka[i].Get_x() + pilka[i].Get_size(), pilka[i].Get_y() + pilka[i].Get_size());
			POINT p[3];
			p[0].x = pilka[i].Get_x();
			p[0].y = pilka[i].Get_y();
			p[1].x = pilka[i].Get_x() + pilka[i].Get_size();
			p[1].y = pilka[i].Get_y();
			p[2].x = pilka[i].Get_x() + pilka[i].Get_size() / 2;
			p[2].y = pilka[i].Get_y() + pilka[i].Get_size() / 2;
			//Polygon(hdcMem,p, 3);
		}

		BitBlt(ps.hdc, ClientRect.left, ClientRect.top, ClientRect.right, ClientRect.bottom, hdcMem, 0, 0, SRCCOPY);

		SelectObject(hdcMem, hbmOld);
		DeleteObject(hbmMem);
		DeleteDC(hdcMem);

		EndPaint(hwnd, &ps);
	}break;
	case WM_RBUTTONDOWN: {
		HDC hdc = GetDC(hwnd);
		for (int i = 0; i < pilka.size();i++) {
			if (klikniecie(pilka[i], x, y, hdc) == TRUE) pilka.erase(pilka.begin() + i);
		}
		ReleaseDC(hwnd, hdc);
	}break;
	case WM_LBUTTONDOWN: {
		Pilki_dodaj(pilka, pilka.size());
	}break;
	case WM_TIMER: {

		switch (wParam)
		{
		case 1: {
			for (int i = 0;i < pilka.size();i++)
			{
				odbicia(pilka[i], ClientRect, hwnd); // co 10 ms zmienia polozenie pilki
				pilka[i].Set_kolor(RGB(random(0, 255), random(0, 255), random(0, 255)));
			}
			InvalidateRect(hwnd, NULL, FALSE);
		}break;
		default: break;
		}
	}break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return (DefWindowProc(hwnd, message, wParam, lParam));

	}
	return (0L);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{

	MSG msg;
	WNDCLASS wndclass;
	HWND hwnd;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WindowProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = NULL;
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = AppName;

	if (RegisterClass(&wndclass) == 0)
		return FALSE;

	hwnd = CreateWindow(AppName, AppName, WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);

	if (hwnd == NULL)
		return FALSE;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}