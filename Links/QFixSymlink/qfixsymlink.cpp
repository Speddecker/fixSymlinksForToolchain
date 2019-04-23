#include "qfixsymlink.h"

QFixSymlink::QFixSymlink(QObject *parent) : QObject(parent)
{
}

QFixSymlink::~QFixSymlink()
{

}

void QFixSymlink::doFixSymlink()
{
    emit signalStartWork();

    if (checkDir(sourceDirPath,targetDirPath))
    {
        if (copyContentDir(sourceDirPath,targetDirPath))
        {
            QStringList symLinks;
            findSymLinks(targetDirPath,&symLinks);
        }
    }

    emit signalStopWork();
}

QString QFixSymlink::getSourceDirPath() const
{
    return sourceDirPath;
}

void QFixSymlink::setSourceDirPath(const QString &value)
{
    sourceDirPath = value;
}

QString QFixSymlink::getTargetDirPath() const
{
    return targetDirPath;
}

void QFixSymlink::setTargetDirPath(const QString &value)
{
    targetDirPath = value;
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
            if (sourceD.entryList(QStringList("*"),QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot).length()>0)
            {
                emit signalInformation(tr("Directories are valid"));
                Output = true;
            } else emit signalError(tr("Source directory is empty"));
        } else emit signalError(tr("Target directory error"));
    } else emit signalError(tr("Source directory error"));

    return Output;
}

bool QFixSymlink::copyContentDir(QString sourceDir,QString targetDir)
{
    bool Output = true;
    emit signalInformation(tr("Start copy rootfs"));

    //Получаем структуру внутренних папок
    QDir currentDir(sourceDir);
    QStringList dirs;

    QString buf;
    QDirIterator it(sourceDir, QDirIterator::Subdirectories);
    QFileInfo fileInfo;
    while (it.hasNext()) {
        buf = it.next();
        fileInfo.setFile(buf);
        if ((buf.indexOf("/.")<0)&&(buf.indexOf("/..")<0)&&(fileInfo.isDir()))
        {
            buf.remove(sourceDir);
            dirs.append(buf);
        }
    }

    QDir targetD (targetDir);


    //Создаем дерево в целевой папке
    for (int i=0;i<dirs.length();i++)
    {
        QDir dir(targetDir+dirs.at(i));
        if (!dir.exists())
            targetD.mkdir(targetDir+dirs.at(i));
    }

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
    copiedFiles.setFileName(targetD.path() + "/copiedFiles.txt");
    if(copiedFiles.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        qDebug() << "Копируем файлы" << files.length();
        for (int i=0;i<files.length();i++)
        {
            qDebug() << i << "of" << files.length() << files.at(i);
            emit checkProgress(static_cast<double>(i)/files.length()*100+0.5);
            if(QFile::copy(currentDir.path()+files.at(i),targetD.path()+files.at(i)))
            {
                *textStream << targetD.path() + files.at(i) << '\n';
            }
        }

        emit signalInformation(tr("Finish copy rootfs"));
    }
    else emit signalError(tr("Cannot open file in target dir"));

    return Output;
}

bool QFixSymlink::findSymLinks(QString sourceDir,QStringList *symLinks)
{
    emit signalInformation(tr("Start search symlinks"));
    bool Output = true;

    QFileInfo fileInfo;

    QStringList files;

    findRecursion(sourceDir,"*",&files);

    for (int i=0;i<files.length();i++)
    {
        fileInfo.setFile(files.at(i));
        if (fileInfo.isSymLink()) symLinks->append(files.at(i));
    }

    emit signalInformation(tr("Found")+" "+QString::number(symLinks->length())+" "+tr("symlinks"));
    return Output;
}

void QFixSymlink::startCopy()
{
    doFixSymlink();
}

void QFixSymlink::startClear()
{
    clearTargetDir(targetDirPath);
}

void QFixSymlink::clearTargetDir(QString targetDir)
{
    emit signalInformation(tr("Start clear target dir"));

    if(targetDir.isEmpty())
    {
        emit signalError(tr("Target dir path is empty"));
    }
    else
    {
        copiedFiles.setFileName(targetDir + "/copiedFiles.txt");
        if(copiedFiles.open(QIODevice::ReadWrite | QIODevice::Text))
        {
            // Получаем список скопированных файлов
            QStringList files;
            QTextStream stream (&copiedFiles);

            while (!copiedFiles.atEnd())
            {
                files.append(stream.readLine());
            }

            // Удаляем файлы
            for (int i = 0; i < files.length(); i++)
            {
                emit checkProgress(static_cast<double>(i)/files.length()*100+0.5);

                QFile file (files.at(i));
                file.remove();
            }

            emit signalInformation(tr("Target dir has been cleared"));
        }
        else emit signalError(tr("Cannot open file in targed dir"));
    }
}

static void findRecursion(const QString &path, const QString &pattern, QStringList *result)
{
    QDir currentDir(path);
    const QString prefix = path + QLatin1Char('/');
    foreach (const QString &match, currentDir.entryList(QStringList(pattern), QDir::Files | QDir::Hidden | QDir::System ))
    {
        if(match.indexOf("libQt") < 0)
            result->append(prefix + match);
    }
    foreach (const QString &dir, currentDir.entryList(QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot))
    {
        if((dir != "home")&&(dir != "etc")&&(dir != "tmp")&&(dir != "bin")&&(dir != "sbin")&&(dir != "var")&&(dir != "dev"))
            findRecursion(prefix + dir, pattern, result);
    }
}


