# TestingOfClustering

Implementation of the algorithms for testing (k, b)-clusterability of a collection of data. 
Based on this [paper](https://doi.org/10.1137/S0036144503437178) by Alon et al.

ExperimentData: the structure that contains the data and relevant information for the experiment.
1. centers (a vector of k vectors of dimension D): the representatives of the clusters.
2. points (a vector of N vectors of dimension D): all data points 
3. labels (a vector of integers): the cluster that the point is assigned to; '-1' denotes that it's an unclassifiable outliers.
4. configs (DataConfigs): 
* N (int): number of points 
* D (int): number of dimensions
* k (int): number of clusters
* b (double): the diameter or radius
* beta (double): the data is about epsilon-far from (k, (1+beta)b) clusterable
* max_beta (double): the data is guranteed (k, (1+max_beta)b) clusterable
* epsilon (double): portion of points to remove to be clusterable 
* metric (string): "euclidean" or "chebyshev"
* cost: "radius" or "diameter"
To load the data, put the data in datasets. The folder includes all data and meta-data described about and a "data.jpg" that provides visualization of the data. ([example](https://nusu-my.sharepoint.com/:u:/g/personal/e0248120_u_nus_edu/EaADFPACn6NMvhXVwD6wouQBAqX0xpVRikhrR6ZQ0aTlVQ))

(For details please check utils/data.h)
