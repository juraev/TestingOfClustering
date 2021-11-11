#include "math_utils.h"
#include <limits>

using namespace std;

double PointDistL2(const vector<double>& p1, const vector<double>& p2);
double PointDistMax(const vector<double>& p1, const vector<double>& p2);
double ComputeClusterRadius(const vector<vector<double> >& points);
double ComputeClusterDiameter(const vector<vector<double> >& points);

using DistFunc=std::function<double(const vector<double>&, const vector<double>&, int)>;
DistFunc getDistFunction(const string& distType);