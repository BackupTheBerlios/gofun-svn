/***************************************************************************
 *   Copyright (C) 2004 by Tobias Glaesser                                 *
 *   tobi.web@gmx.de                                                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef GOFUN_DIRECTORY_BROWSER
#define GOFUN_DIRECTORY_BROWSER

/****************************************************************************
** $Id: qt/dirview.h   3.3.3   edited May 27 2003 $
**
** Copyright (C) 1992-2000 Trolltech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include <vector>

#include <qlistview.h>
#include <qstring.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qtimer.h>
#include <qdialog.h>

class QWidget;
class QDragEnterEvent;
class QDragMoveEvent;
class QDragLeaveEvent;
class QDropEvent;

class GofunFileItem : public QListViewItem
{
public:
    GofunFileItem( QListViewItem *parent, const QString &s1, const QString &s2 )
        : QListViewItem( parent, s1, s2 ), pix( 0 ) {}

    const QPixmap *pixmap( int i ) const;
#if !defined(Q_NO_USING_KEYWORD)
    using QListViewItem::setPixmap;
#endif
    void setPixmap( QPixmap *p );

private:
    QPixmap *pix;

};

class GofunDirectory : public QListViewItem
{
public:
    GofunDirectory( QListView * parent, const QString& filename );
    GofunDirectory( GofunDirectory * parent, const QString& filename, const QString &col2 )
        : QListViewItem( parent, filename, col2 ), pix( 0 ) {}
    GofunDirectory( GofunDirectory * parent, const QString& filename );

    QString text( int column ) const;

    QString fullName();

    void setOpen( bool );
    void setup();

    const QPixmap *pixmap( int i ) const;
#if !defined(Q_NO_USING_KEYWORD)
    using QListViewItem::setPixmap;
#endif
    void setPixmap( QPixmap *p );
    
private:
    QFile f;
    GofunDirectory * p;
    bool readable;
    bool showDirsOnly;
    QPixmap *pix;
};

class GofunDirectoryView;
class QLabel;

class GofunDirectoryBrowser : public QDialog
{
	Q_OBJECT

	public:
	GofunDirectoryBrowser();
	void setStartDirectory(const QString&);
	QString selected();
	
	public slots:
	void showHiddenDirs(bool);
	
	private:
	GofunDirectoryView* dir_view;
	QLabel* current_selection;
};

class GofunDirectoryView : public QListView
{
    Q_OBJECT

public:
    GofunDirectoryView( QWidget *parent = 0, const char *name = 0, bool sdo = FALSE );
    bool showDirsOnly() { return dirsOnly; }
    void showHiddenDirs(bool);

public slots:
    void setDir( const QString & );

signals:
    void folderSelected( const QString & );

protected slots:
    void slotFolderSelected( QListViewItem * );
    void openFolder();

protected:
    void contentsDragEnterEvent( QDragEnterEvent *e );
    void contentsDragMoveEvent( QDragMoveEvent *e );
    void contentsDragLeaveEvent( QDragLeaveEvent *e );
    void contentsDropEvent( QDropEvent *e );
    void contentsMouseMoveEvent( QMouseEvent *e );
    void contentsMousePressEvent( QMouseEvent *e );
    void contentsMouseReleaseEvent( QMouseEvent *e );

private:
    QString fullPath(QListViewItem* item);
    bool dirsOnly;
    QListViewItem *oldCurrent;
    QListViewItem *dropItem;
    QTimer* autoopen_timer;
    QPoint presspos;
    bool mousePressed;
    bool show_hidden;
    std::vector<GofunDirectory*> hidden_directories;

friend class GofunDirectory;
};

#endif

