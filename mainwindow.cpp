#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>
#include <math.h>
#include <QCoreApplication>
#include <QMessageBox>
#include "car.h"
#include "mapa.h"
#include <QFile>
#include <QTextStream>
#include <QPixmap>
#include <tablicamapy.h>
#include <iostream>
#include <QAction>


using namespace std;

/**
* \brief Konstruktor
*
* Tworzy scenę graficzną, wypełnia ją obiektami klasy Mapa oraz obiektami klasy Car,
* jak również zapewnia obsługę zegara animacji orzaz przycisków Start Stop.
*
*/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

       /**
       * \brief dodawanie sceny graficznej.
       *
       */
        scene = new QGraphicsScene(this);
        ui->graphicsView->setScene(scene);
        ui->graphicsView->setRenderHint(QPainter::Antialiasing);
        scene->setSceneRect(0, 0, 320, 320);
        ui->graphicsView->setBaseSize(324,324);

        /**
        * \brief Utworzenie obiektu klasy TablicaMapy oraz wywołanie funkcji: odczyt mapy z pliku tekstowego.
        *
        */
        TablicaMapy ObjTabMapy;
        ObjTabMapy.OdczytZPliku();
        if(ObjTabMapy.czyPlikOk() == false)
            QMessageBox::about(this, "UWAGA" , "Nie znalazłem pliku mapy.\nAplikacja uruchomi się z ograniczoną funkcjonalnością.");


        /**
        * \brief Utworzenie obiektów klasy Mapa oraz wypełnianie sceny graficznej obiektami mapy.
        *
        */
        int KlatkaCount = 4;
        int klPozY =0;
        int klatVal;
        for (int i = 0; i < KlatkaCount; i++){
            for (int j = 0; j < KlatkaCount; j++){
                klatVal = ObjTabMapy.getValTab(i,j);
                klatka = new Mapa(NULL, klatVal);
                klatka->setPos(j*81,klPozY);
                scene->addItem(klatka);
            }
            klPozY = klPozY+ 81;
        }

        /**
        * \brief Zegar animacji.
        *
        */
        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()),scene, SLOT(advance()));


        /**
        * \brief Dodanie przycisków start, stop.
        *
        */
        m_button = new QPushButton("Start", this);
        m_button->setGeometry(QRect(QPoint(350,50),QSize(75,30)));
        s_button = new QPushButton("Stop", this);
        s_button->setGeometry(QRect(QPoint(350,90),QSize(75,30)));

        connect(m_button, SIGNAL (pressed()), this, SLOT (handleButton()));
        connect(s_button, SIGNAL (pressed()), this, SLOT (handleButtons()));

        /**
        * \brief Utworzenie obiktów klasy Car.
        *
        */
        for (int i = 0; i < 7; i++){
                lista.push_back(new Car(90));
                (lista.at(i))->setPos((i*(-28))+240,50);
            }
        /**
        * \brief Dodanie pierwszego pojazdu do sceny graficznej.
        *
        */
        scene->addItem(lista.at(0));
}

/**
* \brief Uruchomienie timera poprzez wciśnięcie przyciski Start.
*
*/
void MainWindow::handleButton() {
     timer->start(timeStep);
}

/**
* \brief Zatrzymanie timera poprzez wciśnięcie przyciski Stop.
*
*/
void MainWindow::handleButtons() {
    timer->stop();
}

/**
* \brief Zmiana prędkości animacji poprzez zmianaę wartości w okienku "prędkośc pojazdów".
*
*/
void MainWindow::on_Speed_valueChanged(int arg1)
{
    timeStep = 120 - arg1;
    timer->setInterval(timeStep);
//    if(timer->isActive())
//    {
//        timer->setInterval(timeStep);
//    }
}

/**
* \brief Zmiana ilości pojazdów na mapie poprzez zmianaę wartości w okienku "ilość pojazdów".
*
*/
void MainWindow::on_IloscPoj_valueChanged(int arg1)
{

        for (int i = 0; i < CarCount; i++){
            scene->removeItem((lista.at(i)));
            }
        CarCount = arg1;
        Car poj(90);
        poj.resetTab();
        for (int i = 0; i < CarCount; i++){
                scene->addItem(lista.at(i));
            }
}

/**
* \brief Destruktor.
* Niszczy obiekt klasy MainWindow oraz usuwa obiekty klasy Car i zamyka interfejs użytkownika.
*
*/
MainWindow::~MainWindow()
{
    for (int i = 0; i < 7; i++){
        delete lista.at(i);
        }
    delete ui;
}
