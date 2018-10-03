#include "tablicamapy.h"
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <iostream>
#include <stdlib.h>
#include <QtWidgets>
#include <math.h>
#include <QCoreApplication>
#include <QPixmap>
#include <QString>
#include <mainwindow.h>
using namespace std;


/**
* \brief Konstruktor.
*
* Tworzy obiekt klasy TablicaMapy
*
*/
TablicaMapy::TablicaMapy()
{

}

/**
* \brief Odczytuje z pliku tekstowego wartości przedstawiające ułożenie kolejnych elementw mapy.
*
*/
void TablicaMapy::OdczytZPliku()
{
    QFile file(QCoreApplication::applicationDirPath()+"/tablica.txt");
    if (file.open(QFile::ReadOnly | QFile::Text))
        {

    bool ok;
    PlikOk = true;
    QTextStream in(&file);
    for(int i=0; i<4; i++)
        {
            for(int j=0; j<4; j++)
                {
                    QString line = in.readLine() ;
                    TabZPliku[i][j]= line;
                    TabMapy[i][j] = TabZPliku[i][j].toInt(&ok);
                     cout << "Tablica mapy  " << endl << TabMapy[i][j] + 1000 << endl;

                }
        }
    file.close();
    }
    else
    {
        cout<<"Nie znalazlem pliku mapy"<<endl;
        PlikOk = false;
    }
}
/**
* \brief Przyjmuje współrzędne klatki mapy i zwraca jej wartość.
*
*/
int TablicaMapy::getValTab(int a, int b)
{
    return TabMapy[a][b];
}

/**
* \brief inicjalizacja tablicy: TabMapy.
*
*/
int TablicaMapy::TabMapy [10][10] = {0};

bool TablicaMapy::czyPlikOk()
{
    return PlikOk;
}
