#ifndef VOLUMEINDEX_H
#define VOLUMEINDEX_H

#include <QString>
#include <QtCore>
#include <QDir>
#include "../../v3d/v3d_core.h"
#include "AnalysisTools.h"
#include "MaskChan.h"

using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////////////////

class IndexSpecification
{
 public:

  QString space;

  double optical_x;
  double optical_y;
  double optical_z;

  int pixel_x;
  int pixel_y;
  int pixel_z;

  int index_unit;
  int bit_depth;
  int binary_threshold;
  
  QString rootPath;

  IndexSpecification() {}

  QString toString() {

    QString q("\n");

    q.append("space=");
    q.append(space);
    q.append("\n");

    q.append("optical_x=");
    q.append(QString::number(optical_x));
    q.append("\n");

    q.append("optical_y=");
    q.append(QString::number(optical_y));
    q.append("\n");

    q.append("optical_z=");
    q.append(QString::number(optical_z));
    q.append("\n");

    q.append("pixel_x=");
    q.append(QString::number(pixel_x));
    q.append("\n");

    q.append("pixel_y=");
    q.append(QString::number(pixel_y));
    q.append("\n");

    q.append("pixel_z=");
    q.append(QString::number(pixel_z));
    q.append("\n");

    q.append("index_unit=");
    q.append(QString::number(index_unit));
    q.append("\n");

    q.append("bits=");
    q.append(QString::number(bit_depth));
    q.append("\n");

    q.append("threshold=");
    q.append(QString::number(binary_threshold));
    q.append("\n");

    q.append("root=");
    q.append(rootPath);
    q.append("\n");

    return q;
  }

};

class FragmentSpecification
{
 public:
  long fragmentId;
  int index;
  QString maskPath;

  QString toString() {

    QString q("\n");

    q.append("  fragmentId=");
    q.append(QString::number(fragmentId));
    q.append("\n");

    q.append("    index=");
    q.append(QString::number(index));
    q.append("\n");

    q.append("    maskPath=");
    q.append(maskPath);
    q.append("\n");

    return q;
  }
    
};

class SampleSpecification
{
 public:
  friend class SampleSort;
  long sampleId;
  QString owner;
  long alignmentResultId;
  QString space;
  long alignedStackId;
  double nccScore;
  long neuronSeparationId;
  QString consolidatedLabelPath;
  QString consolidatedSignalPath;
  QList<FragmentSpecification*> fragmentList;

  QString toString() {
    
    QString q("\n");

    q.append("sampleId=");
    q.append(QString::number(sampleId));
    q.append("\n");

    q.append("owner=");
    q.append(owner);
    q.append("\n");

    q.append("aligmentResultId=");
    q.append(QString::number(alignmentResultId));
    q.append("\n");

    q.append("space=");
    q.append(space);
    q.append("\n");

    q.append("alignedStackId=");
    q.append(QString::number(alignedStackId));
    q.append("\n");

    q.append("nccScore=");
    q.append(QString::number(nccScore));
    q.append("\n");

    q.append("neuronSeparationid=");
    q.append(QString::number(neuronSeparationId));
    q.append("\n");

    q.append("consolidatedLabelPath=");
    q.append(consolidatedLabelPath);
    q.append("\n");

    q.append("consolidatedSignalPath=");
    q.append(consolidatedSignalPath);
    q.append("\n");

    for (int i=0;i<fragmentList.size();i++) {
      FragmentSpecification* fs=fragmentList[i];
      QString f=fs->toString();
      q.append(f);
    }
    
    return q;
  }

};

class SampleSort
{
 public:
  bool operator()(const SampleSpecification *s1, const SampleSpecification *s2 ) const;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////

class VolumeIndex
{
public:

    static const int MODE_UNDEFINED;
    static const int MODE_CREATE_SAMPLE_INDEX_FILE;
    static const int MODE_SEARCH;
    static const int MODE_ADD_SAMPLE_INDEX_FILE_TO_CONSOLIDATED_INDEX;

    VolumeIndex();

    ~VolumeIndex();

    static string getCommandLineDescription() {
        return "volume-index";
    }

    static string getUsage() {
        string usage;
        usage.append("  Volume Index Tool - v2 of earlier volume-pattern-index                                                \n");
        usage.append("                                                                                                        \n");
        usage.append("   Tool for indexing and searching volumes for similar patterns                                         \n");
	usage.append("                                                                                                        \n");
	usage.append("    'sample' creates a binary sample index file from a sample, for adding to main index                 \n");
	usage.append("    'search' searches the main index                                                                    \n");
        usage.append("    'index' adds a sample file to a main consolidated index                                             \n");
        usage.append("                                                                                                        \n");
        usage.append("     -mode [sample|search|index]                                                                        \n");
	usage.append("                                                                                                        \n");
	usage.append("   For all:                                                                                             \n");
        usage.append("     -indexSpecificationFile <index specification file>                                                 \n");
	usage.append("                                                                                                        \n");
	usage.append("   For modes sample and index:                                                                          \n");
	usage.append("     -sampleIndexFile <path to sample index file>                                                       \n");
        usage.append("                                                                                                        \n");
	usage.append("   For mode sample:                                                                                     \n");
	usage.append("     -sampleSpecificationFile <path to file with metadata for stack and mask files>                     \n");
	usage.append("                                                                                                        \n");
	usage.append("   For mode search:                                                                                     \n");
	usage.append("     -query <query stack or mask file>                                                                  \n");
        usage.append("     -maxHits <max number of hits>                                                                      \n");
        usage.append("                                                                                                        \n");
        return usage;
    }

    bool execute();
    int processArgs(vector<char*> *argList);

private:
    FILE* fid;

    bool DEBUG_FLAG;
    int mode;

    QString modeString;
    QString indexSpecificationFilepath;
    QString sampleIndexFilepath;
    QString sampleSpecificationFilepath;
    QString queryFilepath;
    QString maxHitsString;

    int maxHits;

    bool createSampleIndexFile();
    bool doSearch();
    bool addSampleToConsolidatedIndex();
    
    bool readIndexSpecificationFile();
    bool readSampleSpecificationFile();

    void addAlignmentIfMatchingSpace(SampleSpecification* ss);

    QList<SampleSpecification*> sampleSpecificationList;
    IndexSpecification* indexSpecification;
    SampleSpecification* sampleSpecification;

    My4DImage* sampleConsolidatedSignalImage;
    bool loadSampleConsolidatedSignalImage();
    bool validateImageSize(My4DImage* image);

    char* firstStageIndex;
    int firstStageIndexLength;

    char* secondStageIndex;
    int secondStageIndexLength;

    int divideDimensionByUnit(int originalSie, int unit);
    int subsampleAndThresholdToBinaryMask(My4DImage* sourceImage, char* targetMask, int unit, int threshold);

    bool createFirstStageIndex();

};

#endif // VOLUMEPATTERNINDEX_H
