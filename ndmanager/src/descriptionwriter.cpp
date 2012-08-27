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

//application specific include files.
#include "tags.h"
#include "descriptionwriter.h"

//General C++ include files
#include <iostream>
#include <fstream>
//Added by qt3to4:
#include <QTextStream>
#include <Q3ValueList>
using namespace std;

//include files for QT
#include <qfile.h> 
#include <qstring.h> 

using namespace ndmanager;

DescriptionWriter::DescriptionWriter():doc(){
 //create the processing instruction
 QDomProcessingInstruction processingInstruction = doc.createProcessingInstruction("xml","version='1.0'");
 doc.appendChild(processingInstruction);

 //Create the root element.
 root = doc.createElement(PARAMETER);
 doc.appendChild(root);
}

DescriptionWriter::~DescriptionWriter(){}

bool DescriptionWriter::writeTofile(const QString& url){ 
 QFile descriptionFile(url);
 bool status = descriptionFile.open(QIODevice::WriteOnly);
 if(!status) return status;

 root.appendChild(program);
 QString xmlDocument = doc.toString();
 
 QTextStream stream(&descriptionFile);
 stream<< xmlDocument;
 descriptionFile.close();
 
 return true;
}

void DescriptionWriter::setProgramInformation(ProgramInformation& programInformation){
 //Get the program information 
 QString name = programInformation.getProgramName();
 QMap<int, Q3ValueList<QString> > parametersInfo = programInformation.getParameterInformation();  
 QString help = programInformation.getHelp();
 
 program = doc.createElement(PROGRAM);;
 
 QDomElement nameElement = doc.createElement(NAME);
 QDomText nameValue = doc.createTextNode(name);
 nameElement.appendChild(nameValue);
 program.appendChild(nameElement);

 //Take care of the parameters
 QDomElement parameters = doc.createElement(PARAMETERS);
 QMap<int,Q3ValueList<QString> >::Iterator parameterIterator;
 //The iterator gives the keys sorted.
 for(parameterIterator = parametersInfo.begin(); parameterIterator != parametersInfo.end(); ++parameterIterator){
  QDomElement parameter = doc.createElement(PARAMETER);
  Q3ValueList<QString> parameterInfo = parameterIterator.data();
  
  for(uint i = 0; i< parameterInfo.count();++i){
   //the info are NAME, VALUE and STATUS   
   if(i == 0){
    QDomElement nameElement = doc.createElement(NAME);
    QDomText nameValue = doc.createTextNode(parameterInfo[i]);
    nameElement.appendChild(nameValue);
    parameter.appendChild(nameElement);    
   }
   if(i == 1){
    QDomElement valueElement = doc.createElement(VALUE);
    if(parameterInfo[i] != ""){
     QDomText valueValue = doc.createTextNode(parameterInfo[i]);
     valueElement.appendChild(valueValue);    
    }
    parameter.appendChild(valueElement);    
   }
   if(i == 2){
    QDomElement statusElement = doc.createElement(STATUS);
    QDomText statusValue = doc.createTextNode(parameterInfo[i]);
    statusElement.appendChild(statusValue);
    parameter.appendChild(statusElement);    
   }
  }
  parameters.appendChild(parameter);
 }

 program.appendChild(parameters);
 
 QDomElement helpElement = doc.createElement(HELP);
 if(help != ""){
  QDomText helpValue = doc.createTextNode(help);
  helpElement.appendChild(helpValue);    
 }
 program.appendChild(helpElement);
 
}
