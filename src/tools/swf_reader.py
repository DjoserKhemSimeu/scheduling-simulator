import re

class ReaderSWF:
    def __init__(self, filename):
        self.filename = filename
        self.number_of_jobs = None
        self.number_of_processors = None
        self.jobs_info = {'p': [], '~p': [], 'q': [], 'r': []}
        self.read_and_extract_data()

    def read_and_extract_data(self):
        with open(self.filename, 'r') as reader:
            jobs_counter = 0
            for line in reader.readlines():
                
                row = re.split(" +", line.lstrip(" "))
                row[-1] = row[-1].rstrip()


                if row[0].startswith(";"):
                    if "MaxProcs:" in row:
                        self.number_of_processors = int(row[row.index("MaxProcs:") + 1])
                    elif "MaxNodes:" in row:
                        self.number_of_processors = int(row[row.index("MaxNodes:") + 1])
                    continue

                # row[4] is q, row[3] is p, row[8] is ~p, and row[1] is r
                if float(row[4]) > 0 and float(row[4]) <= self.number_of_processors and float(row[3]) > 0 and float(row[8]) > 0:
                    jobs_counter += 1
                    self.jobs_info['p'].append(float(row[3]))
                    self.jobs_info['~p'].append(float(row[8]))
                    self.jobs_info['q'].append(float(row[4]))
                    self.jobs_info['r'].append(float(row[1]))
            
            self.number_of_jobs = jobs_counter
    def write_data(self, filename):
        with open(self.filename, 'r') as reader:
            with open(filename, 'w') as writer:
                jobs_counter = 0
                for line in reader.readlines():
                    
                    row = re.split(" +", line.lstrip(" "))
                    row[-1] = row[-1].rstrip()
                    if not(row[0].startswith(";")) and float(row[4]) > 0 and float(row[4]) <= self.number_of_processors and float(row[3]) > 0 and float(row[8]) > 0:
                        
                        row[3]=str(self.jobs_info['p'][jobs_counter])
                        row[8]=str(self.jobs_info['~p'][jobs_counter])
                        row[4]=str(self.jobs_info['q'][jobs_counter])
                        row[1]=str(self.jobs_info['r'][jobs_counter])
                        jobs_counter += 1
                    new_row=[]
                    for x in row :
                        new_row.append(x)
                        new_row.append(" ")
                    new_row[len(new_row)-1]='\n'
                    writer.writelines(new_row)
