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

#include <qcombobox.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qvalidator.h>
#include <qgroupbox.h>
#include <qfiledialog.h>

#include "gofun_link_item.h"
#include "gofun_url_composer.h"

GofunURLComposer::GofunURLComposer()
{
	setCaption(tr("Url Composer"));
	
	QGridLayout* grid = new QGridLayout(this);
	QGridLayout* grid_url = new QGridLayout();
	
	QGroupBox* gb_advanced = new QGroupBox(tr("Advanced"),this);
	gb_advanced->setColumnLayout(0, Qt::Vertical );
	gb_advanced->layout()->setSpacing( 6 );
	gb_advanced->layout()->setMargin( 5 );
	
	QGridLayout* grid_advanced = new QGridLayout(gb_advanced->layout());
	
	scheme = new QComboBox(gb_advanced);
	scheme->insertItem("http");
	scheme->insertItem("shttp");
	scheme->insertItem("ftp");
	scheme->insertItem("file");
	scheme->insertItem(tr("Other"));
	other_scheme = new QLineEdit(gb_advanced);
	other_scheme->setEnabled(false);
	
	host = new QLineEdit(gb_advanced);
	port = new QLineEdit(gb_advanced);
	QIntValidator* port_validator = new QIntValidator(-1,65535,gb_advanced);
	port->setValidator(port_validator);
	path = new QLineEdit(gb_advanced);
	query = new QLineEdit(gb_advanced);
	
	connect(scheme,SIGNAL(textChanged(const QString&)),this,SLOT(schemeChanged(const QString&)));
	connect(scheme,SIGNAL(activated(const QString& )),this,SLOT(schemeChanged(const QString& )));
	connect(other_scheme,SIGNAL(textChanged(const QString&)),this,SLOT(otherSchemeChanged(const QString&)));
	connect(host,SIGNAL(textChanged(const QString&)),this,SLOT(hostChanged(const QString&)));
	connect(port,SIGNAL(textChanged(const QString&)),this,SLOT(portChanged(const QString&)));
	connect(path,SIGNAL(textChanged(const QString&)),this,SLOT(pathChanged(const QString&)));
	connect(query,SIGNAL(textChanged(const QString&)),this,SLOT(queryChanged(const QString&)));	
	
	grid_advanced->addWidget(new QLabel(tr("Scheme"),gb_advanced),0,0);
	grid_advanced->addWidget(scheme,0,1);
	grid_advanced->addWidget(other_scheme,0,2);
	grid_advanced->addWidget(new QLabel(tr("Host"),gb_advanced),1,0);
	grid_advanced->addMultiCellWidget(host,1,1,1,2);
	grid_advanced->addWidget(new QLabel(tr("Port"),gb_advanced),2,0);
	grid_advanced->addMultiCellWidget(port,2,2,1,2);
	grid_advanced->addWidget(new QLabel(tr("Path"),gb_advanced),3,0);
	grid_advanced->addMultiCellWidget(path,3,3,1,2);
	grid_advanced->addWidget(new QLabel(tr("Query"),gb_advanced),4,0);
	grid_advanced->addMultiCellWidget(query,4,4,1,2);
	
	QGroupBox* gb_fetch = new QGroupBox(tr("Fetch URL"),this);
	gb_fetch->setColumnLayout(2, Qt::Vertical );
	gb_fetch->layout()->setSpacing( 6 );
	gb_fetch->layout()->setMargin( 5 );
	
	QPushButton* fetch_filesystem = new QPushButton(tr("Fetch from\nfilesystem"),gb_fetch);
	QPushButton* fetch_browser = new QPushButton(tr("Fetch with\nweb-browser"),gb_fetch);
	
	connect(fetch_filesystem,SIGNAL(clicked()),this,SLOT(fetchFromFileSystem()));
	connect(fetch_browser,SIGNAL(clicked()),this,SLOT(fetchWithWebBrowser()));
	
	grid_url->addWidget(gb_fetch,0,0);
	grid_url->addWidget(gb_advanced,0,1);
	grid_url->addWidget(new QLabel(tr("URL"),this),1,0);
	
	QGroupBox* gb_url = new QGroupBox(tr("URL"),this);
	gb_url->setColumnLayout(1, Qt::Vertical );
	gb_url->layout()->setSpacing( 6 );
	gb_url->layout()->setMargin( 5 );

	composed_url = new QLineEdit(gb_url);
	
	connect(composed_url,SIGNAL(textChanged(const QString&)),this,SLOT(composedChanged(const QString&)));
	
	grid_url->addMultiCellWidget(gb_url,1,1,0,1);
	
	QPushButton* ok_button = new QPushButton(tr("Ok"),this);
	QPushButton* cancel_button = new QPushButton(tr("Cancel"),this);
	QPushButton* test_button = new QPushButton(tr("Test"),this);
	
	connect(ok_button,SIGNAL(clicked()),this,SLOT(accept()));
	connect(cancel_button,SIGNAL(clicked()),this,SLOT(reject()));
	connect(test_button,SIGNAL(clicked()),this,SLOT(test()));
	
	grid->addMultiCellLayout(grid_url,0,0,0,3);
	
	grid->addWidget(ok_button,1,0);
	grid->addWidget(cancel_button,1,1);
	QSpacerItem* spacer = new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Minimum);
	grid->addItem(spacer,1,2);
	grid->addWidget(test_button,1,3);
	
	link_item = 0;
};

void GofunURLComposer::setStartURL( const QUrl& _url)
{
	url = _url;
	host->setText(url.host());
	port->setText(QString::number(url.port()));
	path->setText(url.path());
	query->setText(url.query());
	emit schemeChanged(url.protocol());
}

QUrl GofunURLComposer::getURL( )
{
	return url;
}

void GofunURLComposer::test()
{
	if(link_item)
		link_item->open(url);
}

void GofunURLComposer::schemeChanged( const QString& str)
{
	if(str == tr("Other"))
	{
		other_scheme->setEnabled(true);
		url.setProtocol(other_scheme->text());
	}
	else
		url.setProtocol(str);
	
	if(!isComposedCurrent())
		composed_url->setText(url);
}

void GofunURLComposer::otherSchemeChanged( const QString & )
{
	emit schemeChanged(tr("Other"));
}

void GofunURLComposer::hostChanged( const QString& str)
{
	url.setHost(str);
	if(!isComposedCurrent())
		composed_url->setText(url);
}

void GofunURLComposer::portChanged( const QString& str)
{
	url.setPort(str.toInt());
	if(!isComposedCurrent())
		composed_url->setText(url);
}

void GofunURLComposer::queryChanged( const QString& str)
{
	url.setQuery(str);
	if(!isComposedCurrent())
		composed_url->setText(url);
}

void GofunURLComposer::pathChanged( const QString& str)
{
	url.setPath(str);
	if(!isComposedCurrent())
		composed_url->setText(url);
}

void GofunURLComposer::composedChanged( const QString& str)
{
	url = str;
	
	for(int i = 0; i < scheme->count(); ++i)
	{
		if(url.protocol() == scheme->text(i))
		{
			scheme->setCurrentItem(i);
			other_scheme->setEnabled(false);
			break;
		}
		
		if(i+1 == scheme->count())
		{
			scheme->setCurrentText(tr("Other"));
			other_scheme->setEnabled(true);
			other_scheme->setText(url.protocol());
			break;
		}
	}
	host->setText(url.host());
	port->setText(QString::number(url.port()));
	path->setText(url.path());
	query->setText(url.query());
}

bool GofunURLComposer::isComposedCurrent()
{
	if(QString(url) == QString(QUrl(composed_url->text())))
		return true;
	else
		return false;
}

void GofunURLComposer::fetchFromFileSystem( )
{
	QString from_fs = QFileDialog::getOpenFileName(QDir::homeDirPath(),"*",this);
	if(!from_fs.isNull())
	{
		composed_url->setText(QUrl(from_fs));
	}
}

void GofunURLComposer::fetchWithWebBrowser( )
{
}

void GofunURLComposer::setLinkItem(GofunLinkItem* _link_item)
{
	link_item = _link_item;
}


