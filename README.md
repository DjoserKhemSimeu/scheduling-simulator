# Job scheduling heuristics using Machine Learning (source material)

This repository contains the companion material for the research work **Job scheduling heuristics using Machine Learning**. All the work done in this study was based on the research paper **An experimental analysis of regression-obtained HPC scheduling heuristics** prepared by Lucas Rosa, Danilo Carastan-Santos and Alfredo Goldman.
The authors are:
Djoser Simeu and Danilo Carastan-Santos

## Inital setup
### Dependencies
The code in this repository has been tested and executed on Debian Bullseye (11) with the following dependencies:
- SimGrid 3.13
- Python 3.9.2 (+ `requirements.txt`)

There are details in the `Dockerfile` on how to locally compile and install this specific version of SimGrid. It's also possible to use nix, the require environement is provided in the file `simgrid.nix`.

### The canonical way
Once every dependency is installed, you must run the `initialize.py` script. This script download the remaining files and compile the C code. On the root of the repository:
```bash
python initialize.py
```

### Using Dev Containers
If you use Visual Studio Code, there is an extension (**Dev Containers**) that allows you to bring VSCode inside the container, providing the runtime environment for the IDE. The `.devcontainer/devcontainer.json` file defines how the environment will be (image, container, extensions, etc.).

In VSCode, just invoke the Command Palette (`F1`) and run **Dev Containers: Open Folder in Container...** to open the current folder in a container. Then, run the `initialize.py` script.

### Using Docker container
Another way to run the code in this repository is through **Docker**. The first step is to build the current project image, which can be done through the command:
```bash
docker build -t build-simgrid .
```

After building the image, the container can be used to run different commands. For instance, you can run the `initialize.py` script as following:
```bash
docker run --rm -v $(pwd):/workspaces/scheduling-simulator/ build-simgrid bash -c "python /workspaces/scheduling-simulator/initialize.py"
```

Note that this command must be executed in the project root, otherwise it will fail.

## The modules
### Simulator
This module is used to generate the distribution `\operatorname{score}(p, q, r)`. The simulation parameters are described as a simple python dictionary at the start of `simulator.py` and `simulator_trials.py` files. Any entry can be modified to satisfy your needs. We define argument for defining the random seed of the algorithm, we also explore the latin hypercube initialization for the first population of the genetic algorithm, you can use it by using the flag `-l`.

The simulations could take some time, depending on the chosen parameters. Because of this, you should execute the code using the `nohup <code> &` structure, for instance
```bash
nohup python src/simulator/simulator.py 42 &
```

This command starts a background Python process running the simulation. It is recommended to leave this process running at least for a couple of days (for parameters of the order used in the paper).

The `simulator/` directory contains two important directories: The `task-sets` directory contains all the task tuples $(S, Q)$ generated - each line in the CSV files contain characteristics (runtimes, no. of processors, submit time) of a job. The `training-data` directory contains all of the trial score distributions generated - each line in the CSV files represents the observed scheduling behavior of a job (characteristics + score).

The `Simulator` class have two methods to manage the generated files. `clear_files()` can be used to clear the generated data (warning: this method deletes all generated files). `gather_training_data()` on the other hand, join all the trial score into one file. Modify any parameter to suit you needs.

If the workload used changes, it will be necessary to change the files `deployment_cluster.xml` and `simple_cluster.xml` (check your workload no. of processors). We implemented to method for the generation of the trainning dataset, the genetic-algo version correspond to the `simulator.py` file and the random search correspond to th `simulator_trials.py`.

### Regressor
The regressor module is the simplest amoung the others. To execute the code is enough to run
```bash
python /src/regressor/regressor.py
```
We defined multiple transformation of the trainning dataset provided by the simulator code to normalized our data and to add the context-info as job attributes. You can define which version of the training dataset could be used for the trainning of the regression models, by modifying the parameter of the parameters of the constructor of the object `Regressor` in the main of the `regressor.py` file. 
The parameters are also defined as variables on the top of the script. The functions (our polynomials) are defined at `polynomials.py`. This module can be flexible to newer functions as long as you follow our functions structure.

### Tester
Our last module is the tester. It is used to evaluate the regression-obtained heuristics as scheduling policies. To use it, edit the `workload_experiments()` function inputs at the end of the script. The avaliable options are the dictionaries `traces`, `simulators`, and `policies_flags` keys. You can also define that your experiments must used normalized job attributes by using the normalization flag `-norm`. Then, to run the module execute
```bash
python /src/tester/tester.py
```

Modifying the module for new functions/parameters may not be trivial at the moment. You need to manually add your functions/parameters on the `.c` and `.h` files and recompile it. 

## Reproduce our results
To reproduce our results use the following parameters. Your results may differ on RNG-dependent parts of our code (generating tuples, etc.).

**Simulator**:
*Trials*:
```python
SIMULATION_PARAMETERS = {
    "workload": str(DATA_DIR / "workloads" / "lublin_256.swf"),
    "application": str(DATA_DIR / "applications" / "deployment_cluster.xml"),
    "platform": str(DATA_DIR / "platforms" / "simple_cluster.xml"),
    "number-of-tuples": 100,
    "number-of-trials": 25600,
    "size-of-S": 16,
    "size-of-Q": 32,
}
```
*Genetic-algorithm*:
```python
SIMULATION_PARAMETERS = {
    "workload": str(DATA_DIR / "workloads" / "lublin_256.swf"),
    "application": str(DATA_DIR / "applications" / "deployment_cluster.xml"),
    "platform": str(DATA_DIR / "platforms" / "simple_cluster.xml"),
    "number-of-tuples": 100,
    "population-size": 40,
    "mutation-prob": 0.05,
    "number-of-generations": 300,
    "size-of-S": 16,
    "size-of-Q": 32,
}
```

**Regressor**:
```python
SCORE_DISTRIBUTION_NORM = DATA_DIR / "global_training_data_GA_norm_minmax.csv"
SCORE_DISTRIBUTION_MEM_NORM = DATA_DIR / "global_training_data_GA_MEM_norm_minmax.csv"
SCORE_DISTRIBUTION = DATA_DIR / "global_training_data_GA.csv"
SCORE_DISTRIBUTION_MEM = DATA_DIR / "global_training_data_GA_MEM.csv"
REPORT_FILE = DATA_DIR / "regression_report.json"
SERIAL_FUNCTIONS =[
    ser_1_1, ser_1_2, ser_1_3,
    ser_2_1, ser_2_2, ser_2_3,
    ser_3_1, ser_3_2, ser_3_3,
    ser_4_1, ser_4_2, ser_4_3,
    ser_5_1, ser_5_2, ser_5_3,
    ser_6_1, ser_6_2, ser_6_3,
    ser_7_1, ser_7_2, ser_7_3,
    ser_8_1, ser_8_2, ser_8_3,
    ser_9_1, ser_9_2, ser_9_3,
    ser_10_1, ser_10_2, ser_10_3
]
```

# Contact
Djoser Simeu - djoser.simeu@etu.univ-grenoble-alpes.fr
