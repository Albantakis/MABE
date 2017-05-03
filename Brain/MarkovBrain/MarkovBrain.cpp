//  MABE is a product of The Hintze Lab @ MSU
//     for general research information:
//         hintzelab.msu.edu
//     for MABE documentation:
//         github.com/ahnt/MABE/wiki
//
//  Copyright (c) 2015 Michigan State University. All rights reserved.
//     to view the full license, visit:
//         github.com/ahnt/MABE/wiki/License

#include "MarkovBrain.h"

shared_ptr<ParameterLink<bool>> MarkovBrain::randomizeUnconnectedOutputsPL = Parameters::register_parameter("BRAIN_MARKOV_ADVANCED-randomizeUnconnectedOutputs", false, "output nodes with no connections will be set randomly (default : false, behavior set to 0)");
shared_ptr<ParameterLink<int>> MarkovBrain::randomizeUnconnectedOutputsTypePL = Parameters::register_parameter("BRAIN_MARKOV_ADVANCED-randomizeUnconnectedOutputsType", 0, "determines type of values resulting from randomizeUnconnectedOutput [0 = int, 1 = double]");
shared_ptr<ParameterLink<double>> MarkovBrain::randomizeUnconnectedOutputsMaxPL = Parameters::register_parameter("BRAIN_MARKOV_ADVANCED-randomizeUnconnectedOutputsMax", 1.0, "random values resulting from randomizeUnconnectedOutput will be in the range of 0 to randomizeUnconnectedOutputsMax");
shared_ptr<ParameterLink<int>> MarkovBrain::hiddenNodesPL = Parameters::register_parameter("BRAIN_MARKOV-hiddenNodes", 8, "number of hidden nodes");  // string parameter for outputMethod;


void MarkovBrain::readParameters(){
	randomizeUnconnectedOutputs = (PT == nullptr) ? randomizeUnconnectedOutputsPL->lookup() : PT->lookupBool("BRAIN_MARKOV_ADVANCED-randomizeUnconnectedOutputs");
	randomizeUnconnectedOutputsType = (PT == nullptr) ? randomizeUnconnectedOutputsTypePL->lookup() : PT->lookupInt("BRAIN_MARKOV_ADVANCED-randomizeUnconnectedOutputsType");
	randomizeUnconnectedOutputsMax = (PT == nullptr) ? randomizeUnconnectedOutputsMaxPL->lookup() : PT->lookupDouble("BRAIN_MARKOV_ADVANCED-randomizeUnconnectedOutputsMax");

	hiddenNodes = (PT == nullptr) ? hiddenNodesPL->lookup() : PT->lookupInt("BRAIN_MARKOV-hiddenNodes");
	nrNodes = nrInputValues + nrOutputValues + hiddenNodes;
	nodes.resize(nrNodes, 0);
	nextNodes.resize(nrNodes, 0);
}

MarkovBrain::MarkovBrain(vector<shared_ptr<AbstractGate>> _gates, int _nrInNodes, int _nrOutNodes, shared_ptr<ParametersTable> _PT) :
		AbstractBrain(_nrInNodes, _nrOutNodes, _PT) {

	readParameters();

	//GLB = nullptr;
	GLB = make_shared<ClassicGateListBuilder>(PT);
	gates = _gates;
	// columns to be added to ave file
	aveFileColumns.clear();
	aveFileColumns.push_back("gates");
	for (auto name : GLB->getInUseGateNames()) {
		aveFileColumns.push_back(name + "Gates");
	}

	fillInConnectionsLists();
}

MarkovBrain::MarkovBrain(shared_ptr<AbstractGateListBuilder> _GLB, int _nrInNodes, int _nrOutNodes, shared_ptr<ParametersTable> _PT) :
		AbstractBrain(_nrInNodes, _nrOutNodes, _PT) {
	GLB = _GLB;
	//make a node map to handle genome value to brain state address look up.

	readParameters();

	makeNodeMap(nodeMap, Gate_Builder::bitsPerBrainAddressPL->lookup(), nrNodes);

	// columns to be added to ave file
	aveFileColumns.clear();
	aveFileColumns.push_back("gates");
	for (auto name : GLB->getInUseGateNames()) {
		aveFileColumns.push_back(name + "Gates");
	}
}

MarkovBrain::MarkovBrain(shared_ptr<AbstractGateListBuilder> _GLB, shared_ptr<AbstractGenome> genome, int _nrInNodes, int _nrOutNodes, shared_ptr<ParametersTable> _PT) :
		MarkovBrain(_GLB, _nrInNodes, _nrOutNodes, _PT) {
	//cout << "in MarkovBrain::MarkovBrain(shared_ptr<Base_GateListBuilder> _GLB, shared_ptr<AbstractGenome> genome, int _nrOfBrainStates)\n\tabout to - gates = GLB->buildGateList(genome, nrOfBrainStates);" << endl;
	gates = GLB->buildGateList(genome, nrNodes, _PT);
	inOutReMap();  // map ins and outs from genome values to brain states
	fillInConnectionsLists();
}

shared_ptr<AbstractBrain> MarkovBrain::makeBrainFromGenome(shared_ptr<AbstractGenome> _genome) {
	shared_ptr<MarkovBrain> newBrain = make_shared<MarkovBrain>(GLB, _genome, nrInputValues, nrOutputValues, PT);
	return newBrain;
}

void MarkovBrain::resetBrain() {
	AbstractBrain::resetBrain();
	nodes.assign(nrNodes, 0.0);
	for (size_t i = 0; i < gates.size(); i++) {
		gates[i]->resetGate();
	}
}

void MarkovBrain::update() {
	nextNodes.assign(nrNodes, 0.0);
	for (int i = 0; i < nrInputValues; i++){
		nodes[i] = inputValues[i];
	}
	for (size_t i = 0; i < gates.size(); i++) {  //update each gate
		gates[i]->update(nodes, nextNodes);
	}
	if (randomizeUnconnectedOutputs) {
		if (randomizeUnconnectedOutputsType == 0) {
			for (int i = 0; i < nrOutputValues; i++) {
				//cout << i << " : " << nextNodesConnections[i] << endl;
				if (nextNodesConnections[nrInputValues+i] == 0) { // note nrInputValues+i gets us the index for the node related to each output
		//cout << ".";
					nextNodes[nrInputValues+i] = Random::getInt(0, (int) randomizeUnconnectedOutputsMax);
				}
			}
		} else if (randomizeUnconnectedOutputsType == 1) {
			for (int i = 0; i < nrOutputValues; i++) {
				if (nextNodesConnections[nrInputValues+i] == 0) {
					nextNodes[nrInputValues+i] = Random::getDouble(0, randomizeUnconnectedOutputsMax);
				}
			}
		} else {
			cout << "  ERROR! BRAIN_MARKOV_ADVANCED::randomizeUnconnectedOutputsType is invalid. current value: " << randomizeUnconnectedOutputsType << endl;
			exit(1);
		}
	}
	swap(nodes, nextNodes);
	for (int i = 0; i < nrOutputValues; i++){
		outputValues[i] = nodes[nrInputValues+i];
	}
}

void MarkovBrain::inOutReMap() {  // remaps genome site values to valid brain state addresses
	for (size_t i = 0; i < gates.size(); i++) {
		gates[i]->applyNodeMap(nodeMap, nrNodes);
	}

}

string MarkovBrain::description() {
	string S = "Markov Briain\n" + gateList();
	return S;
}

void MarkovBrain::fillInConnectionsLists() {
	nodesConnections.resize(nrNodes);
	nextNodesConnections.resize(nrNodes);
	for (auto g : gates) {
		auto gateConnections = g->getConnectionsLists();
		for (auto c : gateConnections.first) {
			nodesConnections[c]++;
		}
		for (auto c : gateConnections.second) {
			nextNodesConnections[c]++;
		}
	}
}
DataMap MarkovBrain::getStats() {
	DataMap dataMap;
	dataMap.Set("gates",(int)gates.size());
	map<string, int> gatecounts;
	for (auto n : GLB->getInUseGateNames()) {
		gatecounts[n + "Gates"] = 0;
	}
	for (auto g : gates) {
		gatecounts[g->gateType() + "Gates"]++;
	}

	for (auto n : GLB->getInUseGateNames()) {
		dataMap.Set(n + "Gates", gatecounts[n + "Gates"]);
	}

	vector<int> nodesConnectionsList;
	vector<int> nextNodesConnectionsList;

	for (int i = 0; i < nrNodes; i++) {
		nodesConnectionsList.push_back(nodesConnections[i]);
		nextNodesConnectionsList.push_back(nextNodesConnections[i]);
	}
	dataMap.Set("nodesConnections", nodesConnectionsList);
	dataMap.setOutputBehavior("nodesConnections", DataMap::LIST);
	dataMap.Set("nextNodesConnections", nextNodesConnectionsList);
	dataMap.setOutputBehavior("nextNodesConnections", DataMap::LIST);

	return (dataMap);
}

string MarkovBrain::gateList() {
	string S = "";
	for (size_t i = 0; i < gates.size(); i++) {
		S = S + gates[i]->description();
	}
	return S;
}

vector<vector<int>> MarkovBrain::getConnectivityMatrix() {
	vector<vector<int>> M;
	M.resize(nrNodes);
	for (int i = 0; i < nrNodes; i++) {
		M[i].resize(nrNodes);
		for (int j = 0; j < nrNodes; j++)
			M[i][j] = 0;
	}
	for (auto G : gates) {
		vector<int> I = G->getIns();
		vector<int> O = G->getOuts();
		for (int i : I)
			for (int o : O)
				M[i][o]++;
	}
	return M;
}

int MarkovBrain::brainSize() {
	return (int) gates.size();
}

int MarkovBrain::numGates() {
	return brainSize();
}

void MarkovBrain::initalizeGenome(shared_ptr<AbstractGenome> _genome) {
	int codonMax = (1 << Gate_Builder::bitsPerCodonPL->lookup()) - 1;
	_genome->fillRandom();

	auto genomeHandler = _genome->newHandler(_genome);

	for (auto gateType : GLB->gateBuilder.inUseGateTypes) {
		for (int i = 0; i < GLB->gateBuilder.intialGateCounts[gateType]; i++) {
			genomeHandler->randomize();
			for (auto value : GLB->gateBuilder.gateStartCodes[gateType]) {
				genomeHandler->writeInt(value, 0, codonMax);
			}
		}
	}
}

shared_ptr<AbstractBrain> MarkovBrain::makeCopy(shared_ptr<ParametersTable> _PT)
{
	if (_PT == nullptr) {
		_PT = PT;
	}
	vector<shared_ptr<AbstractGate>> _gates; 
	for (auto gate : gates) {
		_gates.push_back(gate->makeCopy());
	}
	auto newBrain = make_shared<MarkovBrain>(_gates, nrInputValues, nrOutputValues, _PT);
	newBrain->buildFromGenome = buildFromGenome;
	return newBrain;
}
