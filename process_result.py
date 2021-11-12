import os
import numpy as np
import pandas as pd

epsilons = [ 0.2, 0.4, 0.6, 0.8 ]
betas = [ 0.01, 0.125, 0.25, 0.5, 0.7, 0.84, 0.95 ]
epsilonDict = { k:i for i, k in enumerate(epsilons) }
betaDict = { k:i for i, k in enumerate(betas) }

dataset_line_prefix = '###### experiment on '
dataset_line_suffix = ' ######'

def isfloat(value):
	try:
		float(value)
		return True
	except ValueError:
		return False

def convert_table(data, row_names, col_names):
	df = pd.DataFrame(data.tolist())
	df.columns = col_names
	s = pd.Series(row_names)
	df.set_index([s,])
	return df

class Table:
	def __init__(self, experiment_name, dataset_name):
		self.experiment_name = experiment_name
		self.dataset_name = dataset_name
		self.N_table = np.zeros((len(betas), len(epsilons)))
		self.time_table = np.zeros((len(betas), len(epsilons)))
		self.result_table = np.zeros((len(betas), len(epsilons)))
		self.cell_r = 0
		self.cell_c = 0

	def process(self, line):
		splitted = line.rstrip().split()
		extracted = [ e for e in splitted if isfloat(e) ]
		if line.startswith('='):
			# a line indicating the parameters
			# e.g.: "===epsilon factor: 0.2 beta: 0.25"
			eps, beta = float(extracted[0]), float(extracted[1])
			self.cell_r = betaDict[beta]
			self.cell_c = epsilonDict[eps]
		elif line.startswith('s'):
			# N samples
			# e.g. "samples: 4604 N: 1000"
			N_samples = int(extracted[0])
			self.N_table[self.cell_r, self.cell_c] = N_samples
		elif line.startswith('r'):
			# result and duration
			# e.g. "result: 0 duration: 198.8"
			result, duration = float(extracted[0]), float(extracted[1])
			self.time_table[self.cell_r, self.cell_c] = duration
			self.result_table[self.cell_r, self.cell_c] = result
		else:
			pass

	def save_result(self):
		results_path = os.path.join(
			os.getcwd(), 'tables', 
			self.experiment_name,
			self.dataset_name)
		if not os.path.exists(results_path):
			os.makedirs(results_path)

		# save the result
		N_table = convert_table(self.N_table, betas, epsilons)
		N_table.to_csv(os.path.join(results_path, 'N_samples.csv'))
		time_table = convert_table(self.time_table, betas, epsilons)
		time_table.to_csv(os.path.join(results_path, 'duration.csv'))
		result_table = convert_table(self.result_table, betas, epsilons)
		result_table.to_csv(os.path.join(results_path, 'result.csv'))

def process_file(file, experiment_name):
	with open(file, 'r') as fp:
		line = fp.readline()
		table = None
		while line:
			if line.startswith('#'):
				# save result
				if table is not None:
					table.save_result()
				# read dataset name
				dataset_name = line[len(dataset_line_prefix):-len(dataset_line_suffix)]
				table = Table(experiment_name, dataset_name)
			else:
				table.process(line)
			line = fp.readline()
		if table is not None:
			table.save_result()

if __name__=='__main__':
	input_folder = 'experiment_logs'
	for folder in os.listdir(input_folder):
		file_path = os.path.join(input_folder, folder, 'log.txt')
		process_file(file_path, folder)
