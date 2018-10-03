#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QGraphicsScene>
#include <QPushButton>
#include <QLabel>
#include "mapa.h"
#include "car.h"

/**
 * \brief klasa QMainWindow
 *
 * klasa obiektow odpowiedzialna za tworzenie sceny graficznej w oknie gównym oraz dodawanie do niej elementów graficznych.
 *
 * \version wersja alfa
 */

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Mapa *klatka;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

void handleButton();
void handleButtons();

void on_Speed_valueChanged(int arg1);

void on_IloscPoj_valueChanged(int arg1);

private:

    Ui::MainWindow *ui; /**< Okno interfrjsu uzytkownika */
    QGraphicsScene *scene; /**< Scena graficzna */
    QTimer *timer; /**< Zegar animacji*/
    QPushButton *m_button; /**< Przycisk Start */
    QPushButton *s_button; /**< Przycisk Stop */
    int timeStep = 60; /**< Wartość prędkości animacji */
    int CarCount =1; /**< Ilość zamochodów na mapie */
    QList<QGraphicsItem*> lista; /**< lista obiektów klasy Car */

};

#endif // MAINWINDOW_H
