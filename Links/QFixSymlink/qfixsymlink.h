#ifndef QFIXSYMLINK_H
#define QFIXSYMLINK_H

#include <QObject>
#include <QThread>
#include <QDir>
#include <QDebug>
#include <QProcess>
#include <QDirIterator>
#include "../QLinuxFileLink/qlinuxfilelink.h"

static void findRecursion(const QString &path, const QString &pattern, QStringList *result);

class QFixSymlink : public QObject
{
    Q_OBJECT
public:
    explicit QFixSymlink(QObject *parent = 0);
    ~QFixSymlink();

    void doFixSymlink(QString sourceDir,QString targetDir);

private:
    QLinuxFileLink *fileLink;
    bool checkDir(QString sourceDir,QString targetDir);
    bool copyContentDir(QString sourceDir,QString targetDir);
    bool findSymLinks(QString sourceDir,QStringList *symLinks);

signals:
    void signalStartWork();
    void signalStopWork();
    void signalSetProgressValue(int value);
    void signalInformation(QString message);
    void signalError(QString message);

public slots:
};

#endif // QFIXSYMLINK_H
