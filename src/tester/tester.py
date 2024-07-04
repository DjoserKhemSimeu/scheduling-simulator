import os
import sys
import pathlib
import subprocess
import pandas as pd
from sklearn.preprocessing import StandardScaler,MinMaxScaler

# Add the src directory to the path so we can import the tools
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", "src")))
from tools.swf_reader import *

# Predefined paths (enable the script to be run from anywhere in the project)
EXPERIMENTS_DIR = pathlib.Path(__file__).parent
DATA_DIR = pathlib.Path(__file__).parent.parent.parent / "data"

SECONDS_IN_A_DAY = 86400
SIM_NUM_DAYS = 15
STATE_SIZE = 16

traces = {
    "ANL": [DATA_DIR / "workloads" / "ANL-Intrepid-2009-1.swf", DATA_DIR / "applications" / "deployment_anl.xml", 15],
    "CTC-SP2": [
        DATA_DIR / "workloads" / "CTC-SP2-1996-3.1-cln.swf",
        DATA_DIR / "applications" / "deployment_ctcsp2.xml",
        22,
    ],
    "HPC2N": [
        DATA_DIR / "workloads" / "HPC2N-2002-2.2-cln.swf",
        DATA_DIR / "applications" / "deployment_hpc2n.xml",
        83,
    ],
    "SDSC-BLUE": [
        DATA_DIR / "workloads" / "SDSC-BLUE-2000-4.2-cln.swf",
        DATA_DIR / "applications" / "deployment_blue.xml",
        64,
    ],
    "SDSC-SP2": [
        DATA_DIR / "workloads" / "SDSC-SP2-1998-4.2-cln.swf",
        DATA_DIR / "applications" / "deployment_sdscsp2.xml",
        47,
    ],
    "CURIE": [
        DATA_DIR / "workloads" / "CEA-Curie-2011-2.1-cln.swf",
        DATA_DIR / "applications" / "deployment_curie.xml",
        15,
    ],
    "LUBLIN 256": [
        {
            "actual": DATA_DIR / "workloads" / "lublin_256.swf",
            "estimated": DATA_DIR / "workloads" / "lublin_256_est.swf",
        },
        DATA_DIR / "applications" / "deployment_day.xml",
        50,
    ],
    "LUBLIN 1024": [
        {
            "actual": DATA_DIR / "workloads" / "lublin_1024.swf",
            "estimated": DATA_DIR / "workloads" / "lublin_1024_est.swf",
        },
        DATA_DIR / "applications" / "deployment_day_1024.xml",
        50,
    ],
    "MUSTANG": [
        DATA_DIR / "workloads" / "mustang_release_v1.0beta.swf",
        DATA_DIR / "applications" / "deployment_mustang.xml",
        50,
    ],
    "KIT-FH2": [DATA_DIR / "workloads" / "KIT-FH2-2016-1.swf", DATA_DIR / "applications" / "deployment_kitfh2.xml", 50],
}

simulators = {
    "ACTUAL": "sched-simulator-runtime",
    "ESTIMATED": "sched-simulator-estimate-backfilling",
    "BACKFILLING": "sched-simulator-estimate-backfilling",
}

policies_flags = {
    "FCFS": "",
    "WFP3": "-wfp3",
    "UNICEF": "-unicef",
    "SPT": "-spt",
    "SAF": "-saf",
    "F2": "-f2",
    "LIN": "-lin",
    "QDR": "-qdr",
    "CUB": "-cub",
    "QUA": "-qua",
    "QUI": "-qui",
    "SEX": "-sex",
    "NEW_3_1": "-new_3_1",
    "NEW_3_2": "-new_3_2",
    "NEW_3_3": "-new_3_3",
    "NEW_3_4": "-new_3_4",
    "NEW_3_5": "-new_3_5",
    "NEW_3_6": "-new_3_6",
    "NEW_3_7": "-new_3_7",
    "NEW_3_8": "-new_3_8",
    "NEW_3_9": "-new_3_9",
    "NEW_3_10": "-new_3_10",
    "MEM1": "-mem1",
    "MEM2": "-mem2",
    "MEM3": "-mem3",
    "MEM4": "-mem4",
    "S3_V1_D3":"-s3_v1_d3",
    "S3_V1_D4":"-s3_v1_d4",
    "S3_V2_D1":"-s3_v2_d1",
    "S3_V2_D2":"-s3_v2_d2",
    "S3_V2_D3":"-s3_v2_d3",
    "S3_V2_D4":"-s3_v2_d4",
    "S3_V3_D1":"-s3_v3_d1",
    "S3_V3_D2":"-s3_v3_d2",
    "S3_V3_D3":"-s3_v3_d3",
    "S3_V3_D4":"-s3_v3_d4",
    "S3_V4_D1":"-s3_v4_d1",
    "S3_V4_D2":"-s3_v4_d2",
    "S3_V4_D3":"-s3_v4_d3",
    "S3_V4_D4":"-s3_v4_d4",
    "S3_V5_D1":"-s3_v5_d1",
    "S3_V5_D2":"-s3_v5_d2",
    "S3_V5_D3":"-s3_v5_d3",
    "S3_V5_D4":"-s3_v5_d4",
    "S3_V6_D1":"-s3_v6_d1",
    "S3_V6_D2":"-s3_v6_d2",
    "S3_V6_D3":"-s3_v6_d3",
    "S3_V7_D1":"-s3_v7_d1",
    "S3_V7_D2":"-s3_v7_d2",
    "S3_V7_D3":"-s3_v7_d3",
    "S3_V7_D4":"-s3_v7_d4",
    "S3_V8_D1":"-s3_v8_d1",
    "S3_V8_D2":"-s3_v8_d2",
    "S3_V8_D3":"-s3_v8_d3",
    "S3_V8_D4":"-s3_v8_d4",
    "S3_V9_D1":"-s3_v9_d1",
    "S3_V9_D2":"-s3_v9_d2",
    "S3_V9_D3":"-s3_v9_d3",
    "S3_V9_D4":"-s3_v9_d4",
    "S3_V10_D1":"-s3_v10_d1",
    "S3_V10_D2":"-s3_v10_d2",
    "S3_V10_D3":"-s3_v10_d3",
    "S3_V10_D4":"-s3_v10_d4",
    "S4_V7_D1":"-s4_v7_d1",
    "S4_V4_D3":"-s4_v4_d3",
    "S4_V9_D2":"-s4_v9_d2",
    "S4_V3_D1":"-s4_v3_d1",
    "S4_V9_D4":"-s4_v9_d4",
    "SER_1_1":"-ser_1_1",
    "SER_1_2":"-ser_1_2",
    "SER_1_3":"-ser_1_3",
    "SER_2_1":"-ser_2_1",
    "SER_2_2":"-ser_2_2",
    "SER_2_3":"-ser_2_3",
    "SER_3_1":"-ser_3_1",
    "SER_3_2":"-ser_3_2",
    "SER_3_3":"-ser_3_3",
    "SER_4_1":"-ser_4_1",
    "SER_4_2":"-ser_4_2",
    "SER_4_3":"-ser_4_3",
    "SER_5_1":"-ser_5_1",
    "SER_5_2":"-ser_5_2",
    "SER_5_3":"-ser_5_3",
    "SER_6_1":"-ser_6_1",
    "SER_6_2":"-ser_6_2",
    "SER_6_3":"-ser_6_3",
    "SER_7_1":"-ser_7_1",
    "SER_7_2":"-ser_7_2",
    "SER_7_3":"-ser_7_3",
    "SER_8_1":"-ser_8_1",
    "SER_8_2":"-ser_8_2",
    "SER_8_3":"-ser_8_3",
    "SER_9_1":"-ser_9_1",
    "SER_9_2":"-ser_9_2",
    "SER_9_3":"-ser_9_3",
    "SER_10_1":"-ser_10_1",
    "SER_10_2":"-ser_10_2",
    "SER_10_3":"-ser_10_3",
    "Q3P":"-q3p",
    "Q2P":"-q2p",
}


def workload_experiments(workloads, policies, sim_types,var_zone):
    for workload_trace in workloads:
        for sim_type in sim_types:
            if workload_trace in ["LUBLIN 256", "LUBLIN 1024"]:
                workload_file = (
                    traces[workload_trace][0]["actual"]
                    if sim_type == "ACTUAL"
                    else traces[workload_trace][0]["estimated"]
                )
            else:
                workload_file = traces[workload_trace][0]

            deploy_file = traces[workload_trace][1]
            number_of_experiments = traces[workload_trace][2]

            if sim_type == "BACKFILLING":
                backfilling_flag = "-bf"
            else:
                backfilling_flag = ""

            number_of_policies = len(policies)

            workload_jobs = ReaderSWF(workload_file)
            df=pd.DataFrame.from_dict(workload_jobs.jobs_info)
            scaler1=StandardScaler()
            #scaler2=MinMaxScaler()
            arr2=scaler1.fit_transform(df)
            #arr3=scaler2.fit_transform(df)

            df_norm_std=pd.DataFrame(arr2, columns = ["p","~p","q","r"])
            #df_norm_minmax=pd.DataFrame(arr3, columns = ["p","~p","q","r"])
            #dict_minmax=df_norm_minmax.to_dict('list')
            dict_std=df_norm_std.to_dict('list')
            

            print(
                f"Performing scheduling performance test for the workload trace {workload_trace}.\nConfiguration: {sim_type}"
            )

            # DataFrame to store all slowdowns from all experiments
            slowdowns = pd.DataFrame(columns=policies)

            choose = 0
            for exp in range(number_of_experiments):
                task_file = open(EXPERIMENTS_DIR / "initial-simulation-submit.csv", "w+")
                earliest_submit = workload_jobs.jobs_info["r"][choose]

                number_of_jobs = 0

                state_jobs = {"p": [], "~p": [], "q": [], "r": []}
                for idx in range(STATE_SIZE):
                    
                    state_jobs["p"].append(workload_jobs.jobs_info["p"][choose + idx])
                    state_jobs["q"].append(workload_jobs.jobs_info["q"][choose + idx])
                    state_jobs["r"].append(workload_jobs.jobs_info["r"][choose + idx] - earliest_submit)

                    if sim_type != "ACTUAL":
                        state_jobs["~p"].append(workload_jobs.jobs_info["~p"][choose + idx])
                        task_file.write(
                            f"{state_jobs['p'][idx]},{state_jobs['q'][idx]},{state_jobs['r'][idx]},{state_jobs['~p'][idx]}\n"
                        )
                    else:
                        task_file.write(f"{state_jobs['p'][idx]},{state_jobs['q'][idx]},{state_jobs['r'][idx]}\n")

                    number_of_jobs += 1

                queue_jobs = {"p": [], "~p": [], "q": [], "r": []}
                idx = 0
                while (
                   
                    workload_jobs.jobs_info["r"][choose + STATE_SIZE + idx] - earliest_submit
                ) <= SECONDS_IN_A_DAY * SIM_NUM_DAYS:
                    
                    queue_jobs["p"].append(workload_jobs.jobs_info["p"][STATE_SIZE + choose + idx])
                    queue_jobs["q"].append(workload_jobs.jobs_info["q"][STATE_SIZE + choose + idx])
                    queue_jobs["r"].append(workload_jobs.jobs_info["r"][STATE_SIZE + choose + idx] - earliest_submit)

                    if sim_type != "ACTUAL":
                        queue_jobs["~p"].append(workload_jobs.jobs_info["~p"][STATE_SIZE + choose + idx])
                        task_file.write(
                            f"{queue_jobs['p'][idx]},{queue_jobs['q'][idx]},{queue_jobs['r'][idx]},{queue_jobs['~p'][idx]}\n"
                        )
                    else:
                        task_file.write(f"{queue_jobs['p'][idx]},{queue_jobs['q'][idx]},{queue_jobs['r'][idx]}\n")

                    idx += 1
                    number_of_jobs += 1

                task_file.close()
                choose += STATE_SIZE + idx

                print(f"Performing scheduling experiment {exp + 1}. Number of tasks={number_of_jobs}")

                _buffer = open(EXPERIMENTS_DIR / "plot-temp.dat", "w+")
                for policy in policies:
                    policy_flag = policies_flags[policy]
                    
                    subprocess.run(
                            [
                                f"./{simulators[sim_type]}",
                                DATA_DIR / "platforms" / "plat_day.xml",
                                DATA_DIR / "applications" / deploy_file,
                                backfilling_flag,
                                policy_flag,
                                "-nt",
                                str(number_of_jobs),
                                "-var",
                                str(var_zone),
                                #"-verbose",
                            ],
                            stdout=_buffer,
                            cwd=EXPERIMENTS_DIR,
                        )

                _buffer.close()

                temp_data = pd.DataFrame(columns=policies)

                _buffer = open(EXPERIMENTS_DIR / "plot-temp.dat", "r")
                lines = list(_buffer)
                for i, policy in enumerate(policies):
                    temp_data[policy] = [float(lines[i])]
                _buffer.close()

                slowdowns = pd.concat([slowdowns, temp_data], ignore_index=True)

            slowdowns.to_csv(
                EXPERIMENTS_DIR / f"{workload_trace}_{sim_type}_{number_of_experiments}_{number_of_policies}.csv",
                index=False,
            )


if __name__ == "__main__":
    
    workload_experiments(
        ["LUBLIN 256"],
        ["S3_V3_D3"],
        ["ESTIMATED"],
        4,


    
    )