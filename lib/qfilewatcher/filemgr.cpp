/***************************************************************************
* The MIT License (MIT)
*
* Copyright (c) 2014 ClarkChen, <clarkchen633@gmail.com>, et al.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
***************************************************************************/
#include "filemgr.h"
#include <QVector>
#include <QFile>
#include <QDir>
#include "qsleeper.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FileMgr::FileMgr(QObject *parent)
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FileMgr::~FileMgr()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool FileMgr::addFileWatch(QString fd)
{
    return _qfw.watchDirectory(fd);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool FileMgr::removeFileWatch(QString fd)
{
    return _qfw.unwatchDirectory(fd);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FileMgr::runFileWatch(void)
{
    _qfw.start();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FileMgr::stopFileWatch(void)
{
    _qfw.stop();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool FileMgr::removeDirCore(QString folder)
{
    bool  allRemoved = true;

    QDir dir = QDir(folder);
    QFileInfoList files = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden);
    for (int indf = 0; indf < files.size(); indf++) {
        QFile provFile(files.at(indf).absoluteFilePath());
        provFile.setPermissions(provFile.permissions() | QFile::WriteUser);
        allRemoved = dir.remove(provFile.fileName()) && allRemoved;
    }

    QStringList folders = dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
    QString provFolder;
    for (int indf = 0; indf < folders.size(); indf++) {
        provFolder = folders[indf];
        allRemoved = removeDir(dir.absolutePath() + QDir::separator() + provFolder) && allRemoved;
    }

    return allRemoved;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool FileMgr::removeDir(QString folder)
{
    bool removed;
    QDir dir = QDir(folder);
    // if dir already exists, deleting it
    if (dir.exists()) {
        FileMgr::removeDirCore(folder);

        bool tmp = dir.cdUp();
        dir.refresh();
        removed = dir.rmdir(folder);
        if (!removed) {
            //wait one second before retrying (maybe useful during MOO)
            QSleeper::msleep(1000);
            removed = dir.rmdir(folder);

            if (!removed) {
                //InfoSender::instance()->send(Info(ListInfo::FOLDERUNREMOVABLE,folder));
                return false;
            }
        }

        return removed;

    } else
        return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FileMgr::copyDirCore(QString org, QString dest)
{
    QDir oriDir = QDir(org);
    QDir destDir(dest);

    // if dir does not exist, create it
    if (!destDir.exists())
        destDir.mkpath(dest);

    if (oriDir.exists()) {
        QStringList files = oriDir.entryList();
        for (int indf = 0; indf < files.size(); indf++) {
            QString oriFilePath = oriDir.absoluteFilePath(files[indf]);
            QString dstFilePath = destDir.absoluteFilePath(files[indf]);

            QFile::copy(oriFilePath, dstFilePath);
        }
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FileMgr::copyFilesInFolder(QFileInfoList files, QDir folder)
{
    if (!folder.exists()) {
        QDir tmpDir;
        tmpDir.mkpath(folder.absolutePath());
    }

    for (int i = 0; i < files.size(); i++) {
        QFileInfo fileInfo(files.at(i));
        QFile::copy(fileInfo.absoluteFilePath(), folder.filePath(fileInfo.fileName()));
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FileMgr::copyDir(QString org, QString dest)
{
    //InfoSender::instance()->debug("Copy dir "+org +" to "+dest);

    QDir destDir(dest);
    // if dir already exists, deleting it
    if (destDir.exists())
        removeDir(dest);

    copyDirCore(org, dest);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool FileMgr::mkdir(QString dirPath, bool eraseExisting)
{
    bool ok;

    QDir tmpDir(dirPath);
    QFile tmpDirFile(dirPath);
    tmpDirFile.setPermissions(tmpDirFile.permissions() | QFile::WriteUser);

    if (tmpDir.exists() && eraseExisting)
        FileMgr::removeDir(dirPath);

    tmpDir.mkdir(dirPath);
    tmpDirFile.setPermissions(tmpDirFile.permissions() | QFile::WriteUser);

    ok = tmpDir.exists() && tmpDirFile.isWritable()
         && (tmpDir.entryList(QDir::NoDotAndDotDot).isEmpty() || !eraseExisting);

    return ok;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
