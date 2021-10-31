#include <iostream>
#include <random>
#include "algo/sampling.h"

using namespace clustering;
using std::vector, std::cout;


vector<clustering::Point> generate(double b, int k, int d, int n){
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> distribution{0, 1};
    std::normal_distribution<double> stdn{0.0, 1.0};

    int N = 100000;

    vector<Point> centers(k);
    vector<double> radii(k);

    auto gen = [&mt, &distribution, N](){
        return distribution(mt) * N;
    };

    for (int i = 0; i < k; i ++){
        centers[i].resize(d);
        generate(centers[i].begin(), centers[i].end(), gen);
        radii[i] = b * 0.5 / (1 + mt() % 5);
    }

    vector<Point> dataset(n);

    auto gen_points = [&mt, &stdn](){
        return stdn(mt);
    };

    for (int i = 0; i < n; i++) {
        dataset[i].resize(d);
        generate(dataset[i].begin(), dataset[i].end(), gen_points);

        double norm = std::accumulate(dataset[i].begin(), dataset[i].end(), 0.0,
                                      [](double s, double x) { return s + x * x; });

        norm = sqrt(norm);
        double u = distribution(mt);

        unsigned int y = mt() % k;

        for (int j = 0; j < d; j++) {
            dataset[i][j] = centers[y][j] + radii[y] * pow(u, 1/d) * dataset[i][j] / norm;
        }
    }
    return move(dataset);
}


int main() {

    Dist dist_l2 = [](const Point& a, const Point& b, int d){
        double res = 0;
        for(int i = 0; i < d; i ++){
            res += (a[i] - b[i]) * (a[i] - b[i]);
        }
        return std::sqrt(res);
    };

    int k = 2, d = 2, n = 1000;
    double b = 100;

//    auto dataset = vector<clustering::Point>{{1, 0}, {2, 0}, {10, 0}, {14.000000000000001, 0}};
    auto dataset = generate(b, k, d, n);

    SamplingAlgorithm<Cost::DIAMETER, Metric::L2, K::ANY> samplingAlgorithm{8, b};

    cout << samplingAlgorithm.isClusterable(1, d, dataset, dist_l2, k);

    return 0;
}
