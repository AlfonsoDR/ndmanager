/***************************************************************************
 *   Copyright (C) 2004 by Lynn Hazan                                      *
 *   lynn.hazan@myrealbox.com                                              *
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
#include "managerview.h"

//Include files for KDE
#include <kglobal.h>
#include <klocale.h> 
#include <kmessagebox.h>

// include files for Qt
#include <qtextedit.h> 
#include <qwidget.h> 
#include <qdir.h> 
#include <qstringlist.h> 
#include <qlayout.h>
#include <kprocess.h>

//General C++ include files
#include <iostream>


using namespace std;

ManagerView::ManagerView(QWidget *parent, const char *name)
 : /*QSplitter(parent, name),*/QFrame(parent, name),konsole(0L),isUptoDate(true){
 // setOrientation(Qt::Vertical);
 frameLayout = new QVBoxLayout(this,0,0);
}


ManagerView::~ManagerView(){
}

ManagerView::returnMessage ManagerView::addKonsole(const KURL url,int nbSpikeGroups,QValueList<QString> fileExtensions,const QValueList<QString>& scriptNames){
 if(konsole == 0L){ 
  parameterUrl = KURL(url);

  konsole = new NdKonsole(this,"NdKonsole");
  frameLayout->addWidget(konsole);
      
  toolbar = new KToolBar(this);
  

  toolbar->insertButton("neuroscope-22",1,SIGNAL(clicked(int)),this,SLOT(launchNeuroscope()),true,i18n("NeuroScope"));

   cout<<"url.path() "<<url.directory()<<" url.fileName() "<<url.fileName()<<" parameterUrl.fileName() "<<parameterUrl.fileName()<<endl;
  
  QStringList neuroscopeFiles;
  neuroscopeFiles<<".dat"<<".eeg";
  
  QValueList<QString>::iterator iterator;
  for(iterator = fileExtensions.begin(); iterator != fileExtensions.end(); ++iterator){
   neuroscopeFiles<<QString(".%1").arg(static_cast<QString>(*iterator));   
  }

  neuroscopeComboBox = new QComboBox(toolbar);
  neuroscopeComboBox->setGeometry(QRect(13,53,150,20));
  neuroscopeComboBox->setMinimumSize(QSize(90,20));
  neuroscopeComboBox->setMaximumSize(QSize(150,20));

  neuroscopeComboBox->insertStringList(neuroscopeFiles);
  connect(neuroscopeComboBox,SIGNAL(activated(int)),this,SLOT(neuroscopeFileChange(int)));
  toolbar->insertWidget(-1,neuroscopeComboBox->sizeHint().width(),neuroscopeComboBox);
 // toolbar->insertCombo(neuroscopeFiles,1,false,SIGNAL(activated(int)),this,SLOT(neuroscopeFileChange(int))); 
  
  toolbar->insertSeparator();
  toolbar->insertSeparator();
  toolbar->insertSeparator();
  toolbar->insertLineSeparator();
  toolbar->insertSeparator();
  toolbar->insertSeparator();
  
  toolbar->insertButton("scripts",2,SIGNAL(clicked(int)),this,SLOT(launchScript()),true,i18n("Start"));
  scriptsComboBox = new QComboBox(toolbar);
  scriptsComboBox->setGeometry(QRect(13,53,150,20));
  scriptsComboBox->setMinimumSize(QSize(90,20));
  scriptsComboBox->setMaximumSize(QSize(150,20));

  QStringList scripts;
  QValueList<QString>::const_iterator iterator2;
  for(iterator2 = scriptNames.begin(); iterator2 != scriptNames.end(); ++iterator2){
    scripts<<static_cast<QString>(*iterator2);  
  }
  scriptsComboBox->insertStringList(scripts);
  toolbar->insertWidget(-1,scriptsComboBox->sizeHint().width(),scriptsComboBox);
  toolbar->insertButton("stop",3,SIGNAL(clicked(int)),this,SLOT(stopScript()),true,i18n("Stop"));
  
  toolbar->insertSeparator();
  toolbar->insertSeparator();
  toolbar->insertSeparator();
  toolbar->insertLineSeparator();
  toolbar->insertSeparator();
  toolbar->insertSeparator();
  
  toolbar->insertButton("klusters-22",4,SIGNAL(clicked(int)),this,SLOT(launchKlusters()),true,i18n("Klusters"));
  klustersComboBox = new QComboBox(toolbar);
  klustersComboBox->setGeometry(QRect(13,53,150,20));
  klustersComboBox->setMinimumSize(QSize(90,20));
  klustersComboBox->setMaximumSize(QSize(150,20));
  QStringList klustersFiles;
  for (int i = 0; i < nbSpikeGroups;++i){
   klustersFiles<<QString("%1").arg(i+1);
  }

  klustersComboBox->insertStringList(klustersFiles);
  toolbar->insertWidget(-1,klustersComboBox->sizeHint().width(),klustersComboBox);
  frameLayout->addWidget(toolbar);


  
  //setOpaqueResize(true);   
  connect(konsole, SIGNAL(beingDestroyed()), this, SLOT(konsoleBeingDestroyed()));
 
  return static_cast<returnMessage>(konsole->loadConsoleIfNeeded(url));
 }
 else{
  return static_cast<returnMessage>(konsole->loadConsoleIfNeeded(url));
 }
 return OK;
}

void ManagerView::neuroscopeFileChange(int){

}

void ManagerView::updateSpikeGroupList(int nbGroups){
 klustersComboBox->clear();
 QStringList klustersFiles;
  for (int i = 0; i < nbGroups;++i){
   klustersFiles<<QString("%1").arg(i+1);
  }

 klustersComboBox->insertStringList(klustersFiles);
}

void ManagerView::updateFileList(QValueList<QString> extensions){
  neuroscopeComboBox->clear();
  QStringList neuroscopeFiles;
  neuroscopeFiles<<".dat"<<".eeg";
  
  QValueList<QString>::iterator iterator;
  for(iterator = extensions.begin(); iterator != extensions.end(); ++iterator){
   QString extension = QString(".%1").arg(static_cast<QString>(*iterator));
   neuroscopeFiles<<extension;
  }
  neuroscopeComboBox->insertStringList(neuroscopeFiles);
}

void ManagerView::updateScriptList(const QValueList<QString>& scriptNames){
  scriptsComboBox->clear();
  QStringList scripts;
  
  QValueList<QString>::const_iterator iterator;
  for(iterator = scriptNames.begin(); iterator != scriptNames.end(); ++iterator){
    scripts<<static_cast<QString>(*iterator);  
  }
  scriptsComboBox->insertStringList(scriptNames);
}

void ManagerView::launchNeuroscope(){
 emit checkBeforeLaunchingPrograms();
 //The parameter file is new or has been imported from an existing file.
 if(parameterUrl.fileName() == "Untitled"){
  KMessageBox::error (this,i18n("In order to launch NeuroScope, the parameter file has to be saved first."), i18n("Unsaved file!"));
  return;
 }
 else{
  if(!isUptoDate){
   KMessageBox::error (this,i18n("The parameter file contains unsaved data, please save before launching NeuroScope."), i18n("Unsaved file!"));
   return;   
  }
  //Launch NeuroScope.
  else{
  QString fileName = parameterUrl.fileName();
  QString baseName = fileName.left(fileName.length()-4);
  QString fileToUse = baseName.append(neuroscopeComboBox->currentText());
	KProcess process;
	process << "neuroscope";
	process << parameterUrl.directory(false) + fileToUse;
	process.start(KProcess::DontCare);
  }
 }
}

void ManagerView::launchKlusters(){
 if(klustersComboBox->count() == 0){
  KMessageBox::error (this,i18n("No spike groups have been defined, please define at least one spike group before launching Klusters."), i18n("Undefined spike groups"));
  return;  
 }

 emit checkBeforeLaunchingPrograms();
 //The parameter file is new or has been imported from an existing file.
 if(parameterUrl.fileName() == "Untitled"){
  KMessageBox::error (this,i18n("In order to launch Klusters, the parameter file has to be saved first."), i18n("Unsaved file!"));
  return;
 }
 else{
  if(!isUptoDate){
   KMessageBox::error (this,i18n("The parameter file contains unsaved data, please save before launching Klusters."), i18n("Unsaved file!"));
   return;   
  }
  //Launch Klusters.
  else{
  QString fileName = parameterUrl.fileName();
  QString baseName = fileName.left(fileName.length()-4);
  QString fileToUse = baseName.append(".spk.").append(klustersComboBox->currentText());
	KProcess process;
	process << "klusters";
	process << parameterUrl.directory(false) + fileToUse;
	process.start(KProcess::DontCare);
  }
 }
}

void ManagerView::launchScript(){
 emit checkBeforeLaunchingScripts();
 QString script = scriptsComboBox->currentText();
 //The parameter file is new or has been imported from an existing file.
 if(parameterUrl.fileName() == "Untitled"){
  QString message = QString("In order to launch %1, the parameter file has to be saved first.").arg(script);
  KMessageBox::error (this,i18n(message), i18n("Unsaved file!"));
  return;
 }
 else{
  if(!isUptoDate){
   QString message = QString("The parameter file contains unsaved data or script, please save before launching %1.").arg(script);
   KMessageBox::error (this,i18n(message), i18n("Unsaved data!"));
   return;   
  }
  //Launch the script.
  else{
	  QString fileName = parameterUrl.fileName();
	  QString baseName = fileName.left(fileName.length()-4);
	  konsole->runCommand(script+" "+baseName);
  }
 }
}


void ManagerView::stopScript(){
}


void ManagerView::updateDocUrl(const KURL url){
 parameterUrl = KURL(url);
}

void ManagerView::updateDocumentInformation(const KURL url,bool isUptoDate){
 parameterUrl = KURL(url);
 this->isUptoDate = isUptoDate;
}

#include "managerview.moc"
