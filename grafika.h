#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <gtk/gtk.h>

//---------------------------------------------------------

//---------------------------------------------------------

void Grafika(int argc, char *argv[]);
void StworzOkno(GtkWidget *window);
//ekrany gry
void EkranStartowy(GtkWidget *okno);
void EkranGry(Plansza p1);
//funkcje aktywowane po kliknieciu guzika
void KliknietoGuzikWyboruPoziomu(GtkWidget *guzik, gpointer dane);
void KliknietoGuzikPola(GtkWidget *guzik, Plansza *plansza);
void KliknietoWybranaLiczbe(GtkWidget *guzik, Plansza *plansza);
void Podaj1Podpowiedz(GtkWidget *guzik, Plansza *plansza);
void SprawdzPoprawnosc(GtkWidget *guzik, Plansza *plansza);
//pomocnicze
Plansza StworzPlansze();
int PokazPodpowiedzi(Plansza *p1, int ile_podpowiedzi);

//---------------------------------------------------------
