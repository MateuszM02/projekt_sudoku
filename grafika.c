#include "sudoku.h"
#include "grafika.h"

//zmienne globalne

GtkWidget *GUZIKI_PLANSZY[9][9];

int RZAD = 0;
int KOLUMNA = 0;

//main

void Grafika(int argc, char *argv[])
{
    gtk_init(&argc, &argv);
    GtkWidget *okno = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    StworzOkno(okno);
    //
    EkranStartowy(okno);
    gtk_main();
}

//grafika

Plansza StworzPlansze()
{
    srand(time(NULL));
    Plansza plansza;
    int rzad = 0;
    int kolumna = 0;
    //
    InicjalizujPlansze(&plansza);
    for(int i=0; i < 2000 && rzad <=8; i++)
        GenerujPlansze(&plansza, &rzad, &kolumna);
    return plansza;
}

int PokazPodpowiedzi(Plansza *p1, int ile_podpowiedzi)
{
    int los = 0;
    for(int i = 0; i < ile_podpowiedzi; i++)
    {
        do
        {
            los = rand() % 81;
            if(p1->pole[los/9][los%9].czy_domyslna == false)
            {
                p1->pole[los/9][los%9].wartosc_podana = p1->pole[los/9][los%9].wartosc;
                p1->pole[los/9][los%9].czy_domyslna = true;
            }
        }
        while(p1->pole[los/9][los%9].czy_domyslna == false);
        printf("%d rzad: %d kolumna: %d\n",i, los/9 + 1, los%9 + 1);
    }
    return los;
}

//inicjalizatory

void StworzOkno(GtkWidget *window)
{
    gtk_window_set_title(GTK_WINDOW(window),"Sudoku");
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER_ALWAYS);
    gtk_window_set_default_size(GTK_WINDOW(window),400,400);
    g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit),NULL);
    gtk_widget_show(window);
}

//ekrany gry

void EkranStartowy(GtkWidget *okno)
{
    //siatka
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_container_add(GTK_CONTAINER(okno), grid);
    //entry
    GtkWidget *label = gtk_label_new("Wybierz poziom trudnosci:"); // do wprowadzania wyrazenia
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);
    //przyciski
    for (int i = 0; i < 4; i++)
    {
        const gchar *name;
        if(i==0) name = "latwy";
        else if(i==1) name = "sredni";
        else if(i==2) name = "trudny";
        else if(i==3) name = "niestandardowy";
        else name = "x";
        //
        GtkWidget *button = gtk_button_new_with_label(name);
        //
        g_signal_connect(G_OBJECT(button), "clicked",G_CALLBACK(KliknietoGuzikWyboruPoziomu),okno);
        gtk_widget_set_name(button,name);
        gtk_grid_attach(GTK_GRID(grid), button, 0, i + 1, 1, 1);
    }
    gtk_widget_show_all(okno);
}

void EkranGry(Plansza p1)
{
    GtkWidget *okno = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    StworzOkno(okno);
    //siatka
    GtkWidget *siatka = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(okno),siatka);
    //guziki planszy
    for(int i = 0; i < 9; i++) //rzad
    {
        for(int j = 0; j < 9; j++) //kolumna
        {
            char *indeks = g_strdup_printf("%d", 10*i+j);
            GUZIKI_PLANSZY[i][j] = gtk_button_new();
            gtk_widget_set_name(GUZIKI_PLANSZY[i][j],indeks);
            if(p1.pole[i][j].wartosc_podana != 0)
            {
                char *wartosc = g_strdup_printf("%d",p1.pole[i][j].wartosc);
                gtk_button_set_label(GTK_BUTTON(GUZIKI_PLANSZY[i][j]),wartosc);
            }
            g_signal_connect(G_OBJECT(GUZIKI_PLANSZY[i][j]),"clicked",G_CALLBACK(KliknietoGuzikPola),&p1);
            gtk_grid_attach(GTK_GRID(siatka),GUZIKI_PLANSZY[i][j],j,i,1,1);
        }
    }
    //guziki wyboru
    for(int i = 0; i < 3; i++) //rzad
    {
        for(int j = 0; j < 3; j++) //kolumna
        {
            char *numer = g_strdup_printf("%d", 3*i+j+1);
            GtkWidget *button = gtk_button_new_with_label(numer);
            gtk_widget_set_name(button,numer);
            g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(KliknietoWybranaLiczbe),&p1);
            gtk_grid_attach(GTK_GRID(siatka),button,10+j,i,1,1);
        }
    }
    //inne kontrolki
    GtkWidget *fake_label = gtk_label_new("");
    gtk_widget_set_size_request(fake_label,50,10);
    gtk_grid_attach(GTK_GRID(siatka),fake_label,9,0,1,1);
    //
    GtkWidget *podpowiedz = gtk_button_new_with_label("podpowiedz");
    g_signal_connect(G_OBJECT(podpowiedz),"clicked",G_CALLBACK(Podaj1Podpowiedz),&p1);
    gtk_grid_attach(GTK_GRID(siatka),podpowiedz,10,4,3,2);
    //
    GtkWidget *sprawdz = gtk_button_new_with_label("sprawdz poprawnosc odpowiedzi");
    g_signal_connect(G_OBJECT(sprawdz),"clicked",G_CALLBACK(SprawdzPoprawnosc),okno);
    gtk_grid_attach(GTK_GRID(siatka),sprawdz,10,7,3,2);
    //
    gtk_widget_show_all(siatka);
    gtk_main();
}

//funkcje aktywowane po kliknieciu guzika

void KliknietoGuzikWyboruPoziomu(GtkWidget *guzik, gpointer okno)
{
    Plansza p1 = StworzPlansze();
    //
    const gchar *name = gtk_widget_get_name(guzik);
    if(name[0] == 'l') PokazPodpowiedzi(&p1,50);
    else if(name[0] == 's') PokazPodpowiedzi(&p1,40);
    else if(name[0] == 't') PokazPodpowiedzi(&p1,30);
    else if(name[0] == 'n') PokazPodpowiedzi(&p1,20);
    else printf("zle imie: %s",name);
    //
    gtk_widget_destroy(okno);
    EkranGry(p1);
}

void KliknietoGuzikPola(GtkWidget *guzik, Plansza *plansza)
{
    const gchar *nazwa = gtk_widget_get_name(guzik);
    int liczba = atoi(nazwa);
    RZAD = liczba / 10;
    KOLUMNA = liczba % 10;
}

void KliknietoWybranaLiczbe(GtkWidget *guzik, Plansza *plansza)
{
    if(plansza->pole[RZAD][KOLUMNA].czy_domyslna == true)  return;
    int liczba = atoi(gtk_widget_get_name(guzik));
    plansza->pole[RZAD][KOLUMNA].wartosc_podana = liczba;
    gtk_button_set_label(GTK_BUTTON(GUZIKI_PLANSZY[RZAD][KOLUMNA]),gtk_widget_get_name(guzik));
}

void Podaj1Podpowiedz(GtkWidget *guzik, Plansza *plansza)
{
    int x = PokazPodpowiedzi(plansza,1); //ustala lokalizacje guzika, w ktorym umiesci podpowiedz
    gtk_button_set_label(GTK_BUTTON(GUZIKI_PLANSZY[x/9][x%9]),g_strdup_printf("%d", plansza->pole[x/9][x%9].wartosc));
}

void SprawdzPoprawnosc(GtkWidget *guzik, Plansza *plansza) ///nie dziala
{
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            if(plansza->pole[i][j].wartosc_podana == 0) continue;
            if(plansza->pole[i][j].wartosc != plansza->pole[i][j].wartosc_podana)
                printf("%d %d\n",i,j);
        }
    }
}
