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

#include <qurl.h>
#include <qdialog.h>
#include <qlineedit.h>

#ifndef GOFUN_URL_COMPOSER
#define GOFUN_URL_COMPOSER

class QComboBox;
class GofunLinkItem;

///Clipboard catching line-edit
/** Automatically tries to catch URLs from the
    clipboard. */
class GofunClipboardLineEdit : public QLineEdit
{
	public:
	GofunClipboardLineEdit(QWidget*);
	
	private:
	void focusInEvent(QFocusEvent*);
	
	QUrl last_ignored;
};

class GofunURLComposer : public QDialog
{
	Q_OBJECT
	
	public:
	GofunURLComposer();
	void setStartURL(const QUrl&);
	void setLinkItem(GofunLinkItem*);
	QUrl getURL();
	
	private slots:
	void test();
	void fetchFile();
	void fetchWithWebBrowser();
	void fetchDirectory();
	
	void otherSchemeChanged(const QString&);
	void schemeChanged(const QString&);
	void hostChanged(const QString&);
	void portChanged(const QString&);
	void queryChanged(const QString&);
	void pathChanged(const QString&);
	void composedChanged(const QString&);
	
	bool isComposedCurrent();
		
	private:
	QUrl url;
	
	QLineEdit* host;
	QLineEdit* port;
	QLineEdit* query;
	QLineEdit* path;
	QComboBox* scheme;
	QLineEdit* other_scheme;
	
	GofunClipboardLineEdit* composed_url;
	
	GofunLinkItem* link_item;
};

#endif

