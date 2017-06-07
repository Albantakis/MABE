//  MABE is a product of The Hintze Lab @ MSU
//     for general research information:
//         hintzelab.msu.edu
//     for MABE documentation:
//         github.com/ahnt/MABE/wiki
//
//  Copyright (c) 2015 Michigan State University. All rights reserved.
//     to view the full license, visit:
//         github.com/ahnt/MABE/wiki/License

#ifndef __BasicMarkovBrainTemplate__DefaultArchivist__
#define __BasicMarkovBrainTemplate__DefaultArchivist__

#include <algorithm>
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "../Global.h"
#include "../Organism/Organism.h"
#include "../Utilities/MTree.h"

using namespace std;

class DefaultArchivist {
 public:



	/*shared_ptr<ParameterLink<string>> DefaultArchivist::Arch_outputMethodStrPL = Parameters::register_parameter("ARCHIVIST_OUTPUT_METHOD-outputMethod", (string) "default", "output method, [default, LODwAP (Line of Decent with Aggressive Pruning), snapshot, SSwD (SnapShot with Delay)]");  // string parameter for outputMethod;

shared_ptr<ParameterLink<int>> DefaultArchivist::Arch_realtimeFilesIntervalPL = Parameters::register_parameter("ARCHIVIST-realtimeFilesInterval", 10, "How often to write to realtime data files");
shared_ptr<ParameterLink<bool>> DefaultArchivist::Arch_writeAveFilePL = Parameters::register_parameter("ARCHIVIST-writeAveFile", true, "Save data to average file?");
shared_ptr<ParameterLink<bool>> DefaultArchivist::Arch_writeMaxFilePL = Parameters::register_parameter("ARCHIVIST-writeMaxFile", true, "Save data to Max file?");
shared_ptr<ParameterLink<string>> DefaultArchivist::Arch_AveFileNamePL = Parameters::register_parameter("ARCHIVIST-aveFileName", (string) "ave.csv", "name of average file (saves population averages)");
shared_ptr<ParameterLink<string>> DefaultArchivist::Arch_MaxFileNamePL = Parameters::register_parameter("ARCHIVIST-MaxFileName", (string) "Max.csv", "name of Max file (saves data on Max organism)");
shared_ptr<ParameterLink<string>> DefaultArchivist::Arch_DefaultAveFileColumnNamesPL = Parameters::register_parameter("ARCHIVIST-aveFileColumns", (string) "[]", "data to be saved into average file (must be values that can generate an average). If empty, MABE will try to figure it out");
shared_ptr<ParameterLink<bool>> DefaultArchivist::Arch_MaxFileShowAllListsPL = Parameters::register_parameter("ARCHIVIST-MaxFileShowAllLists", true, "lists named 'all'* in data map will be averaged and added to file. if true, raw 'all'* lists will also be added to the file");
	 */

	static shared_ptr<ParameterLink<string>> Arch_outputMethodStrPL;  // string parameter for outputMethod;

	static shared_ptr<ParameterLink<bool>> Arch_writeAveFilePL;  // if true, ave file will be created
	static shared_ptr<ParameterLink<bool>> Arch_writeMaxFilePL;  // if true, Max file will be created
	static shared_ptr<ParameterLink<string>> Arch_AveFileNamePL;  // name of the Averages file (ave for all brains when file is written to)
	static shared_ptr<ParameterLink<string>> Arch_MaxFileNamePL;  // name of the Max file (all stats for best brain when file is writtne to)
	static shared_ptr<ParameterLink<string>> Arch_DefaultAveFileColumnNamesPL;  // data to be saved into average file (must be values that can generate an average)

	static shared_ptr<ParameterLink<string>> Arch_realtimeSequencePL;  // how often to write out data
	static shared_ptr<ParameterLink<string>> SS_Arch_dataSequencePL;  // how often to save data
	static shared_ptr<ParameterLink<string>> SS_Arch_organismSequencePL;  // how often to save genomes

	static shared_ptr<ParameterLink<string>> SS_Arch_DataFilePrefixPL;  // name of the Data file
	static shared_ptr<ParameterLink<string>> SS_Arch_OrganismsFilePrefixPL;  // name of the Genome file (genomes on LOD)
	static shared_ptr<ParameterLink<bool>> SS_Arch_writeDataFilesPL;  // if true, write data file
	static shared_ptr<ParameterLink<bool>> SS_Arch_writeOrganismsFilesPL;  // if true, write genome file




	bool writeAveFile;  // if true, ave file will be created
	bool writeMaxFile;  // if true, Max file will be created
	string AveFileName;  // name of the Averages file (ave for all brains when file is written to)
	string MaxFileName;  // name of the Max file (all stats for best brain when file is writtne to)
	string AveFileColumnNames;  // data to be saved into average file (must be values that can generate an average)
	shared_ptr<Abstract_MTree> maxFormula;  // what value will be used to determine which organism to write to max file


	vector<int> realtimeSequence;  // how often to write out data
	int realtimeSequenceIndex;

	vector<int> realtimeDataSequence;
	vector<int> realtimeOrganismSequence;
	int realtimeDataSeqIndex;
	int realtimeOrganismSeqIndex;


	string DataFilePrefix;  // name of the Data file
	string OrganismFilePrefix;  // name of the Genome file (genomes on LOD)
	bool writeSnapshotDataFiles;  // if true, write data file
	bool writeSnapshotGenomeFiles;  // if true, write genome file

	string groupPrefix;

	map<string, vector<string>> files;  // list of files (NAME,LIST OF COLUMNS)
	vector<string> DefaultAveFileColumns;  // what columns will be written into the AveFile

	bool finished;  // if finished, then as far as the archivist is concerned, we can stop the run.

	const shared_ptr<ParametersTable> PT;

	DefaultArchivist(shared_ptr<ParametersTable> _PT = nullptr, string _groupPrefix = "");
	DefaultArchivist(vector<string> aveFileColumns, shared_ptr<Abstract_MTree> _maxFormula = nullptr, shared_ptr<ParametersTable> _PT = nullptr, string _groupPrefix = "");
	virtual ~DefaultArchivist() = default;

	//save Max and average file data
	void writeRealTimeFiles(vector<shared_ptr<Organism>> &population);

	void saveSnapshotData(vector<shared_ptr<Organism>> population);

	//void saveSnapshotGenomes(vector<shared_ptr<Organism>> population);
	void saveSnapshotOrganisms(vector<shared_ptr<Organism>> population);

	// save data and manage in memory data
	// return true if next save will be > updates + terminate after
	virtual bool archive(vector<shared_ptr<Organism>> population, int flush = 0);

	//virtual void processAllLists(OldDataMap &dm);

	virtual bool isDataUpdate(int checkUpdate = -1);
	virtual bool isOrganismUpdate(int checkUpdate = -1);

};

#endif /* defined(__BasicMarkovBrainTemplate__DefaultArchivist__) */
