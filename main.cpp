#include <iostream>
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
	string experimentName = "Diam\_L2\_k\=1\_D\=2";
	vector<string> configNames{ "eps_0.2_beta_0.05-0.25", "eps_0.4_beta_0.05-0.25" };
	vector<double> epsilonFactors{ 0.5, 1.5 };
	vector<double> betaCandidates{ 0.1, 0.5, 0.9 };
	int nTrials = 20;

    fs::path folderPath = fs::path(__FILE__).parent_path();
	fs::path inputFolderPath = folderPath / fs::path("data") / fs::path(experimentName);
	for (string configName : configNames) {
		for (double epsilonFactor : epsilonFactors) {
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
							configs.epsilon * epsilonFactor, configs.b};
						bool result = samplingAlgorithm.isClusterable(
							betaCandidate, configs.D, data.points, distFunc, configs.k);
						// results += result;
					}
					else {
						throw std::runtime_error("unknown algorithm type");
					}
				}

				double avaerageResult = results / double(nTrials);
				std::cout << "epsilon factor: " << epsilonFactor 
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

    int k = 2, d = 2, n = 1000;
    double b = 100;
    double beta = 1;
    double epsilon = 8;

    auto dataset = vector<clustering::Point>{{1, 0}, {2, 0}, {10, 0}, {14.000000000000001, 0}};

    SamplingAlgorithm<Cost::DIAMETER, Metric::L2, K::ANY> samplingAlgorithm{epsilon, b};

    cout << samplingAlgorithm.isClusterable(beta, d, dataset, dist_l2, k);
	*/

    return 0;
}
