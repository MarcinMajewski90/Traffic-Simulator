#ifndef MAPA_H
#define MAPA_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QGraphicsView>

/**
 * \brief klasa mapa
 *
 * klasa obiektow odpowiedzialna za tworzenie obiektów graficznych przedstawiających obrazy odpowiednio pobranych elementów mapy.
 *
 * \version wersja alfa
 */
class Mapa : public QGraphicsItem
{
public:

    Mapa(QGraphicsItem *parent =NULL, int a=0);
    QRectF boundingRect() const;
private:
    int mapVal; /**< Wartośc typu elementu mapy */



protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);





};

#endif // MAPA_H
