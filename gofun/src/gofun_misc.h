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

#include <qstring.h>
#include <qfiledialog.h>
#include <qlabel.h>
#include <qwidget.h>
#include <qtoolbutton.h>
 
#ifndef GOFUN_MISC
#define GOFUN_MISC

enum Side {
	D_Left,
	D_Right,
	D_Above,
	D_Under,
	D_None
};

class QProcess;

///Mixed methods needed in GoFun
struct GofunMisc
{
	static QString shell_call(const QString&);
	static QString ext_filestring(const QString&);
	static QString bin_dir();
	static void center_window(QWidget*, int, int);
	static QString fileDialogGetImage(const QString& start_dir,const QString& caption, const QString& filter_desc);
	static QPixmap get_icon(const QString&,int = 32,int = 32);
	static void attach_window(QWidget* base,QWidget* to_attach, Side pref, Side alt, int width = -1, int height = -1);
	static bool makeDir(const QString&);
	static QString shellify_path(const QString&);
	static bool stringToBool(const QString&);
	static QString boolToString(bool);
	
	static QStringList icon_files;
};

class GofunProcessLogger : public QObject
{
	Q_OBJECT
	public:
	static GofunProcessLogger* get() { _instance ? _instance : _instance = new GofunProcessLogger(); return _instance; }
	void connectProcToStdout(const QProcess*);
		
	public slots:
	void readProcStdout();
	void readProcStderr();
	
	private:
	GofunProcessLogger() {}; //Singleton
	
	static GofunProcessLogger* _instance;
};

class GofunFileDialogPreview : public QLabel, public QFilePreview
{
	public:
        GofunFileDialogPreview( QWidget *parent=0 ) : QLabel( parent ) {}

        void previewUrl( const QUrl &u );
};

class GofunFileIconProvider : public QFileIconProvider
{
	public:
	virtual const QPixmap * pixmap ( const QFileInfo & info );
};

class GofunLineEdit : public QWidget
{
	public:
	GofunLineEdit();
	
	private:
	QLineEdit* lineedit;
	QToolButton* button;
};

#endif


