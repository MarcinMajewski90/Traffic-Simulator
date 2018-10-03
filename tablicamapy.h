#ifndef TABLICAMAPY_H
#define TABLICAMAPY_H

#include <QString>
#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QGraphicsScene>
#include <QPushButton>
#include <QLabel>
#include "mapa.h"

/**
 * \brief klasa TablicaMapy
 *
 * klasa obiektow odpowiedzialna za pobieranie informacji na temat budowy mapy z odpowiednio przygotowanego pliku tekstowego,
 * oraz wpisywanie informacji o wartości kolejnych elementów mapy do tablicy TabMapy
 *
 * \version wersja alfa
 */

class TablicaMapy
{
public:
    TablicaMapy();
    void OdczytZPliku();
    int getValTab(int a, int b);
    bool czyPlikOk();
private:
    bool PlikOk;
    static int TabMapy [10][10]; /**< tablica wartości typów elementów mapy odczytanych z pliku tekstowego przekonwertowanych na typ int */
    QString TabZPliku [10][10]; /**< tablica wartości typów elementów mapy odczytanych z pliku tekstowego */


};

#endif // TABLICAMAPY_H
