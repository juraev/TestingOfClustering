#include <iostream>
#include "algo/sampling.h"

using namespace clustering;
using std::vector, std::cout;

int main() {

    SamplingAlgorithm<Cost::RADIUS, Metric::L2, K::ANY> samplingAlgorithm{0.5f, 4.0f};


    samplingAlgorithm.isClusterable(vector<clustering::Point>(), NULL);

    return 0;
}
