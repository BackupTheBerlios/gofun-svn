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

/****************************************************************************
** $Id: qt/dirview.cpp   3.3.3   edited Oct 28 2003 $
**
** Copyright (C) 1992-2000 Trolltech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "gofun_directory_browser.h"

#include <qdir.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qpixmap.h>
#include <qevent.h>
#include <qpoint.h>
#include <qmessagebox.h>
#include <qdragobject.h>
#include <qmime.h>
#include <qstrlist.h>
#include <qstringlist.h>
#include <qapplication.h>
#include <qheader.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qcheckbox.h>

static const char* folder_closed_xpm[]={
    "16 16 9 1",
    "g c #808080",
    "b c #c0c000",
    "e c #c0c0c0",
    "# c #000000",
    "c c #ffff00",
    ". c None",
    "a c #585858",
    "f c #a0a0a4",
    "d c #ffffff",
    "..###...........",
    ".#abc##.........",
    ".#daabc#####....",
    ".#ddeaabbccc#...",
    ".#dedeeabbbba...",
    ".#edeeeeaaaab#..",
    ".#deeeeeeefe#ba.",
    ".#eeeeeeefef#ba.",
    ".#eeeeeefeff#ba.",
    ".#eeeeefefff#ba.",
    ".##geefeffff#ba.",
    "...##gefffff#ba.",
    ".....##fffff#ba.",
    ".......##fff#b##",
    ".........##f#b##",
    "...........####."};

static const char* folder_open_xpm[]={
    "16 16 11 1",
    "# c #000000",
    "g c #c0c0c0",
    "e c #303030",
    "a c #ffa858",
    "b c #808080",
    "d c #a0a0a4",
    "f c #585858",
    "c c #ffdca8",
    "h c #dcdcdc",
    "i c #ffffff",
    ". c None",
    "....###.........",
    "....#ab##.......",
    "....#acab####...",
    "###.#acccccca#..",
    "#ddefaaaccccca#.",
    "#bdddbaaaacccab#",
    ".eddddbbaaaacab#",
    ".#bddggdbbaaaab#",
    "..edgdggggbbaab#",
    "..#bgggghghdaab#",
    "...ebhggghicfab#",
    "....#edhhiiidab#",
    "......#egiiicfb#",
    "........#egiibb#",
    "..........#egib#",
    "............#ee#"};

static const char * folder_locked[]={
    "16 16 10 1",
    "h c #808080",
    "b c #ffa858",
    "f c #c0c0c0",
    "e c #c05800",
    "# c #000000",
    "c c #ffdca8",
    ". c None",
    "a c #585858",
    "g c #a0a0a4",
    "d c #ffffff",
    "..#a#...........",
    ".#abc####.......",
    ".#daa#eee#......",
    ".#ddf#e##b#.....",
    ".#dfd#e#bcb##...",
    ".#fdccc#daaab#..",
    ".#dfbbbccgfg#ba.",
    ".#ffb#ebbfgg#ba.",
    ".#ffbbe#bggg#ba.",
    ".#fffbbebggg#ba.",
    ".##hf#ebbggg#ba.",
    "...###e#gggg#ba.",
    ".....#e#gggg#ba.",
    "......###ggg#b##",
    ".........##g#b##",
    "...........####."};

static const char * pix_file []={
    "16 16 7 1",
    "# c #000000",
    "b c #ffffff",
    "e c #000000",
    "d c #404000",
    "c c #c0c000",
    "a c #ffffc0",
    ". c None",
    "................",
    ".........#......",
    "......#.#a##....",
    ".....#b#bbba##..",
    "....#b#bbbabbb#.",
    "...#b#bba##bb#..",
    "..#b#abb#bb##...",
    ".#a#aab#bbbab##.",
    "#a#aaa#bcbbbbbb#",
    "#ccdc#bcbbcbbb#.",
    ".##c#bcbbcabb#..",
    "...#acbacbbbe...",
    "..#aaaacaba#....",
    "...##aaaaa#.....",
    ".....##aa#......",
    ".......##......."};

QPixmap *folderLocked = 0;
QPixmap *folderClosed = 0;
QPixmap *folderOpen = 0;
QPixmap *fileNormal = 0;

/*****************************************************************************
 *
 * Class Directory
 *
 *****************************************************************************/

GofunDirectory::GofunDirectory( GofunDirectory * parent, const QString& filename )
		: QListViewItem( parent ), f(filename),
		showDirsOnly( parent->showDirsOnly ),
		pix( 0 )
{
	p = parent;
	readable = QDir( fullName() ).isReadable();

	if ( !readable )
		setPixmap( folderLocked );
	else
		setPixmap( folderClosed );
}


GofunDirectory::GofunDirectory( QListView * parent, const QString& filename )
		: QListViewItem( parent ), f(filename),
		showDirsOnly( ( (GofunDirectoryView*)parent )->showDirsOnly() ),
		pix( 0 )
{
	p = 0;
	readable = QDir( fullName() ).isReadable();
}


void GofunDirectory::setPixmap( QPixmap *px )
{
	pix = px;
	setup();
	widthChanged( 0 );
	invalidateHeight();
	repaint();
}


const QPixmap *GofunDirectory::pixmap( int i ) const
{
	if ( i )
		return 0;
	return pix;
}

void GofunDirectory::setOpen( bool o )
{
	if ( o )
		setPixmap( folderOpen );
	else
		setPixmap( folderClosed );

	if ( o && !childCount() )
	{
		QString s( fullName() );
		QDir thisDir( s );
		if ( !thisDir.isReadable() )
		{
			readable = FALSE;
			setExpandable( FALSE );
			return;
		}
		thisDir.setFilter(QDir::Hidden | QDir::Dirs);

		listView()->setUpdatesEnabled( FALSE );
		const QFileInfoList * files = thisDir.entryInfoList();
		if ( files )
		{
			QFileInfoListIterator it( *files );
			QFileInfo * fi;
			while( (fi=it.current()) != 0 )
			{
				++it;
				if (fi->fileName() == "." || fi->fileName() == ".." ) //TODO Code for making hidden files visible as option
					; // nothing
				else if ( fi->isSymLink() && !showDirsOnly )
				{
					GofunFileItem *item = new GofunFileItem( this, fi->fileName(),
					                      "Symbolic Link" );
					item->setPixmap( fileNormal );
				}
				else if ( fi->isDir() )
				{
					GofunDirectory* item = new GofunDirectory( this, fi->fileName() );
					if( !dynamic_cast<GofunDirectoryView*>(listView())->show_hidden && (fi->fileName().find(".") == 0))
					{
						item->setVisible(false);
						dynamic_cast<GofunDirectoryView*>(listView())->hidden_directories.push_back(item);
					}
				}
				else if ( !showDirsOnly )
				{
					GofunFileItem *item
					= new GofunFileItem( this, fi->fileName(),
					                     fi->isFile()?"File":"Special" );
					item->setPixmap( fileNormal );
				}
			}
		}
		listView()->setUpdatesEnabled( TRUE );
	}
	QListViewItem::setOpen( o );
}


void GofunDirectory::setup()
{
	setExpandable( TRUE );
	QListViewItem::setup();
}


QString GofunDirectory::fullName()
{
	QString s;
	if ( p )
	{
		s = p->fullName();
		s.append( f.name() );
		s.append( "/" );
	}
	else
	{
		s = f.name();
	}
	return s;
}


QString GofunDirectory::text( int column ) const
{
	if ( column == 0 )
		return f.name();
	else if ( readable )
		return "Directory";
	else
		return "Unreadable Directory";
}

/*****************************************************************************
 *
 * Class DirectoryView
 *
 *****************************************************************************/

GofunDirectoryView::GofunDirectoryView( QWidget *parent, const char *name, bool sdo )
		: QListView( parent, name ), dirsOnly( sdo ), oldCurrent( 0 ),
		dropItem( 0 ), mousePressed( FALSE )
{
	autoopen_timer = new QTimer( this );
	if ( !folderLocked )
	{
		folderLocked = new QPixmap( folder_locked );
		folderClosed = new QPixmap( folder_closed_xpm );
		folderOpen = new QPixmap( folder_open_xpm );
		fileNormal = new QPixmap( pix_file );
	}

	connect( this, SIGNAL( doubleClicked( QListViewItem * ) ),
	         this, SLOT( slotFolderSelected( QListViewItem * ) ) );
	connect( this, SIGNAL( returnPressed( QListViewItem * ) ),
	         this, SLOT( slotFolderSelected( QListViewItem * ) ) );

	setAcceptDrops( TRUE );
	viewport()->setAcceptDrops( TRUE );

	connect( autoopen_timer, SIGNAL( timeout() ),
	         this, SLOT( openFolder() ) );

	show_hidden = false;
}

void GofunDirectoryView::slotFolderSelected( QListViewItem *i )
{
	if ( !i || !showDirsOnly() )
		return;

	GofunDirectory *dir = (GofunDirectory*)i;
	emit folderSelected( dir->fullName() );
}

void GofunDirectoryView::openFolder()
{
	autoopen_timer->stop();
	if ( dropItem && !dropItem->isOpen() )
	{
		dropItem->setOpen( TRUE );
		dropItem->repaint();
	}
}

static const int autoopenTime = 750;


void GofunDirectoryView::contentsDragEnterEvent( QDragEnterEvent *e )
{
	if ( !QUriDrag::canDecode(e) )
	{
		e->ignore();
		return;
	}

	oldCurrent = currentItem();

	QListViewItem *i = itemAt( contentsToViewport(e->pos()) );
	if ( i )
	{
		dropItem = i;
		autoopen_timer->start( autoopenTime );
	}
}


void GofunDirectoryView::contentsDragMoveEvent( QDragMoveEvent *e )
{
	if ( !QUriDrag::canDecode(e) )
	{
		e->ignore();
		return;
	}

	QPoint vp = contentsToViewport( ( (QDragMoveEvent*)e )->pos() );
	QListViewItem *i = itemAt( vp );
	if ( i )
	{
		setSelected( i, TRUE );
		e->accept();
		if ( i != dropItem )
		{
			autoopen_timer->stop();
			dropItem = i;
			autoopen_timer->start( autoopenTime );
		}
		switch ( e->action() )
		{
		case QDropEvent::Copy:
			break;
		case QDropEvent::Move:
			e->acceptAction();
			break;
		case QDropEvent::Link:
			e->acceptAction();
			break;
		default:
			;
		}
	}
	else
	{
		e->ignore();
		autoopen_timer->stop();
		dropItem = 0;
	}
}

void GofunDirectoryView::contentsDragLeaveEvent( QDragLeaveEvent * )
{
	autoopen_timer->stop();
	dropItem = 0;

	setCurrentItem( oldCurrent );
	setSelected( oldCurrent, TRUE );
}

void GofunDirectoryView::contentsDropEvent( QDropEvent *e )
{
	autoopen_timer->stop();

	if ( !QUriDrag::canDecode(e) )
	{
		e->ignore();
		return;
	}

	QListViewItem *item = itemAt( contentsToViewport(e->pos()) );
	if ( item )
	{

		QStrList lst;

		QUriDrag::decode( e, lst );

		QString str;

		switch ( e->action() )
		{
		case QDropEvent::Copy:
			str = "Copy";
			break;
		case QDropEvent::Move:
			str = "Move";
			e->acceptAction();
			break;
		case QDropEvent::Link:
			str = "Link";
			e->acceptAction();
			break;
		default:
			str = "Unknown";
		}

		str += "\n\n";

		e->accept();

		for ( uint i = 0; i < lst.count(); ++i )
		{
			QString filename = QDir::convertSeparators(QUriDrag::uriToLocalFile(lst.at(i)));
			str += filename + "\n";
		}
		str += QString( "\nTo\n\n   %1" )
		       .arg( QDir::convertSeparators(fullPath(item)) );

		QMessageBox::information( this, "Drop target", str, "Not implemented" );
	}
	else
		e->ignore();

}


QString GofunDirectoryView::fullPath(QListViewItem* item)
{
	QString fullpath = item->text(0);
	while ( (item=item->parent()) )
	{
		if ( item->parent() )
			fullpath = item->text(0) + "/" + fullpath;
		else
			fullpath = item->text(0) + fullpath;
	}
#ifdef Q_WS_WIN
	if (fullpath.length() > 2 && fullpath[1] != ':')
	{
		QDir dir(fullpath);
		fullpath = dir.currentDirPath().left(2) + fullpath;
	}
#endif

	return fullpath;
}

void GofunDirectoryView::contentsMousePressEvent( QMouseEvent* e )
{
	QListView::contentsMousePressEvent(e);
	QPoint p( contentsToViewport( e->pos() ) );
	QListViewItem *i = itemAt( p );
	if ( i )
	{
		// if the user clicked into the root decoration of the item, don't try to start a drag!
		if ( p.x() > header()->cellPos( header()->mapToActual( 0 ) ) +
		        treeStepSize() * ( i->depth() + ( rootIsDecorated() ? 1 : 0) ) + itemMargin() ||
		        p.x() < header()->cellPos( header()->mapToActual( 0 ) ) )
		{
			presspos = e->pos();
			mousePressed = TRUE;
		}
	}
}

void GofunDirectoryView::contentsMouseMoveEvent( QMouseEvent* e )
{
	if ( mousePressed && ( presspos - e->pos() ).manhattanLength() > QApplication::startDragDistance() )
	{
		mousePressed = FALSE;
		QListViewItem *item = itemAt( contentsToViewport(presspos) );
		if ( item )
		{
			QString source = fullPath(item);
			if ( QFile::exists(source) )
			{
				QUriDrag* ud = new QUriDrag(viewport());
				ud->setFileNames( source );
				if ( ud->drag() )
					QMessageBox::information( this, "Drag source",
					                          QString("Delete ") + QDir::convertSeparators(source), "Not implemented" );
			}
		}
	}
}

void GofunDirectoryView::contentsMouseReleaseEvent( QMouseEvent * )
{
	mousePressed = FALSE;
}

void GofunDirectoryView::showHiddenDirs(bool _bool)
{
	show_hidden = _bool;
	for(std::vector<GofunDirectory*>::iterator it = hidden_directories.begin(); it != hidden_directories.end(); ++it)
	{
		(*it)->setVisible(_bool);
	}
}

void GofunDirectoryView::setDir( const QString &s )
{
	QListViewItemIterator it( this );
	++it;
	for ( ; it.current(); ++it )
	{
		it.current()->setOpen( FALSE );
	}
	
	QStringList lst( QStringList::split( "/", s ) );
	QListViewItem *item = firstChild();
	QStringList::Iterator it2 = lst.begin();
	
	QListViewItemIterator it3( this );
	for ( ; it2 != lst.end(); ++it2 ) //TODO: make sure this works like expected and there are no 'special' circumstances. Also speed-optimize and clean this code.
	{
		while ( it3.current() )
		{
			if(it3.current()->text(0) == *it2)
			{
				if(!(*it3)->isVisible())
					(*it3)->setVisible(true);
				(*it3)->setOpen( TRUE );
				item = (*it3);
				break;
			}
		++it3;
		}
	}
	
	if ( item )
	{
		setCurrentItem( item );
		setContentsPos(0, item->itemPos());
	}
}

void GofunFileItem::setPixmap( QPixmap *p )
{
	pix = p;
	setup();
	widthChanged( 0 );
	invalidateHeight();
	repaint();
}


const QPixmap *GofunFileItem::pixmap( int i ) const
{
	if ( i )
		return 0;
	return pix;
}

GofunDirectoryBrowser::GofunDirectoryBrowser()
{
	setCaption( tr("Directory Browser") );
	QGridLayout* grid = new QGridLayout(this,4,2);
	grid->setMargin(3);

	current_selection = new QLabel(this);
	
	dir_view = new GofunDirectoryView(this,0,true);
	dir_view->addColumn( tr("Directory tree") );
	dir_view->setTreeStepSize( 20 );
	
	connect(dir_view,SIGNAL(folderSelected(const QString&)),current_selection,SLOT(setText(const QString&)));

	const QFileInfoList* roots = QDir::drives();
	QPtrListIterator<QFileInfo> i(*roots);
	QFileInfo* fi;
	while ( (fi = *i) )
	{
		++i;
		GofunDirectory * root = new GofunDirectory( dir_view, fi->filePath() );
		if ( roots->count() <= 1 )
			root->setOpen( TRUE ); // be interesting
	}

	dir_view->setAllColumnsShowFocus( TRUE );
	
	QCheckBox* hidden_check = new QCheckBox(tr("Show hidden directories"),this);
	
	connect(hidden_check,SIGNAL(toggled(bool)),this,SLOT(showHiddenDirs(bool)));

	QPushButton* select = new QPushButton(tr("Select"),this);
	QPushButton* cancel = new QPushButton(tr("Cancel"),this);
	connect(select,SIGNAL(clicked()),this,SLOT(accept()));
	connect(cancel,SIGNAL(clicked()),this,SLOT(reject()));
		
	grid->addMultiCellWidget(current_selection,0,0,0,1);
	grid->addMultiCellWidget(dir_view,1,1,0,1);
	grid->addMultiCellWidget(hidden_check,2,2,0,1);
	grid->addWidget(select,3,0);
	grid->addWidget(cancel,3,1);
}

void GofunDirectoryBrowser::setStartDirectory(const QString& start_dir)
{
	current_selection->setText(start_dir);
	dir_view->setDir(start_dir);
}

QString GofunDirectoryBrowser::selected()
{
	return current_selection->text();
}

void GofunDirectoryBrowser::showHiddenDirs(bool b)
{
	dir_view->setDir(current_selection->text());
	dir_view->showHiddenDirs(b);
}




