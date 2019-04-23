#ifndef QFIXSYMLINK_H
#define QFIXSYMLINK_H

#include <QObject>
#include <QThread>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QProcess>
#include <QDirIterator>
#include <QTextCodec>
#include "../QLinuxFileLink/qlinuxfilelink.h"
#include <QDebug>

static void findRecursion(const QString &path, const QString &pattern, QStringList *result);

class QFixSymlink : public QObject
{
    Q_OBJECT
public:
    explicit QFixSymlink(QObject *parent = nullptr);
    ~QFixSymlink();

    //void doFixSymlink(QString sourceDir,QString targetDir);
    void doFixSymlink();

    QString getSourceDirPath() const;
    void setSourceDirPath(const QString &value);

    QString getTargetDirPath() const;
    void setTargetDirPath(const QString &value);

private:
    QLinuxFileLink *fileLink;

    QString sourceDirPath;
    QString targetDirPath;
    bool checkDir(QString sourceDir,QString targetDir);
    bool copyContentDir(QString sourceDir,QString targetDir);
    bool findSymLinks(QString sourceDir,QStringList *symLinks);
    void clearTargetDir(QString targetDir);

    QFile copiedFiles;
    QTextStream* textStream = new QTextStream(&copiedFiles);

signals:
    void signalStartWork();
    void signalStopWork();
    void signalSetProgressValue(int value);
    void signalInformation(QString message);
    void signalError(QString message);
    void checkProgress(int value);

public slots:
    void startCopy();
    void startClear();
};

#endif // QFIXSYMLINK_H
