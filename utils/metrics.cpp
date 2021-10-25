#include "metrics.h"
using namespace std;

double PointDist(vector<double> p1, vector<double> p2) {
	vector<double> diff = VecDiff<double>(p1, p2);
	return VecNorm<double>(diff);
}

double ComputeClusterRadius(vector<vector<double> > points) {
	// TODO
	return 0.0;
}

double ComputeClusterDiameter(vector<vector<double> > points) {
	double max_dist = 0.0;
	for (int i = 0; i < points.size(); i++) {
		for (int j = i + 1; j < points.size(); j++) {
			double dist = PointDist(points[i], points[j]);
			max_dist = max(max_dist, dist);
		}
	}
	return max_dist;
}
