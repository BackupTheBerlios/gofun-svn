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

#include <map>
 
#include <qtextbrowser.h>
#include <qhttp.h>
#include <qdialog.h>
#include <qtoolbutton.h>
#include <qpushbutton.h>

#ifndef GOFUN_HELP
#define GOFUN_HELP

///Specialised IconView for GoFun 
class GofunHelp : public QDialog
{
	Q_OBJECT
	
	public:
	GofunHelp();
	
	public slots:
	void httpData(int,bool);
	void openLink(const QString&);
	void print();
	
	private:
	QTextBrowser* textbrowser;
	QToolButton* print_bt;
	QToolButton* back_bt;
	QToolButton* forward_bt;
	QToolButton* home_bt;
	QPushButton* close_bt;
	QHttp* http;
	QString base_url;

	friend class GofunMimeSourceFactory;
};
	
class GofunMimeSourceFactory : public QObject, public QMimeSourceFactory
{
	Q_OBJECT
		
	public:
	GofunMimeSourceFactory();
	const QMimeSource* data( const QString& abs_name ) const;
	GofunHelp* p_gofun_help;
	
	public slots:
	void setImageThere(int,bool);
	
	private:
	QHttp* http;
	bool image_there;
	static std::map<QString,QPixmap*> images;
	static std::map<QString,int> image_countdown;
};

#endif

