#include <iostream>
#include "algo/sampling.h"
#include "utils/data.h" 
#include <filesystem>

using namespace clustering;
using std::vector;
using std::cout;
namespace fs = std::filesystem;

int main() {
	fs::path cwd = fs::current_path();
	fs::path ds("datasets/A");
	fs::path dsPath = cwd / ds;
	ExperimentData data = loadExperimentData(dsPath);
	cout << data.centers.size() << std::endl;
	// cout << data.points[0][0] << " " << data.points[0][1] << std::endl;

    // SamplingAlgorithm<Cost::RADIUS, Metric::L2, K::ANY> samplingAlgorithm{0.5f, 4.0f};

    // samplingAlgorithm.isClusterable(vector<clustering::Point>(), NULL);

    return 0;
}
