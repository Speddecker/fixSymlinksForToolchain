#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QFileInfo>

#include "QLinuxFileLink/qlinuxfilelink.h"



class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

    QLinuxFileLink *fileLink;
};

#endif // WIDGET_H
