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
#pragma once
#ifndef FILEMGR_H
#define FILEMGR_H

#include <QObject>
#include <QThread>
#include <QFileInfo>
#include "qfilewatcher.h"

class FileMgr : public QObject
{
        Q_OBJECT
    public:
        struct Sleep : QThread {
            static void millisec(unsigned long ms) {
                QThread::usleep(ms * 1000);
            }
        };
        explicit FileMgr(QObject *parent = 0);
        ~FileMgr();

        bool removeDir(QString folder);
        void copyDir(QString org, QString dest);
        void copyFilesInFolder(QFileInfoList files, QDir folder);
        bool mkdir(QString dirPath, bool eraseExisting);

        bool addFileWatch(QString fd);
        bool removeFileWatch(QString fd);
        void runFileWatch(void);
        void stopFileWatch(void);

    private:
        QFileWatcher _qfw;

        bool removeDirCore(QString folder);
        void copyDirCore(QString org, QString dest);

};

#endif // FILEMGR_H
