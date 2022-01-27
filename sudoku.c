#include "sudoku.h"

///funkcje inicjalizujace

void InicjalizujPlansze(Plansza *p1) //nadaje wartosci poczatkowe planszy
{
    for(int i=0;i<9;i++)
    {
        for(int j=0;j<9;j++)
        {
            InicjalizujPole(&p1->pole[i][j]);
        }
    }
}

void InicjalizujPole(Pole *pole1) //nadaje wartosci poczatkowe polu
{
    pole1->wartosc = 0;
    pole1->wartosc_podana = 0;
    pole1->czy_domyslna = false;
    for(int i=0;i<9;i++)
    {
        pole1->czy_moze[i] = true;
    }
}

///funkcje generujace plansze

void GenerujPlansze(Plansza *p1, int *rzad, int *kolumna) //losuje wartosc danego pola
{
    int ile_popr = IlePoprawnychMozliwosci(p1,*rzad,*kolumna); //wyznacza ilosc poprawnych mozliwosci uzupelnienia pola
    //printf("%d %d, %d\n",*rzad,*kolumna,ile_popr);
    if(*rzad + *kolumna == 0 && p1->pole[0][0].wartosc != 0)
        {printf("Nie udalo sie stworzyc poprawnej planszy!\n"); WypiszPlansze(p1); exit(1);} //zakoncz program w przypadku bledu
    if(ile_popr < 1) //jesli nie ma mozliwosci poprawnego uzupelnienia pola
        PoprzedniePole(p1,rzad,kolumna);
    else
        AktualnePole(p1,rzad,kolumna,ile_popr);
    //for(int i = 0; i < 9; i++)
    //{
        //printf("%d ",p1->pole[*rzad][*kolumna].czy_moze[i]);
    //}
}

void LosujLiczbe(Pole *pole1, int *poprawne_liczby, int rozmiar)
{
    int indeks = rand() % rozmiar;
    pole1->wartosc = poprawne_liczby[indeks];
    pole1->czy_moze[pole1->wartosc - 1] = false;
}

///funkcje operujace na danym polu

void AktualnePole(Plansza *p1, int *rzad, int *kolumna, int ile_popr)
{
    int *poprawne_liczby = malloc(sizeof(int)*ile_popr);
    int indeks = 0;
    for(int i = 0; i < 9; i++)
    {
        if(CzyPoprawnaLiczba(p1, *rzad, *kolumna, i + 1))
        {
            poprawne_liczby[indeks] = i + 1;
            indeks++;
        }
    }
    LosujLiczbe(&p1->pole[*rzad][*kolumna],poprawne_liczby,indeks);
    free(poprawne_liczby);
    ZmienPole(rzad,kolumna,nastepne); //nastepne pole
}

void PoprzedniePole(Plansza *p1, int *rzad, int *kolumna)
{
    InicjalizujPole(&p1->pole[*rzad][*kolumna]);
    ZmienPole(rzad,kolumna,poprzednie); //cofnij sie o 1 pole
}

///funkcje sprawdzajace poprawnosc

int IlePoprawnychMozliwosci(Plansza *p1,int rzad,int kolumna)
{
    int ilosc = 0;
    for(int liczba = 1; liczba <= 9; liczba++)
        if(CzyPoprawnaLiczba(p1,rzad,kolumna,liczba)) ilosc++;
    return ilosc;
}

bool CzyPoprawnaLiczba(Plansza *p1, int rzad, int kolumna, int liczba)
{
    if(p1->pole[rzad][kolumna].czy_moze[liczba-1] == false) return false;
    if(CzyPoprawnaLiczbaRzad(p1, rzad, kolumna, liczba) == false)   return false;
    if(CzyPoprawnaLiczbaKolumna(p1, rzad, kolumna, liczba) == false)   return false;
    if(CzyPoprawnaLiczbaKwadrat(p1, rzad, kolumna, liczba) == false)   return false;
    return true;
}

bool CzyPoprawnaLiczbaRzad(Plansza *p1, int rzad, int kolumna, int liczba)
{
    for(int i = 0; i < kolumna; i++)
    {
        if(p1->pole[rzad][i].wartosc == liczba) return false;
    }
    return true;
}

bool CzyPoprawnaLiczbaKolumna(Plansza *p1, int rzad, int kolumna, int liczba)
{
    for(int i = 0; i < rzad; i++)
    {
        if(p1->pole[i][kolumna].wartosc == liczba) return false;
    }
    return true;
}

bool CzyPoprawnaLiczbaKwadrat(Plansza *p1, int rzad, int kolumna, int liczba)
{
    for(int i = rzad - (rzad % 3); i < rzad; i++) //poprzednie rzedy
    {
        for(int j = kolumna - (kolumna % 3); j < 3 + kolumna - (kolumna % 3); j++) //kolumna
        {
            if(p1->pole[i][j].wartosc == liczba) return false;
        }
    }
    return true;
}

///funkcje pomocnicze

void WypiszPlansze(Plansza *p1)
{
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            printf("%d ",p1->pole[i][j].wartosc);
            if(j % 3 == 2) printf("  ");
        }
        if(i % 3 == 2) printf("\n\n");
        else printf("\n");
    }
}

void ZmienPole(int *rzad, int *kolumna, Opcja opcja)
{
    if(opcja == poprzednie) //idzie 1 pole do tylu
    {
        if(*kolumna > 0) *kolumna -= 1;
        else {*rzad -= 1; *kolumna = 8;}
    }
    else if(opcja == nastepne) //idzie 1 pole do przodu
    {
        if(*kolumna < 8) *kolumna += 1;
        else {*rzad += 1; *kolumna = 0;}
    }
}

///sprawdzajace poprawnosc planszy

void IleBledow(Plansza *p1)
{
    int ilosc = 0;
    for(int i = 0; i < 9; i++) //rzad
    {
        for(int j = 0; j < 9; j++) //kolumna
        {
            for(int k = j + 1; k < 9; k++) //kolumna 2
            {
                if(p1->pole[i][j].wartosc == p1->pole[i][k].wartosc && p1->pole[i][j].wartosc > 0) //sprawdza rzad
                {
                    printf("poziomo: %d %d, %d %d\n",i+1,j+1,i+1,k+1);
                    ilosc++;
                }
            }
            for(int k = i + 1; k < 9; k++)
            {
                if(p1->pole[i][j].wartosc == p1->pole[k][j].wartosc && p1->pole[i][j].wartosc > 0) //sprawdza kolumne
                {
                    printf("pionowo: %d %d, %d %d\n",i+1,j+1,k+1,j+1);
                    ilosc++;
                }
            }
        }
    }
    for(int i = 0; i < 9; i+=3) //rzad
    {
        for(int j = 0; j < 9; j+=3) //kolumna
        {
            if(CzyDobryKwadrat(p1,i,j) == false) {printf("Kwadratowo: %d %d\n",i/3 + 1,j/3 + 1);ilosc++;}
        }
    }
    if(ilosc==0) printf("Stworzono poprawna plansze do gry w Sudoku!\n");
    else printf("Wykryto %d bledow\n",ilosc);
}

bool CzyDobryKwadrat(Plansza *p1,int i,int j)
{
    int ilosc = 0;
    bool wystapilo[9] = {false,false,false,false,false,false,false,false,false};
    for(int x = 0; x < 3; x++)
    {
        for(int y = 0; y < 3; y++)
        {
            if(wystapilo[p1->pole[i+x][j+y].wartosc - 1] == false) {wystapilo[p1->pole[i+x][j+y].wartosc - 1] = true; ilosc++;}
        }
    }
    if(ilosc == 9) return true;
    return false;
}
