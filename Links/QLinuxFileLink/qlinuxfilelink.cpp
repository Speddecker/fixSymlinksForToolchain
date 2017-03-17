#include "qlinuxfilelink.h"

QLinuxFileLink::QLinuxFileLink(QObject *parent) : QObject(parent)
{

}

QLinuxFileLink::QLinuxFileLink(QString linkName,QObject *parent) : QObject(parent)
{
    currentLinkName = linkName;
    loadLinkFile();
}

QLinuxFileLink::~QLinuxFileLink()
{

}

QString QLinuxFileLink::getLinkName()
{
    return currentLinkName;
}

QString QLinuxFileLink::getTargenName()
{
    return currentTargetName;
}

void QLinuxFileLink::setLinkName(QString linkName)
{
    currentLinkName = linkName;
    loadLinkFile();
}

bool QLinuxFileLink::setTargetName(QString targetName)
{
    currentTargetName = targetName;
    return saveLinkFile();
}

void QLinuxFileLink::loadLinkFile()
{
    struct stat sb;
    char *linkname;
    ssize_t r, bufsiz;

    qDebug()<<"lstat="<<lstat(currentLinkName.toStdString().c_str(), &sb);
    bufsiz = sb.st_size + 1;
    if (sb.st_size == 0) bufsiz = PATH_MAX;
    linkname = (char*)malloc(bufsiz);
    r = readlink(currentLinkName.toStdString().c_str(), linkname, bufsiz);
    linkname[r] = '\0';

    currentTargetName = QString(linkname);
}

bool QLinuxFileLink::saveLinkFile()
{
    bool Output = false;

    if ((!currentLinkName.isEmpty())&&(!currentTargetName.isEmpty()))
    {
        remove(currentLinkName.toStdString().c_str());
        if (symlink(currentTargetName.toStdString().c_str(),currentLinkName.toStdString().c_str())!=-1)
            Output = true;
    }

    return Output;
}

QStringList QLinuxFileLink::getSymLinkList(QString path)
{
    QStringList result;
    QStringList result2;

    findRecursion(path,"*",&result);

    QFileInfo fileInfo;
    for (int i=0;i<result.length();i++)
    {
        fileInfo.setFile(result.at(i));
        if (fileInfo.isSymLink())
            result2.append(result.at(i));
    }

    return result2;
}

const bool QLinuxFileLink::isLink(QString fileName)
{
    QFileInfo fileInfo(fileName);

    return fileInfo.isSymLink();
}

static void findRecursion(const QString &path, const QString &pattern, QStringList *result)
{
    QDir currentDir(path);
    const QString prefix = path + QLatin1Char('/');
    foreach (const QString &match, currentDir.entryList(QStringList(pattern), QDir::Files ))
        result->append(prefix + match);
    foreach (const QString &dir, currentDir.entryList(QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot))
        findRecursion(prefix + dir, pattern, result);
}
