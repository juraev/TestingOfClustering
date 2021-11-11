#include <iostream>
#include <fstream>
#include <filesystem>
#include <random>
#include <stdexcept>
#include "algo/sampling.h"
#include "utils/data.h"
#include "utils/metrics.h"

using namespace clustering;
using std::vector;
using std::string;
using std::to_string;
namespace fs = std::filesystem;

int main() {
	string experimentName = "Diam_L2_k=1_D=2";
	vector<string> configNames{ "eps_0.2_beta_0.05-0.25", "eps_0.4_beta_0.05-0.25" };
	vector<double> epsilonCandidates{ 0.2, 0.4, 0.6, 0.8 };
	vector<double> betaCandidates{ 0.01, 0.125, 0.25, 0.5, 0.7, 0.84, 0.95 };
	int nTrials = 20;

    fs::path folderPath = fs::path(__FILE__).parent_path();
	fs::path inputFolderPath = folderPath / fs::path("data") / fs::path(experimentName);
	fs::path outputFolderPath = folderPath / fs::path("experiment_logs") / fs::path(experimentName);
	fs::path outputPath = outputFolderPath / fs::path("log.txt");
	fs::create_directory(outputFolderPath);
	std::ofstream out(outputPath);
	std::streambuf* coutbuf = std::cout.rdbuf();
	std::cout.rdbuf(out.rdbuf());

	for (string configName : configNames) {
		for (double epsilonCandidate : epsilonCandidates) {
			for (double betaCandidate : betaCandidates) {
				double results = 0;
				for (int trialIdx = 0; trialIdx < nTrials; trialIdx++) {
					// load the data
					fs::path dsPath = inputFolderPath / fs::path(configName) / fs::path(to_string(trialIdx)) / fs::path("inputs");
					ExperimentData data = loadExperimentData(dsPath);
					
					DataConfigs configs = data.configs;

					// define dist function
					string distType = configs.metric;
					Dist distFunc = getDistFunction(distType);

					bool isDiameter = configs.cost.compare("diameter") == 0;
					bool isL2 = configs.metric.compare("euclidean") == 0;
					bool isSingle = configs.k == 1;
					if (isDiameter && isL2 && isSingle) {
						SamplingAlgorithm<Cost::DIAMETER, Metric::L2, K::ONE> samplingAlgorithm{ 
							epsilonCandidate, configs.b};
						bool result = samplingAlgorithm.isClusterable(
							betaCandidate, configs.D, data.points, distFunc, configs.k);
						results += result;
					}
					else {
						throw std::runtime_error("unknown algorithm type");
					}
				}

				double avaerageResult = results / double(nTrials);
				std::cout << "epsilon factor: " << epsilonCandidate 
					<< " beta: " << betaCandidate 
					<< " result: " << results << std::endl;
			}
		}
	}

	/*
    Dist dist_l2 = [](const Point& a, const Point& b, int d){
        double res = 0;
        for(int i = 0; i < d; i ++){
            res += (a[i] - b[i]) * (a[i] - b[i]);
        }
        return std::sqrt(res);
    };

    int k = 2, d = 3, n = 1000;
    double b = 2;
    double beta = 0.01;
    double epsilon = 0.7;

//    auto dataset = vector<clustering::Point>{{1, 0}, {2, 0}, {10, 0}, {14.000000000000001, 0}};
    auto dataset = generate_k_clusters(b, k, d, n);

    SamplingAlgorithm<Cost::RADIUS, Metric::L2, K::ANY> samplingAlgorithm{epsilon, b/(20 + beta)};

    cout << samplingAlgorithm.isClusterable(beta, d, dataset, dist_l2, k);
	*/

	std::cout.rdbuf(coutbuf);
	std::cout << "end" << std::endl;

    return 0;
}
