#ifndef CAR_H
#define CAR_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QPixmap>

/**
 * \brief klasa Car
 *
 * klasa obiektow odpowiedzialna za tworzenie bryły pojazdu oraz za ruch pojazdu na mapie
 *
 * \version wersja alfa
 */

class Car : public QGraphicsItem
{
public:

    Car(int angle90);

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

          ~Car();
    void resetTab();

protected:
      void advance(int step);

private:
      static int counter; /**< Licznik utworzonych obiektów klasy Car */
      int ID; /**< indywidualny identyfikator dla każdego obiektu klasy Car */
      qreal angle; /**< kąt ułożenia obiektu graficznego klasy Car względem sceny graficznej */
      qreal speed; /**< krok przesunięcia pojazdu przy wywołaniu timera */
      qreal stepX = speed; /**< krok przesunięcia pojazdu w osi x */
      qreal stepY = 0; /**< krok przesunięcia pojazdu w osi y */
      QColor color; /**< kolor pojazdu */
      int turn7 = 9; /**< zmienna wskazująca na obecnie podejmowany manewr na skrzyżowaniu typu 7 */
      int turn8 = 9;  /**< zmienna wskazująca na obecnie podejmowany manewr na skrzyżowaniu typu 8 */
      int turn9 = 9; /**< zmienna wskazująca na obecnie podejmowany manewr na skrzyżowaniu typu 9 */
      int turn10 = 0; /**< zmienna wskazująca na obecnie podejmowany manewr na skrzyżowaniu typu 10 */
      int turn11 = 99; /**< zmienna wskazująca na obecnie podejmowany manewr na skrzyżowaniu typu 11 */
      int acctPosI; /**< numer klatki mapy w poziomie, w której aktualnie znajduje się pojazd  */
      int acctPosJ; /**< numer klatki mapy w pionie, w której aktualnie znajduje się pojazd  */
      int accPosMapVal; /**< wartość mówiąca o typie elementu mapy w klatce, w której obecnie znajduje się pojazd */
      int accPosInKlI; /**< aktualne połeżenie pojazdu w klatce mapy w poziomie*/
      int accPosInKlJ; /**< aktualne połeżenie pojazdu w klatce mapy w pionie*/
      static int carsPos [16][16]; /**< tablica wskazująca pozycję każdego z pojazdu znajdującego się na mapie */
      void setCarPos();
      void moveCalc();
  };



#endif // CAR_H
