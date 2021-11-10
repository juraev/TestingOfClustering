#include "metrics.h"
using namespace std;

double PointDistL2(const vector<double>& p1, const vector<double>& p2) {
	vector<double> diff = VecDiff<double>(p1, p2);
	return VecNorm<double>(diff);
}

double PointDistMax(const vector<double>& p1, const vector<double>& p2) {
	vector<double> diff = VecDiff<double>(p1, p2);
	vector<double> vecAbs = VecAbs<double>(diff);
	return *max_element(vecAbs.begin(), vecAbs.end());
}

double ComputeClusterRadius(const vector<vector<double> >& points) {
	// TODO
	return 0.0;
}

double ComputeClusterDiameter(const vector<vector<double> >& points) {
	double max_dist = 0.0;
	for (int i = 0; i < points.size(); i++) {
		for (int j = i + 1; j < points.size(); j++) {
			double dist = PointDistL2(points[i], points[j]);
			max_dist = max(max_dist, dist);
		}
	}
	return max_dist;
}

DistFunc getDistFunction(const string& distType) {
	if (distType.compare("chebyshev") == 0) {
		return [](const vector<double>& a, const vector<double>& b, int d) {
			return PointDistMax(a, b);
		};
	}
	else {
		return [](const vector<double>& a, const vector<double>& b, int d) {
			return PointDistL2(a, b);
		};
	}
}


