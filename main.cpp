#include <iostream>
#include "algo/sampling.h"
#include "utils/data.h" 
#include <filesystem>

using namespace clustering;
using std::vector;
using std::cout;
namespace fs = std::filesystem;

int main() {
	fs::path foldePath = fs::path(__FILE__).parent_path();
	fs::path dsPath = foldePath / fs::path("datasets") / fs::path("A");
	ExperimentData data = loadExperimentData(dsPath);

    SamplingAlgorithm<Cost::RADIUS, Metric::L2, K::ANY> samplingAlgorithm{0.5f, 4.0f};
    samplingAlgorithm.isClusterable(vector<clustering::Point>(), NULL);

    return 0;
}
