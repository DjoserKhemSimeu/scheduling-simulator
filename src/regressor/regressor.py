import json
import pathlib
import numpy as np
from scipy.optimize import curve_fit
from polynomials import *

DATA_DIR = pathlib.Path(__file__).parent.parent.parent / "data"
TRAINING_DIR = pathlib.Path(__file__).parent / "simulator" / "training-data"
SCORE_DISTRIBUTION = DATA_DIR / "global_training_data_GA_norm_minmax.csv"
SCORE_DISTRIBUTION_MEM = DATA_DIR / "global_training_data_GA_MEM_norm_minmax.csv"
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
FUNCTIONS = [lin]


class Regressor:
    """
    A class to perform a multiple linear regression on a given dataset
    using a set of functions.
    """

    def __init__(self, data_file,data_file_mem, functions):
        """9.003280802672648e-22,
        Initialize the regressor.

        Parameters
        ----------
        data_file : str
            The path to the file containing the data set.
        functions : list
            The list of functions to use for the regression.
        """
        self.functions = functions
        self.data_set = self._read_data_set(data_file)
        self.data_set_mem = self._read_data_set_mem(data_file_mem)
        self.number_of_samples = self.data_set.shape[0]

    def _read_data_set(self, filename):
        """
        Read the data set from a given file.

        Parameters
        ----------
        filename : str
            The path to the file containing the data set.

        Returns
        -------
        array
            The data set.
        """
        data_set = np.genfromtxt(
            filename,
            delimiter=",",
            dtype=[np.double, np.double, np.double, np.double],
            names=["p", "q", "r", "score"],
        )
        return data_set[1:]

    def _read_data_set_mem(self, filename):
        """
        Read the data set from a given file.

        Parameters
        ----------
        filename : str
            The path to the file containing the data set.

        Returns
        -------
        array
            The data set.
        """
        data_set = np.genfromtxt(
            filename,
            delimiter=",",
            dtype=[np.double, np.double, np.double,np.double,np.double,np.double, np.double],
            names=["p", "q", "r","p_mean","q_mean","r_mean", "score"],
        )


        return data_set[1:]

    def _compute_weights(self):
        """
        Compute the weights for the regression.
        """
        sigma = [1.0 / (p * q) for p, q in zip(self.data_set["p"], self.data_set["q"])]
        return sigma

    def _fit_function(self, function):
        """
        Fit a given function to the data set using scipy.optimize.curve_fit.

        Parameters
        ----------
        function : function
            The function to fit.

        Returns
        -------
        tuple
            The optimal parameters and the covariance matrix.
        """
        if function in SERIAL_FUNCTIONS:
            optimal_parameters, optimal_covariance = curve_fit(
            function,
            (self.data_set_mem["p"], self.data_set_mem["q"], self.data_set_mem["r"],self.data_set_mem["p_mean"], self.data_set_mem["q_mean"], self.data_set_mem["r_mean"]),
            self.data_set_mem["score"],
            #method='trf',
            #sigma=self._compute_weights(),
            #absolute_sigma=True,
            )
        else :
            optimal_parameters, optimal_covariance = curve_fit(
                function,
                (self.data_set["p"], self.data_set["q"], self.data_set["r"]),
                self.data_set["score"],
                #method='trf',
                #sigma=self._compute_weights(),
                #absolute_sigma=True,
            )

        return optimal_parameters, optimal_covariance

    def _predict_y(self, function, optimal_parameters):
        """
        Predict the values of y for the data set.

        Parameters
        ----------
        function : function
            The function to fit.
        optimal_parameters : array
            The optimal parameters of the function.

        Returns
        -------
        array
            The predicted values of y.
        """
        if function in SERIAL_FUNCTIONS:
            
            p = lambda x: function(x, *optimal_parameters)
            x_data = zip(self.data_set_mem["p"], self.data_set_mem["q"], self.data_set_mem["r"],self.data_set_mem["p_mean"], self.data_set_mem["q_mean"], self.data_set_mem["r_mean"])
            predicted_y = np.array([p(x) for x in x_data])
        else:
            p = lambda x: function(x, *optimal_parameters)
            x_data = zip(self.data_set["p"], self.data_set["q"], self.data_set["r"])
            predicted_y = np.array([p(x) for x in x_data])

        return predicted_y

    def _compute_mae(self, predicted_y):
        """
        Compute the mean absolute error.

        Parameters
        ----------
        predicted_y : array
            The predicted values of y.

        Returns
        -------
        float
            The mean absolute error of the current regression.
        """
        return np.mean(np.abs(predicted_y - self.data_set["score"]))

    def _generate_report(
        self,
        function,
        optimal_parameters,
        optimal_covariance,
        predicted_y,
        include_covariance,
    ):
        """
        Generate a report of the regression for a given function.

        Parameters
        ----------
        function : function
            A function used for the regression.
        optimal_parameters : array
            The optimal parameters of the function.
        optimal_covariance : array
            The covariance matrix of the function.
        predicted_y : array
            The predicted values of y.
        include_covariance : bool
            Whether to include the covariance matrix in the report.

        Returns
        -------
        dict
            The report of the regression.
        """
        report = {
            "fitted_function": function.__name__,
            "coeficients": optimal_parameters.tolist(),
            "mean_absolute_error": self._compute_mae(predicted_y),
        }

        if include_covariance:
            report["covariance"] = optimal_covariance.tolist()

        return report

    def regression(self, output_file, include_covariance=False):
        """
        Perform the regression on the data set and write the report to a file.

        Parameters
        ----------
        output_file : str
            The path to the file where the report will be written.
        include_covariance : bool, optional
            Whether to include the covariance matrix in the report.
        """
        reports = []
        for function in self.functions:
            optimal_parameters, optimal_covariance = self._fit_function(function)
            predicted_y = self._predict_y(function, optimal_parameters)
            report = self._generate_report(
                function,
                optimal_parameters,
                optimal_covariance,
                predicted_y,
                include_covariance,
            )
            reports.append(report)

        with open(output_file, "w") as f:
            json.dump(reports, f, indent=4)


if __name__ == "__main__":

    
    print("___________SIZE = 3______________")
    funct=[
    [ser_1_1, ser_1_2, ser_1_3],
    [ser_2_1, ser_2_2, ser_2_3],
    [ser_3_1, ser_3_2, ser_3_3],
    [ser_4_1, ser_4_2, ser_4_3],
    [ser_5_1, ser_5_2, ser_5_3],
    [ser_6_1, ser_6_2, ser_6_3],
    [ser_7_1, ser_7_2, ser_7_3],
    [ser_8_1, ser_8_2, ser_8_3],
    [ser_9_1, ser_9_2, ser_9_3],
    [ser_10_1, ser_10_2, ser_10_3]
]
    for i in range (0,10):
        print(f"Performing the regression {i+1}")
        regressor = Regressor(SCORE_DISTRIBUTION, SCORE_DISTRIBUTION_MEM, funct[i])
        report = f"vif_data/vif_ser_norm_{i+1}_report.json"
        regressor.regression(report)
        print("Done!")
        print("Regression report saved to '{}'".format(report))
 