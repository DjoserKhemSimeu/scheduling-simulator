import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

import itertools
import math
from tqdm import tqdm
from multiprocessing import Process
from statsmodels.stats.outliers_influence import variance_inflation_factor

def convert_temporal_data(dataframe):
  '''
  Convert temporal variables from seconds to hours.

  This conversion is necessary. Otherwise, large predictor values (e.g., p^6)
  will be irrelevant for OLS.
  '''

  for column in ['p', 'r']:
    dataframe[column] = dataframe[column] / 3600

  return dataframe

def read_score_distribution(csv_file):
  columns = ['p', 'q', 'r', 'score']
  distribution = pd.read_csv(csv_file, names=columns)
  distribution = convert_temporal_data(distribution)

  return distribution
variables = ['p', 'q', 'r']
degre_max = 4

# Générer les combinaisons
possibilites = []
possibilites.append("sqrt(p)")
possibilites.append("sqrt(q)")
possibilites.append("sqrt(r)")
for degre in range(1, degre_max + 1):
    for variable in variables:
        possibilites.append(variable + str(degre))

for degre in range(1, degre_max + 1):
    for i in range(len(variables)):
        for j in range(i + 1, len(variables)):
            possibilites.append(variables[i] + str(degre) + variables[j])
            possibilites.append(variables[j] + str(degre) + variables[i])



# Afficher les combinaisons
print (possibilites)
def compute_vif(features):
    vif = pd.DataFrame()
    vif['VIF'] = [variance_inflation_factor(features.values, i) for i in range(features.shape[1])]
    vif['feature'] = features.columns
    return vif
def grid_search (dataframe, n):
    best_vif=100000000000000
    best_set=[]
    comb=list(itertools.combinations(possibilites,n))
    for var_set in tqdm(comb, desc="Progression"):
        
        df_copy = dataframe.copy()
        
        # Appliquer les transformations pour chaque variable dans l'ensemble
        for var in var_set:
            if var.startswith('sqrt'):
                root_var = var[-2]  # Récupérer la variable ('p', 'q' ou 'r') à partir de la variable sqrt
                df_copy[var] = df_copy[root_var].apply(math.sqrt)
            elif var.startswith(('p', 'q', 'r'))and len(var)==2:
                power = int(var[1:]) if len(var) > 1 else 1  # Récupérer l'exposant du polynôme
                base_var = var[0]  # Récupérer la variable ('p', 'q' ou 'r') à partir de la variable polynomiale
                df_copy[var] = df_copy[base_var] ** power
            elif var in ['p2q', 'q2p', 'p2r', 'r2p', 'q2r', 'r2q']:
                var_1, var_2 = var[0], var[2]  # Récupérer les deux variables ('p', 'q' ou 'r') de la combinaison
                df_copy[var] = (df_copy[var_1] **2) * df_copy[var_2]
            elif var in ['p3q', 'q3p', 'p3r', 'r3p', 'q3r', 'r3q']:
                var_1, var_2 = var[0], var[2]  # Récupérer les trois variables ('p', 'q' ou 'r') de la combinaison
                df_copy[var] = (df_copy[var_1]**3) * df_copy[var_2] 
            elif var in ['p4q', 'q4p', 'p4r', 'r4p', 'q4r', 'r4q']:
                var_1, var_2 = var[0], var[2] # Récupérer les quatre variables ('p', 'q' ou 'r') de la combinaison
                df_copy[var] = (df_copy[var_1]**4) * df_copy[var_2]
        res=compute_vif(df_copy)
        max=np.max(res['VIF'])
        if max<best_vif:
            best_vif=max
            best_set=var_set
    with open("grid.csv", "a+") as res:
        res.write(str(n))
        res.write(',')
        res.write(str(best_vif))
        res.write(',')
        res.write(str(best_set))
        res.write('\n')
    print(best_vif)
    print(best_set)

csv_file = "data/global_training_data_GA.csv"
raw_dist = read_score_distribution(csv_file)
features_label = ['p', 'q', 'r']
target_label = ["score"]
features = raw_dist[features_label]
target = raw_dist[target_label]
p=[]
for i in range (1,16):
    p.append(Process(target=grid_search,args=(features,i)))
    p[i-1].start()

for proc in p:
    proc.join()
    
        
    
