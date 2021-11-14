#include <iostream>
#include <fstream>
#include <filesystem>
#include <random>
#include <stdexcept>
#include <ctime>
#include "algo/sampling.h"
#include "utils/data.h"
#include "utils/metrics.h"

using namespace clustering;
using std::vector;
using std::string;
using std::to_string;
namespace fs = std::filesystem;

int main() {
	string experimentName = "Radius_L2_k=2_D=2_ver2";
	vector<string> configNames{ 
		"N_10000_eps_0.1_beta_0.15-0.35", "N_10000_eps_0.1_beta_0.45-0.65",
		"N_10000_eps_0.1_beta_0.75-0.95", "N_10000_eps_0.3_beta_0.15-0.35", 
		"N_10000_eps_0.3_beta_0.45-0.65", "N_10000_eps_0.3_beta_0.75-0.95",
		"N_10000_eps_0.5_beta_0.15-0.35", "N_10000_eps_0.5_beta_0.45-0.65",
		"N_10000_eps_0.5_beta_0.75-0.95"};
	int nTrials = 20;

    fs::path folderPath = fs::path(__FILE__).parent_path();
	fs::path inputFolderPath = folderPath / fs::path("data") / fs::path(experimentName);
	fs::path outputFolderPath = folderPath / fs::path("experiment_logs") / fs::path(experimentName);
	fs::path outputPath = outputFolderPath / fs::path("log.txt");
	if (!fs::is_directory(outputFolderPath)) {
		fs::create_directory(outputFolderPath);
	}
	
	std::ofstream out(outputPath);
	std::streambuf* coutbuf = std::cout.rdbuf();
	std::cout.rdbuf(out.rdbuf());

	for (string configName : configNames) {
		std::cout << "###### experiment on " << configName << " ######" << std::endl;
				double results = 0.0;
				double duration = 0.0;
				int n_samples = 0;
				int n_points = 0;
				std::clock_t timerStart;

				double epsilonCandidate, betaCandidate;

				for (int trialIdx = 0; trialIdx < nTrials; trialIdx++) {
					// load the data
					fs::path dsPath = inputFolderPath / fs::path(configName) / fs::path(to_string(trialIdx)) / fs::path("inputs");
					ExperimentData data = loadExperimentData(dsPath);
					
					DataConfigs configs = data.configs;
					epsilonCandidate = configs.epsilon - 0.05;
					betaCandidate = configs.beta - 0.05;

					// define dist function
					string distType = configs.metric;
					Dist distFunc = getDistFunction(distType);

					n_points = configs.N;
					bool isDiameter = configs.cost.compare("diameter") == 0;
					bool isL2 = configs.metric.compare("euclidean") == 0;
					bool isSingle = configs.k == 1;
					bool result = true;
					if (isDiameter && isL2 && isSingle) {
						SamplingAlgorithm<Cost::DIAMETER, Metric::L2, K::ONE> samplingAlgorithm{ 
							epsilonCandidate, configs.b};
						timerStart = std::clock();
						result = samplingAlgorithm.isClusterable(
							betaCandidate, configs.D, data.points, distFunc, configs.k);
						n_samples = samplingAlgorithm.getM();
					} else if (isDiameter && isL2 && (!isSingle)) {
						SamplingAlgorithm<Cost::DIAMETER, Metric::L2, K::ANY> samplingAlgorithm{
							epsilonCandidate, configs.b };
						timerStart = std::clock();
						result = samplingAlgorithm.isClusterable(
							betaCandidate, configs.D, data.points, distFunc, configs.k);
						n_samples = samplingAlgorithm.getM();
					} else if ((!isDiameter) && isL2 && isSingle) {
						SamplingAlgorithm<Cost::RADIUS, Metric::L2, K::ONE> samplingAlgorithm{
							epsilonCandidate, configs.b };
						timerStart = std::clock();
						result = samplingAlgorithm.isClusterable(
							betaCandidate, configs.D, data.points, distFunc, configs.k);
						n_samples = samplingAlgorithm.getM();
					} else if ((!isDiameter) && isL2 && (!isSingle)) {
						SamplingAlgorithm<Cost::RADIUS, Metric::L2, K::ANY> samplingAlgorithm{
							epsilonCandidate, configs.b };
						timerStart = std::clock();
						result = samplingAlgorithm.isClusterable(
							betaCandidate, configs.D, data.points, distFunc, configs.k);
						n_samples = samplingAlgorithm.getM();
					} else {
						throw std::runtime_error("unknown algorithm type");
					}

					duration += std::clock() - timerStart;
					results += result;
				}

				std::cout << "===epsilon factor: " << epsilonCandidate
					<< " beta: " << betaCandidate << std::endl;
				std::cout << "samples: " << n_samples 
					<< " N: " << n_points << std::endl;
				std::cout << "result: " << results / nTrials
					<< " duration: " << duration / nTrials << std::endl;
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
