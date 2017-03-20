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

    fixSymlink = new QFixSymlink;
    thrFixSymlink = new QThread;
    fixSymlink->moveToThread(thrFixSymlink);

    connect(fixSymlink,SIGNAL(signalError(QString)),this,SLOT(slotError(QString)));
    connect(fixSymlink,SIGNAL(signalInformation(QString)),this,SLOT(slotInformation(QString)));
    connect(fixSymlink,SIGNAL(signalStartWork()),this,SLOT(slotStartWork()));
    connect(fixSymlink,SIGNAL(signalStopWork()),this,SLOT(slotStopWork()));

    createForm();
    slotInformation(tr("Start programm"));
}

Widget::~Widget()
{
    delete fileLink;
}

void Widget::createForm()
{
    mainLayout = new QVBoxLayout(this);

    layoutPath = new QGridLayout;
    layoutPath->setMargin(0);
    mainLayout->addLayout(layoutPath);

    labelSourcePath = new QLabel(tr("Source path"),this);
    layoutPath->addWidget(labelSourcePath,0,0,1,1);
    editSourcePath = new QLineEdit(this);
    editSourcePath->setReadOnly(true);
    layoutPath->addWidget(editSourcePath,0,1,1,1);
    buttonSourcePath = new QPushButton("...",this);
    connect(buttonSourcePath,SIGNAL(pressed()),this,SLOT(slotPushSourceButton()));
    layoutPath->addWidget(buttonSourcePath,0,2,1,1);

    labelTargetPath = new QLabel(tr("Target path"),this);
    layoutPath->addWidget(labelTargetPath,1,0,1,1);
    editTargetPath = new QLineEdit(this);
    editTargetPath->setReadOnly(true);
    layoutPath->addWidget(editTargetPath,1,1,1,1);
    buttonTargetPath = new QPushButton("...",this);
    connect(buttonTargetPath,SIGNAL(pressed()),this,SLOT(slotPushTargetButton()));
    layoutPath->addWidget(buttonTargetPath,1,2,1,1);

    buttonDoIt = new QPushButton(tr("Do it"),this);
    connect(buttonDoIt,SIGNAL(pressed()),this,SLOT(slotPushButtonDoIt()));
    layoutPath->addWidget(buttonDoIt,2,2,1,1);

    logWidget = new QListWidget(this);
    mainLayout->addWidget(logWidget);

    progressWork = new QProgressBar(this);
    connect(fixSymlink,SIGNAL(signalSetProgressValue(int)),progressWork,SLOT(setValue(int)));
    mainLayout->addWidget(progressWork);
}

void Widget::slotPushSourceButton()
{
    QString sourcePath = QFileDialog::getExistingDirectory(this,tr("Select source Directory"));
    if (!sourcePath.isEmpty())
        editSourcePath->setText(sourcePath);
}

void Widget::slotPushTargetButton()
{
    QString targetPath = QFileDialog::getExistingDirectory(this,tr("Select target Directory"));
    if (!targetPath.isEmpty())
        editTargetPath->setText(targetPath);
}

void Widget::slotPushButtonDoIt()
{
    fixSymlink->doFixSymlink(editSourcePath->text(),editTargetPath->text());
}

void Widget::slotStartWork()
{
    layoutPath->setEnabled(false);
    progressWork->setValue(0);

}

void Widget::slotStopWork()
{
    layoutPath->setEnabled(true);
}

void Widget::slotInformation(QString message)
{
    QListWidgetItem *item = new QListWidgetItem(QIcon(":/icon/info.png"),QTime::currentTime().toString("hh:mm:ss")+" "+message);
    logWidget->addItem(item);
}

void Widget::slotError(QString message)
{
    QListWidgetItem *item = new QListWidgetItem(QIcon(":/icon/error.png"),QTime::currentTime().toString("hh:mm:ss")+" "+message);
    logWidget->addItem(item);
}
