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
#include <qpushbutton.h>
#include <qcursor.h>
#include <qapplication.h> 
#include <qheader.h>
#include <qvbuttongroup.h>
#include <qfiledialog.h>

#include "gofun_command_editor.h"
#include "gofun_misc.h"
#include "gofun_shell_operations.h"
#include "gofun_list_popup.h"
#include "gofun_directory_browser.h"
#include "gofun_application_item.h"
#include "gofun_application_entry_settings.h"
#include "gofun_executable_browser.h"
#include "gofun_settings_container.h"
#include "gofun_data.h"
#include "gofun_command_editor_options.h"
#include "gofun_settings_container.h"

GofunCommandEditor::GofunCommandEditor()
{
	setCaption(tr("Command Editor"));
	
	expand_list = new GofunListPopup;
	expand_list->addColumn("files");
	expand_list->header()->hide();
	
	connect(expand_list,SIGNAL(keyPressed(const QString& )),this,SLOT(expandListKey(const QString&)));		connect(expand_list,SIGNAL(clicked(QListViewItem*)),this,SLOT(commandCompletion(QListViewItem*)));
	connect(expand_list,SIGNAL(returnPressed(QListViewItem*)),this,SLOT(commandCompletion(QListViewItem*)));
	connect(expand_list,SIGNAL(spacePressed(QListViewItem*)),this,SLOT(commandCompletion(QListViewItem*)));
		
	text = new QTextEdit(this);
	text->setTextFormat(Qt::PlainText);
	text->setWordWrap(QTextEdit::NoWrap);
	QPushButton* apply = new QPushButton(tr("Ok"),this);
	QPushButton* cancel = new QPushButton(tr("Cancel"),this);
	QPushButton* test = new QPushButton(tr("Test"),this);
	
	connect(text,SIGNAL(textChanged()),this,SLOT(commandExpand()));
	connect(apply,SIGNAL(clicked()),this,SLOT(accept()));
	connect(cancel,SIGNAL(clicked()),this,SLOT(reject()));
	connect(test,SIGNAL(clicked()),this,SLOT(test()));
	
	QPushButton* options = new QPushButton(tr("Options"),this);
	
	connect(options,SIGNAL(clicked()),this,SLOT(optionsDialog()));
	
	QVButtonGroup* browse_for = new QVButtonGroup(tr("Browse for"),this);
	QPushButton* browse_for_executable = new QPushButton(tr("Executable"),browse_for);
	QPushButton* browse_for_directory = new QPushButton(tr("Directory"),browse_for);
	QPushButton* browse_for_file = new QPushButton(tr("File"),browse_for);
	
	connect(browse_for_executable,SIGNAL(clicked()),this,SLOT(browseForExecutable()));
	connect(browse_for_directory,SIGNAL(clicked()),this,SLOT(browseForDirectory()));
	connect(browse_for_file,SIGNAL(clicked()),this,SLOT(browseForFile()));
	
	QVButtonGroup* import = new QVButtonGroup(tr("Import from"),this);
	QPushButton* import_from_script = new QPushButton(tr("Script"),import);
	QPushButton* import_from_entry = new QPushButton(tr("Desktop Entry"),import);
	
	connect(import_from_script,SIGNAL(clicked()),this,SLOT(importFromScript()));
	connect(import_from_entry,SIGNAL(clicked()),this,SLOT(importFromEntry()));
	
	QGridLayout* grid = new QGridLayout(this,4,4);
	grid->addMultiCellWidget(text,0,2,0,2);
	grid->addWidget(apply,3,0);
	grid->addWidget(cancel,3,1);
	QSpacerItem* spacer = new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Minimum);
	grid->addItem(spacer,3,2);
	grid->addWidget(test,3,3);
	grid->addWidget(options,0,3);
	grid->addWidget(browse_for,1,3);
	grid->addWidget(import,2,3);
	
	settings_widget = 0;
}

void GofunCommandEditor::commandExpand()
{
	if(!GofunMisc::stringToBool(GSC::get()->ce_completion_popup))
		return;

	{
		
	if( (text->text()[text->text().length()-1] == '/' && text->text()[text->text().length()-2] == '.' )
	|| (text->text()[text->text().length()-1] == '$'))
	{
		//fill completition list
		qDebug(text->text(text->paragraphs()-1));
		qDebug("find . -maxdepth 1 -path \\*"+text->text(text->paragraphs()-1)+"\\*");
		QStringList files = QStringList::split('\n',GofunShellOperations::shellCall("find . -maxdepth 1 -path \\*"+text->text(text->paragraphs()-1).stripWhiteSpace()+"\\*"));
		expand_list->fill(files);
		}
		qDebug(text->text(text->paragraphs()-1));
		QStringList echos = QStringList::split(' ',GofunShellOperations::shellCall("cd $HOME; echo "+text->text(text->paragraphs()-1).stripWhiteSpace()+"*"));
		//if(echos.count() > 1)
		
		expand_list->clear();
		expand_list->fill(echos);
		
		QFontMetrics metrics(text->font());
		int x,y;
		text->getCursorPosition(&y,&x);
		
		int xp = metrics.size(Qt::SingleLine,text->text(y)).width();
		int yp = metrics.lineSpacing() * (y+1);
		expand_list->popup(text->mapToGlobal(QPoint(xp,yp)));
		

	}
}

void GofunCommandEditor::setCommand(const QString& _cmd)
{
	cmd = _cmd;
	text->setText(cmd.replace(';',"\n"));
}

QString GofunCommandEditor::command()
{
	cmd = text->text();
	cmd = cmd.replace('\n',";");
	return cmd;
}

void GofunCommandEditor::commandCompletion(QListViewItem* item)
{
	if(!item)
		return;
		
	text->insert(item->text(0));
}

void GofunCommandEditor::test()
{
	QString tmp = text->text(); //Wtf? Had to workaround a gcc4 flaw. :(
	if(settings_widget)
	{
		GofunApplicationEntryData app_entry;
		settings_widget->apply(&app_entry);
		app_entry.Exec = tmp;
		app_entry.execute();
	}
}

void GofunCommandEditor::browseForDirectory()
{
	GofunDirectoryBrowser dir_browser;
	if(dir_browser.exec() == QDialog::Accepted)
		text->insert(dir_browser.selected());
}

void GofunCommandEditor::browseForExecutable()
{
	GofunExecutableBrowser exec_browser;
	if(exec_browser.exec() == QDialog::Accepted)
		text->insert(exec_browser.getExecutable());
}

void GofunCommandEditor::browseForFile()
{
	QString s = QFileDialog::getOpenFileName(
                    "/home",
                    "*.*",
                    this,
                    tr("Browse for file"),
                    tr("Choose a file"));
	if(s.isNull())
		return;
		
	text->insert(s);
}

void GofunCommandEditor::setSettingsWidget(GofunApplicationEntrySettings *_settings_widget)
{
	settings_widget = _settings_widget;
}

void GofunCommandEditor::expandListKey(const QString& key)
{
	if(!key.isEmpty())
		text->insert(key);
}

void GofunCommandEditor::importFromScript()
{
	QString s = QFileDialog::getOpenFileName("/home","*",this,tr("Import from script"),tr("Choose a script"));
	if(s.isNull())
		return;
	
	QFile file(s);
	if ( file.open( IO_ReadOnly ) )
	{
		QTextStream stream( &file );
		QString line;
		text->insert("\n");
		while ( !stream.atEnd() )
		{
			line = stream.readLine(); // line of text excluding '\n'
			text->insert(line+"\n");
		}
		file.close();
	}
}

void GofunCommandEditor::importFromEntry()
{
	QString s = QFileDialog::getOpenFileName(GSC::get()->gofun_dir,"*.desktop",this,tr("Import from Desktop Entry"),tr("Choose a Desktop Entry"));
	if(s.isNull())
		return;
	
	GofunDesktopEntryData* ded = GofunDataLoader::parseGofunFile(s);
	if(ded->Type == "Application")
	{
		text->insert("\n");
		text->insert(dynamic_cast<GofunApplicationEntryData*>(ded)->Exec);
	}
	delete ded;
}

void GofunCommandEditor::optionsDialog()
{
	GofunCommandEditorOptions* options = new GofunCommandEditorOptions();
	if(options->exec() == QDialog::Accepted)
		options->apply();
	delete options;
}





