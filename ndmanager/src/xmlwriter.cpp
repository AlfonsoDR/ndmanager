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
#include "xmlwriter.h"
#include "tags.h"
#include "channelcolors.h"
#include <helper.h>

//General C++ include files
#include <iostream>
#include <fstream>
using namespace std;

//include files for QT
#include <qfile.h> 
#include <qstring.h> 

using namespace ndmanager;

extern QString version;
const QString  XmlWriter::parameterVersion = "1.0";

XmlWriter::XmlWriter():doc(){
 //create the processing instruction
 QDomProcessingInstruction processingInstruction = doc.createProcessingInstruction("xml","version='1.0'");
 doc.appendChild(processingInstruction);

 //Create the root element and its attributes.
 root = doc.createElement(PARAMETERS);
 root.setAttribute(DOC__VERSION,parameterVersion);
 root.setAttribute(CREATOR,"ndManager-" + version);
 doc.appendChild(root);
}

XmlWriter::~XmlWriter(){}

bool XmlWriter::writeTofile(const KURL& url){ 
 QFile parameterFile(url.path());
 bool status = parameterFile.open(IO_WriteOnly);
 if(!status) return status;

 QDomElement neuroscope = doc.createElement(NEUROSCOPE);
 neuroscope.setAttribute(DOC__VERSION,"1.2.5");
 neuroscope.appendChild(miscellaneous);
 neuroscope.appendChild(neuroscopeVideo);
 neuroscope.appendChild(spikes);
 neuroscope.appendChild(channels);
 neuroscope.appendChild(channelDefaultOffsets);
 
 root.appendChild(generalInfo);
 root.appendChild(acquisitionSystem);
 if(!video.isNull())root.appendChild(video);
 root.appendChild(lfp);
 if(!files.isNull()) root.appendChild(files);
 root.appendChild(anatomicalDescription);
 root.appendChild(spikeDetection);
 root.appendChild(units);
 root.appendChild(neuroscope);
 if(!programs.isNull()) root.appendChild(programs);

 QString xmlDocument = doc.toString();
 
 QTextStream stream(&parameterFile);
 stream.setEncoding(QTextStream::UnicodeUTF8);
 stream<< xmlDocument;
 parameterFile.close();
 
 return true;
}

void XmlWriter::setGeneralInformation(GeneralInformation& generalInformation){
 generalInfo = doc.createElement(GENERAL);
 
 QDomElement dateElement = doc.createElement(DATE);
 QDomText dateValue = doc.createTextNode(generalInformation.getDate().toString(Qt::ISODate));
 dateElement.appendChild(dateValue);

 QDomElement experimentersElement = doc.createElement(EXPERIMENTERS); 
 if(generalInformation.getExperimenters() != ""){
  QDomText experimentersValue =doc.createTextNode(generalInformation.getExperimenters());
  experimentersElement.appendChild(experimentersValue);
 }

 QDomElement descriptionElement = doc.createElement(DESCRIPTION);
 if(generalInformation.getDescription() != ""){
  QDomText descriptionValue = doc.createTextNode(generalInformation.getDescription());
  descriptionElement.appendChild(descriptionValue);
 }
 
 QDomElement notesElement = doc.createElement(NOTES);
 if(generalInformation.getNotes() != ""){
  QDomText notesValue = doc.createTextNode(generalInformation.getNotes());
  notesElement.appendChild(notesValue);    
 }
 
 generalInfo.appendChild(dateElement);
 generalInfo.appendChild(experimentersElement);
 generalInfo.appendChild(descriptionElement);
 generalInfo.appendChild(notesElement);
}

void XmlWriter::setAcquisitionSystemInformation(QMap<QString,double>& acquisitionSystemInfo){
 acquisitionSystem = doc.createElement(ACQUISITION);
 
 QDomElement resolutionElement = doc.createElement(BITS);
 QDomText resolutionValue = doc.createTextNode(QString("%1").arg(acquisitionSystemInfo[BITS]));
 resolutionElement.appendChild(resolutionValue);

 QDomElement nbChannelsElement = doc.createElement(NB_CHANNELS);
 QDomText nbChannelsValue = doc.createTextNode(QString("%1").arg(acquisitionSystemInfo[NB_CHANNELS]));
 nbChannelsElement.appendChild(nbChannelsValue);

 QDomElement samplingRateElement = doc.createElement(SAMPLING_RATE);
 QDomText samplingRateValue = doc.createTextNode(Helper::doubleToString(acquisitionSystemInfo[SAMPLING_RATE]));
 samplingRateElement.appendChild(samplingRateValue);

 QDomElement voltageRangeElement = doc.createElement(VOLTAGE_RANGE);
 QDomText voltageRangeValue = doc.createTextNode(QString("%1").arg(acquisitionSystemInfo[VOLTAGE_RANGE]));
 voltageRangeElement.appendChild(voltageRangeValue);

 QDomElement amplificationElement = doc.createElement(AMPLIFICATION);
 QDomText amplificationValue = doc.createTextNode(QString("%1").arg(acquisitionSystemInfo[AMPLIFICATION]));
 amplificationElement.appendChild(amplificationValue);
  
 QDomElement offsetElement = doc.createElement(OFFSET);
 QDomText offsetValue = doc.createTextNode(QString("%1").arg(acquisitionSystemInfo[OFFSET]));
 offsetElement.appendChild(offsetValue);
 
 acquisitionSystem.appendChild(resolutionElement);
 acquisitionSystem.appendChild(nbChannelsElement);
 acquisitionSystem.appendChild(samplingRateElement);
 acquisitionSystem.appendChild(voltageRangeElement);
 acquisitionSystem.appendChild(amplificationElement); 
 acquisitionSystem.appendChild(offsetElement);
}

void XmlWriter::setVideoInformation(QMap<QString,double>& videoInformation){
 video = doc.createElement(VIDEO);
 
 QDomElement samplingRateElement = doc.createElement(SAMPLING_RATE);
 QDomText samplingRateValue = doc.createTextNode(Helper::doubleToString(videoInformation[SAMPLING_RATE]));
 samplingRateElement.appendChild(samplingRateValue);
 
 QDomElement widthElement = doc.createElement(WIDTH);
 QDomText widthValue = doc.createTextNode(QString("%1").arg(videoInformation[WIDTH]));
 widthElement.appendChild(widthValue);

 QDomElement heightElement = doc.createElement(HEIGHT);
 QDomText heightValue = doc.createTextNode(QString("%1").arg(videoInformation[HEIGHT]));
 heightElement.appendChild(heightValue);

 video.appendChild(samplingRateElement);
 video.appendChild(widthElement);
 video.appendChild(heightElement);
}

void XmlWriter::setLfpInformation(double lfpSamplingRate){
 lfp = doc.createElement(FIELD_POTENTIALS);
 QDomElement lfpElement = doc.createElement(LFP_SAMPLING_RATE);
 QDomText lfpValue = doc.createTextNode(Helper::doubleToString(lfpSamplingRate));
 lfpElement.appendChild(lfpValue);

 lfp.appendChild(lfpElement);
}

void XmlWriter::setFilesInformation(QValueList<FileInformation>& fileList){
 files = doc.createElement(FILES);
 
 QValueList<FileInformation>::iterator iterator;
 for(iterator = fileList.begin(); iterator != fileList.end(); ++iterator){
  //Get the file information 
  double samplingRate = static_cast<FileInformation>(*iterator).getSamplingRate();    
  QString extension = static_cast<FileInformation>(*iterator).getExtension();
  QMap<int, QValueList<int> > mapping = static_cast<FileInformation>(*iterator).getChannelMapping();
  
  QDomElement fileElement = doc.createElement(ndmanager::FILE);
  
  QDomElement samplingRateElement = doc.createElement(SAMPLING_RATE);
  QDomText samplingRateValue = doc.createTextNode(Helper::doubleToString(samplingRate));
  samplingRateElement.appendChild(samplingRateValue);
  fileElement.appendChild(samplingRateElement);

  QDomElement extensionElement = doc.createElement(EXTENSION);
  QDomText extensionValue = doc.createTextNode(extension);
  extensionElement.appendChild(extensionValue);
  fileElement.appendChild(extensionElement);

  //Take care of the channel mapping
  QDomElement channelMapping = doc.createElement(CHANNEL_MAPPING);
  QMap<int,QValueList<int> >::Iterator mappingIterator;
  //The iterator gives the keys sorted.
  for(mappingIterator = mapping.begin(); mappingIterator != mapping.end(); ++mappingIterator){
   QDomElement originalChannels = doc.createElement(ORIGINAL_CHANNELS);
   QValueList<int> channelIds = mappingIterator.data();
   
   QValueList<int>::iterator channelIterator;
   for(channelIterator = channelIds.begin(); channelIterator != channelIds.end(); ++channelIterator){
    QDomElement idElement = doc.createElement(CHANNEL);
    QDomText idValue = doc.createTextNode(QString("%1").arg(*channelIterator));
    idElement.appendChild(idValue);
    originalChannels.appendChild(idElement);
   }
   channelMapping.appendChild(originalChannels);
  }

  if(mapping.size() != 0) fileElement.appendChild(channelMapping);
  files.appendChild(fileElement);
 } 
}


void XmlWriter::setAnatomicalDescription(QMap<int, QValueList<int> >& anatomicalGroups,QMap<QString, QMap<int,QString> >& attributes){
 anatomicalDescription = doc.createElement(ANATOMY);
 QDomElement channelGroupsElement = doc.createElement(CHANNEL_GROUPS);

 //Create the anatomical groups
 QMap<int,QValueList<int> >::Iterator iterator;
 //The iterator gives the keys sorted.
 for(iterator = anatomicalGroups.begin(); iterator != anatomicalGroups.end(); ++iterator){
  QValueList<int> channelIds = iterator.data();
  QValueList<int>::iterator channelIterator;

  QDomElement groupElement = doc.createElement(GROUP);
  
  for(channelIterator = channelIds.begin(); channelIterator != channelIds.end(); ++channelIterator){
   QDomElement idElement = doc.createElement(CHANNEL);
   QDomText idValue = doc.createTextNode(QString("%1").arg(*channelIterator));
   idElement.appendChild(idValue);
   //the attributs are hard coded for the moment, and there is only SKIP for the time being
   idElement.setAttribute(SKIP,attributes["Skip"][*channelIterator]);
   groupElement.appendChild(idElement);
  }

  channelGroupsElement.appendChild(groupElement);
 }//end of groups loop
 
 if(channelGroupsElement.hasChildNodes()) anatomicalDescription.appendChild(channelGroupsElement); 

}

void  XmlWriter::setSpikeDetectionInformation(QMap<int, QValueList<int> >& spikeGroups,QMap<int, QMap<QString,QString> >& information){
 spikeDetection = doc.createElement(SPIKE);
 QDomElement channelGroupsElement = doc.createElement(CHANNEL_GROUPS);

 //Create the spike groups
 QMap<int,QValueList<int> >::Iterator iterator;
 //The iterator gives the keys sorted.
 for(iterator = spikeGroups.begin(); iterator != spikeGroups.end(); ++iterator){
  QValueList<int> channelIds = iterator.data();
  QValueList<int>::iterator channelIterator;

  QDomElement groupElement = doc.createElement(GROUP);
  QDomElement channelListElement = doc.createElement(CHANNELS);

  for(channelIterator = channelIds.begin(); channelIterator != channelIds.end(); ++channelIterator){
   QDomElement idElement = doc.createElement(CHANNEL);
   QDomText idValue = doc.createTextNode(QString("%1").arg(*channelIterator));
   idElement.appendChild(idValue);
   channelListElement.appendChild(idElement);
  }

  groupElement.appendChild(channelListElement);
  //Add the other information if need it
  QString nbSamples = information[iterator.key()][NB_SAMPLES];
  if(nbSamples != ""){
   QDomElement nbSamplesElement = doc.createElement(NB_SAMPLES);
   QDomText nbSamplesValue = doc.createTextNode(nbSamples);
   nbSamplesElement.appendChild(nbSamplesValue);
   groupElement.appendChild(nbSamplesElement);
  }
  
  QString peak = information[iterator.key()][PEAK_SAMPLE_INDEX];
  if(peak != ""){
   QDomElement peakElement = doc.createElement(PEAK_SAMPLE_INDEX);
   QDomText peakValue = doc.createTextNode(peak);
   peakElement.appendChild(peakValue);
   groupElement.appendChild(peakElement);
  }
  
  QString nbFeatures = information[iterator.key()][NB_FEATURES];
  if(nbFeatures != ""){
   QDomElement nbFeaturesElement = doc.createElement(NB_FEATURES);
   QDomText nbFeaturesValue = doc.createTextNode(nbFeatures);
   nbFeaturesElement.appendChild(nbFeaturesValue);
   groupElement.appendChild(nbFeaturesElement);
  }
     
  channelGroupsElement.appendChild(groupElement);
 }//end of groups loop


 if(channelGroupsElement.hasChildNodes()) spikeDetection.appendChild(channelGroupsElement);
}

void XmlWriter::setMiscellaneousInformation(float screenGain,QString traceBackgroundImage){
 miscellaneous = doc.createElement(MISCELLANEOUS);

 QDomElement gainElement = doc.createElement(SCREENGAIN);
 QDomText gainValue = doc.createTextNode(QString("%1").arg(screenGain,0,'f',6));
 gainElement.appendChild(gainValue);
 
 QDomElement imageElement = doc.createElement(TRACE_BACKGROUND_IMAGE);
 QDomText imageValue = doc.createTextNode(QString("%1").arg(traceBackgroundImage));
 imageElement.appendChild(imageValue);  

 miscellaneous.appendChild(gainElement);
 miscellaneous.appendChild(imageElement);
}

void XmlWriter::setNeuroscopeVideoInformation(NeuroscopeVideoInfo& videoInfo){
 neuroscopeVideo = doc.createElement(VIDEO);

 QDomElement rotationElement = doc.createElement(ROTATE);
 QDomText rotationValue = doc.createTextNode(QString("%1").arg(videoInfo.getRotation()));
 rotationElement.appendChild(rotationValue);

 QDomElement flipElement = doc.createElement(FLIP);
 QDomText flipValue = doc.createTextNode(QString("%1").arg(videoInfo.getFlip()));
 flipElement.appendChild(flipValue);

 QDomElement pathElement = doc.createElement(VIDEO_IMAGE);
 QDomText pathValue = doc.createTextNode(videoInfo.getBackgroundImage());
 pathElement.appendChild(pathValue);
 
 QDomElement drawTrajectoryElement = doc.createElement(POSITIONS_BACKGROUND);
 QDomText drawTrajectoryValue = doc.createTextNode(QString("%1").arg(videoInfo.getTrajectory()));
 drawTrajectoryElement.appendChild(drawTrajectoryValue);
 
 neuroscopeVideo.appendChild(rotationElement);
 neuroscopeVideo.appendChild(flipElement);
 neuroscopeVideo.appendChild(pathElement);
 neuroscopeVideo.appendChild(drawTrajectoryElement);
}

void XmlWriter::setNeuroscopeSpikeInformation(int nbSamples,int peakSampleIndex){
 spikes = doc.createElement(SPIKES);
 
 QDomElement nbSamplesElement = doc.createElement(NB_SAMPLES);
 QDomText nbSamplesValue = doc.createTextNode(QString("%1").arg(nbSamples));
 nbSamplesElement.appendChild(nbSamplesValue);

 QDomElement peakElement = doc.createElement(PEAK_SAMPLE_INDEX);
 QDomText peakValue = doc.createTextNode(QString("%1").arg(peakSampleIndex));
 peakElement.appendChild(peakValue);

 spikes.appendChild(nbSamplesElement);
 spikes.appendChild(peakElement);
}

void XmlWriter::setChannelDisplayInformation(QValueList<ChannelColors>& colorList,QMap<int,int>& channelDefaultOffsets){
 channels = doc.createElement(CHANNELS);
 
 QValueList<ChannelColors>::iterator iterator;
 for(iterator = colorList.begin(); iterator != colorList.end(); ++iterator){
  //Get the channel information (id and colors)
  int channelId = static_cast<ChannelColors>(*iterator).getId();
  QColor color = static_cast<ChannelColors>(*iterator).getColor();
  QColor anatomicalColor = static_cast<ChannelColors>(*iterator).getGroupColor();
  QColor spikeColor = static_cast<ChannelColors>(*iterator).getSpikeGroupColor();
  int offset = channelDefaultOffsets[channelId];
  
  QDomElement idElement = doc.createElement(CHANNEL);
  QDomText idValue = doc.createTextNode(QString("%1").arg(channelId));
  idElement.appendChild(idValue);

  QDomElement colorElement = doc.createElement(COLOR);
  QDomText colorValue = doc.createTextNode(color.name());
  colorElement.appendChild(colorValue);

  QDomElement anatomicalColorElement = doc.createElement(ANATOMY_COLOR);
  QDomText anatomicalColorValue = doc.createTextNode(anatomicalColor.name());
  anatomicalColorElement.appendChild(anatomicalColorValue);

  QDomElement spikeColorElement = doc.createElement(SPIKE_COLOR);
  QDomText spikeColorValue = doc.createTextNode(spikeColor.name());
  spikeColorElement.appendChild(spikeColorValue);

  QDomElement channelColors = doc.createElement(CHANNEL_COLORS);
  channelColors.appendChild(idElement);
  channelColors.appendChild(colorElement);
  channelColors.appendChild(anatomicalColorElement);
  channelColors.appendChild(spikeColorElement);

  QDomElement idElement2 = doc.createElement(CHANNEL);
  QDomText idValue2 = doc.createTextNode(QString("%1").arg(channelId));
  idElement2.appendChild(idValue2);
  
  QDomElement offsetElement = doc.createElement(DEFAULT_OFFSET);
  QDomText offsetValue = doc.createTextNode(QString("%1").arg(offset));
  offsetElement.appendChild(offsetValue);
  
  QDomElement channelOffset = doc.createElement(CHANNEL_OFFSET);
  channelOffset.appendChild(idElement2);
  channelOffset.appendChild(offsetElement);
  
  channels.appendChild(channelColors);
  channels.appendChild(channelOffset);
 }
}


void XmlWriter::setProgramsInformation(QValueList<ProgramInformation>& programList){
 programs = doc.createElement(PROGRAMS);
 
 QValueList<ProgramInformation>::iterator iterator;
 for(iterator = programList.begin(); iterator != programList.end(); ++iterator){
  //Get the program information 
  QString name = static_cast<ProgramInformation>(*iterator).getProgramName();
  QMap<int, QValueList<QString> > parametersInfo = static_cast<ProgramInformation>(*iterator).getParameterInformation();  
  QString help = static_cast<ProgramInformation>(*iterator).getHelp();
  
  QDomElement programElement = doc.createElement(PROGRAM);
  
  QDomElement nameElement = doc.createElement(NAME);
  QDomText nameValue = doc.createTextNode(name);
  nameElement.appendChild(nameValue);
  programElement.appendChild(nameElement);

  //Take care of the parameters
  QDomElement parameters = doc.createElement(PARAMETERS);
  QMap<int,QValueList<QString> >::Iterator parameterIterator;
  //The iterator gives the keys sorted.
  for(parameterIterator = parametersInfo.begin(); parameterIterator != parametersInfo.end(); ++parameterIterator){
   QDomElement parameter = doc.createElement(PARAMETER);
   QValueList<QString> parameterInfo = parameterIterator.data();
   
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

  programElement.appendChild(parameters);
  
  QDomElement helpElement = doc.createElement(HELP);
  if(help != ""){
   QDomText helpValue = doc.createTextNode(help);
   helpElement.appendChild(helpValue);
  }
  programElement.appendChild(helpElement);
  
  programs.appendChild(programElement);
 }  
}



void  XmlWriter::setUnitsInformation(QMap<int, QValueList<QString> >& units){
 this->units = doc.createElement(UNITS);

 //Create the unit elements
 QMap<int,QValueList<QString> >::Iterator iterator;

 //The iterator gives the keys sorted.
 for(iterator = units.begin(); iterator != units.end(); ++iterator){
  QValueList<QString> unit = iterator.data();
  QValueList<QString>::iterator unitIterator;

  QDomElement unitElement = doc.createElement(UNIT);

//   for(unitIterator = unit.begin(); unitIterator != unit.end(); ++unitIterator){
   unitIterator = unit.begin();
	QDomElement groupElement = doc.createElement(GROUP);
   QDomText groupValue = doc.createTextNode(*unitIterator);
   groupElement.appendChild(groupValue);
   unitElement.appendChild(groupElement);

	++unitIterator;
	QDomElement clusterElement = doc.createElement(CLUSTER);
   QDomText clusterValue = doc.createTextNode(*unitIterator);
   clusterElement.appendChild(clusterValue);
   unitElement.appendChild(clusterElement);

	++unitIterator;
	QDomElement structureElement = doc.createElement(STRUCTURE);
	QDomText structureValue = doc.createTextNode(*unitIterator);
	structureElement.appendChild(structureValue);
	unitElement.appendChild(structureElement);

	++unitIterator;
	QDomElement typeElement = doc.createElement(TYPE);
	QDomText typeValue = doc.createTextNode(*unitIterator);
	typeElement.appendChild(typeValue);
	unitElement.appendChild(typeElement);

	++unitIterator;
	QDomElement idElement = doc.createElement(ISOLATION_DISTANCE);
   QDomText idValue = doc.createTextNode(*unitIterator);
   idElement.appendChild(idValue);
   unitElement.appendChild(idElement);

	++unitIterator;
	QDomElement qualityElement = doc.createElement(QUALITY);
   QDomText qualityValue = doc.createTextNode(*unitIterator);
   qualityElement.appendChild(qualityValue);
   unitElement.appendChild(qualityElement);

	++unitIterator;
	QDomElement notesElement = doc.createElement(NOTES);
   QDomText notesValue = doc.createTextNode(*unitIterator);
   notesElement.appendChild(notesValue);
   unitElement.appendChild(notesElement);
	
	this->units.appendChild(unitElement);
//   }

 }//end of units loop
}
