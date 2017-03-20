#include "qfixsymlink.h"

QFixSymlink::QFixSymlink(QObject *parent) : QObject(parent)
{
    /*
    qDebug()<<"find symlinks";
    QStringList sym;
    findSymLinks("/opt/QtBuild/rootfs",&sym);

    for (int i=0;i<sym.length();i++)
        qDebug()<<sym.at(i);
        */
}

QFixSymlink::~QFixSymlink()
{

}

void QFixSymlink::doFixSymlink(QString sourceDir,QString targetDir)
{
    emit signalStartWork();

    if (checkDir(sourceDir,targetDir))
    {
        qDebug()<<"dir ok";
        if (copyContentDir(sourceDir,targetDir))
        {
            QStringList symLinks;
            if (findSymLinks(targetDir,&symLinks))
            {
                qDebug()<<"Find symlinks Ok";
            }
        }
    }

    emit signalStopWork();
}

bool QFixSymlink::checkDir(QString sourceDir,QString targetDir)
{
    bool Output = false;

    QDir sourceD(sourceDir);
    QDir targetD(targetDir);

    //Проверяем папки
    if ((sourceD.exists())&&(!sourceDir.isEmpty()))
    {
        if ((targetD.exists())&&(!targetDir.isEmpty()))
        {
            if (targetD.entryList(QStringList("*"),QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot).length()==0)
            {
                if (sourceD.entryList(QStringList("*"),QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot).length()>0)
                {
                    emit signalInformation(tr("Folders checked"));
                    Output = true;
                } else emit signalError(tr("Source directory is empty"));
            } else emit signalError(tr("Target directory not empty"));
        } else emit signalError(tr("Target directory error"));
    } else emit signalError(tr("Source directory error"));

    return Output;
}

bool QFixSymlink::copyContentDir(QString sourceDir,QString targetDir)
{
    bool Output = true;
    emit signalInformation(tr("Start copy rootfs"));
    //Получаем структуру внутренних папок
    //QDir currentDir("/opt/QtBuild/rootfs");
    QDir currentDir(sourceDir);
    QStringList dirs;

    QString buf;
    QDirIterator it("/opt/QtBuild/rootfs", QDirIterator::Subdirectories);
    QFileInfo fileInfo;
    while (it.hasNext()) {
        buf = it.next();
        fileInfo.setFile(buf);
        if ((buf.indexOf(".")<0)&&(buf.indexOf("..")<0)&&(fileInfo.isDir()))
        {

            buf.remove("/opt/QtBuild/rootfs");
            dirs.append(buf);
        }
    }

    QDir targetD (targetDir);
    //Создаем лерево в целевой папке
    for (int i=0;i<dirs.length();i++)
        targetD.mkdir(targetDir+dirs.at(i));



    // Составляем дерево файлов
    QStringList files;
    findRecursion(currentDir.path(),"*",&files);

    for (int i=0;i<files.length();i++)
    {
        buf = files.at(i);
        buf.remove(currentDir.path());
        files.replace(i,buf);
    }


    // Копируем файлы
    for (int i=0;i<files.length();i++)
    {
       QFile::copy(currentDir.path()+files.at(i),targetD.path()+files.at(i));
    }
    emit signalInformation(tr("Finish copy rootfs"));
    return Output;
}

bool QFixSymlink::findSymLinks(QString sourceDir,QStringList *symLinks)
{
    emit signalInformation(tr("Find symlinks"));
    bool Output = true;

    QFileInfo fileInfo;

    QStringList files;

    findRecursion(sourceDir,"*",&files);

    for (int i=0;i<files.length();i++)
    {
        fileInfo.setFile(files.at(i));
        if (fileInfo.isSymLink()) symLinks->append(files.at(i));
    }

    emit signalInformation(tr("Fount")+" "+QString::number(symLinks->length())+" "+tr("symlinks"));
    return Output;
}

static void findRecursion(const QString &path, const QString &pattern, QStringList *result)
{
    QDir currentDir(path);
    const QString prefix = path + QLatin1Char('/');
    foreach (const QString &match, currentDir.entryList(QStringList(pattern), QDir::Files | QDir::Hidden | QDir::System ))
        result->append(prefix + match);
    foreach (const QString &dir, currentDir.entryList(QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot))
        findRecursion(prefix + dir, pattern, result);
}


