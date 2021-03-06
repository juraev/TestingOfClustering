#include <vector>
#include <assert.h>
#include <math.h>
#include <numeric>
#include <algorithm>
#include <functional>
using namespace std;

template <typename T>
vector<T> VecAdd(const vector<T>& v1, const vector<T>& v2) {
	assert(v1.size() == v2.size());
	vector<T> result;
	transform(
		v1.begin(), v1.end(),
		v2.begin(), back_inserter(result),
		plus<T>());
	return result;
};

template <typename T>
vector<T> VecDiff(const vector<T>& v1, const vector<T>& v2) {
	assert(v1.size() == v2.size());
	vector<T> result;
	transform(
		v1.begin(), v1.end(),
		v2.begin(), back_inserter(result),
		minus<T>());
	return result;
};

template <typename T>
vector<T> VecMul(const vector<T>& v1, const vector<T>& v2) {
	assert(v1.size() == v2.size());
	vector<T> result;
	transform(
		v1.begin(), v1.end(),
		v2.begin(), back_inserter(result),
		multiplies<T>());
	return result;
};

template <typename T>
vector<T> VecAbs(const vector<T>& v) {
	vector<T> result;
	transform(
		v.begin(), v.end(),
		back_inserter(result),
		[](T x) -> T { return (x>0)?x:-x;});
	return result;
};

template <typename T>
double VecNorm(const vector<T>& v) {
	vector<T> sq = VecMul<T>(v, v);
	double s = double(accumulate(
		sq.begin(), sq.end(), 0.0));
	return sqrt(s);
};


