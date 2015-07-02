
// Kodowanie LZ77.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

//ZMIENNE
string ciag;
string pusty;
const int roz_buf = 20;  
const int roz_sl = 100; 

string bufor;
string slownik;

int pozycja = 0;
fstream wej, wyj;
int nline = 1;

struct wyjscie
{
	unsigned char p; //pozycja slowa w slowniku
	unsigned char k; //dlugosc slowa
	string s; //nast. symbol w buforze
} wy;

//FUNKCJE
void Init()
{
	bufor.resize(roz_buf);
	slownik.resize(roz_sl);
	pusty.resize(roz_buf);
	fill(pusty.begin(), pusty.end(), NULL);
}
void zapis(void)
{
	wyj << wy.p << wy.k << wy.s;
}
void reset(void)
{
	pozycja = 0;
}
void inicjuj_slownik(void)
{
	getline(wej, ciag);
	for (int i = 0; i < roz_sl; i++)
	{
		slownik[i] = ciag[0];
	}
	pozycja++;
}
void inicjuj_bufor(void)
{
	for (int i = 0; i < roz_buf; i++)
	{
		bufor[i] = ciag[pozycja];
		pozycja++;
		if (pozycja > ciag.length() - 1)
		{
			getline(wej, ciag);
			reset();
		}
	}
	wy.p = 0;
	wy.k = 0;
	wy.s = ciag.substr(0, 1);
}
void przes_lewo_buf(int ile)
{
	int j;
	for (int i = 0; i < ile; i++)
	{
		for (j = 0; j < roz_buf-1; j++)
		{
			bufor[j] = bufor[j + 1];
		}
		bufor[roz_buf - 1] = NULL;
	}
}
void przes_lewo_sl(int ile)
{
	for (int i = 0; i < ile; i++)
	{
		for (int i = 0; i < roz_sl-1; i++)
		{
			slownik[i] = slownik[i + 1];
		}
	}
}
bool bufor_pusty(void)
{
	int licz=0;

	if (bufor.compare(pusty) == 0)
	{
		return true;
	}
	else
	{
		for (int i = 0; bufor[i] == NULL; i++)
			licz++;
		przes_lewo_buf(licz);
	}
	return false;
}
void wyszukaj(void)
{
	int ile = 0, poz = 0, i = 0;
	string temp = bufor;
	for (i = 0; i < roz_buf; ++i)
	{
		poz = slownik.find(temp, 0);
		
		if (poz == -1)
		{
			temp.clear();
			temp = bufor.substr(0, roz_buf - i - 1);
		}
		else
			break;	
	}
	if (poz == -1)
	{
		wy.p = 0;
		wy.k = 0;
		wy.s = bufor.substr(0, 1);
	}
	else
	{
		wy.p = poz;
		if (temp.length() == roz_buf)
		{
			wy.k = temp.length()-1;
			wy.s = bufor.substr(wy.k, 1);
		}
		else
		{
			wy.k = temp.length();
			wy.s = bufor.substr(wy.k, 1);
		}
	}
}
void aktualizuj_slownik()
{
	for (int i = 0; i <= wy.k; i++)
	{
		slownik[roz_sl - (wy.k + 1) + i] = bufor[i];
	}
}
void aktualizuj_bufor()
{
	if (pozycja < ciag.length())
	{
		for (int i = 0; i <= wy.k; i++)
		{
			bufor[roz_buf - (wy.k + 1) + i] = ciag[pozycja];
			pozycja++;
			if (pozycja > ciag.length() - 1) break;
		}
	}
	else
	{
		if (!wej.eof())
		{
			reset(); 
			getline(wej, ciag);
			for (int i = 0; i <= wy.k; i++)
			{
				bufor[roz_buf - (wy.k + 1) + i] = ciag[pozycja];
				pozycja++;
			}
		}
	}
}
void aktualizuj(void)
{
	przes_lewo_sl(wy.k + 1);
	aktualizuj_slownik();
	przes_lewo_buf(wy.k + 1);
	aktualizuj_bufor();
}
void kodowanie(void)
{
	wyszukaj();
	aktualizuj();
}

int main(int argc, _TCHAR* argv[])
{
	Init();
	
	wej.open("we.txt", ios::in);
	wyj.open("wy.txt", ios::out);

	inicjuj_slownik();
	inicjuj_bufor();
	
	cout << "Koduje...\n";
	while (!wej.eof())
	{
			kodowanie();
			zapis();
	}
	while (!bufor_pusty())
	{
		kodowanie();
		zapis();
	}

	wyj.close();
	wej.close();

	return 0;
}
