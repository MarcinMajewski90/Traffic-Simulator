#include "car.h"
#include "tablicamapy.h"
#include "mainwindow.h"
#include <QGraphicsScene>
#include <QPainter>
#include <QColor>
#include <QStyleOption>
#include <qmath.h>
#include <iostream>
#include <ctime>

using namespace std;

 /**
 * \brief Konstruktor.
 *
 * Tworzy obiekt klasy Car, automatycznie nadając pojazdowi począdkowy kąt oraz losowy kolor.
 *
 */

Car::Car(int angle90)
    : angle(90), color(qrand() %256, qrand() %256, qrand() %256)
{
    angle = angle90;
    setRotation(angle);
    speed = 3;
    counter++;
    ID = counter;
    cout<< "konstr  " << ID << endl;

}

/**
* \brief Tworzy obszar, w którym znajdzie się bryła pojazdu
*
*/
QRectF Car::boundingRect() const
{
        return QRectF(-6, -25, 12, 25);
}

/**
* \brief Tworzy krawędź bryły pojazdu
*
*/
QPainterPath Car::shape() const
{
    QPainterPath path;
    path.addRect(-6, -25, 12, 25);
    return path;
}

/**
* \brief Tworzy elementy geometryczne skłądające się na bryłę pojazdu.
*
*/
void Car::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // Nadwozie
    painter->setBrush(color);
    painter->drawRect(-6, -25, 12, 25);


    // Swiatla
    painter->setBrush(Qt::yellow);
    painter->drawEllipse(-7,-26,6,6);
    painter->drawEllipse(1,-26,6,6);
}

/**
* \brief Przy każdym wywołaniu timera odświerza pozycję pojazdu względem mapy.
*
*/
void Car::advance(int step)
{
    QPointF pos;
    if (!step)
        return;
    if (step)
    {
        this->setCarPos();
        moveCalc();
        setRotation(angle);
        setPos(mapToParent(-(stepY),-(stepX)));
        pos = this->scenePos();
        cout << pos.rx() << endl;
        cout << pos.ry() << endl;
    }
}

/**
* \brief zapisuje aktualną pozycję pojazdu względem mapy w tablicy carsPos.
*
*/
void Car::setCarPos()
{
    cout << "setCarPos ruszyl" << endl;
    for(int i=0; i<16; i++)
    {
        for(int j=0; j<16; j++)
        {
            if (((scenePos().rx()) > j*20) & ((scenePos().rx()) <= (j*20)+20) & ((scenePos().ry()) > i*20) & ((scenePos().ry()) <= (i*20)+20))
            {
                carsPos [i][j] = ID;
            }
            else if (carsPos [i][j] == ID)
                carsPos [i][j] = 0;
        }
    }
}

/**
* \brief Wyznacza następną pozycje auta uwzględniając wartość elementu mapy, na którym się znajduje
* oraz pozycję innych pojazdów znajdujących się na mapie.
*
*/
void Car::moveCalc()
{
    TablicaMapy Obj2TabMapy;

    for(int i=0; i<16; i++)
    {
        for(int j=0; j<16; j++)
        {
            if(carsPos[i][j] ==  ID)
            {
                acctPosI = i;
                acctPosJ = j;
                cout << "znaleziono ID:" << ID << "  na pozycji i: " << acctPosI << "j: " << acctPosJ << endl;
            }
        }
    }
    accPosMapVal = Obj2TabMapy.getValTab(acctPosI / 4, acctPosJ / 4);
    cout << "car nr "<< ID <<"  accPosMapVal =  " <<  accPosMapVal <<endl;
    accPosInKlI = acctPosI %4;
    accPosInKlJ = acctPosJ %4;
    cout << "poz w kl i: " << accPosInKlI << "  j: " << accPosInKlJ << endl;
    if (((scenePos().rx()) <=0) && ((scenePos().ry()) == 50))
    {
            accPosMapVal = 99;
    }

    switch (accPosMapVal) {

    case 1:
        // jadę w prawo
        if((accPosInKlI == 2) & (carsPos[acctPosI][acctPosJ +1] == 0)){
            stepX = speed;
            stepY = 0;
            cout << "wykonal sie case 1, w prawo" << endl;
        }
        // jadę w lewo
        else if((accPosInKlI == 1) & (carsPos[acctPosI][acctPosJ -1] == 0)){
            stepX = speed;
            stepY = 0;
            cout << "wykonal sie case 1, w lewo" << endl;
        }
        else{
            stepX = 0;
            cout << "wykonal sie case 1 else" << endl;
        }
        break;
    case 2:
        // prosto ( pionowo )
        // w gore
        if((angle == 0) & (carsPos[acctPosI-1][acctPosJ] == 0) & (carsPos[acctPosI-2][acctPosJ] == 0)){
            stepX = speed;
            stepY = 0;
            angle = 0;
            cout << "wykonal sie case 2, w gore" << endl;
        }
        // w dol
        else if((angle == 180) & (carsPos[acctPosI+1][acctPosJ] == 0) & (carsPos[acctPosI+2][acctPosJ] == 0)){
            stepX = speed;
            stepY = 0;
            angle = 180;
            cout << "wykonal sie case 2, w dol" << endl;
        }
        else{
            stepX = 0;
            stepY = 0;
            cout << "wykonal sie case 2, else" << endl;
        }
        break;

     case 3: //ZGP
        //jade w prawo z dolu 1/3
        if((accPosInKlI == 3) & (accPosInKlJ == 2) & (carsPos[acctPosI - 1][acctPosJ] == 0) & (carsPos[acctPosI - 1][acctPosJ + 1] == 0)){
            if((scenePos().ry()) <= (acctPosI*20+17)){
                stepX = speed  ;
                stepY = speed /4;
                angle = angle + (speed/20 *50);
            }
            else{
                stepX = 0  ;
                stepY = 0;
                setPos((scenePos().rx()), (acctPosI*20+17));
            }
            cout << "wykonal sie case 3,  prawo z dolu 1/3" << endl;
        }
        //jade w prawo z dolu 2/3
        else if((accPosInKlI == 2) & (accPosInKlJ == 2) & (carsPos[acctPosI][acctPosJ + 1] == 0)){
            stepX = speed ;
            stepY = speed / 3;
            angle = angle + ((speed/20)*50 );
            cout << "wykonal sie case 3,  prawo z dolu 2/3" << endl;
        }
        //jade w prawo z dolu 3/3
        else if((accPosInKlI == 2) & (accPosInKlJ == 3) & (carsPos[acctPosI][acctPosJ + 1] == 0)){
            stepX = speed;
            stepY = 0;
            angle = 90;
            cout << "wykonal sie case 3,  prawo z dolu 3/3" << endl;
        }
        //jade w lewo z prawej 1/5
        else if((accPosInKlI == 1) & (accPosInKlJ == 3) &(carsPos[acctPosI][acctPosJ-1] == 0)){
            stepX = speed;
            stepY = 0;
            angle = 270;
            cout << " wykonal sie case 3 w lewo z prawej 1/5" << endl;
        }
        // lewo z prawej 2/5
        else if((accPosInKlI == 1) & (accPosInKlJ == 2) & (carsPos[acctPosI][acctPosJ-1] == 0) & (carsPos[acctPosI][acctPosJ-2] == 0)){
            if((scenePos().rx()) <= (acctPosJ*20+17)){
                stepX = speed;
                stepY = (speed/3)*(-1);
                angle = angle - (speed/20 *60);
            }
            else{
                stepX = 0;
                stepY = 0;
                setPos((acctPosJ*20+17), (scenePos().ry()));
            }
            cout << "wykonal sie case 3,  lewo z prawej 2/5" << endl;
        }
        // lewo z prawej 3/5
        else if((accPosInKlI == 1) & (accPosInKlJ == 1)& (carsPos[acctPosI+1][acctPosJ] == 0)){
            stepX = speed;
            stepY = (speed /3) *(-1);
            angle = angle - (speed/20 *50);
            cout << "wykonal sie case 3,  lewo z prawej 3/5" << endl;
        }
        // lewo z prawej 4/5
        else if((accPosInKlI == 2) & (accPosInKlJ == 1) & (carsPos[acctPosI+1][acctPosJ ] == 0)){
            stepX = speed;
            stepY = 0;
            angle = 180;
            cout << "wykonal sie case 3,  lewo z prawej 4/5" << endl;
        }
        // lewo z prawej 5/5
        else if((accPosInKlI == 3) & (accPosInKlJ == 1) & (carsPos[acctPosI+1][acctPosJ] == 0)){
            stepX = speed;
            stepY = 0;
            angle = 180;
            cout << "wykonal sie case 3,  lewo z prawej 5/5" << endl;
        }
         else{
             stepX = 0;
             stepY = 0;
             cout << "wykonal sie case 3, else" << endl;
         }
         break;
    case 4: //ZGL
        //jade w prawo z lewej 1/3
        if((accPosInKlI == 2) & (accPosInKlJ == 0) & (carsPos[acctPosI][acctPosJ +1] == 0) & (carsPos[acctPosI +1][acctPosJ+1] == 0)){
            if((scenePos().rx()) >= (acctPosJ*20+3)){
                stepX = speed  ;
                stepY = speed / 1.5;
                angle = angle + (speed/20 *60);
            }
            else{
                stepX = 0  ;
                stepY = 0;
                setPos((acctPosJ*20+3), (scenePos().ry()));
            }
            cout << "wykonal sie case 4,  prawo z lewej 1/3" << endl;
        }
        //jade w prawo z lewej 2/3
        else if ((accPosInKlI == 2) & (accPosInKlJ == 1) & (carsPos[acctPosI +1][acctPosJ] == 0)){
            stepX = speed ;
            stepY = speed / 1.5;
            angle = angle + ((speed/20)*50 );
            cout << "wykonal sie case 4,  prawo z lewej 2/3" << endl;
        }
        //jade w prawo z lewej 3/3
        else if((accPosInKlI == 3) & (accPosInKlJ == 1) & (carsPos[acctPosI +1][acctPosJ] == 0)){
            stepY = 0;
            stepX = speed;
            angle = 180;
            cout << "wykonal sie case 4,  prawo z lewej 3/3" << endl;
        }
        //jade w lewo z dolu 1/5
        else if((accPosInKlI == 3) & (accPosInKlJ == 2) & (carsPos[acctPosI - 1][acctPosJ] == 0)){
            stepX = speed;
            stepY = 0;
            angle = 0;
            cout << "wykonal sie case 4, w lewo z dolu 1/5" << endl;
        }
        //jade w lewo z dolu 2/5
        else if((accPosInKlI == 2) & (accPosInKlJ == 2) & (carsPos[acctPosI -1][acctPosJ] == 0) & (carsPos[acctPosI -1][acctPosJ -1] == 0)){
            if((scenePos().ry()) <= (acctPosI*20+17)){
                stepX = speed;
                stepY = (speed/3)*(-1);
                angle = angle - (speed/20 *60);
            }
            else{
                stepX = 0;
                stepY = 0;
                setPos((scenePos().rx()), (acctPosI*20+17));
            }
            cout << "wykonal sie case 4, w lewo z dolu 2/5" << endl;
        }
        //jade w lewo z dolu 3/5
        else if((accPosInKlI == 1) & (accPosInKlJ == 2) & (carsPos[acctPosI][acctPosJ -1] == 0)){
            stepX = speed;
            stepY = (speed /3) *(-1);
            angle = angle - (speed/20 *50);
            cout << "wykonal sie case 4, w lewo z dolu 3/5" << endl;
        }
        //jade w lewo z dolu 4/5
        else if((accPosInKlI == 1) & (accPosInKlJ == 1) & (carsPos[acctPosI][acctPosJ -1] == 0)){
            stepX = speed;
            stepY = 0;
            angle = 270;
            cout << "wykonal sie case 4, w lewo z dolu 4/5" << endl;
        }
        //jade w lewo z dolu 5/5
        else if((accPosInKlI == 1) & (accPosInKlJ == 0) & (carsPos[acctPosI][acctPosJ -1] == 0)){
            stepX = speed;
            stepY = 0;
            angle = 270;
            cout << "wykonal sie case 4, w lewo z dolu 5/5" << endl;
        }
        else{
            stepX = 0;
            stepY = 0;
            cout << "wykonal sie case 4, else" << endl;
        }
        break;

    case 5:  //ZDP
        //jade w prawo z gory 1/3
        if((accPosInKlI == 0) & (accPosInKlJ == 1) & (carsPos[acctPosI+1][acctPosJ ] == 0) & (carsPos[acctPosI +1][acctPosJ-1] == 0)){
            if((scenePos().ry()) >= (acctPosI*20+3)){
                stepX = speed  ;
                stepY = speed / 2;
                angle = angle + (speed/20 *60);
            }
            else{
                stepX = 0  ;
                stepY = 0;
                setPos((scenePos().rx()), (acctPosI*20+3));
            }
            cout << "wykonal sie case 5,  prawo z gory 1/3" << endl;
        }
        //jade w prawo z gory 2/3
        else if ((accPosInKlI == 1) & (accPosInKlJ == 1) & (carsPos[acctPosI][acctPosJ-1] == 0)){
            stepX = speed ;
            stepY = speed / 1.5;
            angle = angle + ((speed/20)*50);
            cout << "wykonal sie case 5,  prawo z gory 2/3" << endl;
        }
        //jade w prawo z gory 3/3
        else if((accPosInKlI == 1) & (accPosInKlJ == 0) & (carsPos[acctPosI][acctPosJ-1] == 0)){
            stepY = 0;
            stepX = speed;
            angle = 270;
            cout << "wykonal sie case 5,  prawo z gory 3/3" << endl;
        }
        //jade w lewo z lewej 1/5
        else if((accPosInKlI == 2) & (accPosInKlJ == 0) & (angle ==90) & (carsPos[acctPosI][acctPosJ +1] == 0)){
            stepX = speed;
            stepY = 0;
            angle = 90;
            cout << "wykonal sie case 5, w lewo z lewej 1/5" << endl;
        }
        //jade w lewo z dolu 2/5
        else if((accPosInKlI == 2) & (accPosInKlJ == 1) & (carsPos[acctPosI][acctPosJ+1] == 0) & (carsPos[acctPosI -1][acctPosJ +1] == 0)){
            if((scenePos().ry()) >= (acctPosI*20+3)){
                stepX = speed;
                stepY = (speed /2)*(-1);
                angle = angle - (speed/20 *65);
            }
            else{
                stepX = 0;
                stepY = 0;
                setPos((scenePos().rx()), (acctPosI*20+3));
            }
            cout << "wykonal sie case 5, w lewo z lewej2/5" << endl;
        }
        //jade w lewo z lewej 3/5
        else if((accPosInKlI == 2) & (accPosInKlJ == 2) & (carsPos[acctPosI-1][acctPosJ ] == 0)){
            stepX = speed;
            stepY = (speed / 2) *(-1);
            angle = angle - (speed/20 *40);
            cout << "wykonal sie case 5, w lewo z lewej 3/5" << endl;
        }
        //jade w lewo z lewej 4/5
        else if((accPosInKlI == 1) & (accPosInKlJ == 2) & (carsPos[acctPosI-1][acctPosJ] == 0)){
            stepX = speed;
            stepY = 0;
            angle = 0;
            cout << "wykonal sie case 5, w lewo z lewej 4/5" << endl;
        }
        //jade w lewo z lewej 5/5
        else if((accPosInKlI == 0) & (accPosInKlJ == 2)  & (angle ==0) & (carsPos[acctPosI-1][acctPosJ] == 0)){
            stepX = speed;
            stepY = 0;
            angle = 0;
            cout << "wykonal sie case 5, w lewo z lewej 5/5" << endl;
        }
        else{
            stepX = 0;
            stepY = 0;
            cout << "wykonal sie case 5, else" << endl;
        }
        break;
    case 6:  //ZDL
        //jade w prawo z prawej 1/3
        if((accPosInKlI == 1) & (accPosInKlJ == 3) & (carsPos[acctPosI][acctPosJ -1] == 0) & (carsPos[acctPosI -1][acctPosJ-1] == 0)){
            if((scenePos().rx()) <= (acctPosJ*20+17)){
                stepX = speed  ;
                stepY = speed / 5;
                angle = angle + (speed/20 *50);
            }
            else{
                stepX = 0  ;
                stepY = 0;
                setPos((acctPosJ*20+17), (scenePos().ry()));
            }
            cout << "wykonal sie case 6,  prawo z prawej 1/3" << endl;
        }
        //jade w prawo z prawej 2/3
        else if ((accPosInKlI == 1) & (accPosInKlJ == 2) & (carsPos[acctPosI -1][acctPosJ] == 0)){
            stepX = speed ;
            stepY = speed / 4;
            angle = angle + ((speed/20)*50 );
            cout << "wykonal sie case 6,  prawo z prawej 2/3" << endl;
        }
        //jade w prawo z prawej 3/3
        else if((accPosInKlI == 0) & (accPosInKlJ == 2) & (carsPos[acctPosI -1][acctPosJ] == 0)){
            stepY = 0;
            stepX = speed;
            angle = 0;
            cout << "wykonal sie case 6,  prawo z prawej 3/3" << endl;
        }
        //jade w lewo z gory 1/5
        else if((accPosInKlI == 0) & (accPosInKlJ == 1) & (carsPos[acctPosI+1][acctPosJ] == 0)){
            stepX = speed;
            stepY = 0;
            angle = 180;
            cout << "wykonal sie case 6, w lewo z gory 1/5" << endl;
        }
        //jade w lewo z gory 2/5
        else if((accPosInKlI == 1) & (accPosInKlJ == 1) & (carsPos[acctPosI+1][acctPosJ] == 0) & (carsPos[acctPosI+1][acctPosJ +1] == 0)){
            if((scenePos().ry()) >= (acctPosI*20+3)){
                stepX = speed;
                stepY = (speed /2)*(-1);
                angle = angle - (speed/20 *60);
            }
            else{
                stepX = 0;
                stepY = 0;
                setPos((scenePos().rx()), (acctPosI*20+3));
            }
            cout << "wykonal sie case 6, w lewo z gory 2/5" << endl;
        }
        //jade w lewo z gory 3/5
        else if((accPosInKlI == 2) & (accPosInKlJ == 1) & (carsPos[acctPosI][acctPosJ +1] == 0)){
            stepX = speed;
            stepY = (speed / 1.5) *(-1);
            angle = angle - (speed/20 *50);
            cout << "wykonal sie case 6 w lewo z gory 3/5" << endl;
        }
        //jade w lewo z gory 4/5
        else if((accPosInKlI == 2) & (accPosInKlJ == 2) & (carsPos[acctPosI][acctPosJ +1] == 0)){
            stepX = speed;
            stepY = 0;
            angle = 90;
            cout << "wykonal sie case 6, w lewo z gory 4/5" << endl;
        }
        //jade w lewo z gory 5/5
        else if((accPosInKlI == 2) & (accPosInKlJ == 3) & (carsPos[acctPosI][acctPosJ +1] == 0)){
            stepX = speed;
            stepY = 0;
            angle = 90;
            cout << "wykonal sie case 6, w lewo z gory 5/5" << endl;
        }
        else{
            stepX = 0;
            stepY = 0;
            cout << "wykonal sie case 6, else" << endl;
        }
         break;
    case 7:
       //  START Z DOŁU
       // jade z dolu na gore lub w prawo, wybieram kierunek ruchu
       if((accPosInKlI == 3) & (accPosInKlJ == 2) & (turn7 == 9) & (carsPos[acctPosI -1][acctPosJ] == 0) & (carsPos[acctPosI -1][acctPosJ +1] == 0)  & (carsPos[acctPosI -2][acctPosJ +1] == 0) & (carsPos[acctPosI -2][acctPosJ] == 0) & (carsPos[acctPosI -3][acctPosJ] == 0)){
           qsrand((unsigned)time(0));
           turn7 = qrand() %2;
       }
       //moge tylko w prawo
       else if((accPosInKlI == 3) & (accPosInKlJ == 2)& (turn7 == 9) & (carsPos[acctPosI -1][acctPosJ] == 0) & (carsPos[acctPosI -1][acctPosJ +1] == 0)){
           turn7=0;
       }
       // moge tylko prosto
       else if((accPosInKlI == 3) & (accPosInKlJ == 2)& (turn7 == 9) & (carsPos[acctPosI -1][acctPosJ] == 0) & (carsPos[acctPosI -2][acctPosJ] == 0) & (carsPos[acctPosI -3][acctPosJ] == 0) & (carsPos[acctPosI -4][acctPosJ] == 0)){
           turn7=1;
       }
       // START Z PRAWEJw prawo lub w lewo, wybieram kierunek ruchu
       if((accPosInKlI == 1) & (accPosInKlJ == 3)& (turn7 == 9) & (carsPos[acctPosI][acctPosJ -1] == 0) & (carsPos[acctPosI -1][acctPosJ -1] == 0) & (carsPos[acctPosI -1][acctPosJ -2] == 0) & (carsPos[acctPosI][acctPosJ -2] == 0) & (carsPos[acctPosI +1][acctPosJ -2] == 0)){
           qsrand((unsigned)time(0));
           turn7 = (qrand() %2)+2;
       }
       // Moge tylko w prawo
       else if((accPosInKlI == 1) & (accPosInKlJ == 3)& (turn7 == 9) & (carsPos[acctPosI][acctPosJ -1] == 0) & (carsPos[acctPosI -1][acctPosJ -1] == 0)) {
           turn7 = 2;
       }
       // START Z GÓRY w lewo lub w dol, wybieram kierunek ruchu
       if((accPosInKlI == 0) & (accPosInKlJ == 1)& (turn7 == 9) & (carsPos[acctPosI+1][acctPosJ] == 0) & (carsPos[acctPosI+2][acctPosJ] == 0) & (carsPos[acctPosI+2][acctPosJ+1] == 0) & (carsPos[acctPosI+2][acctPosJ+2] == 0)  & (carsPos[acctPosI+3][acctPosJ+1] == 0)){
           qsrand((unsigned)time(0));
           turn7 = (qrand() %2)+4;
       }
       // moge tylko prosto
       else if((accPosInKlI == 0) & (accPosInKlJ == 1)& (turn7 == 9) & (carsPos[acctPosI+1][acctPosJ] == 0) & (carsPos[acctPosI+2][acctPosJ] == 0)) {
           turn7 = 4;
       }
       //******************************************
       // PROSTO Z DOLU
       // Prosto z Dolu 1/4
       if((turn7 == 0) & (accPosInKlI == 3) & (accPosInKlJ == 2) & (carsPos[acctPosI -1][acctPosJ] == 0) & (carsPos[acctPosI -2][acctPosJ] == 0) & (carsPos[acctPosI -2][acctPosJ+1] == 0)){
            stepX = speed;
            stepY = 0;
            angle = 0;
            cout << " wykonal sie case 7 prosto z dolu 1/4" << endl;
       }
       // prosto z dolu 2/4
       else if((accPosInKlI == 2) & (accPosInKlJ == 2) & (turn7 == 0) & (carsPos[acctPosI - 1][acctPosJ] == 0)& (carsPos[acctPosI - 1][acctPosJ+1] == 0)){
           stepX = speed;
           stepY = 0;
           angle = 0;

           cout << " wykonal sie case 7 prosto z dolu 2/4" << endl;
       }
       //prosto, 3/4
       else if((accPosInKlI == 1) & (accPosInKlJ == 2) & (turn7 == 0) & (carsPos[acctPosI - 1][acctPosJ] == 0)){
           stepX = speed;
           stepY = 0;
           angle = 0;
           cout << " wykonal sie case 7, wdol 3/4" << endl;
       }
       //prosto, 4/4
       else if((accPosInKlI == 0) & (accPosInKlJ == 2) & (turn7 == 0) & (carsPos[acctPosI - 1][acctPosJ] == 0)){
           stepX = speed;
           stepY = 0;
           angle = 0;
           turn7 = 9;
           cout << " wykonal sie case 7, w dol 4/4" << endl;
       }
       // W PRAWO  Z DOŁU
       // w Prawo z dolu 1/3
       else if((accPosInKlI == 3) & (accPosInKlJ == 2) & (turn7 == 1) & (carsPos[acctPosI][acctPosJ -1] == 0) & (carsPos[acctPosI -1][acctPosJ -1] == 0)){
           if((scenePos().ry()) <= (acctPosI*20+17)){
               stepX = speed  ;
               stepY = speed / 3;
               angle = angle + ((speed/20) *50);
           }
           else{
               stepX = 0  ;
               stepY = 0;
               setPos((scenePos().rx()), (acctPosI*20+17));
           }
            cout << " wykonal sie case 7 w prawo z dolu 1/3" << endl;
       }
       //w Prawo z dolu  2/3
       else if((accPosInKlI == 2) & (accPosInKlJ == 2) & (turn7 == 1) & (carsPos[acctPosI][acctPosJ + 1] == 0)){
                stepX = speed ;
                stepY = speed / 4;
                angle = angle + (speed/20 *55);
                cout << " wykonal sie case 7 w prawo z dolu 2/3" << endl;
       }
       //w Prawo z dolu , 3/3
       else if((accPosInKlI == 2) & (accPosInKlJ == 3) & (carsPos[acctPosI][acctPosJ + 1] == 0)){
           stepY = 0;
           stepX = speed;
           angle = 90;
           turn7 = 9;
           cout << " wykonal sie case 7 w prawo z dolu  3/3" << endl;
       }
       //**********************************************************************************
       // PRAWO Z PRAWEJ
       // turn == 0 - prawo z prawej 1/3
       else if((accPosInKlI == 1) & (accPosInKlJ == 3) & (turn7 == 2)& (carsPos[acctPosI ][acctPosJ-1] == 0)){
           if((scenePos().rx()) <= (acctPosJ*20+17)){
               stepX = speed  ;
               stepY = speed / 4;
               angle = angle + ((speed/20) *50);
           }
           else{
               stepX = 0  ;
               stepY = 0;
               setPos((acctPosJ*20+17), (scenePos().ry()));
           }
           cout << " wykonal sie case 7 prawo z prawej 1/3" << endl;
       }
       //jade w prawo z prawej 2/3
       else if((accPosInKlI == 1) & (accPosInKlJ == 2) & (turn7==2) & (carsPos[acctPosI -1][acctPosJ] == 0)){
           stepX = speed ;
           stepY = speed / 3;
           angle = angle + (speed/20 *50);
           cout << "wykonal sie case 7,  prawo z prawej 2/3" << endl;
       }
       //jade w prawo z prawej 3/3
       else if((accPosInKlI == 0) & (accPosInKlJ == 2) & (carsPos[acctPosI -1][acctPosJ] == 0)){
           stepX = speed;
           stepY = 0;
           angle = 0;
           turn7 = 9;
           cout << "wykonal sie case 7,  prawo z prawej 3/3" << endl;
       }
       // LEWO Z PRAWEJ
       //jade w  lewo z prawej 1/5
       else if((accPosInKlI == 1) & (accPosInKlJ == 3) & (turn7 == 3) &(carsPos[acctPosI][acctPosJ-1] == 0)){
           stepX = speed;
           stepY = 0;
           angle = 270;
           cout << " wykonal sie case 7 w lewo z prawej 1/5" << endl;
       }
       // lewo z prawej 2/5
       else if((accPosInKlI == 1) & (accPosInKlJ == 2) & (turn7==3) & (carsPos[acctPosI - 1][acctPosJ-1] == 0)  & (carsPos[acctPosI][acctPosJ-1] == 0)& (carsPos[acctPosI+1][acctPosJ-1] == 0)){
               if((scenePos().rx()) <= (acctPosJ*20+17)){
        stepX = speed;
        stepY = (speed /1.5)*(-1);
        angle = angle - (speed/20 *65);
    }
    else{
        stepX = 0;
        stepY = 0;
        setPos((acctPosJ*20+17), (scenePos().ry()));
    }
           cout << "wykonal sie case 7,  lewo z prawej 2/5" << endl;
       }
       // lewo z prawej 3/5
       else if((accPosInKlI == 1) & (accPosInKlJ == 1) & (turn7==3) & (carsPos[acctPosI+1][acctPosJ] == 0)){
               stepX = speed;
    stepY = (speed / 2) *(-1);
    angle = angle - (speed/20 *65);
           cout << "wykonal sie case 7,  lewo z prawej 3/5" << endl;
       }
       // lewo z prawej 4/5
       else if((accPosInKlI == 2) & (accPosInKlJ == 1) & (turn7==3) & (carsPos[acctPosI+1][acctPosJ ] == 0)){
           stepX = speed;
           stepY = 0;
           angle = 180;
           cout << "wykonal sie case 7,  lewo z prawej 4/5" << endl;
       }
       // lewo z prawej 5/5
       else if((accPosInKlI == 3) & (accPosInKlJ == 1) & (carsPos[acctPosI+1][acctPosJ] == 0)){
           stepX = speed;
           stepY = 0;
           angle = 180;
           turn7 = 9;
           cout << "wykonal sie case 7,  lewo z prawej 5/5" << endl;
       }
       //**********************************************************************************
       // Prosto z Góry 1/4
       else if((accPosInKlI == 0) & (accPosInKlJ == 1)& (turn7 == 4) & (carsPos[acctPosI+1][acctPosJ] == 0)){
           stepX = speed;
           stepY = 0;
           angle = 180;
           cout << " wykonal sie case 7 prosto z gory 1/4" << endl;
       }
       // prosto z gory , 2/4
       else if((accPosInKlI == 1) & (accPosInKlJ == 1) & (turn7 == 4) & (carsPos[acctPosI+1][acctPosJ] == 0)){
           stepX = speed;
           stepY = 0;
           angle = 180;
           cout << " wykonal sie case 7 prosto z gory  2/4" << endl;
       }
       //prosto z gory , 3/4
       else if((accPosInKlI == 2) & (accPosInKlJ == 1) & (turn7 == 4) & (carsPos[acctPosI+1][acctPosJ] == 0)){
           stepX = speed;
           stepY = 0;
           angle = 180;
           cout << " wykonal sie case 7, prosto z gory 3/4" << endl;
       }
       //prosto z gory , 4/4
       else if((accPosInKlI == 3) & (accPosInKlJ == 1) & (turn7 == 4) & (carsPos[acctPosI+1][acctPosJ] == 0)){
           stepX = speed;
           stepY = 0;
           angle = 180;
           turn7 = 9;
           cout << " wykonal sie case 7, prosto z gory 4/4" << endl;
       }
       //W Lewo Z Góry
       //jade w  lewo z gory 1/5
       else if((accPosInKlI == 0) & (accPosInKlJ == 1) & (turn7 == 5) &(carsPos[acctPosI+1][acctPosJ] == 0)){
       stepX = speed;
       stepY = 0;
       angle = 180;
       cout << " wykonal sie case 7 w lewo z gory 1/5" << endl;
       }
       // lewo z gory 2/5
       else if((accPosInKlI == 1) & (accPosInKlJ == 1) & (turn7==5) & (carsPos[acctPosI +1][acctPosJ] == 0)  & (carsPos[acctPosI+1][acctPosJ] == 0)& (carsPos[acctPosI+1][acctPosJ+1] == 0)){
               if((scenePos().ry()) >= (acctPosI*20+3)){
            stepX = speed;
            stepY = (speed /2)*(-1);
            angle = angle - (speed/20 *50);
        }
        else{
            stepX = 0;
            stepY = 0;
            setPos((scenePos().rx()), (acctPosI*20+3));
        }
           cout << "wykonal sie case 7,  lewo z gory 2/5" << endl;
       }
       // lewo z gory 3/5
       else if((accPosInKlI == 2) & (accPosInKlJ == 1) & (turn7==5) & (carsPos[acctPosI+1][acctPosJ+1] == 0)  & (carsPos[acctPosI][acctPosJ+1] == 0)){
               stepX = speed;
                stepY = (speed / 2) *(-1);
                angle = angle - (speed/20 *53);
           cout << "wykonal sie case 7,  lewo z gory 3/5" << endl;
       }
       // lewo z gory 4/5
       else if((accPosInKlI == 2) & (accPosInKlJ == 2) & (carsPos[acctPosI][acctPosJ+1 ] == 0)){
           stepX = speed;
           stepY = 0;
           angle = 90;
           cout << "wykonal sie case 7,  lewo z gory 4/5" << endl;
       }
       // lewo z gory 5/5
       else if((accPosInKlI == 2) & (accPosInKlJ == 3) & (carsPos[acctPosI][acctPosJ+1] == 0)){
           stepX = speed;
           stepY = 0;
           angle = 90;
           turn7 = 9;
           cout << "wykonal sie case 7,  lewo z gory 5/5" << endl;
       }
       else{
           stepX = 0;
           stepY = 0;
           cout << "wykonal sie case 7, else" << endl;
       }
       cout << "wartosc turn7 " << turn7 << endl;
       break;
    case 8:
       // Start z gory
       // jade z gory na dol lub w prawo, wybieram kierunek ruchu 0-prosto, 1-prawo
       if((accPosInKlI == 0) & (accPosInKlJ == 1) & (turn8 == 9) & (carsPos[acctPosI +1][acctPosJ] == 0)  & (carsPos[acctPosI +1][acctPosJ-1] == 0) & (carsPos[acctPosI +2][acctPosJ] == 0) & (carsPos[acctPosI +3][acctPosJ] == 0)  & (carsPos[acctPosI +2][acctPosJ-1] == 0)){
           qsrand((unsigned)time(0));
           turn8 = qrand() %2;
       }
       //moge tylko w prawo
       else if((accPosInKlI == 0) & (accPosInKlJ == 1) & (turn8 == 9) & (carsPos[acctPosI +1][acctPosJ] == 0)  & (carsPos[acctPosI +1][acctPosJ-1] == 0)){
           turn8=0;
       }
       // moge tylko prosto
       else if((accPosInKlI == 0) & (accPosInKlJ == 1) & (turn8 == 9) & (carsPos[acctPosI +1][acctPosJ] == 0) & (carsPos[acctPosI +2][acctPosJ] == 0) & (carsPos[acctPosI +3][acctPosJ] == 0)  & (carsPos[acctPosI +2][acctPosJ-1] == 0)){
           turn8=1;
       }
       // START Z Lewej w prawo lub w lewo, wybieram kierunek ruchu 2- prawo, 3 - lewo
       if((accPosInKlI == 2) & (accPosInKlJ == 0) & (turn8 == 9) & (carsPos[acctPosI][acctPosJ +1] == 0) & (carsPos[acctPosI +1][acctPosJ] == 0) & (carsPos[acctPosI][acctPosJ +2] == 0) & (carsPos[acctPosI-1][acctPosJ +2] == 0) & (carsPos[acctPosI -2][acctPosJ +2] == 0)){
           qsrand((unsigned)time(0));
           turn8 = (qrand() %2)+2;
       }
       else if((accPosInKlI == 2) & (accPosInKlJ == 0) & (turn8 == 9) & (carsPos[acctPosI][acctPosJ +1] == 0) & (carsPos[acctPosI +1][acctPosJ +1] == 0)) {
           turn8 = 2;
       }
       // START Z Dołu prosto lub w lewo, wybieram kierunek ruchu 4 -prosto, 5- lewo
       if((accPosInKlI == 3) & (accPosInKlJ == 2) & (turn8 == 9) & (carsPos[acctPosI-1][acctPosJ] == 0) & (carsPos[acctPosI-2][acctPosJ] == 0) & (carsPos[acctPosI-2][acctPosJ-1] == 0) & (carsPos[acctPosI-2][acctPosJ-2] == 0)){
           qsrand((unsigned)time(0));
           turn8 = (qrand() %2)+4;
       }
       else if((accPosInKlI == 3) & (accPosInKlJ == 2) & (turn8 == 9) & (carsPos[acctPosI-1][acctPosJ] == 0) & (carsPos[acctPosI+2][acctPosJ] == 0)) {
           turn8 = 4;
       }
       //***********************************************
       // START Z GORY
       // Prosto z gory 1/4
       if((accPosInKlI == 0) & (accPosInKlJ == 1) & (carsPos[acctPosI +1][acctPosJ] == 0)&(turn8 == 0)){
           stepX = speed;
           stepY = 0;
           angle = 180;
           cout << " wykonal sie case 8 prosto z gory 1/4" << endl;
       }
       // Prosto z gory 2/4
       else if((accPosInKlI == 1) & (accPosInKlJ == 1) & (turn8 == 0) & (carsPos[acctPosI + 1][acctPosJ] == 0)){
           stepX = speed;
           stepY = 0;
           angle = 180;
           cout << " wykonal sie case 8 prosto z gory 2/4" << endl;
       }
       //Prosto z gory 3/4
       else if((accPosInKlI == 2) & (accPosInKlJ == 1) & (turn8 == 0) & (carsPos[acctPosI + 1][acctPosJ] == 0)){
           stepX = speed;
           stepY = 0;
           angle = 180;
           cout << " wykonal sie case 8 prosto z gory 3/4" << endl;
       }
       //Prosto z gory 4/4
       else if((accPosInKlI == 3) & (accPosInKlJ == 1) & (carsPos[acctPosI + 1][acctPosJ] == 0)){
           stepX = speed;
           stepY = 0;
           angle = 180;
           turn8 = 9;
           cout << " wykonal sie case 8 prosto z gory 4/4" << endl;
       }
       // W Prawo z gory 1/3
       else if((accPosInKlI == 0) & (accPosInKlJ == 1) & (turn8 == 1)){
           if((scenePos().ry()) >= (acctPosI*20+3)){
               stepX = speed  ;
               stepY = speed / 2;
               angle = angle + ((speed/20) *53);
           }
           else{
               stepX = 0  ;
               stepY = 0;
               setPos((scenePos().rx()), (acctPosI*20+3));
           }
           cout << " wykonal sie case 8 w prawo z gory 1/3" << endl;
       }
       //  Prawo z gory 2/3
       else if((accPosInKlI == 1) & (accPosInKlJ == 1) & (turn8 == 1) & (carsPos[acctPosI][acctPosJ - 1] == 0)){
               stepX = speed ;
    stepY = speed / 2;
    angle = angle + (speed/20 *50);
           cout << " wykonal sie case 8 w prawo z gory  2/3" << endl;
       }
       //  Prawo z gory 3/3
       else if((accPosInKlI == 1) & (accPosInKlJ == 0) & (carsPos[acctPosI][acctPosJ - 1] == 0)){
           stepY = 0;
           stepX = speed;
           turn8 = 9;
           angle = 270;
           cout << " wykonal sie case 8 w prawo z gory  3/3" << endl;
       }
       //**********************************************************************************
       // PRAWO Z Lewej
       // turn == 2 - prawo z lewej 1/3
       else if((accPosInKlI == 2) & (accPosInKlJ == 0) & (turn8 == 2)& (carsPos[acctPosI ][acctPosJ+1] == 0)){
           if((scenePos().rx()) >= (acctPosJ*20+3)){
               stepX = speed  ;
               stepY = speed / 2;
               angle = angle + ((speed/20) *53);
           }
           else{
               stepX = 0  ;
               stepY = 0;
               setPos((acctPosJ*20+3), (scenePos().ry()));
           }
            cout << " wykonal sie case 8 prawo z lewej 1/3" << endl;
        }
        //jade w pPRAWO Z Lewej 2/3
        else if((accPosInKlI == 2) & (accPosInKlJ == 1) & (turn8==2) & (carsPos[acctPosI+1][acctPosJ] == 0)){
               stepX = speed ;
                stepY = speed / 2;
                angle = angle + (speed/20 *53);
            cout << "wykonal sie case 8 prawo z lewej 2/3" << endl;
        }
        //jade w PRAWO Z Lewej 3/3
        else if((accPosInKlI == 3) & (accPosInKlJ == 1) & (carsPos[acctPosI+1][acctPosJ] == 0)){
            stepX = speed;
            stepY = 0;
            angle = 180;
            turn8 = 9;
            cout << "wykonal sie case 8 prawo z lewej 3/3" << endl;
        }
        // LEWO Z LEWEJ
        //jade w  lewo z lewej 1/5
        else if((accPosInKlI == 2) & (accPosInKlJ == 0) & (turn8 == 3) &(carsPos[acctPosI][acctPosJ+1] == 0)){
        stepX = speed;
        stepY = 0;
        angle = 90;
        cout << " wykonal sie case 8 w lewo z lewej 1/5" << endl;
        }
        // lewo z lewej 2/5
        else if((accPosInKlI == 2) & (accPosInKlJ == 1) & (turn8==3) & (carsPos[acctPosI][acctPosJ+1] == 0)  & (carsPos[acctPosI-1][acctPosJ+1] == 0)& (carsPos[acctPosI-2][acctPosJ+1] == 0)){
               if((scenePos().rx()) >= (acctPosJ*20+3)){
        stepX = speed;
        stepY = (speed /1.5)*(-1);
        angle = angle - (speed/20 *65);
    }
    else{
        stepX = 0;
        stepY = 0;
        setPos((acctPosJ*20+3), (scenePos().ry()));
    }
            cout << "wykonal sie case 8 w lewo z lewej 2/5" << endl;
        }
        // lewo z lewej 3/5
        else if((accPosInKlI == 2) & (accPosInKlJ == 2) & (turn8==3) & (carsPos[acctPosI-1][acctPosJ] == 0)& (carsPos[acctPosI-2][acctPosJ+1] == 0)){
               stepX = speed;
    stepY = (speed / 2) *(-1);
    angle = angle - (speed/20 *65);
            cout << "wykonal sie case 8 w lewo z lewej 3/5" << endl;
        }
        // lewo z lewej 4/5
        else if((accPosInKlI == 1) & (accPosInKlJ == 2) & (turn8==3) & (carsPos[acctPosI-1][acctPosJ ] == 0)){
            stepX = speed;
            stepY = 0;
            angle = 0;
            cout << "wykonal sie case 8 w lewo z lewej 4/5" << endl;
        }
        // lewo z lewej 5/5
        else if((accPosInKlI == 0) & (accPosInKlJ == 2) & (carsPos[acctPosI-1][acctPosJ] == 0)){
            stepX = speed;
            stepY = 0;
            angle = 0;
            turn8 = 9;
            cout << "wykonal sie case 8 w lewo z lewej 5/5" << endl;
        }
        //**********************************************************************************
        // Prosto z Dołu
        // Prosto z Dołu 1/4
        else if((accPosInKlI == 3) & (accPosInKlJ == 2)& (turn8 == 4) & (carsPos[acctPosI-1][acctPosJ] == 0)){
            stepX = speed;
            stepY = 0;
            angle = 0;
            cout << " wykonal sie case 8 prosto z dolu 1/4" << endl;
        }
        // prosto z Dołu , 2/4
        else if((accPosInKlI == 2) & (accPosInKlJ == 2) & (turn8 == 4) & (carsPos[acctPosI-1][acctPosJ] == 0)){
            stepX = speed;
            stepY = 0;
            angle = 0;
            cout << " wykonal sie case 8 prosto z dolu  2/4" << endl;
        }
        //prosto z Dołu , 3/4
        else if((accPosInKlI == 1) & (accPosInKlJ == 2) & (turn8 == 4) & (carsPos[acctPosI-1][acctPosJ] == 0)){
            stepX = speed;
            angle = 0;
            cout << " wykonal sie case 8 prosto z dolu 3/4" << endl;
        }
        //prosto z Dołu , 4/4
        else if((accPosInKlI ==0) & (accPosInKlJ == 2) & (carsPos[acctPosI-1][acctPosJ] == 0)){
            stepX = speed;
            stepY = 0;
            angle = 0;
            turn8 = 9;
            cout << " wykonal sie case 8 prosto z dolu 4/4" << endl;
        }
        //W Lewo Z Dolu
        //jade w  lewo z dolu 1/5
        else if((accPosInKlI == 3) & (accPosInKlJ == 2) & (turn8 == 5) &(carsPos[acctPosI-1][acctPosJ] == 0)){
        stepX = speed;
        stepY = 0;
        angle = 0;
        cout << " wykonal sie case 8 w lewo z dolu 1/5" << endl;
        }
        // lewo z dolu 2/5
        else if((accPosInKlI == 2) & (accPosInKlJ == 2) & (turn8==5) & (carsPos[acctPosI -1][acctPosJ] == 0)  & (carsPos[acctPosI-1][acctPosJ-1] == 0)& (carsPos[acctPosI-1][acctPosJ-2] == 0)){
               if((scenePos().ry()) <= (acctPosI*20+17)){
        stepX = speed;
        stepY = (speed /1.5)*(-1);
        angle = angle - (speed/20 *65);
    }
    else{
        stepX = 0;
        stepY = 0;
        setPos((scenePos().rx()), (acctPosI*20+17));
    }
            cout << "wykonal sie case 8 w lewo z dolu 2/5" << endl;
        }
        // lewo z dolu 3/5
        else if((accPosInKlI == 1) & (accPosInKlJ == 2) & (turn8==5) & (carsPos[acctPosI][acctPosJ-1] == 0)){
               stepX = speed;
    stepY = (speed / 2) *(-1);
    angle = angle - (speed/20 *65);
            cout << "wykonal sie case 8 w lewo z dolu 3/5" << endl;
        }
        // lewo z dolu 4/5
        else if((accPosInKlI == 1) & (accPosInKlJ == 1) & (turn8==5) & (carsPos[acctPosI][acctPosJ-1 ] == 0)){
            stepX = speed;
            stepY = 0;
            angle = 270;
            cout << "wykonal sie case 8 w lewo z dolu 4/5" << endl;
        }
        // lewo z dolu 5/5
        else if((accPosInKlI == 1) & (accPosInKlJ == 0) & (carsPos[acctPosI][acctPosJ-1] == 0)){
            stepX = speed;
            stepY = 0;
            angle = 0;
            turn8 = 9;
            cout << "wykonal sie case 8 w lewo z dolu 5/5" << endl;
        }
       else{
           stepX = 0;
           stepY = 0;
           cout << "wykonal sie case 8, else" << endl;
       }
       cout << "wartosc turn8 " << turn8 << endl;
       break;

       //  CASE 9*****************************************************
    case 9:
       // Start z prawej,  prosto lub w prawo, wybieram kierunek ruchu
       if((accPosInKlI == 1) & (accPosInKlJ == 3)& (turn9 == 9) & (carsPos[acctPosI][acctPosJ - 1] == 0) & (carsPos[acctPosI -1][acctPosJ - 1] == 0) & (carsPos[acctPosI][acctPosJ - 2] == 0) & (carsPos[acctPosI][acctPosJ - 3] == 0)  & (carsPos[acctPosI-1][acctPosJ - 2] == 0)){
           qsrand((unsigned)time(0));
           turn9 = qrand() %2;
       }
       //moge tylko w prawo
       else if((accPosInKlI == 1) & (accPosInKlJ == 3)& (turn9 == 9) & (carsPos[acctPosI][acctPosJ - 1] == 0) & (carsPos[acctPosI -1][acctPosJ - 1] == 0) & (carsPos[acctPosI][acctPosJ - 2] == 0)){
           turn9=0;
       }
       // moge tylko prosto
       else if((accPosInKlI == 1) & (accPosInKlJ == 3)& (turn9 == 9) & (carsPos[acctPosI][acctPosJ - 1] == 0) & (carsPos[acctPosI][acctPosJ - 2] == 0) & (carsPos[acctPosI][acctPosJ - 3] == 0)  & (carsPos[acctPosI-1][acctPosJ - 2] == 0)){
           turn9=1;
       }
       // START Z Gory w prawo lub w lewo, wybieram kierunek ruchu 2- prawo, 3 - lewo
       if((accPosInKlI == 0) & (accPosInKlJ == 1)& (turn9 == 9) & (carsPos[acctPosI+1][acctPosJ] == 0) & (carsPos[acctPosI+1][acctPosJ-1] == 0) & (carsPos[acctPosI+2][acctPosJ] == 0) & (carsPos[acctPosI+2][acctPosJ +1] == 0) & (carsPos[acctPosI +2][acctPosJ +2] == 0)){
           qsrand((unsigned)time(0));
           turn9 = (qrand() %2)+2;
       }
       else if((accPosInKlI == 0) & (accPosInKlJ == 1) & (turn9 == 9)& (carsPos[acctPosI+1][acctPosJ] == 0) & (carsPos[acctPosI+1][acctPosJ-1] == 0)) {
           turn9 = 2;
       }
       // START Z Lewej prosto lub w lewo, wybieram kierunek ruchu 4 -prosto, 5- lewo
       if((accPosInKlI == 2) & (accPosInKlJ == 0)& (turn9 == 9) & (carsPos[acctPosI][acctPosJ+1] == 0) & (carsPos[acctPosI][acctPosJ+2] == 0) & (carsPos[acctPosI-1][acctPosJ+2] == 0) & (carsPos[acctPosI-2][acctPosJ+2] == 0)){
           qsrand((unsigned)time(0));
           turn9 = (qrand() %2)+4;
       }
       else if((accPosInKlI == 2) & (accPosInKlJ == 0)& (turn9 == 9) & (carsPos[acctPosI][acctPosJ+1] == 0) & (carsPos[acctPosI][acctPosJ+2] == 0)) {
           turn9 = 4;
       }
       //*********************************************
       // Prosto z Prawej 1/4
       if((accPosInKlI == 1) & (accPosInKlJ == 3) & (carsPos[acctPosI][acctPosJ+1] == 0) & (carsPos[acctPosI][acctPosJ+2] == 0)& (turn9 == 0)){
           stepX = speed;
           stepY = 0;
           angle = 270;
           cout << " wykonal sie case 9 prosto  z prawej 1/4" << endl;
       }
       // prosto z Prawej 2/4
       else if((accPosInKlI == 1) & (accPosInKlJ == 2) & (turn9 == 0) & (carsPos[acctPosI][acctPosJ - 1] == 0)){
           stepX = speed;
           stepY = 0;
           angle = 270;

           cout << " wykonal sie case 9 prosto z Prawej 2/4" << endl;
       }
       //prosto z Prawej 3/4
       else if((accPosInKlI == 1) & (accPosInKlJ == 1) & (turn9 == 0) & (carsPos[acctPosI][acctPosJ -1] == 0)){
           stepX = speed;
           stepY = 0;
           angle = 270;
           cout << " wykonal sie case 9, prosto z Prawej 3/4" << endl;
       }
       //prosto z Prawej 4/4
       else if((accPosInKlI == 1) & (accPosInKlJ == 0) & (carsPos[acctPosI ][acctPosJ -1] == 0)){
           stepX = speed;
           stepY = 0;
           angle = 270;
           turn9 =9;
           cout << " wykonal sie case 9,prosto z Prawej 4/4" << endl;
       }
       // W prawo z prawej 1/3
       else if((accPosInKlI == 1) & (accPosInKlJ == 3) & (carsPos[acctPosI][acctPosJ-1] == 0) & (carsPos[acctPosI-1][acctPosJ-1] == 0) & (turn9 == 1)){
           if((scenePos().rx()) <= (acctPosJ*20+17)){
               stepX = speed  ;
               stepY = speed / 5;
               angle = angle + ((speed/20) *50);
           }
           else{
               stepX = 0  ;
               stepY = 0;
               setPos((acctPosJ*20+17), (scenePos().ry()));
           }
           cout << " wykonal sie case 9 w prawo z prawej 1/3" << endl;
       }
       //W prawo z prawej  2/3
       else if((accPosInKlI == 1) & (accPosInKlJ == 2) & (turn9 == 1) & (carsPos[acctPosI -1][acctPosJ] == 0)){
               stepX = speed ;
            stepY = speed / 4;
            angle = angle + (speed/20 *50);
           cout << " wykonal sie case 9  w prawo z prawej 2/3" << endl;
       }
       //W prawo z prawej 3/3
       else if((accPosInKlI == 0) & (accPosInKlJ == 2) & (carsPos[acctPosI -1][acctPosJ] == 0)){
           stepY = 0;
           stepX = speed;
           angle = 0;
           turn9 =9;
           cout << " wykonal sie case 9  w prawo z prawej 3/3" << endl;
       }
       //**********************************************************************************
       // PRAWO Z Gory
       // turn == 2 - prawo z gory 1/3
       else if((accPosInKlI == 0) & (accPosInKlJ == 1) & (turn9 == 2)& (carsPos[acctPosI+1][acctPosJ] == 0)){
           if((scenePos().ry()) >= (acctPosI*20+3)){
               stepX = speed  ;
               stepY = speed / 2;
               angle = angle + ((speed/20) *53);
           }
           else{
               stepX = 0  ;
               stepY = 0;
               setPos((scenePos().rx()), (acctPosI*20+3));
           }
           cout << " wykonal sie case 9 prawo z gory 1/3" << endl;
       }
       //jade w pPRAWO Z gory 2/3
       else if((accPosInKlI == 1) & (accPosInKlJ == 1) & (turn9==2) & (carsPos[acctPosI][acctPosJ-1] == 0)){
               stepX = speed ;
               stepY = speed / 2;
               angle = angle + (speed/20 *53);
           cout << "wykonal sie case 9 prawo z gory 2/3" << endl;
       }
       //jade w PRAWO Z gory 3/3
       else if((accPosInKlI == 1) & (accPosInKlJ == 0) & (carsPos[acctPosI][acctPosJ-1] == 0)){
           stepX = speed;
           stepY = 0;
           angle = 270;
           turn9 =9;
           cout << "wykonal sie case 9 prawo z gory 3/3" << endl;
       }
       // LEWO Z gory
       //jade w  lewo z prawej 1/5
       else if((accPosInKlI == 0) & (accPosInKlJ == 1) & (turn9 == 3) &(carsPos[acctPosI+1][acctPosJ] == 0)){
           stepX = speed;
           stepY = 0;
           angle = 180;
           cout << " wykonal sie case 9 w lewo z gory 1/5" << endl;
       }
       // lewo z gory 2/5
       else if((accPosInKlI == 1) & (accPosInKlJ == 1) & (turn9==3) & (carsPos[acctPosI+1][acctPosJ] == 0)  & (carsPos[acctPosI+2][acctPosJ] == 0)& (carsPos[acctPosI+2][acctPosJ+1] == 0)){
               if((scenePos().ry()) >= (acctPosI*20+3)){
                    stepX = speed;
                    stepY = (speed /1.5)*(-1);
                    angle = angle - (speed/20 *65);
    }
    else{
        stepX = 0;
        stepY = 0;
        setPos((scenePos().rx()), (acctPosI*20+3));
    }
           cout << "wykonal sie case 9 w lewo z gory 2/5" << endl;
       }
       // lewo z gory 3/5
       else if((accPosInKlI == 2) & (accPosInKlJ ==1) & (turn9==3) & (carsPos[acctPosI][acctPosJ+1] == 0)& (carsPos[acctPosI][acctPosJ+2] == 0)){
               stepX = speed;
    stepY = (speed / 2) *(-1);
    angle = angle - (speed/20 *65);
           cout << "wykonal sie case 9 w lewo z gory 3/5" << endl;
       }
       // lewo z gory 4/5
       else if((accPosInKlI == 2) & (accPosInKlJ == 2) & (turn9==3) & (carsPos[acctPosI][acctPosJ+1 ] == 0)){
           stepX = speed;
           stepY = 0;
           angle = 90;
           cout << "wykonal sie case 9 w lewo z gory 4/5" << endl;
       }
       // lewo z gory 5/5
       else if((accPosInKlI == 2) & (accPosInKlJ == 3) & (carsPos[acctPosI][acctPosJ+1] == 0)){
           stepX = speed;
           stepY = 0;
           angle = 90;
           turn9 =9;
           cout << "wykonal sie case 9 w lewo z gory 5/5" << endl;
       }
       //**********************************************************************************
       // Prosto z Lewej
       // Prosto z Lewej 1/4
       else if((accPosInKlI == 2) & (accPosInKlJ == 0)& (turn9 == 4) & (carsPos[acctPosI][acctPosJ+1] == 0) & (carsPos[acctPosI][acctPosJ+2] == 0)){
           stepX = speed;
           stepY = 0;
           angle = 90;
           cout << " wykonal sie case 9 prosto z Lewej 1/4" << endl;
       }
       // prosto zLewej , 2/4
       else if((accPosInKlI == 2) & (accPosInKlJ == 1) & (turn9 == 4) & (carsPos[acctPosI][acctPosJ+1] == 0)){
           stepX = speed;
           stepY = 0;
           angle = 90;
           cout << " wykonal sie case 9 prosto z Lewej 2/4" << endl;
       }
       //prosto z Lewej , 3/4
       else if((accPosInKlI == 2) & (accPosInKlJ == 2) & (turn9 == 4) & (carsPos[acctPosI-1][acctPosJ] == 0)){
           stepX = speed;
           stepY = 0;
           angle = 90;
           cout << " wykonal sie case 9 prosto z Lewej 3/4" << endl;
       }
       //prosto z Lewej , 4/4
       else if((accPosInKlI ==2) & (accPosInKlJ == 3) & (carsPos[acctPosI-1][acctPosJ] == 0)){
           stepX = speed;
           stepY = 0;
           angle = 90;
           turn9 =9;
           cout << " wykonal sie case 10 prosto z Lewej 4/4" << endl;
       }
       //W Lewo Z Lewej
       //jade w  lewo z Lewej 1/5
       else if((accPosInKlI == 2) & (accPosInKlJ == 0) & (turn9 == 5) &(carsPos[acctPosI][acctPosJ+1] == 0)){
           stepX = speed;
           stepY = 0;
           angle = 90;
           cout << " wykonal sie case 9 w lewo z Lewej 1/5" << endl;
       }
       // lewo z Lewej 2/5
       else if((accPosInKlI == 2) & (accPosInKlJ == 1) & (turn9==5) & (carsPos[acctPosI][acctPosJ+1] == 0)  & (carsPos[acctPosI-1][acctPosJ+1] == 0)& (carsPos[acctPosI-2][acctPosJ+1] == 0)){
               if((scenePos().rx()) >= (acctPosJ*20+3)){
        stepX = speed;
        stepY = (speed /1.5)*(-1);
        angle = angle - (speed/20 *65);
    }
    else{
        stepX = 0;
        stepY = 0;
        setPos((acctPosJ*20+3), (scenePos().ry()));
    }
           cout << "wykonal sie case 9 w lewo z Lewej 2/5" << endl;
       }
       // lewo z Lewej 3/5
       else if((accPosInKlI == 2) & (accPosInKlJ == 2) & (turn9==5) & (carsPos[acctPosI-1][acctPosJ] == 0)){
               stepX = speed;
    stepY = (speed / 2) *(-1);
    angle = angle - (speed/20 *65);
           cout << "wykonal sie case 9 w lewo z Lewej 3/5" << endl;
       }
       // lewo z Lewej 4/5
       else if((accPosInKlI == 1) & (accPosInKlJ == 2) & (turn9==5) & (carsPos[acctPosI-1][acctPosJ] == 0)){
           stepX = speed;
           stepY = 0;
           angle = 0;
           cout << "wykonal sie case 9 w lewo z Lewej 4/5" << endl;
       }
       // lewo z Lewej 5/5
       else if((accPosInKlI == 0) & (accPosInKlJ == 2) & (carsPos[acctPosI-1][acctPosJ] == 0)){
           stepX = speed;
           stepY = 0;
           angle = 0;
           turn9 =9;
           cout << "wykonal sie case 9 w lewo z Lewej 5/5" << endl;
       }
       else{
           stepX = 0;
           stepY = 0;
           cout << "wykonal sie case 9, else" << endl;
       }
       cout << "wartosc turn9 " << turn9 << endl;
       break;

     case 10:
        //Start z  Lewej
        // stoje w i2, j0 - dwie wolne opcje, wybieram kierunek ruchu -> prosto na i2, j1, skret -> i2, j1
        if((accPosInKlI == 2) & (accPosInKlJ == 0)&(turn10 == 9) & (carsPos[acctPosI][acctPosJ + 1] == 0) & (carsPos[acctPosI][acctPosJ + 2] == 0)){
            qsrand((unsigned)time(0));
            turn10 = qrand() %2;
        }
        else if((accPosInKlI == 2) & (accPosInKlJ == 0)&(turn10 == 9) & (carsPos[acctPosI][acctPosJ + 1] == 0) &(turn10 == 0)){
            turn10=0;
        }
        // START Z Dolu w prawo lub w lewo, wybieram kierunek ruchu 2- prawo, 3 - lewo
        if((accPosInKlI == 3) & (accPosInKlJ == 2)&(turn10 == 9) & (carsPos[acctPosI-1][acctPosJ] == 0) & (carsPos[acctPosI-1][acctPosJ+1] == 0) & (carsPos[acctPosI-2][acctPosJ] == 0) & (carsPos[acctPosI-2][acctPosJ -2] == 0) & (carsPos[acctPosI -2][acctPosJ -3] == 0)){
            qsrand((unsigned)time(0));
            turn10 = (qrand() %2)+2;
        }
        else if((accPosInKlI == 3) & (accPosInKlJ == 2) &(turn10 == 9)& (carsPos[acctPosI][acctPosJ -1] == 0) & (carsPos[acctPosI -1][acctPosJ +1] == 0)) {
            turn10 = 2;
        }
        // START Z Prawej prosto lub w lewo, wybieram kierunek ruchu 4 -prosto, 5- lewo
        if((accPosInKlI == 1) & (accPosInKlJ == 3)&(turn10 == 9) & (carsPos[acctPosI][acctPosJ-1] == 0) & (carsPos[acctPosI][acctPosJ-2] == 0) & (carsPos[acctPosI+1][acctPosJ-2] == 0) & (carsPos[acctPosI+2][acctPosJ-2] == 0)){
            qsrand((unsigned)time(0));
            turn10 = (qrand() %2)+4;
        }
        else if((accPosInKlI == 1) & (accPosInKlJ == 3)&(turn10 == 9) & (carsPos[acctPosI][acctPosJ-1] == 0) & (carsPos[acctPosI][acctPosJ-2] == 0)) {
            turn10 = 4;
        }

        //**********************************************************************
        // 1 - prosto z Lewej 1/4
        if((accPosInKlI == 2) & (accPosInKlJ == 0) & (carsPos[acctPosI][acctPosJ + 1] == 0) &(turn10 == 0)){
            stepX = speed;
            stepY = 0;
            angle = 90;
            cout << " wykonal sie case 10 prosto z Lewej 1/4" << endl;
        }
        // prosto z Lewej turn=0, 2/4
        else if((accPosInKlI == 2) & (accPosInKlJ == 1) &  (turn10 == 0) & (carsPos[acctPosI][acctPosJ+1] == 0)){
            stepX = speed;
            stepY = 0;
            angle = 90;

            cout << " wykonal sie case 10 prosto z Lewej 2/4" << endl;
        }
        //prosto z Lewej 3/4
        else if((accPosInKlJ == 2) & (accPosInKlI == 2) & (turn10 == 0) & (carsPos[acctPosI][acctPosJ+1] == 0)){
            stepX = speed;
            stepY = 0;
            angle = 90;
            cout << " wykonal sie case 10 prosto z Lewej 3/4" << endl;
        }
        //prosto z Lewej 4/4
        else if((accPosInKlJ == 3) & (accPosInKlI == 2) & (carsPos[acctPosI][acctPosJ+1] == 0)){
            stepX = speed;
            stepY = 0;
            angle = 90;
            turn10 = 9;
            cout << " wykonal sie case 10 prosto z Lewej 4/4" << endl;
        }
        // W Prawo z Lewej 1/3
        else if((accPosInKlI == 2) & (accPosInKlJ == 0) & (turn10 == 1)& (carsPos[acctPosI][acctPosJ + 1] == 0) & (carsPos[acctPosI+1][acctPosJ + 1] == 0)){
            if((scenePos().rx()) >= (acctPosJ*20+3)){
                stepX = speed  ;
                stepY = speed / 2;
                angle = angle + ((speed/20) *50);
            }
            else{
                stepX = 0  ;
                stepY = 0;
                setPos((acctPosJ*20+3), (scenePos().ry()));
            }
             cout << " wykonal sie case 10 W Prawo z Lewej 1/3" << endl;
         }
        //W Prawo z Lewej 2/3
        else if((accPosInKlI == 2) & (accPosInKlJ == 1) & (turn10 == 1) & (carsPos[acctPosI+1][acctPosJ] == 0)){
                stepX = speed ;
                stepY = speed / 2;
                angle = angle + (speed/20 *53);
            cout << " wykonal sie case 10 W Prawo z Lewej 2/3" << endl;
        }
        //W Prawo z Lewej 3/3
        else if((accPosInKlI == 3) & (accPosInKlJ == 1) & (carsPos[acctPosI+1][acctPosJ] == 0)){
            stepY = 0;
            stepX = speed;
            angle = 180;
            turn10 = 9;
            cout << " wykonal sie case 10 W Prawo z Lewej 3/3" << endl;
        }
        //**********************************************************************************
        // PRAWO Z Dolu
        // turn == 2 - prawo z dolu 1/3
         else if((accPosInKlI == 3) & (accPosInKlJ == 2) & (turn10 == 2)& (carsPos[acctPosI-1 ][acctPosJ] == 0)){
            if((scenePos().ry()) <= (acctPosI*20+17)){
                stepX = speed  ;
                stepY = speed / 3;
                angle = angle + ((speed/20) *50);
            }
            else{
                stepX = 0;
                stepY = 0;
                setPos((scenePos().rx()), (acctPosI*20+17));
            }
             cout << " wykonal sie case 10 prawo z dolu 1/3" << endl;
         }
         //jade w pPRAWO Z Dolu 2/3
         else if((accPosInKlI == 2) & (accPosInKlJ == 2) & (turn10==2) & (carsPos[acctPosI][acctPosJ+1] == 0)){
                stepX = speed ;
    stepY = speed / 3;
    angle = angle + (speed/20 *53);
             cout << "wykonal sie case 10 prawo z dolu 2/3" << endl;
         }
         //jade w PRAWO Z Dolu 3/3
         else if((accPosInKlI == 2) & (accPosInKlJ == 3) & (carsPos[acctPosI][acctPosJ+1] == 0)){
             stepX = speed;
             stepY = 0;
             angle = 90;
             turn10 = 9;
             cout << "wykonal sie case 10 prawo z dolu 3/3" << endl;
         }
         // LEWO Z Dolu
         //jade w  lewo z prawej 1/5
         else if((accPosInKlI == 3) & (accPosInKlJ == 2) & (turn10 == 3) &(carsPos[acctPosI-1][acctPosJ] == 0)){
         stepX = speed;
         stepY = 0;
         angle = 0;
         cout << " wykonal sie case 10 w lewo z dolu 1/5" << endl;
         }

         // lewo z Dolu 2/5
         else if((accPosInKlI == 2) & (accPosInKlJ == 2) & (turn10==3) & (carsPos[acctPosI-1][acctPosJ] == 0)  & (carsPos[acctPosI-1][acctPosJ-1] == 0)& (carsPos[acctPosI-1][acctPosJ-2] == 0)){

                if((scenePos().ry()) <= (acctPosI*20+17)){
                    stepX = speed;
                    stepY = (speed /4)*(-1);
                    angle = angle - (speed/20 *50);
                }
                else{
                    stepX = 0;
                    stepY = 0;
                    setPos((scenePos().rx()), (acctPosI*20+17));
         }
             cout << "wykonal sie case 10 w lewo z dolu 2/5" << endl;
         }
         // lewo z Dolu 3/5
         else if((accPosInKlI == 1) & (accPosInKlJ == 2) & (turn10==3) & (carsPos[acctPosI][acctPosJ-1] == 0)& (carsPos[acctPosI][acctPosJ-2] == 0)){
                stepX = speed;
                stepY = (speed / 3) *(-1);
                angle = angle - (speed/20 *55);
                cout << "wykonal sie case 10 w lewo z dolu 3/5" << endl;
         }
         // lewo z Dolu 4/5
         else if((accPosInKlI == 1) & (accPosInKlJ == 1) & (turn10==3) & (carsPos[acctPosI][acctPosJ-1 ] == 0)){
             stepX = speed;
             stepY = 0;
             angle = 270;
             cout << "wykonal sie case 10 w lewo z dolu 4/5" << endl;
         }
         // lewo z Dolu 5/5
         else if((accPosInKlI == 1) & (accPosInKlJ == 0) & (carsPos[acctPosI][acctPosJ-1] == 0)){
             stepX = speed;
             stepY = 0;
             angle = 270;
             turn10 = 9;
             cout << "wykonal sie case 10 w lewo z dolu 5/5" << endl;
         }
         //**********************************************************************************

         // Prosto z Prawej

         // Prosto z Prawej 1/4
         else if((accPosInKlI == 1) & (accPosInKlJ == 3)& (turn10 == 4) & (carsPos[acctPosI][acctPosJ-1] == 0) & (carsPos[acctPosI][acctPosJ-2] == 0)){
             stepX = speed;
             stepY = 0;
             angle = 270;
             cout << " wykonal sie case 10 prosto z prawej 1/4" << endl;
         }
         // prosto z Prawej , 2/4
         else if((accPosInKlI == 1) & (accPosInKlJ == 2) & (turn10 == 4) & (carsPos[acctPosI-1][acctPosJ] == 0)){
             stepX = speed;
             stepY = 0;
             angle = 270;

             cout << " wykonal sie case 10 prosto z prawej 2/4" << endl;
         }
         //prosto z Prawej , 3/4
         else if((accPosInKlI == 1) & (accPosInKlJ == 1) & (turn10 == 4) & (carsPos[acctPosI-1][acctPosJ] == 0)){
             stepX = speed;
             stepY = 0;
             angle = 270;
             cout << " wykonal sie case 10 prosto z prawej 3/4" << endl;
         }
         //prosto z Prawej , 4/4
         else if((accPosInKlI ==1) & (accPosInKlJ == 0) & (carsPos[acctPosI-1][acctPosJ] == 0)){
             stepX = speed;
             stepY = 0;
             angle = 270;
             turn10 = 9;
             cout << " wykonal sie case 10 prosto z prawej 4/4" << endl;
         }
         //W Lewo Z Prawej
         //jade w  lewo z Prawej 1/5
         else if((accPosInKlI == 1) & (accPosInKlJ == 3) & (turn10 == 5) &(carsPos[acctPosI][acctPosJ-1] == 0)){
         stepX = speed;
         stepY = 0;
         angle = 270;
         cout << " wykonal sie case 10 w lewo z prawej 1/5" << endl;
         }

         // lewo z Prawej 2/5
         else if((accPosInKlI == 1) & (accPosInKlJ == 2) & (turn10==5) & (carsPos[acctPosI][acctPosJ-1] == 0)  & (carsPos[acctPosI][acctPosJ-2] == 0)& (carsPos[acctPosI+1][acctPosJ-2] == 0)){
                if((scenePos().rx()) <= (acctPosJ*20+17)){
        stepX = speed;
        stepY = (speed /1.5)*(-1);
        angle = angle - (speed/20 *65);
    }
    else{
        stepX = 0;
        stepY = 0;
        setPos((acctPosJ*20+17), (scenePos().ry()));
    }
             cout << "wykonal sie case 10 w lewo z prawej 2/5" << endl;
         }
         // lewo z Prawej 3/5
         else if((accPosInKlI == 1) & (accPosInKlJ == 1) & (turn10==5) & (carsPos[acctPosI+1][acctPosJ] == 0)){

                stepX = speed;
    stepY = (speed / 2) *(-1);
    angle = angle - (speed/20 *65);
             cout << "wykonal sie case 10 w lewo z prawej 3/5" << endl;
         }
         // lewo z Prawej 4/5
         else if((accPosInKlI == 2) & (accPosInKlJ == 1) & (turn10==5) & (carsPos[acctPosI+1][acctPosJ] == 0)){
             stepX = speed;
             stepY = 0;
             angle = 180;
             cout << "wykonal sie case 10 w lewo z prawej 4/5" << endl;
         }
         // lewo z Prawej 5/5
         else if((accPosInKlI == 3) & (accPosInKlJ == 1) & (carsPos[acctPosI+1][acctPosJ] == 0)){
             stepX = speed;
             stepY = 0;
             angle = 180;
             turn10 = 9;
             cout << "wykonal sie case 10 w lewo z prawej 5/5" << endl;
         }
         else{
            stepX = 0;
            stepY = 0;
            cout << "wykonal sie case 10 else" << endl;
         }
         cout << "wartosc turn10 " << turn10 << endl;
         break;
    case 11:
         // z DOLU 0- w prawo, 1- prosto, 2- lewo
         // stje w poz dol, wszystkie wolne opcje
         if((accPosInKlI == 3) & (accPosInKlJ == 2) & (turn11==99) & (carsPos[acctPosI -1][acctPosJ] == 0) & (carsPos[acctPosI -1][acctPosJ +1]  == 0) & (carsPos[acctPosI -2][acctPosJ]  == 0) & (carsPos[acctPosI -2][acctPosJ -1]  == 0) & (carsPos[acctPosI -3][acctPosJ -1]  == 0) & (carsPos[acctPosI -2][acctPosJ -2]  == 0)){
            qsrand((unsigned)time(0));
            turn11 = qrand() %3;
         }
         // stje w poz dol, mogę jechać prosto lub w prawo
         else if((accPosInKlI == 3) & (accPosInKlJ == 2) & (turn11==99) & (carsPos[acctPosI -1][acctPosJ] == 0) & (carsPos[acctPosI -1][acctPosJ + 1] == 0) & (carsPos[acctPosI -2][acctPosJ]  == 0) & (carsPos[acctPosI -2][acctPosJ +1]  == 0)){
            qsrand((unsigned)time(0));
            turn11 = qrand() %2;
         }
         // stje w poz dol, mogę jechać tylko w prawo

         else if((accPosInKlI == 3) & (accPosInKlJ == 2) & (turn11==99) & (carsPos[acctPosI -1][acctPosJ] == 0) & (carsPos[acctPosI -2][acctPosJ]  == 0) & (carsPos[acctPosI -2][acctPosJ +1]  == 0)){
            qsrand((unsigned)time(0));
            turn11 = 0;
         }
         //z GORA 3- w prawo, 4- prosto, 5- lewo
         // Z GORA, wszystkie wolne opcje
         if((accPosInKlI == 0) & (accPosInKlJ == 1) & (turn11==99) & (carsPos[acctPosI +1][acctPosJ] == 0) & (carsPos[acctPosI +1][acctPosJ -1]  == 0) & (carsPos[acctPosI +1][acctPosJ+1]  == 0) & (carsPos[acctPosI+1][acctPosJ +2]  == 0) & (carsPos[acctPosI +2][acctPosJ ]  == 0) & (carsPos[acctPosI +2][acctPosJ+1]  == 0)){
             qsrand((unsigned)time(0));
             turn11 =3;// (qrand() %3)+3;
         }
         // Z GORA, mogę jechać prosto lub w prawo
         else if((accPosInKlI == 0) & (accPosInKlJ == 1) & (turn11==99) & (carsPos[acctPosI +1][acctPosJ] == 0) & (carsPos[acctPosI +1][acctPosJ -1]  == 0) & (carsPos[acctPosI +1][acctPosJ+1]  == 0) & (carsPos[acctPosI+1][acctPosJ +2]  == 0) & (carsPos[acctPosI +2][acctPosJ ]  == 0)){
            qsrand((unsigned)time(0));
            turn11 = (qrand() %2)+3;
         }
         // Z GORA, mogę jechać tylko w prawo

         else if((accPosInKlI == 0) & (accPosInKlJ == 1) & (turn11==99) & (carsPos[acctPosI +1][acctPosJ] == 0) & (carsPos[acctPosI +1][acctPosJ -1]  == 0)){
             qsrand((unsigned)time(0));
             turn11 = 3;
         }
         //z LEWO 6- w prawo, 7- prosto, 8- lewo
         // Z Lewo, wszystkie wolne opcje
         if((accPosInKlI == 2) & (accPosInKlJ == 0) & (turn11==99) & (carsPos[acctPosI][acctPosJ+1] == 0) & (carsPos[acctPosI +1][acctPosJ +1]  == 0) & (carsPos[acctPosI ][acctPosJ+2]  == 0) & (carsPos[acctPosI+1][acctPosJ +2]  == 0) & (carsPos[acctPosI -1][acctPosJ+2 ]  == 0) & (carsPos[acctPosI -2][acctPosJ+2]  == 0)){
              qsrand((unsigned)time(0));
              turn11 = (qrand() %3)+6;
         }
         // Z Lewo, mogę jechać prosto lub w prawo
         else if((accPosInKlI == 2) & (accPosInKlJ == 0) & (turn11==99) & (carsPos[acctPosI][acctPosJ+1] == 0) & (carsPos[acctPosI +1][acctPosJ +1]  == 0) & (carsPos[acctPosI ][acctPosJ+2]  == 0) & (carsPos[acctPosI+1][acctPosJ +2]  == 0)){
             qsrand((unsigned)time(0));
             turn11 = (qrand() %2)+6;
         }
         // Z Lewo, mogę jechać tylko w prawo

         else if((accPosInKlI == 2) & (accPosInKlJ == 0) & (turn11==99) & (carsPos[acctPosI][acctPosJ+1] == 0) & (carsPos[acctPosI +1][acctPosJ +1]  == 0)){
             qsrand((unsigned)time(0));
             turn11 = 6;
         }
         //z Prawo 9- w prawo, 10- prosto, 11- lewo
         // Z Lewo, wszystkie wolne opcje
         if((accPosInKlI == 1) & (accPosInKlJ == 3) & (turn11==99) & (carsPos[acctPosI][acctPosJ-1] == 0) & (carsPos[acctPosI -1][acctPosJ -1]  == 0) & (carsPos[acctPosI ][acctPosJ-2]  == 0) & (carsPos[acctPosI-1][acctPosJ -2]  == 0) & (carsPos[acctPosI +1][acctPosJ-2 ]  == 0) & (carsPos[acctPosI +2][acctPosJ-2]  == 0)){
             qsrand((unsigned)time(0));
             turn11 = (qrand() %3)+9;
         }
         // Z Lewo, mogę jechać prosto lub w prawo
         else if((accPosInKlI == 1) & (accPosInKlJ == 3) & (turn11==99) & (carsPos[acctPosI][acctPosJ-1] == 0) & (carsPos[acctPosI -1][acctPosJ -1]  == 0) & (carsPos[acctPosI ][acctPosJ-2]  == 0) & (carsPos[acctPosI-1][acctPosJ -2]  == 0)){
            qsrand((unsigned)time(0));
            turn11 = (qrand() %2)+9;
         }
         // Z Lewo, mogę jechać tylko w prawo

         else if((accPosInKlI == 1) & (accPosInKlJ == 3) & (turn11==99) & (carsPos[acctPosI][acctPosJ-1] == 0) & (carsPos[acctPosI -1][acctPosJ -1]  == 0)){
             qsrand((unsigned)time(0));
             turn11 = 9;
         }
         //***********************************************************
         // START Z DOŁU
         // skrecam w prawo z dolu 1/3
//         qreal tmpI;
//         tmpI = 3 - accPosInKlJ;
//         qreal tmpJ;
//         tmpJ = accPosInKlI;
//         qreal tmpScenePosI;
//         tmpScenePosI = 320 - (scenePos().rx());
//         qreal tmpScenePosJ;
//         tmpScenePosJ = (scenePos().ry());
//         qreal tempAcctPosJ;
//         tempAcctPosJ = acctPosI;

         if((accPosInKlI == 3) & (accPosInKlJ == 2) & (turn11==0)){
             if((scenePos().ry()) <= (acctPosI*20+17)){
                 stepX = speed  ;
                 stepY = speed / 3;
                 angle = angle + ((speed/20) *50);
             }
             else{
                 stepX = 0  ;
                 stepY = 0;
                 setPos((scenePos().rx()), (acctPosI*20+17));
             }
             cout << "wykonal sie case 11,  prawo z dolu 1/3" << endl;
         }
         //jade w prawo z dolu 2/3
         else if((accPosInKlI == 2) & (accPosInKlJ == 2) & (turn11==0) & (carsPos[acctPosI][acctPosJ + 1] == 0)){
             stepX = speed ;
             stepY = speed / 3;
             angle = angle + (speed/20 *53);
             cout << "wykonal sie case 11,  prawo z dolu 2/3" << endl;
         }
         //jade w prawo z dolu 3/3
         else if((accPosInKlI == 2) & (accPosInKlJ == 3) & (carsPos[acctPosI][acctPosJ + 1] == 0)){
             stepX = speed;
             stepY = 0;
             angle = 90;
             turn11 = 99;
             cout << "wykonal sie case 11,  prawo z dolu 3/3" << endl;
         }

         //jade prosto z dolu 1/4
         else if((accPosInKlI == 3) & (accPosInKlJ == 2) & (turn11==1)){
             stepX = speed;
             stepY = 0;
             angle = 0;
             cout << " wykonal sie case 11 prosto z dolu 1/4" << endl;
         }
         // prosto z dolu 2/4
         else if((accPosInKlI == 2) & (accPosInKlJ == 2) & (turn11==1) & (carsPos[acctPosI - 1][acctPosJ] == 0)){
             stepX = speed;
             stepY = 0;
             angle = 0;
             cout << " wykonal sie case 11 prosto z dolu 2/4" << endl;
         }
         //prosto z dolu 3/4
         else if((accPosInKlI == 1) & (accPosInKlJ == 2) & (turn11==1) & (carsPos[acctPosI - 1][acctPosJ] == 0)){
             stepX = speed;
             stepY = 0;
             angle = 0;
             cout << " wykonal sie case 11, prosto 3/4" << endl;
         }
         //prosto, 4/4
         else if((accPosInKlI == 0) & (accPosInKlJ == 2) & (carsPos[acctPosI - 1][acctPosJ] == 0)){
             stepX = speed;
             stepY = 0;
             angle = 0;
             turn11 = 99;
             cout << " wykonal sie case 11, prosto z dolu 4/4" << endl;
         }
         // jade w lewo z dołu 1/5
         else if((accPosInKlI == 3) & (accPosInKlJ == 2) & (turn11==2)){
             stepX = speed;
             stepY = 0;
             angle = 0;
             cout << " wykonal sie case 11 lewo z dołu 1/5" << endl;
         }
         // lewo z dolu 2/5
         else if((accPosInKlI == 2) & (accPosInKlJ == 2) & (turn11==2) & (carsPos[acctPosI - 1][acctPosJ] == 0)  & (carsPos[acctPosI - 1][acctPosJ+1] == 0)){
             if((scenePos().ry()) <= (acctPosI*20+17)){
                stepX = speed;
                stepY = (speed /3)*(-1);
                angle = angle - (speed/20 *50);
             }
             else{
                stepX = 0;
                stepY = 0;
                setPos((scenePos().rx()), (acctPosI*20+17));
             }
         cout << "wykonal sie case 11,  lewo z dolu 2/5" << endl;
         }
         // lewo z dolu 3/5
         else if((accPosInKlI == 1) & (accPosInKlJ == 2) & (turn11==2) & (carsPos[acctPosI][acctPosJ -1] == 0) & (carsPos[acctPosI - 1][acctPosJ-1] == 0)){
             stepX = speed;
             stepY = (speed / 4) *(-1);
             angle = angle - (speed/20 *55);
             cout << "wykonal sie case 11,  lewo z dolu 3/5" << endl;
         }
         // lewo z dolu 4/5
         else if((accPosInKlI == 1) & (accPosInKlJ == 1)  & (turn11==2) & (carsPos[acctPosI][acctPosJ -1] == 0)){
             stepX = speed;
             stepY = 0;
             angle = 270;
             cout << "wykonal sie case 11,  lewo z dolu 4/5" << endl;
         }
         // lewo z dolu 5/5
         else if((accPosInKlI == 1) & (accPosInKlJ == 0) & (carsPos[acctPosI][acctPosJ -1] == 0)){
             stepX = speed;
             stepY = 0;
             angle = 270;
             turn11 = 99;
             cout << "wykonal sie case 11,  lewo z dolu 5/5" << endl;
         }
         //*********************************************
         // START Z GÓRY

         // EKSPERYMENT!



//                  // skrecam w prawo z gory 1/3
//                  else if((tmpI == 2) & (tmpJ == 0) & (turn11==3)){
//                     moveToRightT1(tempAcctPosJ, tmpScenePosJ, tmpScenePosI);
//                     cout << "wykonal sie case 11,  prawo z gory 1/3" << endl;
//                  }
//                  //jade w prawo z gory 2/3
//                  else if((tmpI == 2) & (tmpJ == 1) & (turn11==3) & (carsPos[acctPosI][acctPosJ - 1] == 0)){
//                     moveToRightT2();
//                     cout << "wykonal sie case 11,  prawo z gory 2/3" << endl;
//                  }
//                  //jade w prawo z gory 3/3
//                  else if((tmpI == 3) & (tmpJ == 1) & (carsPos[acctPosI][acctPosJ -1] == 0)){
//                      stepX = speed;
//                      stepY = 0;
//                      angle = 90;
//                      turn11 = 99;
//                      cout << "wykonal sie case 11,  prawo z gory 3/3" << endl;
//                  }

      // KONIEC EKSPERYMNETU

         // skrecam w prawo z gory 1/3
        else if((accPosInKlI == 0) & (accPosInKlJ == 1) & (turn11==3)){
                if((scenePos().ry()) >= (acctPosI*20+3)){
                 stepX = speed  ;
                 stepY = speed / 2;
                 angle = angle + ((speed/20) *53);
             }
             else{
                 stepX = 0;
                 stepY = 0;
                 setPos((scenePos().rx()), (acctPosI*20+3));
             }
            cout << "wykonal sie case 11,  prawo z gory 1/3" << endl;
         }
         //jade w prawo z gory 2/3
         else if((accPosInKlI == 1) & (accPosInKlJ == 1) & (turn11==3) & (carsPos[acctPosI][acctPosJ - 1] == 0)){
                stepX = speed ;
                stepY = speed / 2;
                angle = angle + (speed/20 *53);
            cout << "wykonal sie case 11,  prawo z gory 2/3" << endl;
         }
         //jade w prawo z gory 3/3
         else if((accPosInKlI == 1) & (accPosInKlJ == 0) & (carsPos[acctPosI][acctPosJ -1] == 0)){
             stepX = speed;
             stepY = 0;
             angle = 90;
             turn11 = 99;
             cout << "wykonal sie case 11,  prawo z gory 3/3" << endl;
         }

         //jade prosto z gory 1/4
         else if((accPosInKlI == 0) & (accPosInKlJ == 1) & (turn11==4) & (carsPos[acctPosI + 1][acctPosJ+1] == 0)){
             stepX = speed;
             stepY = 0;
             angle = 180;
             cout << " wykonal sie case 11 prosto z gory 1/4" << endl;
         }
         // prosto z gory 2/4
         else if((accPosInKlI == 1) & (accPosInKlJ == 1) & (turn11==4) & (carsPos[acctPosI + 1][acctPosJ] == 0) & (carsPos[acctPosI + 1][acctPosJ-1] == 0)){
             stepX = speed;
             stepY = 0;
             angle = 180;
             cout << " wykonal sie case 11 prosto z gory 2/4" << endl;
         }
         //prosto z gory 3/4
         else if((accPosInKlI == 2) & (accPosInKlJ == 1) & (turn11==4) & (carsPos[acctPosI + 1][acctPosJ] == 0)){
             stepX = speed;
             stepY = 0;
             angle = 180;
             cout << " wykonal sie case 11, prosto gory 3/4" << endl;
         }
         //prosto, gory 4/4
         else if((accPosInKlI == 3) & (accPosInKlJ == 1) & (carsPos[acctPosI +1][acctPosJ] == 0)){
             stepX = speed;
             stepY = 0;
             angle = 180;
             turn11 = 99;
             cout << " wykonal sie case 11, prosto z gory 4/4" << endl;
         }
         // jade w lewo z gory 1/5
         else if((accPosInKlI == 0) & (accPosInKlJ == 1) & (turn11==5)){
             stepX = speed;
             stepY = 0;
             angle = 0;
             cout << " wykonal sie case 11 lewo z gory 1/5" << endl;
         }
         // lewo z gory 2/5
         else if((accPosInKlI == 1) & (accPosInKlJ == 1) & (turn11==5) & (carsPos[acctPosI + 1][acctPosJ] == 0)  & (carsPos[acctPosI + 1][acctPosJ+1] == 0)){
                 if((scenePos().rx()) >= (acctPosJ*20+3)){
                    stepX = speed;
                    stepY = (speed /1.5)*(-1);
                    angle = angle - (speed/20 *65);
        }
        else{
            stepX = 0;
            stepY = 0;
            setPos((acctPosJ*20+3), (scenePos().ry()));
        }
        cout << "wykonal sie case 11,  lewo z gory 2/5" << endl;
        }
        // lewo z gory 3/5
        else if((accPosInKlI == 2) & (accPosInKlJ == 1) & (turn11==5) & (carsPos[acctPosI][acctPosJ +1] == 0) & (carsPos[acctPosI +1][acctPosJ+1] == 0)){
                  stepX = speed;
                  stepY = (speed / 2) *(-1);
                  angle = angle - (speed/20 *65);
         cout << "wykonal sie case 11,  lewo z gory 3/5" << endl;
         }
         // lewo z gory 4/5
         else if((accPosInKlI == 2) & (accPosInKlJ == 2) & (turn11==5) & (carsPos[acctPosI][acctPosJ +1] == 0)){
             stepX = speed;
             stepY = 0;
             angle = 270;
             cout << "wykonal sie case 11,  lewo z gory 4/5" << endl;
         }
         // lewo z gory 5/5
         else if((accPosInKlI == 2) & (accPosInKlJ == 3) & (carsPos[acctPosI][acctPosJ +1] == 0)){
             stepX = speed;
             stepY = 0;
             angle = 270;
             turn11 = 99;
             cout << "wykonal sie case 11,  lewo z gory 5/5" << endl;
         }
         //********************************************************************
         //  START z Lewej
         //z LEWO 6- w prawo, 7- prosto, 8- lewo
         else if((accPosInKlI == 2) & (accPosInKlJ == 0) & (turn11 == 6)& (carsPos[acctPosI ][acctPosJ+1] == 0)){
             if((scenePos().rx()) >= (acctPosJ*20+3)){
                 stepX = speed  ;
                 stepY = speed / 2;
                 angle = angle + ((speed/20) *50);
             }
             else{
                 stepX = 0  ;
                 stepY = 0;
                 setPos((acctPosJ*20+3), (scenePos().ry()));
             }
             cout << " wykonal sie case 11 prawo z lewej 1/3" << endl;
          }
          //jade w pPRAWO Z Lewej 2/3
          else if((accPosInKlI == 2) & (accPosInKlJ == 1) & (turn11==6) & (carsPos[acctPosI+1][acctPosJ] == 0)){
                 stepX = speed ;
                 stepY = speed / 2;
                 angle = angle + (speed/20 *53);
             cout << "wykonal sie case 11 prawo z lewej 2/3" << endl;
          }
          //jade w PRAWO Z Lewej 3/3
          else if((accPosInKlI == 3) & (accPosInKlJ == 1) & (carsPos[acctPosI+1][acctPosJ] == 0)){
              stepX = speed;
              stepY = 0;
              angle = 180;
              turn11=99;
              cout << "wykonal sie case 11 prawo z lewej 3/3" << endl;
          }
         // 1 - prosto z Lewej 1/4
         else if((accPosInKlI == 2) & (accPosInKlJ == 0) & (carsPos[acctPosI][acctPosJ + 1] == 0) &(turn11 == 7)){
             stepX = speed;
             stepY = 0;
             angle = 90;
             cout << " wykonal sie case 11 prosto z Lewej 1/4" << endl;
         }

         // prosto z Lewej turn=0, 2/4
         else if((accPosInKlI == 2) & (accPosInKlJ == 1) &  (turn11 == 7) & (carsPos[acctPosI][acctPosJ+1] == 0)){
             stepX = speed;
             stepY = 0;
             angle = 90;

             cout << " wykonal sie case 11 prosto z Lewej 2/4" << endl;
         }
         //prosto z Lewej 3/4
         else if((accPosInKlJ == 2) & (accPosInKlI == 2) & (turn11 == 7) & (carsPos[acctPosI][acctPosJ+1] == 0)){
             stepX = speed;
             stepY = 0;
             angle = 90;
             cout << " wykonal sie case 11 prosto z Lewej 3/4" << endl;
         }
         //prosto z Lewej 4/4
         else if((accPosInKlJ == 3) & (accPosInKlI == 2) & (carsPos[acctPosI][acctPosJ+1] == 0)){
             stepX = speed;
             stepY = 0;
             angle = 90;
             turn11=99;
             cout << " wykonal sie case 11 prosto z Lewej 4/4" << endl;
         }
         // LEWO Z LEWEJ
         //jade w  lewo z lewej 1/5
         else if((accPosInKlI == 2) & (accPosInKlJ == 0) & (turn11 == 8) &(carsPos[acctPosI][acctPosJ+1] == 0)){
             stepX = speed;
             stepY = 0;
             angle = 90;
             cout << " wykonal sie case 11 w lewo z lewej 1/5" << endl;
          }

          // lewo z lewej 2/5
          else if((accPosInKlI == 2) & (accPosInKlJ == 1) & (turn11==8) & (carsPos[acctPosI][acctPosJ+1] == 0)  & (carsPos[acctPosI-1][acctPosJ+1] == 0)& (carsPos[acctPosI-2][acctPosJ+1] == 0)){

                 if((scenePos().rx()) >= (acctPosJ*20+3)){
                    stepX = speed;
                    stepY = (speed /1.5)*(-1);
                    angle = angle - (speed/20 *65);
                }
                else{
                    stepX = 0;
                    stepY = 0;
                    setPos((acctPosJ*20+3), (scenePos().ry()));
                }
              cout << "wykonal sie case 11 w lewo z lewej 2/5" << endl;
          }
          // lewo z lewej 3/5
          else if((accPosInKlI == 2) & (accPosInKlJ == 2) & (turn11==8) & (carsPos[acctPosI-1][acctPosJ] == 0)& (carsPos[acctPosI-1][acctPosJ+1] == 0)){

                stepX = speed;
                stepY = (speed / 2) *(-1);
                angle = angle - (speed/20 *65);
                cout << "wykonal sie case 11 w lewo z lewej 3/5" << endl;
          }
          // lewo z lewej 4/5
          else if((accPosInKlI == 1) & (accPosInKlJ == 2) & (turn11==8) & (carsPos[acctPosI-1][acctPosJ ] == 0)){
              stepX = speed;
              stepY = 0;
              angle = 0;
              cout << "wykonal sie case 11 w lewo z lewej 4/5" << endl;
          }
          // lewo z lewej 5/5
          else if((accPosInKlI == 0) & (accPosInKlJ == 2) & (carsPos[acctPosI-1][acctPosJ] == 0)){
              stepX = speed;
              stepY = 0;
              angle = 0;
              turn11=99;
              cout << "wykonal sie case 11 w lewo z lewej 5/5" << endl;
          }
         //********************************************************************
         //  START z Prawej 9- w prawo, 10- prosto, 11- lewo
         //prawo z prawej 1/3
         else if((accPosInKlI == 1) & (accPosInKlJ == 3) & (turn11 == 9)& (carsPos[acctPosI ][acctPosJ-1] == 0)){
             if((scenePos().rx()) <= (acctPosJ*20+17)){
                 stepX = speed  ;
                 stepY = speed / 5;
                 angle = angle + ((speed/20) *50);
             }
             else{
                 stepX = 0  ;
                 stepY = 0;
                 setPos((acctPosJ*20+17), (scenePos().ry()));
             }
             cout << " wykonal sie case 11 prawo z prawej 1/3" << endl;
         }
         //jade w prawo z prawej 2/3
         else if((accPosInKlI == 1) & (accPosInKlJ == 2) & (turn11==9) & (carsPos[acctPosI -1][acctPosJ] == 0)){
             stepX = speed ;
             stepY = speed / 4;
             angle = angle + (speed/20 *50);
             cout << "wykonal sie case 11,  prawo z prawej 2/3" << endl;
         }
         //jade w prawo z prawej 3/3
         else if((accPosInKlI == 0) & (accPosInKlJ == 2) & (carsPos[acctPosI -1][acctPosJ] == 0)){
             stepX = speed;
             stepY = 0;
             angle = 0;
             turn11 = 99;
             cout << "wykonal sie case 11,  prawo z prawej 3/3" << endl;
         }
         // Prosto z Prawej 1/4
         else if((accPosInKlI == 1) & (accPosInKlJ == 3) & (carsPos[acctPosI][acctPosJ+1] == 0) & (carsPos[acctPosI][acctPosJ+2] == 0)& (turn11 == 10)){
             stepX = speed;
             angle = 270;
             cout << " wykonal sie case 11 prosto  z prawej 1/4" << endl;
         }
         // prosto z Prawej 2/4
         else if((accPosInKlI == 1) & (accPosInKlJ == 2) & (turn11 == 10) & (carsPos[acctPosI][acctPosJ - 1] == 0)){
             stepX = speed;
             angle = 270;

             cout << " wykonal sie case 11 prosto z Prawej 2/4" << endl;
         }
         //prosto z Prawej 3/4
         else if((accPosInKlI == 1) & (accPosInKlJ == 1) & (turn11 == 10) & (carsPos[acctPosI][acctPosJ -1] == 0)){
             stepX = speed;
             angle = 270;
             cout << " wykonal sie case 11, prosto z Prawej 3/4" << endl;
         }
         //prosto z Prawej 4/4
         else if((accPosInKlI == 1) & (accPosInKlJ == 0) & (carsPos[acctPosI ][acctPosJ -1] == 0)){
             stepX = speed;
             angle = 270;
             turn11=99;
             cout << " wykonal sie case 11,prosto z Prawej 4/4" << endl;
         }

         // LEWO Z PRAWEJ
         //jade w  lewo z prawej 1/5
         else if((accPosInKlI == 1) & (accPosInKlJ == 3) & (turn11 == 11) &(carsPos[acctPosI][acctPosJ-1] == 0)){
             stepX = speed;
             stepY = 0;
             angle = 270;
             cout << " wykonal sie case 11 w lewo z prawej 1/5" << endl;
         }

         // lewo z prawej 2/5
         else if((accPosInKlI == 1) & (accPosInKlJ == 2) & (turn11==11) & (carsPos[acctPosI - 1][acctPosJ-1] == 0)  & (carsPos[acctPosI][acctPosJ-1] == 0)& (carsPos[acctPosI+1][acctPosJ-1] == 0)){
                 if((scenePos().rx()) <= (acctPosJ*20+17)){
                    stepX = speed;
                    stepY = (speed /1.5)*(-1);
                    angle = angle - (speed/20 *65);
                }
                else{
                    stepX = 0;
                    stepY = 0;
                    setPos((acctPosJ*20+17), (scenePos().ry()));
                }
             cout << "wykonal sie case 11,  lewo z prawej 2/5" << endl;
         }
         // lewo z prawej 3/5
         else if((accPosInKlI == 1) & (accPosInKlJ == 1) & (turn11==11) & (carsPos[acctPosI+1][acctPosJ] == 0)){
                 stepX = speed;
                stepY = (speed / 2) *(-1);
                angle = angle - (speed/20 *65);
             cout << "wykonal sie case 11,  lewo z prawej 3/5" << endl;
         }
         // lewo z prawej 4/5
         else if((accPosInKlI == 2) & (accPosInKlJ == 1) & (turn11==11) & (carsPos[acctPosI+1][acctPosJ ] == 0)){
             stepX = speed;
             stepY = 0;
             angle = 180;
             cout << "wykonal sie case 11,  lewo z prawej 4/5" << endl;
         }
         // lewo z prawej 5/5
         else if((accPosInKlI == 3) & (accPosInKlJ == 1) & (carsPos[acctPosI+1][acctPosJ] == 0)){
             stepX = speed;
             stepY = 0;
             angle = 180;
             turn11 = 99;
             cout << "wykonal sie case 11,  lewo z prawej 5/5" << endl;
         }
         else{
             stepX = 0;
             stepY = 0;
             cout << "wykonal sie case 11,  else" << endl;
         }
         break;
    case 99:
        angle = 90;
        stepX = speed;
        stepY = 0;
        break;

    default:
         stepX = 0;
         stepY = 0;
         break;
    }
}

/**
* \brief Destruktor
*
* Niszczy obiekt klasy Car.
*
*/
Car::~Car(){
    cout << "destr car  " << ID << endl;
}

/**
* \brief zeruje wartości tablicy mapa po zmianie ilości pojazdów na mapie.
*
*/
void Car::resetTab(){
    //carsPos [16][16] = {0};
    for(int i=0; i<16; i++)
    {
        for(int j=0; j<16; j++)
        {
                carsPos [i][j] = 0;
        }
    }
}

int Car::carsPos [16][16] = {0};
int Car::counter = 0;
