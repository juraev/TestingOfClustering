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
		int m_history;
        std::mt19937 myrand;
        double _epsilon{};
        double _b{};
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
        explicit SamplingAlgorithm(double epsilon, double b/*dist*/);
        bool isClusterable(double beta, int d, const vector<Point>&, Dist, int);
		inline int getM() { return m_history;  };
    };

    /**
     * Partial specialisation for RADIUS cost and L2 metric.
     * No need to specify
     * @tparam kkk
     */
    template<K kkk>
    class SamplingAlgorithm<Cost::RADIUS, Metric::L2, kkk> : BaseAlgorithm {
    public:
        explicit SamplingAlgorithm(double epsilon, double b);
        bool isClusterable(double beta, int d, const vector<Point>&, Dist, int);
		inline int getM() { return m_history; };
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
    SamplingAlgorithm<cost, metric, k>::SamplingAlgorithm(double epsilon, double b) {
        _epsilon = epsilon;
        _b = b;
		m_history = 0;
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
    SamplingAlgorithm<Cost::RADIUS, Metric::L2, k> ::SamplingAlgorithm(double epsilon, double b) {
        _epsilon = epsilon;
        _b = b;
		m_history = 0;
        std::random_device rd;
        myrand = std::mt19937(rd());
    }

    /**
     * Specialized implementation for radius cost and L2 metric
     * and any k (argument k is ignored and not used if K::ONE is given as a template argument)
     * @return
     */
    template<>
    bool SamplingAlgorithm<Cost::RADIUS, Metric::L2, K::ANY>::isClusterable(double beta, int d, const vector<Point>& dataset, Dist dist, int k) {
        int m = floor(d*k / _epsilon * log(d*k/_epsilon));
        int n = dataset.size();

		if (m <= 0) {
			m_history = 0;
			std::cout << "invalid m: " << m << std::endl;
			return false;
		}

		m_history = m;
		int max_M = 15;
		if (m > max_M) {
			std::cout << m << " larger than max M: " << max_M << std::endl;
			m = max_M;
		}

        vector<int> indices(m, 0);

        if (m < n) {

            std::mt19937 &mt = myrand;
            std::uniform_int_distribution<int> distribution{0, n - 1};

            auto gen = [&mt, &distribution]() {
                return distribution(mt);
            };
            std::generate(indices.begin(), indices.end(), gen);

        } else {
            m = n;
            indices.resize(m);
            std::iota(indices.begin(), indices.end(), 0);
        }

        vector<Point> samples(m);

        for(int i = 0; i < m; i ++)
            samples[i] = dataset[indices[i]];

        return euclidean_k_centers(samples, k, _b, dist, d);
    }

    /**
     * Specialized implementation for radius cost and L2 metric
     * for k=1.
     * @return
     */
    template<>
    bool SamplingAlgorithm<Cost::RADIUS, Metric::L2, K::ONE>::isClusterable(double beta, int d, const vector<Point>& dataset, Dist dist, int k) {
        int m = static_cast<int>(2 * floor(log(1/beta) / (_epsilon * beta)));
        int n = static_cast<int>(dataset.size());

		if (m < 0) {
			m_history = 0;
			std::cout << "invalid m: " << m << std::endl;
			return false;
		}
		m_history = m;

        std::mt19937& mt = myrand;
        std::uniform_int_distribution<int> distribution{0, n - 1};

        auto gen = [&mt, &distribution](){
            return distribution(mt);
        };

        vector<int> indices(m, 0);
        std::generate(indices.begin(), indices.end(), gen);

        vector<Cgal_Point> sample(m);
		vector<FT> coord_vec(d);
		FT* coord = &coord_vec[0];

        int i = 0;
        for(int ind : indices){
            int j = 0;
            for (double x : dataset[ind])
                coord[j++] = FT{x};
            sample[i++] = Cgal_Point (d, coord, coord + d);
        }

        double rad = radius(sample, d);
        return rad <= _b;
    }

    /**
     * Specialized implementation for diameter cost and L2 metric
     * and k = 1
     * @return
     */
    template<>
    bool SamplingAlgorithm<Cost::DIAMETER, Metric::L2, K::ONE>::isClusterable(double beta, int d, const vector<Point>& dataset, Dist dist, int k) {
        int m = static_cast < int>(2 * floor(1 / _epsilon * pow(d, 3 / 2) * log(1 / beta) * pow(2 / beta, d)));
        int n = static_cast<int>(dataset.size());
		m_history = m;

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
    bool SamplingAlgorithm<Cost::DIAMETER, Metric::L2, K::ANY>::isClusterable(double beta, int d, const vector<Point>& dataset, Dist dist, int k) {
        int m = static_cast<int>(2 * floor((k*k) * log(k) / _epsilon * d * pow((2/beta), 2*d)));
        int n = static_cast<int>(dataset.size());

		if (m <= 0) {
			m_history = 0;
			std::cout << "invalid m: " << m << std::endl;
			return false;
		}

		int max_M = 15;
		m_history = m;
		if (m > max_M) {
			std::cout << m << " larger than max M: " << max_M << std::endl;
			m = max_M;
		}

        vector<int> indices(m, 0);

        std::mt19937& mt = myrand;
        std::uniform_int_distribution<int> distribution{0, n - 1};

        auto gen = [&mt, &distribution](){
            return distribution(mt);
        };

        std::generate(indices.begin(), indices.end(), gen);

        vector<Point> samples(m);

		for (int i = 0; i < m; i++) {
			samples[i] = dataset[indices[i]];
		}

        return euclidean_k_diameters(samples, k, _b, dist, d);
    }

    vector<Point> generate_k_clusters(double b, int k, int d, int n){
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
}

#endif //TESTINGOFCLUSTERING_ABSTRACTSAMPLINGALGO_H
