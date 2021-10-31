#include <iostream>
#include <filesystem>
#include <random>
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

    int k = 2, d = 2, n = 1000;
    double b = 100;
    double beta = 1;
    double epsilon = 8;

    auto dataset = vector<clustering::Point>{{1, 0}, {2, 0}, {10, 0}, {14.000000000000001, 0}};

    SamplingAlgorithm<Cost::DIAMETER, Metric::L2, K::ANY> samplingAlgorithm{epsilon, b};

    cout << samplingAlgorithm.isClusterable(beta, d, dataset, dist_l2, k);

    return 0;
}
