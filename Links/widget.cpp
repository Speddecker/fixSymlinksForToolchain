#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QString linkName = "/home/stanislav/Desktop/fixSymlinkForToolchain/link2.txt";
    fileLink = new QLinuxFileLink(linkName);

    fixSymlink = new QFixSymlink;
    thrFixSymlink = new QThread;
    fixSymlink->moveToThread(thrFixSymlink);
    thrFixSymlink->start();

    connect(fixSymlink,SIGNAL(signalError(QString)),this,SLOT(slotError(QString)));
    connect(fixSymlink,SIGNAL(signalInformation(QString)),this,SLOT(slotInformation(QString)));
    connect(fixSymlink,SIGNAL(signalStartWork()),this,SLOT(slotStartWork()));
    connect(fixSymlink,SIGNAL(signalStopWork()),this,SLOT(slotStopWork()));
    connect(fixSymlink,SIGNAL(checkProgress(int)), this, SLOT(slotCheckProgress(int)));
    connect(this, SIGNAL(readyToCopy()), fixSymlink, SLOT(startCopy()));
    connect(this, SIGNAL(readyToClear()), fixSymlink, SLOT(startClear()));

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
    layoutPath->addWidget(labelSourcePath,0,0);
    editSourcePath = new QLineEdit(this);
    editSourcePath->setReadOnly(true);
    layoutPath->addWidget(editSourcePath,0,1);
    buttonSourcePath = new QPushButton("...",this);
    connect(buttonSourcePath,SIGNAL(pressed()),this,SLOT(slotPushSourceButton()));
    layoutPath->addWidget(buttonSourcePath,0,2);

    labelTargetPath = new QLabel(tr("Target path"),this);
    layoutPath->addWidget(labelTargetPath,1,0);
    editTargetPath = new QLineEdit(this);
    editTargetPath->setReadOnly(true);
    layoutPath->addWidget(editTargetPath,1,1);
    buttonTargetPath = new QPushButton("...",this);
    connect(buttonTargetPath,SIGNAL(pressed()),this,SLOT(slotPushTargetButton()));
    layoutPath->addWidget(buttonTargetPath,1,2);

    clearSysrootButton = new QPushButton(tr("Clear target dir"), this);
    connect(clearSysrootButton, SIGNAL(clicked(bool)), this, SLOT(clearTargetDir()));
    layoutPath->addWidget(clearSysrootButton, 2, 1);
    buttonDoIt = new QPushButton(tr("Do it"),this);
    connect(buttonDoIt,SIGNAL(pressed()),this,SLOT(slotPushButtonDoIt()));
    layoutPath->addWidget(buttonDoIt,2,2);

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
    fixSymlink->setSourceDirPath(editSourcePath->text());
    fixSymlink->setTargetDirPath(editTargetPath->text());
    emit readyToCopy();
}

void Widget::clearTargetDir()
{
    fixSymlink->setTargetDirPath(editTargetPath->text());
    emit readyToClear();
}

void Widget::slotStartWork()
{
    buttonDoIt->setEnabled(false);
    clearSysrootButton->setEnabled(false);
    progressWork->setValue(0);
}

void Widget::slotStopWork()
{
    buttonDoIt->setEnabled(true);
    clearSysrootButton->setEnabled(true);
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

void Widget::slotCheckProgress(int value)
{
    progressWork->setValue(value);
}
