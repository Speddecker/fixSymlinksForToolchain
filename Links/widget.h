#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QProgressBar>
#include <QFileDialog>
#include <QThread>
#include <QTime>

#include "QLinuxFileLink/qlinuxfilelink.h"
#include "QFixSymlink/qfixsymlink.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

private:
    QLinuxFileLink *fileLink;
    QFixSymlink *fixSymlink;
    QThread *thrFixSymlink;

    void createForm();
    QVBoxLayout *mainLayout;
    QGridLayout *layoutPath;
    QLabel *labelSourcePath;
    QLineEdit *editSourcePath;
    QPushButton *buttonSourcePath;
    QLabel *labelTargetPath;
    QLineEdit *editTargetPath;
    QPushButton *buttonTargetPath;

    QPushButton *buttonDoIt;

    QListWidget *logWidget;
    QProgressBar *progressWork;

    //Элементы для очистки sysroot
    QPushButton* clearSysrootButton = nullptr;

signals:
    void readyToCopy();
    void readyToClear();

private slots:
    void slotPushSourceButton();
    void slotPushTargetButton();
    void slotPushButtonDoIt();
    void clearTargetDir();

    void slotStartWork();
    void slotStopWork();
    void slotInformation(QString message);
    void slotError(QString message);
    void slotCheckProgress(int value);
};

#endif // WIDGET_H
