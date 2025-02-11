#include<iostream>
#include<fstream>
using namespace std;

struct BFH		//struktura dla danych z naglowka pliku
{
	uint16_t bfType;
	uint32_t bfSize;
	uint16_t bfReserved1;
	uint16_t bfReserved2;
	uint32_t bfOffBits;
};

struct BIH		//struktura dla danych z naglowka obrazu
{
	uint32_t biSize;
	uint32_t biWidth;
	uint32_t biHeight;
	uint16_t biPlanes;
	uint16_t biBitCount;
	uint32_t biCompression;
	uint32_t biSizeImage;
	uint32_t biXpelsPerMeter;
	uint32_t biYpelsPerMeter;
	uint32_t biCrlUses;
	uint32_t biCrlImportant;
};

struct BRG	//struktura dla kolorow pikseli
{
	uint8_t B;
	uint8_t R;
	uint8_t G;
};

struct BGR	//pomocnicza struktura dla kolorow pikseli do wykonywania obliczen
{
	int B;
	int R;
	int G;
};

void BFH_odczyt(ifstream& odczyt, BFH& bfh)  //odczytanie naglowka pliku
{
	odczyt.read(reinterpret_cast<char*>(&bfh.bfType), 2);
	odczyt.read(reinterpret_cast<char*>(&bfh.bfSize), 4);
	odczyt.read(reinterpret_cast<char*>(&bfh.bfReserved1), 2);
	odczyt.read(reinterpret_cast<char*>(&bfh.bfReserved2), 2);
	odczyt.read(reinterpret_cast<char*>(&bfh.bfOffBits), 4);
}

void BIH_odczyt(ifstream& odczyt, BIH& bih)  //odczytanie naglowka obrazu
{
	odczyt.read(reinterpret_cast<char*>(&bih.biSize), 4);
	odczyt.read(reinterpret_cast<char*>(&bih.biWidth), 4);
	odczyt.read(reinterpret_cast<char*>(&bih.biHeight), 4);
	odczyt.read(reinterpret_cast<char*>(&bih.biPlanes), 2);
	odczyt.read(reinterpret_cast<char*>(&bih.biBitCount), 2);
	odczyt.read(reinterpret_cast<char*>(&bih.biCompression), 4);
	odczyt.read(reinterpret_cast<char*>(&bih.biSizeImage), 4);
	odczyt.read(reinterpret_cast<char*>(&bih.biXpelsPerMeter), 4);
	odczyt.read(reinterpret_cast<char*>(&bih.biYpelsPerMeter), 4);
	odczyt.read(reinterpret_cast<char*>(&bih.biCrlUses), 4);
	odczyt.read(reinterpret_cast<char*>(&bih.biCrlImportant), 4);
}

void BFH_zapis(ofstream& zapis, BFH& bfh) //zapisanie naglowka pliku
{
	zapis.write(reinterpret_cast<char*>(&bfh.bfType), 2);
	zapis.write(reinterpret_cast<char*>(&bfh.bfSize), 4);
	zapis.write(reinterpret_cast<char*>(&bfh.bfReserved1), 2);
	zapis.write(reinterpret_cast<char*>(&bfh.bfReserved2), 2);
	zapis.write(reinterpret_cast<char*>(&bfh.bfOffBits), 4);
}

void BIH_zapis(ofstream& zapis, BIH& bih)  //zapisanie naglowka obrazu
{
	zapis.write(reinterpret_cast<char*>(&bih.biSize), 4);
	zapis.write(reinterpret_cast<char*>(&bih.biWidth), 4);
	zapis.write(reinterpret_cast<char*>(&bih.biHeight), 4);
	zapis.write(reinterpret_cast<char*>(&bih.biPlanes), 2);
	zapis.write(reinterpret_cast<char*>(&bih.biBitCount), 2);
	zapis.write(reinterpret_cast<char*>(&bih.biCompression), 4);
	zapis.write(reinterpret_cast<char*>(&bih.biSizeImage), 4);
	zapis.write(reinterpret_cast<char*>(&bih.biXpelsPerMeter), 4);
	zapis.write(reinterpret_cast<char*>(&bih.biYpelsPerMeter), 4);
	zapis.write(reinterpret_cast<char*>(&bih.biCrlUses), 4);
	zapis.write(reinterpret_cast<char*>(&bih.biCrlImportant), 4);
}

void tworz_tablice(int& pion, int& poziom, BRG**& obraz, ifstream& odczyt) //tworzy tablice pikseli, przypisuje ich wartosci, dla ramki kolor czarny
{
	for (int i = 0; i < poziom + 2; i++)
	{
		if (i == 0 || i == (poziom + 1))
		{
			for (int j = 0; j < pion + 2; j++)	//poziome czesci ramki
			{
				obraz[i][j].B = 0;
				obraz[i][j].R = 0;
				obraz[i][j].G = 0;
			}
		}
		else
		{
			for (int j = 0; j < pion + 2; j++)
			{
				if (j == 0 || j == (pion + 1))	//pionowe czesci ramki
				{
					obraz[i][j].B = 0;
					obraz[i][j].R = 0;
					obraz[i][j].G = 0;
				}
				else
				{
					odczyt.read(reinterpret_cast<char*>(&obraz[i][j].B), 1);
					odczyt.read(reinterpret_cast<char*>(&obraz[i][j].G), 1);
					odczyt.read(reinterpret_cast<char*>(&obraz[i][j].R), 1);
				}
			}
		}
	}
}

void konwolucja(BRG**& obraz, int& pion, int& poziom, int* macierz) //detekcja krawedzi
{
	BGR** kopia_obraz;
	BGR** suma_obraz;
	kopia_obraz = new BGR * [poziom + 2]; //pomocnicza tablica do chwilowego przechowania wartosci b g r obliczonych jedną maską
	if (pion % 4 == 0)
	{
		for (int i = 0; i < poziom + 2; i++)
		{
			kopia_obraz[i] = new BGR[pion + 2];
		}
	}
	else
	{
		pion = pion + (4 - pion % 4);
		for (int i = 0; i < poziom + 2; i++)
		{
			kopia_obraz[i] = new BGR[pion + 2];
		}
	}

	suma_obraz = new BGR * [poziom + 2]; //pomocnicza tablica do sumowania chwilowych wartosci z tablicy kopia_obraz
	if (pion % 4 == 0)
	{
		for (int i = 0; i < poziom + 2; i++)
		{
			suma_obraz[i] = new BGR[pion + 2];
		}
	}
	else
	{
		pion = pion + (4 - pion % 4);
		for (int i = 0; i < poziom + 2; i++)
		{
			suma_obraz[i] = new BGR[pion + 2];
		}
	}

	for (int i = 0; i < poziom + 2; i++)
	{
		for (int j = 0; j < pion + 2; j++)
		{
			suma_obraz[i][j].B = 0;
			suma_obraz[i][j].G = 0;
			suma_obraz[i][j].R = 0;
		}
	}

	int S[3][3];  //pierwsza maska
	int ilosc_masek = 8;
	S[0][0] = -1;
	S[0][1] = 0;
	S[0][2] = 1;
	S[1][0] = -2;
	S[1][1] = 0;
	S[1][2] = 2;
	S[2][0] = -1;
	S[2][1] = 0;
	S[2][2] = 1;

	if (macierz != nullptr)		//sprawdzenie czy wykonujemy pierwsza czy trzecia funkcje programu
	{
		ilosc_masek = 1;
		S[0][0] = macierz[0];
		S[0][1] = macierz[1];
		S[0][2] = macierz[2];
		S[1][0] = macierz[3];
		S[1][1] = macierz[4];
		S[1][2] = macierz[5];
		S[2][0] = macierz[6];
		S[2][1] = macierz[7];
		S[2][2] = macierz[8];
	}

	for (int s = 0; s < ilosc_masek; s++)	//w przypadku korzystania z pierwszej funkcji programu petla musi przejsc 8 razy, w przypadku trzeciej tylko raz dla konkretnej maski wczytanej z pliku
	{
		for (int i = 1; i < poziom + 1; i++)
		{
			for (int j = 1; j < pion + 1; j++)
			{
				kopia_obraz[i][j].B = S[0][0] * obraz[i - 1][j - 1].B + S[0][1] * obraz[i - 1][j].B + S[0][2] * obraz[i - 1][j + 1].B + S[1][0] * obraz[i][j - 1].B + S[1][1] * obraz[i][j].B + S[1][2] * obraz[i][j + 1].B + S[2][0] * obraz[i + 1][j - 1].B + S[2][1] * obraz[i + 1][j].B + S[2][2] * obraz[i + 1][j + 1].B;
				kopia_obraz[i][j].G = S[0][0] * obraz[i - 1][j - 1].G + S[0][1] * obraz[i - 1][j].G + S[0][2] * obraz[i - 1][j + 1].G + S[1][0] * obraz[i][j - 1].G + S[1][1] * obraz[i][j].G + S[1][2] * obraz[i][j + 1].G + S[2][0] * obraz[i + 1][j - 1].G + S[2][1] * obraz[i + 1][j].G + S[2][2] * obraz[i + 1][j + 1].G;
				kopia_obraz[i][j].R = S[0][0] * obraz[i - 1][j - 1].R + S[0][1] * obraz[i - 1][j].R + S[0][2] * obraz[i - 1][j + 1].R + S[1][0] * obraz[i][j - 1].R + S[1][1] * obraz[i][j].R + S[1][2] * obraz[i][j + 1].R + S[2][0] * obraz[i + 1][j - 1].R + S[2][1] * obraz[i + 1][j].R + S[2][2] * obraz[i + 1][j + 1].R;

				if (kopia_obraz[i][j].B > 255)   // wartosci b g r musza miescic sie w przedziale [0,255]
					kopia_obraz[i][j].B = 255;
				else if (kopia_obraz[i][j].B < 0)
					kopia_obraz[i][j].B = 0;
				if (kopia_obraz[i][j].G > 255)
					kopia_obraz[i][j].G = 255;
				else if (kopia_obraz[i][j].G < 0)
					kopia_obraz[i][j].G = 0;
				if (kopia_obraz[i][j].R > 255)
					kopia_obraz[i][j].R = 255;
				else if (kopia_obraz[i][j].R < 0)
					kopia_obraz[i][j].R = 0;
				suma_obraz[i][j].B += kopia_obraz[i][j].B;
				suma_obraz[i][j].G += kopia_obraz[i][j].G;
				suma_obraz[i][j].R += kopia_obraz[i][j].R;
			}
		}
		int pomoc = S[0][0]; //obrocenie maski 
		S[0][0] = S[0][1];
		S[0][1] = S[0][2];
		S[0][2] = S[1][2];
		S[1][2] = S[2][2];
		S[2][2] = S[2][1];
		S[2][1] = S[2][0];
		S[2][0] = S[1][0];
		S[1][0] = pomoc;
	}
	for (int i = 0; i < poziom + 2; i++)
	{
		for (int j = 0; j < pion + 2; j++)
		{
			obraz[i][j].B = suma_obraz[i][j].B / 8;   //przypisanie podstawowej tablicy koncowych wartosci po wykonaniu obliczen
			obraz[i][j].G = suma_obraz[i][j].G / 8;
			obraz[i][j].R = suma_obraz[i][j].R / 8;
		}
	}
}

void zapisz_tablice(int& pion, int& poziom, BRG**& obraz, ofstream& zapis) //zapisanie nowych wartosci pikseli do pliku
{
	for (int i = 1; i < poziom + 1; i++)
	{

		for (int j = 1; j < pion + 1; j++)
		{

			zapis.write(reinterpret_cast<char*>(&obraz[i][j].B), 1);
			zapis.write(reinterpret_cast<char*>(&obraz[i][j].G), 1);
			zapis.write(reinterpret_cast<char*>(&obraz[i][j].R), 1);
		}
	}
}

void tworz_tablice_fragmentu(int b, int& pion, BRG**& fragment, ifstream& odczyt)
{
	for (int i = 1; i < b; i++)		//zamiana linijek w celu "wspinania sie" po pliku kolejnymi fragmentami
	{
		swap(fragment[i], fragment[i - 1]);
	}


	for (int j = 0; j < pion + 2; j++)
	{
		if (j == 0 || j == (pion + 1))  //nadanie czarnego koloru dla pionowych czesci ramki
		{
			fragment[b - 1][j].B = 0;
			fragment[b - 1][j].R = 0;
			fragment[b - 1][j].G = 0;
		}
		else
		{
			odczyt.read(reinterpret_cast<char*>(&fragment[b - 1][j].B), 1);		//odczytanie wartosci pikseli na danym fragmencie
			odczyt.read(reinterpret_cast<char*>(&fragment[b - 1][j].G), 1);
			odczyt.read(reinterpret_cast<char*>(&fragment[b - 1][j].R), 1);
			;
		}
	}

}

void konwolucja_i_zapis_fragmentu(int b, int& pion, BRG**& fragment, ifstream& odczyt, ofstream& zapis)
{
	BGR* kopia_obraz;
	BGR* suma_obraz;
	kopia_obraz = new BGR; // pomocnicza tablica do chwilowego przechowania wartosci b g r obliczonych jedną maską

	kopia_obraz = new BGR[pion + 2];

	suma_obraz = new BGR; // pomocnicza tablica do sumowania chwilowych wartosci z tablicy kopia_obraz

	suma_obraz = new BGR[pion + 2];

	for (int j = 0; j < pion + 2; j++)
	{
		suma_obraz[j].B = 0;
		suma_obraz[j].G = 0;
		suma_obraz[j].R = 0;
	}

	int S[3][3]; // pierwsza maska
	S[0][0] = -1;
	S[0][1] = 0;
	S[0][2] = 1;
	S[1][0] = -2;
	S[1][1] = 0;
	S[1][2] = 2;
	S[2][0] = -1;
	S[2][1] = 0;
	S[2][2] = 1;

	for (int s = 0; s < 8; s++)
	{
		for (int i = 1; i < 2; i++)
		{
			for (int j = 1; j < pion - 1; j++)
			{
				kopia_obraz[j].B = S[0][0] * fragment[i - 1][j - 1].B + S[0][1] * fragment[i - 1][j].B + S[0][2] * fragment[i - 1][j + 1].B + S[1][0] * fragment[i][j - 1].B + S[1][1] * fragment[i][j].B + S[1][2] * fragment[i][j + 1].B + S[2][0] * fragment[i + 1][j - 1].B + S[2][1] * fragment[i + 1][j].B + S[2][2] * fragment[i + 1][j + 1].B;
				kopia_obraz[j].G = S[0][0] * fragment[i - 1][j - 1].G + S[0][1] * fragment[i - 1][j].G + S[0][2] * fragment[i - 1][j + 1].G + S[1][0] * fragment[i][j - 1].G + S[1][1] * fragment[i][j].G + S[1][2] * fragment[i][j + 1].G + S[2][0] * fragment[i + 1][j - 1].G + S[2][1] * fragment[i + 1][j].G + S[2][2] * fragment[i + 1][j + 1].G;
				kopia_obraz[j].R = S[0][0] * fragment[i - 1][j - 1].R + S[0][1] * fragment[i - 1][j].R + S[0][2] * fragment[i - 1][j + 1].R + S[1][0] * fragment[i][j - 1].R + S[1][1] * fragment[i][j].R + S[1][2] * fragment[i][j + 1].R + S[2][0] * fragment[i + 1][j - 1].R + S[2][1] * fragment[i + 1][j].R + S[2][2] * fragment[i + 1][j + 1].R;

				if (kopia_obraz[j].B > 255) // wartosci b g r musza miescic sie w przedziale [0,255]
					kopia_obraz[j].B = 255;
				else if (kopia_obraz[j].B < 0)
					kopia_obraz[j].B = 0;
				if (kopia_obraz[j].G > 255)
					kopia_obraz[j].G = 255;
				else if (kopia_obraz[j].G < 0)
					kopia_obraz[j].G = 0;
				if (kopia_obraz[j].R > 255)
					kopia_obraz[j].R = 255;
				else if (kopia_obraz[j].R < 0)
					kopia_obraz[j].R = 0;
				suma_obraz[j].B += kopia_obraz[j].B;
				suma_obraz[j].G += kopia_obraz[j].G;
				suma_obraz[j].R += kopia_obraz[j].R;
			}
		}
		int pomoc = S[0][0]; // obrocenie maski
		S[0][0] = S[0][1];
		S[0][1] = S[0][2];
		S[0][2] = S[1][2];
		S[1][2] = S[2][2];
		S[2][2] = S[2][1];
		S[2][1] = S[2][0];
		S[2][0] = S[1][0];
		S[1][0] = pomoc;
	}

	for (int j = 0; j < pion + 2; j++)
	{
		suma_obraz[j].B /= 8; // przypisanie podstawowej tablicy koncowych wartosci po wykonaniu obliczen
		suma_obraz[j].G /= 8;
		suma_obraz[j].R /= 8;
	}

	for (int j = 1; j < pion + 1; j++)
	{

		zapis.write(reinterpret_cast<char*>(&suma_obraz[j].B), 1);	//zapisanie informacji do pliku
		zapis.write(reinterpret_cast<char*>(&suma_obraz[j].G), 1);
		zapis.write(reinterpret_cast<char*>(&suma_obraz[j].R), 1); 
	}


}

int main()
{
	string nazwa_wejscia;
	cout << "Podaj nazwe pliku: (wraz z .bmp)" << endl;
	cin >> nazwa_wejscia;
	string nazwa_wyjscie;
	cout << "Podaj nazwe wyjsciowego pliku BMP: (wraz z .bmp)" << endl;
	cin >> nazwa_wyjscie;

	ifstream odczyt;
	odczyt.open(nazwa_wejscia, ios::binary);
	if (!odczyt)
	{
		cout << "Brak dostepu do pliku." << endl;
		return 0;
	}
	ofstream zapis;
	zapis.open(nazwa_wyjscie, ios::binary);

	BFH bfh;
	BIH bih;
	BRG brg;
	BFH_odczyt(odczyt, bfh); //odczyt danych z naglowkow
	BIH_odczyt(odczyt, bih);
	int poziom = bih.biHeight;	//ilosc wierszy pikseli
	int pion = bih.biWidth;		//iosc kolumn pikseli

	cout << endl;	//wypisanie danych z naglowka obrazu
	cout << "Budowa naglowka obrazu BMP:" << endl;
	cout << endl;
	cout << "Rozmiar naglowka informacyjnego: " << bih.biSize << endl;
	cout << "Szerokosc obrazu w pikselach: " << bih.biWidth << endl;
	cout << "Wysokosc obrazu w pikselach: " << bih.biHeight << endl;
	cout << "Liczba platow (musi byc 1): " << bih.biPlanes << endl;
	cout << "Liczba bitow na piksel (1, 4, 8, 16, 24 lub 32): " << bih.biBitCount << endl;
	cout << "Algorytm kompresji (BI_RGB=0, BI_RLE8=1, BI_RLE4=2, BI_BITFIELDS=3): " << bih.biCompression << endl;
	cout << "Rozmiar rysunku: " << bih.biSizeImage << endl;
	cout << "Rozdzielczosc pozioma: " << bih.biXpelsPerMeter << endl;
	cout << "Rozdzielczosc pionowa: " << bih.biYpelsPerMeter << endl;
	cout << "Liczba kolorow w palecie: " << bih.biCrlUses << endl;
	cout << "Liczba waznych kolorow w palecie: " << bih.biCrlImportant << endl;
	cout << endl;
	int a;
	cout << "W jakim trybie chcesz przetwarzac plik? Podaj odpowiednia cyfre." << endl;
	cout << " 1 - przetworzenie calego pliku na raz." << endl;
	cout << " 2 - przetworzenie calego pliku fragmentami." << endl;
	cout << " 3 - przetworzenie calego pliku z zastosowaniem 'filtra'." << endl;
	while (cin >> a)
	{
		if (a == 1)
		{
			cout << "Przetwarzasz caly plik na raz." << endl;

			BRG** obraz;				//stworzenie tablicy pikseli o odpowiedniej wielkosci

			obraz = new BRG * [poziom + 2];
			if (pion % 4 == 0)
			{
				for (int i = 0; i < poziom + 2; i++)
				{
					obraz[i] = new BRG[pion + 2];
				}
			}
			else
			{
				pion = pion + (4 - pion % 4);   //dolaczenie ewentualnie potrzebnych pustych miejsc do tablicy
				for (int i = 0; i < poziom + 2; i++)
				{
					obraz[i] = new BRG[pion + 2];
				}
			}

			tworz_tablice(pion, poziom, obraz, odczyt);
			konwolucja(obraz, pion, poziom, nullptr);
			BFH_zapis(zapis, bfh);
			BIH_zapis(zapis, bih);
			zapisz_tablice(pion, poziom, obraz, zapis);
			zapis.close();
			odczyt.close();
			break;
		}
		if (a == 2)
		{
			cout << "Przetwarzasz plik fragmentami o wysokosci 3 pikseli." << endl; 

			const int b = 3;
			BRG** fragment;
			fragment = new BRG * [b];   // stworzenie rozmiaru tablicy 
			for (int i = 0; i < b; i++)
			{
				fragment[i] = new BRG[pion + 2];
			}
			BFH_zapis(zapis, bfh);
			BIH_zapis(zapis, bih);
			
			for (int i = 0; i < pion + 2; i++) //wypelnienie pierwszego wiersza zerami
			{
				fragment[0][i].R = 0;
				fragment[0][i].B = 0;
				fragment[0][i].G = 0;
			}
			for (int i = 0; i < 2; i++)
			{
				tworz_tablice_fragmentu(b, pion, fragment, odczyt);  //stworzenie tablicy dla pierwszego przerabianego fragmentu
			}
			for (int i = 0; i < poziom; i++)
			{
				konwolucja_i_zapis_fragmentu(b, pion, fragment, odczyt, zapis); //konwolucja na fragmencie i zapisanie pracy do pliku		
				if (i < poziom - 1) {
					tworz_tablice_fragmentu(b, pion, fragment, odczyt);		//stworzenie nowej tablicy dla nastepnego fragmentu
				}
				else
				{
					for (int i = 0; i < pion + 2; i++) //wypelnienie ostatniego wiersza zerami
					{
						fragment[2][i].R = 0;
						fragment[2][i].B = 0;
						fragment[2][i].G = 0;
					}
				}
			}
			zapis.close();
			odczyt.close();
			break;
		}
		if (a == 3)
		{
			cout << "Przetwarzasz plik nadajac mu 'filtr'." << endl;

			int macierz[9];
			ifstream filtr("plik.txt");  //otworzenie pliku z filtrem
			if (!filtr)
			{
				cout << "Plik nie istnieje." << endl;
			}
			for (int i = 0; i < 9; i++)
			{
				filtr >> macierz[i];	//przypisanie tablicy danych z pliku
			}

			BRG** obraz;		//stworzenie tablicy pikseli

			obraz = new BRG * [poziom + 2];
			if (pion % 4 == 0)
			{
				for (int i = 0; i < poziom + 2; i++)
				{
					obraz[i] = new BRG[pion + 2];
				}
			}
			else
			{
				pion = pion + (4 - pion % 4);
				for (int i = 0; i < poziom + 2; i++)
				{
					obraz[i] = new BRG[pion + 2];
				}
			}

			tworz_tablice(pion, poziom, obraz, odczyt);
			konwolucja(obraz, pion, poziom, macierz);
			BFH_zapis(zapis, bfh);
			BIH_zapis(zapis, bih);
			zapisz_tablice(pion, poziom, obraz, zapis);
			zapis.close();
			odczyt.close();
			break;
		}
		else
		{
			cout << "Niepoprawny wybor, wybierz odpowiednia liczbe wedlug powyzszej instrukcji." << endl;
		}
	}
	return 0;
}