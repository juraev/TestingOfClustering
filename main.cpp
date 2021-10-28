#include <iostream>
#include <filesystem>
#include "algo/sampling.h"
#include "utils/data.h" 

using namespace clustering;
using std::vector;
using std::cout;
namespace fs = std::filesystem;

int main() {
    fs::path foldePath = fs::path(__FILE__).parent_path();
    fs::path dsPath = foldePath / fs::path("datasets") / fs::path("example");
    ExperimentData data = loadExperimentData(dsPath);
    
    // TODO: apply the algorithms to the data

    Dist dist_l2 = [](const Point& a, const Point& b, int d){
        double res = 0;
        for(int i = 0; i < d; i ++){
            res += (a[i] - b[i]) * (a[i] - b[i]);
        }
        return std::sqrt(res);
    };

    auto dataset = vector<clustering::Point>{{1}, {2}, {3}, {9.01}};

    SamplingAlgorithm<Cost::RADIUS, Metric::L2, K::ONE> samplingAlgorithm{0.5f, 4.0f};

    cout << samplingAlgorithm.isClusterable(0.5, 1, dataset, dist_l2);

    return 0;
}
