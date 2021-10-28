#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stdlib.h>
using namespace std;

struct DataConfigs {
	int N; 
	int D; 
	int k; 
	double b;
	double beta; 
	double max_beta;
	double epsilon; 
	string metric;
	string cost;
};

struct ExperimentData {
	vector<vector<double> > centers;
	vector<vector<double> > points;
	vector<int> labels;
	DataConfigs configs;
};

// read file of vectors (points)
vector<vector<double> > readVectors(const string& source_path);
// read file of labels
vector<int> readLabels(const string& source_path);
// read file of the configurations of data generation
DataConfigs getDataConfigs(const string& source_path);
ExperimentData loadExperimentData(const string& source_path);
