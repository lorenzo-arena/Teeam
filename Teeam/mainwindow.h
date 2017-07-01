#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "abstractview.h"

#include <QMainWindow>

#include <KDGanttDateTimeGrid>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public AbstractView
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void UpdateView();

private:
    Ui::MainWindow *ui;
    KDGantt::DateTimeGrid* dateTimeGrid;
    void initGanttView();
};

#endif // MAINWINDOW_H
