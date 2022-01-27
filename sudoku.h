#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <gtk/gtk.h>

//struktury

typedef struct
{
    int wartosc; //wartosc pola wylosowana przez algorytm
    int wartosc_podana; //wartosc pola podana przez uzytkownika
    bool czy_moze[9]; //tablica okreslajaca, czy dana liczba 1-9 moze wystapic na danym polu
    bool czy_domyslna; //okresla, czy wartosc pola jest podawana uzytkownikowi na starcie rozgrywki
}Pole;

typedef struct
{
    Pole pole[9][9];
}Plansza;

typedef enum
{
    poprzednie,nastepne
}Opcja;

//funkcje inicjalizujace
void InicjalizujPlansze(Plansza *p1);
void InicjalizujPole(Pole *pole1);
void NowyGuzik(GtkWidget *guzik);
//funkcje generujace plansze
void GenerujPlansze(Plansza *p1, int *rzad, int *kolumna);
void LosujLiczbe(Pole *pole1, int *poprawne_liczby, int rozmiar);
//funkcje operujace na danym polu
void AktualnePole(Plansza *p1, int *rzad, int *kolumna, int ile_popr);
void PoprzedniePole(Plansza *p1, int *rzad, int *kolumna);
//funkcje sprawdzajace poprawnosc
int IlePoprawnychMozliwosci(Plansza *p1,int rzad,int kolumna);
bool CzyPoprawnaLiczba(Plansza *p1, int rzad, int kolumna, int liczba);
bool CzyPoprawnaLiczbaRzad(Plansza *p1, int rzad, int kolumna, int liczba);
bool CzyPoprawnaLiczbaKolumna(Plansza *p1, int rzad, int kolumna, int liczba);
bool CzyPoprawnaLiczbaKwadrat(Plansza *p1, int rzad, int kolumna, int liczba);
//funkcje pomocnicze
void WypiszPlansze(Plansza *p1);
void ZmienPole(int *rzad, int *kolumna, Opcja opcja);
//sprawdzajace poprawnosc planszy
void IleBledow(Plansza *p1);
bool CzyDobryKwadrat(Plansza *p1,int i,int j);
