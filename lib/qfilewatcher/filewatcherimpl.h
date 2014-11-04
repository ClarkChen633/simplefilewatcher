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
#ifndef FILEWATCHERIMPL_H
#define FILEWATCHERIMPL_H
#include <QObject>
#include <QString>
#include <QStringList>
#include <QDebug>

#define DEBUG_INFORMATION 1
class FileWatcherImpl: public QObject
{
        Q_OBJECT
    public:
        virtual void start() = 0;
        virtual void stop() = 0;
        virtual bool watchDirectory(QString path) = 0;
        virtual bool unwatchDirectory(QString path) = 0;

    protected:
        void debug(QStringList debugInfo);

    signals:
#ifdef DEBUG_INFORMATION
        void debugInformation(QStringList data);
#endif
        void directoryCreated(QString path);
        void directoryDeleted(QString path);
        void directoryChanged(QString path);
        void directoryMoved(QString oldPath, QString newPath);
        void fileCreated(QString path);
        void fileDeleted(QString path);
        void fileMoved(QString oldPath, QString newPath);
        void fileUpdated(QString path);
};

Q_DECLARE_INTERFACE(FileWatcherImpl, "QFileWatcher/1.0")
#endif//FILEWATCHERIMPL_H
