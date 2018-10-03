#include "mapa.h"
#include <QPixmap>
#include <tablicamapy.h>
#include <QWidget>
#include <QStyleOptionGraphicsItem>
#include <QPointer>

/**
* \brief Konstruktor.
* Tworzy obiekt klasy Mapa./n
* przyjmuje parametr a określający typ elementu mapy.
*
*/
Mapa::Mapa(QGraphicsItem *parent, int a): QGraphicsItem(parent)
{
    mapVal = a;
}

/**
* \brief Tworzy obszar elementu mapy.
*
*/
QRectF Mapa::boundingRect() const
{
    return QRectF(0,0,200,200);

}

/**
* \brief Pobiera odpowiednią grafikę w zależności od aktualnej wartości typu elementu mapy.
*
*/
void Mapa::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    switch (mapVal) {
    case 1:
        painter->drawPixmap(1,1, QPixmap(":/img/PPoz.png"));
        break;
    case 2:
        painter->drawPixmap(1,1, QPixmap(":/img/PPion.png"));
        break;
    case 3:
        painter->drawPixmap(1,1, QPixmap(":/img/ZGP.png"));
        break;
    case 4:
        painter->drawPixmap(1,1, QPixmap(":/img/ZGL.png"));
        break;
    case 5:
        painter->drawPixmap(1,1, QPixmap(":/img/ZDP.png"));
        break;
    case 6:
        painter->drawPixmap(1,1, QPixmap(":/img/ZDL.png"));
        break;
    case 7:
        painter->drawPixmap(1,1, QPixmap(":/img/TP.png"));
        break;
    case 8:
        painter->drawPixmap(1,1, QPixmap(":/img/TL.png"));
        break;
    case 9:
        painter->drawPixmap(1,1, QPixmap(":/img/TG.png"));
        break;
    case 10:
        painter->drawPixmap(1,1, QPixmap(":/img/TD.png"));
        break;
    case 11:
        painter->drawPixmap(1,1, QPixmap(":/img/X.png"));
        break;
    case 12:
        painter->drawPixmap(1,1, QPixmap(":/img/Trawa.png"));
        break;
    default:
        break;
    }

}
