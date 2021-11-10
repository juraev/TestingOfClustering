import os
import sys
import json
import matplotlib
import numpy as np
import matplotlib.pyplot as plt
from scipy.spatial.distance import cdist
from sklearn.manifold import TSNE
from shutil import rmtree
matplotlib.use('Agg')

def generate_cluster_by_radius(center, radius, N, D, beta, metric):
	# confirm the diameter
	radius = radius * (1 + beta * np.random.rand())
	R_vec = np.random.rand(D) * 2 - 1
	if metric == 'euclidean':
		R_vec = R_vec / np.linalg.norm(R_vec)
	elif metric == 'chebyshev':
		R_vec = R_vec / np.max(np.abs(R_vec))
	else:
		raise ValueError('unknown metric')
	p1 = center + R_vec * radius
	p2 = center - R_vec * radius
	# generate the remaining points
	deviation_vecs = np.random.rand(N-2, D) * 2 - 1.0
	if metric == 'euclidean':
		original_norms = np.linalg.norm(deviation_vecs, axis=1)
	elif metric == 'chebyshev':
		original_norms = np.max(np.abs(deviation_vecs), axis=1)
	else:
		raise ValueError('unknown metric')
	deviation_vecs = deviation_vecs / original_norms[:, None]
	Rs = radius * np.random.rand(N-2)
	deviation = deviation_vecs * Rs[:, None]
	other_samples = center + deviation
	return np.vstack((p1, p2, other_samples))


def generate_cluster_by_diameter(center, diameter, N, D, beta, metric):
	# TODO: better?
	radius = diameter / 2
	return generate_cluster_by_radius(center, radius, N, D, beta, metric)


def generate_unclusterable_by_radius(centers, radius, N, D, beta, max_beta, metric):
	k = len(centers)
	results = []
	n_remain = N
	iters = 100
	while n_remain > 0 and iters > 0:
		# generate the points
		cluster_indices = np.random.randint(0, k, size=N)
		deviation_vectors = np.random.rand(N, D) * 2 - 1
		if metric == 'euclidean':
			original_norms = np.linalg.norm(deviation_vectors, axis=1)
		elif metric == 'chebyshev':
			original_norms = np.max(np.abs(deviation_vectors), axis=1)
		else:
			raise ValueError('unknown metric')
		deviation_vectors = deviation_vectors / original_norms[:, None]
		deviation_norms = radius * (1 + np.random.rand(N) * (max_beta - beta) + beta)
		deviations = deviation_vectors * deviation_norms[:, None]
		new_samples = centers[cluster_indices] + deviations
		# filter valid outliers
		min_dist = radius * (1 + beta)
		dists_to_centers = cdist(new_samples, centers, metric=metric)
		valid = np.all(dists_to_centers > min_dist, axis=1)
		valid_indices = np.where(valid)[0]
		valid_samples = new_samples[valid_indices, :]
		if len(valid_samples) > n_remain:
			valid_samples = valid_samples[:n_remain]
		results.append(valid_samples)
		n_remain -= len(valid_samples)
		iters -= 1
	if n_remain > 0:
		raise ValueError('cannot get enough points')
	return np.vstack(results)

def generate_unclusterable_by_diameter(centers, diameter, N, D, beta, max_beta, metric):
	# TODO: better?
	radius = diameter / 2
	return generate_unclusterable_by_radius(centers, radius, N, D, beta, max_beta, metric)


def generate_all_clusters(
		N, D, k, centers, cluster_portions, epsilon, beta, max_beta, mode, metric, b):
	# generate cluster labels
	N_unclusterable = int(N * epsilon) + 1
	unclusterable_labels = -np.ones(N_unclusterable).astype(np.int32)
	N_clusterable = N - N_unclusterable
	cluster_prob = np.array(cluster_portions) / np.sum(cluster_portions)
	clusterable_labels = np.random.choice(k, size=N_clusterable, p=cluster_prob)
	cluster_labels = np.concatenate((unclusterable_labels, clusterable_labels))
	np.random.shuffle(cluster_labels)
	# generate the data points
	samples = np.zeros((N, D))
	# get clusterable samples
	for kid in range(k):
		indices = np.where(cluster_labels==kid)[0]
		if mode == 'radius':
			cluster = generate_cluster_by_radius(
				centers[kid], b, len(indices), D, beta, metric)
		elif mode == 'diameter':
			cluster = generate_cluster_by_diameter(
				centers[kid], b, len(indices), D, beta, metric)
		else:
			raise NotImplementedError('unknown mode')
		samples[indices, :] = cluster

	# generate points not clusterable
	unclusterable_indices = np.where(cluster_labels < 0)[0]
	if mode == 'radius':
		to_remove = generate_unclusterable_by_radius(
			centers, b, len(unclusterable_indices), D, beta, max_beta, metric)
	elif mode == 'diameter':
		to_remove = generate_unclusterable_by_diameter(
			centers, b, len(unclusterable_indices), D, beta, max_beta, metric)
	else:
		raise NotImplementedError('unknown mode')
	samples[unclusterable_indices, :] = to_remove

	return samples, cluster_labels


def generate_cluster_centers(k, D, space_radius, beta, mode, metric, b):
	# the centers should be far enough
	if mode == 'radius':
		min_dist = b * (1 + beta) * 2
	elif mode == 'diameter':
		min_dist = b * (1 + beta)
	else:
		raise ValueError('unknown mode')
	n_remain = k
	results = []
	iters = 0
	max_iters = k * 20
	batch = 10
	while n_remain > 0 and iters < max_iters:
		new_centers = np.random.rand(batch, D) * 2 - 1
		new_centers *= space_radius
		if len(results) == 0:
			results.append(new_centers[0])
			n_remain -= 1
		else:
			dists = cdist(new_centers, results, metric=metric)
			valid = np.all(dists > min_dist, axis=1)
			valid_indices = np.where(valid)[0]
			if len(valid_indices) > 0:
				results.append(new_centers[valid_indices[0]])
				n_remain -= 1
		iters += 1
	if n_remain > 0:
		raise ValueError('cannot find k centers that satisfy the condition')
	return np.array(results)


def visualize_data(X, y, k):
	if X.shape[1] > 2:
		model = TSNE(n_components=2, random_state=0)
		transformed = model.fit_transform(X)
	else:
		transformed = X
	colors = y/k
	plt.scatter(transformed[:, 0], transformed[:, 1], c=colors, s=0.1)
	plt.axis('scaled')


def generate_data_by_configs(**kwargs):
	center_params = [
		'k', 'D', 'space_radius', 'beta', 'mode', 'b', 'metric']
	cluster_params = [
		'N', 'D', 'k', 'epsilon', 'beta', 'max_beta', 'mode', 'b', 'metric']
	centers = generate_cluster_centers(**{p: kwargs[p] for p in center_params})
	update_params = {p: kwargs[p] for p in cluster_params}
	update_params['centers'] = centers
	update_params['cluster_portions'] = np.ones(kwargs.get('k'))
	data, labels = generate_all_clusters(**update_params)
	return centers, data, labels

def write_data_to_txt(data, fpath, mode):
	with open(fpath, 'w') as fp:
		if mode == 'numeric':
			for d in data:
				data_line = ' '.join([str(x) for x in d])
				fp.write(f'{data_line}\n')
		elif mode == 'list':
			for d in data:
				fp.write(f'{str(d)}\n')
		else:
			raise ValueError('unknown mode')

PARAM_TO_STORE = ['N', 'D', 'k', 'b', 'beta', 'max_beta', 'epsilon', 'metric', 'mode']

def generate_and_store_data(config, folder_path):
	print(f"======generating {title}======")
	# build the directory
	if os.path.exists(folder_path):
		rmtree(folder_path)
	os.makedirs(folder_path)
	# generate the data
	centers, data, labels = generate_data_by_configs(**config)
	# store the data
	write_data_to_txt(centers, os.path.join(folder_path, 'centers.txt'), 'numeric')
	write_data_to_txt(data, os.path.join(folder_path, 'data.txt'), 'numeric')
	write_data_to_txt(labels, os.path.join(folder_path, 'labels.txt'), 'list')
	# write the configs
	with open(os.path.join(folder_path, 'configs.json'), 'w') as fp:
		json.dump(config, fp)
	write_data_to_txt(
		[config[p] for p in PARAM_TO_STORE], 
		os.path.join(folder_path, 'configs.txt'), 'list')
	# store the visualization
	visualize_data(data, labels, config['k'])
	plt.savefig(os.path.join(folder_path, 'data.jpg'))
	plt.clf()

if __name__=='__main__':
	config_name = sys.argv[1]
	config_path = sys.argv[2]
	n_trials = int(sys.argv[3])
	with open(config_path, 'r') as fp:
		all_configs = json.load(fp)
	for title, config in all_configs.items():
		for i in range(n_trials):
			generate_and_store_data(config,
				os.path.join(os.getcwd(), 'data', config_name, title, str(i), 'inputs'))
