#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <filesystem>
using namespace std;
namespace fs=filesystem;

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
vector<vector<double> > readVectors(fs::path source_path);
// read file of labels
vector<int> readLabels(fs::path source_path);
// read file of the configurations of data generation
DataConfigs getDataConfigs(fs::path source_path);
ExperimentData loadExperimentData(fs::path source_path);
