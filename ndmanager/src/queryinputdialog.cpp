/***************************************************************************
 *   Copyright (C) 2006 by Michael Zugaro                                  *
 *   michael.zugaro@college-de-france.fr                                   *
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
// application specific includes
#include "queryinputdialog.h"

// include files for KDE
#include <klocale.h>

//General C++ include files
#include <iostream>
using namespace std;

QueryInputDialog::QueryInputDialog(QWidget *parent,const QString& caption,const QString& urltext) :
KDialogBase(parent,"Query",true,caption,Ok|Cancel,Ok,true)
{
	//Disable the OK button
	enableButtonOK (false);
	
	page = new QWidget(this);
	setMainWidget(page);
	layout = new QVBoxLayout(page,0,spacingHint());

	QLabel *label1 = new QLabel(i18n("Query"),page,"query_label");
	layout->addWidget(label1);

	query = new QLineEdit(page,"query");
	query->setMinimumWidth(fontMetrics().maxWidth()*20);
	query->setFocus();
	layout->addWidget(query);

	QLabel *label2 = new QLabel(i18n("Path"),page,"path_label");
	layout->addWidget(label2);

	path = new KURLRequester(page,"path");
	path->setMode(KFile::Directory);
	path->setMinimumWidth(fontMetrics().maxWidth()*20);
	path->setURL(urltext);
	layout->addWidget(path);

	layout->addStretch(10);

	//connections
	connect(path,SIGNAL(textChanged (const QString &)),this,SLOT(pathChanged(const QString &)));
	
}

QueryInputDialog::~QueryInputDialog()
{
}

void QueryInputDialog::pathChanged(const QString & newPath){
	
	if(newPath != QString("")){
		//Enable the OK button
		enableButtonOK (true);
	}
	else if(newPath == QString("")){
		//Disable the OK button
		enableButtonOK (false);
	}
}
