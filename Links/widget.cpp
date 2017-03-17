#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    qDebug()<<"Старт программы";
    QString linkName = "/home/homer/!QT/workWithLinks/link2.txt";
    fileLink = new QLinuxFileLink(linkName);

    qDebug()<<"Link name="<<fileLink->getLinkName();
    qDebug()<<"Target name ="<<fileLink->getTargenName();

    //qDebug()<<"Set target name="<<fileLink->setTargetName("source.txt");

    //QDir currentDir("/usr/lib");
    //qDebug()<<"symlinks="<<currentDir.entryList(QStringList("*"), QDir::Files | QDir::Dirs);

    /*
    QStringList links = fileLink->getSymLinkList("/usr/lib");

    qDebug()<<"links.length()="<<links.length();

    for (int i=0;i<links.length();i++)
        qDebug()<<links.at(i);
     */
}

Widget::~Widget()
{
    delete fileLink;
}


