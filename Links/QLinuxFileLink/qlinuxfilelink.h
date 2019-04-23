#ifndef QLINUXFILELINK_H
#define QLINUXFILELINK_H

#include <QObject>
#include <QDebug>
#include <QDir>
#include <QFileInfo>

#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <unistd.h>
#include <stdio.h>

static void findRecursion(const QString &path, const QString &pattern, QStringList *result);

class QLinuxFileLink : public QObject
{
    Q_OBJECT
public:
    explicit QLinuxFileLink(QObject *parent = nullptr);
    explicit QLinuxFileLink(QString linkName,QObject *parent = nullptr);
    ~QLinuxFileLink();

    QString getLinkName();
    QString getTargenName();
    QStringList getSymLinkList(QString path);

    void setLinkName(QString linkName);
    bool setTargetName(QString targetName);

    bool isLink(QString fileName);
private:
    QString currentLinkName;
    QString currentTargetName;
    void loadLinkFile();
    bool saveLinkFile();

signals:

public slots:
};

#endif // QLINUXFILELINK_H
