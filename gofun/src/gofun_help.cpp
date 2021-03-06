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

#include <qlayout.h>
#include <qprinter.h>
#include <qpaintdevice.h>
#include <qpaintdevicemetrics.h>
#include <qpainter.h>
#include <qsimplerichtext.h>
#include <qimage.h>
#include <qlabel.h>
#include <qurl.h>
#include <qtoolbutton.h>
#include <qpushbutton.h>
#include <qhttp.h>

#include "gofun_help.h"

std::map<QString,QPixmap*> GofunMimeSourceFactory::images;
std::map<QString,int> GofunMimeSourceFactory::image_countdown;

GofunHelp::GofunHelp(WFlags f) : QDialog(0,0,0,f)
{
	setCaption(tr("GoFun Help"));
	setIcon(QPixmap("help.png"));

	QSpacerItem* space_left = new QSpacerItem(-1,-1,QSizePolicy::Expanding, QSizePolicy::Minimum);
	QSpacerItem* space_right = new QSpacerItem(-1,-1,QSizePolicy::Expanding, QSizePolicy::Minimum);
	
	QGridLayout* gl = new QGridLayout(this,2,7);
	print_bt = new QToolButton(this);
	print_bt->setPixmap(QPixmap("print.png"));
	forward_bt = new QToolButton(this);
	forward_bt->setPixmap(QPixmap("forward.png"));
	forward_bt->setEnabled(false);
	back_bt = new QToolButton(this);
	back_bt->setPixmap(QPixmap("backward.png"));
	back_bt->setEnabled(false);
	home_bt = new QToolButton(this);
	home_bt->setPixmap(QPixmap("home.png"));
	close_bt = new QPushButton(tr("Close"),this);
	
	textbrowser = new QTextBrowser(this);
	gmf = new GofunMimeSourceFactory;
	gmf->p_gofun_help = this;
	textbrowser->setMimeSourceFactory(gmf);
	textbrowser->setPalette(QColor(255,255,255));
	
	connect(back_bt,SIGNAL(clicked()),textbrowser,SLOT(backward()));
	connect(forward_bt,SIGNAL(clicked()),textbrowser,SLOT(forward()));
	connect(home_bt,SIGNAL(clicked()),textbrowser,SLOT(home()));
	connect(print_bt,SIGNAL(clicked()),this,SLOT(print()));
	connect(close_bt,SIGNAL(clicked()),this,SLOT(close()));
	connect(textbrowser,SIGNAL(backwardAvailable(bool)),back_bt,SLOT(setEnabled(bool)));
	connect(textbrowser,SIGNAL(forwardAvailable(bool)),forward_bt,SLOT(setEnabled(bool)));
	
	gl->addWidget(print_bt,0,0);
	gl->addItem(space_left,0,1);
	gl->addWidget(back_bt,0,2);
	gl->addWidget(home_bt,0,3);
	gl->addWidget(forward_bt,0,4);
	gl->addItem(space_right,0,5);
	gl->addWidget(close_bt,0,6);
	gl->addMultiCellWidget(textbrowser,1,1,0,6);
	
	http = new QHttp();
	connect(http,SIGNAL(requestFinished(int,bool)),this,SLOT(httpData(int,bool)));
	
	base_url = "http://gofun.berlios.de/tipiwiki/";
	host = "www.berlios.de";
	textbrowser->setSource( "index.php");
}

GofunHelp::~GofunHelp()
{
	gmf->p_gofun_help = 0;
}

void GofunHelp::httpData(int id, bool error)
{
	if(error)
	{
		qDebug(http->errorString());
		return;
	}

	QString str = http->readAll();
	str.remove("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtm\nl1/DTD/xhtml1-strict.dtd\">");
	str.replace("<img src=\"http://gofun.berlios.de/tipiwiki/gofun.png\" width=128>","<img src=\"http://gofun.berlios.de/tipiwiki/gofun.png\" align=right width=128>");
	textbrowser->setText(str);
}

void GofunHelp::openLink(const QString& link)
{;
	QString final_link = link;
	if(link.find("http") == 0)
	{
		base_url = link;
		base_url.remove(link.findRev("/"),link.length()-link.findRev("/"));
		final_link.remove(0,link.findRev("/"));
		host = QUrl(link).host();
	}
	http->setHost(host);
	http->get(base_url + final_link);
}

void GofunHelp::print()
{
#ifndef QT_NO_PRINTER
    QPrinter printer;
    printer.setFullPage(true);
    if ( printer.setup() ) {
	QPainter p( &printer );
	QPaintDeviceMetrics metrics(p.device());
	int dpix = metrics.logicalDpiX();
	int dpiy = metrics.logicalDpiY();
	const int margin = 72; // pt
	QRect body(margin*dpix/72, margin*dpiy/72,
		   metrics.width()-margin*dpix/72*2,
		   metrics.height()-margin*dpiy/72*2 );
	QFont font("times", 10);
	QSimpleRichText richText( textbrowser->text(), font, textbrowser->context(), textbrowser->styleSheet(),
				  textbrowser->mimeSourceFactory(), body.height() );
	richText.setWidth( &p, body.width() );
	QRect view( body );
	int page = 1;
	do {
	    p.setClipRect( body );
	    richText.draw( &p, body.left(), body.top(), view, textbrowser->colorGroup() );
	    p.setClipping( FALSE );
	    view.moveBy( 0, body.height() );
	    p.translate( 0 , -body.height() );
	    p.setFont( font );
	    p.drawText( view.right() - p.fontMetrics().width( QString::number(page) ),
			view.bottom() + p.fontMetrics().ascent() + 5, QString::number(page) );
	    if ( view.top()  >= richText.height() )
		break;
	    printer.newPage();
	    page++;
	} while (TRUE);
    }
#endif
}


GofunMimeSourceFactory::GofunMimeSourceFactory() : QMimeSourceFactory()
{
	http = new QHttp();
	connect(http,SIGNAL(requestFinished(int,bool)),this,SLOT(setImageThere(int,bool)));
	image_there = false;
}


const QMimeSource* GofunMimeSourceFactory::data(const QString& abs_name) const
{
	if(abs_name.contains(".png") || abs_name.contains("bslogo"))
	{
	
		if(images.find(abs_name) != images.end())
		{
			QMimeSourceFactory* mf = new QMimeSourceFactory();
			mf->setPixmap(abs_name,*images[abs_name]);
			const QMimeSource* r = mf->data(abs_name);
			return r;
		}
	
		
		http->setHost("www.berlios.de");
		http->get(abs_name);
		
		
		if(!image_countdown[http->currentRequest().path()])
		{
			image_countdown[http->currentRequest().path()] = 2; //We start from 2, because 0 is the default and we will count down to 1.
		}
		else
		{
			++image_countdown[http->currentRequest().path()];
		}
	}
	else //if(abs_name.contains("index.php"))
	{
		if(p_gofun_help)
			p_gofun_help->openLink(abs_name);
	}
	
	const QMimeSource * r = QMimeSourceFactory::data( abs_name );
	return r;
}

void GofunMimeSourceFactory::setImageThere(int id,bool error)
{
	if(error)
		return;

	image_there = true;

	QPixmap* img = new QPixmap(http->readAll());

	images[http->currentRequest().path()] = img;
	--image_countdown[http->currentRequest().path()];

	if(image_countdown[http->currentRequest().path()] == 1 && p_gofun_help)
		p_gofun_help->textbrowser->setSource(p_gofun_help->textbrowser->source());
}







