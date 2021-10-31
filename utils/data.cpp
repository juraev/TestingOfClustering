#include "data.h"
using namespace std;

template <typename T>
T line2other(const string& line) {
	T value;
	stringstream ss(line);
	ss >> value;
	return value;
}

string pathAppend(const string& p1, const string& p2) {
	char sep = '/';
	string tmp = p1;
	#ifdef _WIN32
		sep = '\\';
	#endif

	if (p1[p1.length()] != sep) { // Need to add a
		tmp += sep;                // path separator
		return(tmp + p2);
	}
	else
		return(p1 + p2);
}

vector<vector<double> > readVectors(fs::path source_path) {
	fstream source(source_path);
	string line;
	vector<vector<double> > results;
	int i = 0;

	while (getline(source, line))
	{
		double value;
		stringstream ss(line);

		results.push_back(vector<double>());

		while (ss >> value)
		{
			results[i].push_back(value);
		}
		++i;
	}
	return results;
}

vector<int> readLabels(fs::path source_path) {
	fstream source(source_path);
	string line;
	vector<int> results;
	while (getline(source, line))
	{
		int label = line2other<int>(line);
		results.push_back(label);
	}
	return results;
}

DataConfigs getDataConfigs(fs::path source_path) {
	fstream source(source_path);
	string line;
	DataConfigs configs;
	getline(source, line);
	configs.N = line2other<int>(line);
	getline(source, line);
	configs.D = line2other<int>(line);
	getline(source, line);
	configs.k = line2other<int>(line);
	getline(source, line);
	configs.b = line2other<double>(line);
	getline(source, line);
	configs.beta = line2other<double>(line);
	getline(source, line);
	configs.max_beta = line2other<double>(line);
	getline(source, line);
	configs.epsilon = line2other<double>(line);
	getline(source, line);
	configs.metric = line2other<string>(line);
	getline(source, line);
	configs.cost = line2other<string>(line);
	return configs;
}

ExperimentData loadExperimentData(fs::path source_path) {
	fs::path points_path = source_path / fs::path("data.txt");
	fs::path centers_path = source_path / fs::path("centers.txt");
	fs::path labels_path = source_path / fs::path("labels.txt");
	fs::path configs_path = source_path / fs::path("configs.txt");

	ExperimentData data;
	data.centers = readVectors(centers_path);
	data.points = readVectors(points_path);
	data.labels = readLabels(labels_path);
	data.configs = getDataConfigs(configs_path);

	return data;
}
