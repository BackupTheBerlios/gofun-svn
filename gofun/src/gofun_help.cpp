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

#include "gofun_help.h"


	QString mcur_image;

	std::map<QString,QPixmap*> images;
GofunHelp::GofunHelp()
{
	setCaption("GoFun Help");
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
	close_bt = new QPushButton("Close",this);
	
	textbrowser = new QTextBrowser(this);
	GofunMimeSourceFactory* gmf = new GofunMimeSourceFactory;
	gmf->p_gofun_help = this;
	textbrowser->setMimeSourceFactory(gmf);
	
	connect(back_bt,SIGNAL(clicked()),textbrowser,SLOT(backward()));
	connect(forward_bt,SIGNAL(clicked()),textbrowser,SLOT(forward()));
	connect(home_bt,SIGNAL(clicked()),textbrowser,SLOT(home()));
	connect(print_bt,SIGNAL(clicked()),this,SLOT(print()));
	connect(close_bt,SIGNAL(clicked()),this,SLOT(hide()));
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
	textbrowser->setSource( "index.php");
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
	textbrowser->setText(str);
}

void GofunHelp::openLink(const QString& link)
{
	http->setHost("www.berlios.de");
	http->get(base_url + link);
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
	    richText.draw( &p, body.left(), body.top(), view, colorGroup() );
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
		mcur_image = abs_name;
		/*while(!image_there)
		{
		sleep(1);
		}*/
		
	}
	qDebug("Retrieve source");
	qDebug(abs_name);
	const QMimeSource * r = QMimeSourceFactory::data( abs_name );
	if(abs_name.contains("index.php"))
	{
		p_gofun_help->openLink(abs_name);
	}
	if(!r)
	{
		qDebug("MimeSourcing failed my friend cheeze");
	}
	return r;
}

void GofunMimeSourceFactory::setImageThere(int id,bool error)
{
if(error)
{
	qDebug("Image receive errorrrrrrrrrrrrrrrrrrrrrrrr");
	return;
}

	qDebug("Image THERE");
image_there = true;

QPixmap* img = new QPixmap(http->readAll());
/*QDialog* d = new QDialog();
d->setGeometry(50,50,400,400);
QLabel* l = new QLabel(d);
l->setGeometry(0,0,400,400);
l->setPixmap(img);
d->show();*/

images[mcur_image] = img;

}


