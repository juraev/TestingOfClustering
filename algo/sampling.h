//
// Created by gitarist on 23/10/21.
//

#ifndef TESTINGOFCLUSTERING_ABSTRACTSAMPLINGALGO_H
#define TESTINGOFCLUSTERING_ABSTRACTSAMPLINGALGO_H

#include <stdexcept>
#include <functional>
#include <random>
#include "euclidean_center.h"

namespace clustering {

    using std::vector;
    using Point=vector<double>;

    // a function to calculate distance between two points
    // it is supposed to be provided by the client of the library
    using Dist=std::function<double(const Point&, const Point&, int)>;

    //declarations

    enum class Cost {
        DIAMETER, RADIUS
    };

    enum class Metric {
        L2, ANY
    };

    enum class K {
        ONE, ANY
    };

    class BaseAlgorithm {
    protected:
        std::mt19937 myrand;
        float _epsilon;
        float _b;
    };

    /**
     * Sampling algorithm template.
     * Specialized implementations are given below.
     * @tparam cost
     * @tparam metric
     * @tparam k
     */
    template<Cost cost, Metric metric, K k = K::ANY>
    class SamplingAlgorithm : BaseAlgorithm {
    public:
        explicit SamplingAlgorithm(float epsilon, float b/*dist*/);
        bool isClusterable(float beta, int d, const vector<Point>&, Dist);
    };

    /**
     * Partial specialisation for RADIUS cost and L2 metric.
     * No need to specify
     * @tparam kkk
     */
    template<K kkk>
    class SamplingAlgorithm<Cost::RADIUS, Metric::L2, kkk> : BaseAlgorithm {
    public:
        explicit SamplingAlgorithm(float epsilon, float b);
        bool isClusterable(float beta, int d, const vector<Point>&, Dist);
    };

    // definitions

    /**
     * Common constructor for Sampling algorithms
     * @tparam cost
     * @tparam metric
     * @tparam k
     * @param epsilon
     * @param b
     */
    template<Cost cost, Metric metric, K k>
    SamplingAlgorithm<cost, metric, k>::SamplingAlgorithm(float epsilon, float b) {
        _epsilon = epsilon;
        _b = b;
        std::random_device rd;
        myrand = std::mt19937(rd());
    }

    /**
     * Constructor for Sampling algorithms for Radius cost
     * @tparam cost
     * @tparam metric
     * @tparam k
     * @param epsilon
     * @param b
     */
    template<K k>
    SamplingAlgorithm<Cost::RADIUS, Metric::L2, k> ::SamplingAlgorithm(float epsilon, float b) {
        _epsilon = epsilon;
        _b = b;
        std::random_device rd;
        myrand = std::mt19937(rd());
    }

    /**
     * Specialized implementation for radius cost and L2 metric
     * and any k
     * @return
     */
    template<>
    bool SamplingAlgorithm<Cost::RADIUS, Metric::L2, K::ANY>::isClusterable(float beta, int d, const vector<Point>& dataset, Dist dist) {
        throw std::runtime_error("Not implemented: SamplingAlgorithm<Cost::RADIUS, Metric::L2, k>");
    }

    /**
     * Specialized implementation for radius cost and L2 metric
     * for k=1. There is no specific algorithm.
     * @return
     */
    template<>
    bool SamplingAlgorithm<Cost::RADIUS, Metric::L2, K::ONE>::isClusterable(float beta, int d, const vector<Point>& dataset, Dist dist) {
        int m = 5 * floor(log(1/beta) / (_epsilon * beta));
        int n = dataset.size();

        std::mt19937& mt = myrand;
        std::uniform_int_distribution<int> distribution{0, n - 1};

        auto gen = [&mt, &distribution](){
            return distribution(mt);
        };

        vector<int> indices(m, 0);
        std::generate(indices.begin(), indices.end(), gen);

        vector<Cgal_Point> sample(m);

        FT coord[d];

        int i = 0;
        for(int ind : indices){
            int j = 0;
            for (double x : dataset[ind])
                coord[j ++] = FT{x};
            sample[i ++] = Cgal_Point (d, coord, coord + d);
        }

        double rad = radius(sample, d);

        std::cout << rad << " " << m << std::endl;

        return rad <= _b;
    }

    /**
     * Specialized implementation for diameter cost and L2 metric
     * and any k
     * @return
     */
    template<>
    bool SamplingAlgorithm<Cost::DIAMETER, Metric::L2, K::ONE>::isClusterable(float beta, int d, const vector<Point>& dataset, Dist dist) {

        int m = 2 * floor(1 / _epsilon * pow(d, 3 / 2) * log(1 / beta) * pow(2 / beta, d));
        int n = dataset.size();

        vector<int> indices(m, 0);

        std::mt19937& mt = myrand;
        std::uniform_int_distribution<int> distribution{0, n - 1};

        auto gen = [&mt, &distribution](){
            return distribution(mt);
        };

        std::generate(indices.begin(), indices.end(), gen);

        for (int i : indices){
            for (int j : indices) {
                if (dist(dataset[i], dataset[j], d) > _b){
                    return false;
                }
            }
        }
        return true;
    }

    /**
     * Specialized implementation for diameter cost and L2 metric
     * and any k
     * @return
     */
    template<>
    bool SamplingAlgorithm<Cost::DIAMETER, Metric::L2, K::ANY>::isClusterable(float beta, int d, const vector<Point>& dataset, Dist dist) {
        throw std::runtime_error("Not implemented: SamplingAlgorithm<Cost::DIAMETER, Metric::ANY, K::ANY>");
    }


}

#endif //TESTINGOFCLUSTERING_ABSTRACTSAMPLINGALGO_H
