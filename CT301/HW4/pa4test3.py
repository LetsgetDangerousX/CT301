import subprocess
import random
import os
import shutil
import math

#class that holds information about a generated ppm image
class ppm_image:
    def __init__(self,filename: str, rows: int, cols: int, max: int, values: list, corrupted = False, corruption_type = -1):
        self.filename = filename
        self.rows = rows
        self.cols = cols
        self.max = max #value ceiling
        self.values = values #list of all values in the image, excluding header
        self.lowest_val, self.highest_val = self.find_extremes() #lowest and highest color values in image
        if corruption_type != 3:
            self.normalized_values = self.normalize_values() #list of normalized values
        else:
            self.normalized_values = []
        self.corrupted = corrupted
        self.corruption_type = corruption_type
    
    def find_extremes(self):
        low, high = self.max + 1, -1
        for value in self.values:
            if value > high:
                high = value
            if value < low:
                low = value
        return low, high
    
    def normalize_values(self):
        n_vals = []
        for value in self.values:
            normalized = (value - self.lowest_val) * (self.max / (self.highest_val - self.lowest_val))
            n_vals.append(int(math.floor(normalized + 0.500000001)))
        return n_vals

#this function creates a ppm file and returns a ppm_image object that describes it - the ppm_image object will be used to evaluate the outputs of the cpp program
def makeppm(rows, cols, max, filenum = "") -> ppm_image:
    ppmfilename = "Test" + filenum + ".ppm" #change the name as youd like, here im just using Test{filenum}.ppm
    with open(ppmfilename, 'w') as ppmfile:
        ppmfile.write("P3\n"),ppmfile.write(cols + "\n"),ppmfile.write(rows + "\n"),ppmfile.write(max + "\n") #adding header information
        rows, cols, max= int(rows), int(cols), int(max) #parsing ints from string inputs
        vals = []
        for i in range(rows):
            for j in range(cols): #rows*cols*3 values added to file
                r,g,b = random.randint(0, max),random.randint(0, max),random.randint(0, max) #generate random r g b values within [0,max]
                vals.append(r), vals.append(g), vals.append(b)
                r,g,b = str(r), str(g), str(b)
                ppmfile.write(r + " " + g + " " + b + "\n") #write rgb files seperated by space, newline after each pixel
    im = ppm_image(ppmfilename, rows, cols, max, vals)
    return im

def makeppm_corrupted(rows,cols,max, filenum="") -> ppm_image:
    ppmfilename = "corrupted_Test" + filenum + ".ppm"
    corruption_type = random.randint(0, 3)
    with open(ppmfilename, 'w') as ppmfile:
        if corruption_type == 0: #P3 missformat
            ppmfile.write("P1\n"),ppmfile.write(cols + "\n"),ppmfile.write(rows + "\n"),ppmfile.write(max + "\n")
            rows, cols, max= int(rows), int(cols), int(max)
            vals = []
            for i in range(rows):
                for j in range(cols):
                    r,g,b = random.randint(0, max),random.randint(0, max),random.randint(0, max)
                    vals.append(r), vals.append(g), vals.append(b)
                    r,g,b = str(r), str(g), str(b)
                    ppmfile.write(r + " " + g + " " + b + "\n")
        if corruption_type == 1: #mismatched dimension specifications (too little or too many values)
            ppmfile.write("P3\n"),ppmfile.write(cols + "\n"),ppmfile.write(rows + "\n"),ppmfile.write(max + "\n")
            combination_decider = random.randint(1,4)
            if combination_decider == 1:
                rows, cols, max= int(rows) - random.randint(1,  10), int(cols) + random.randint(1,  11), int(max)
            elif combination_decider == 2:
                rows, cols, max= int(rows) + random.randint(1,  9), int(cols) + random.randint(1,  5), int(max)
            elif combination_decider == 3:
                rows, cols, max= int(rows) - random.randint(1,  6), int(cols) - random.randint(1,  18), int(max)
            else:
                rows, cols, max= int(rows) + random.randint(1,  3), int(cols) - random.randint(1,  7), int(max)
            vals = []
            for i in range(rows):
                for j in range(cols):
                    r,g,b = random.randint(0, max),random.randint(0, max),random.randint(0, max)
                    vals.append(r), vals.append(g), vals.append(b)
                    r,g,b = str(r), str(g), str(b)
                    ppmfile.write(r + " " + g + " " + b + "\n")
        if corruption_type == 2: #max value exceeded
            ppmfile.write("P3\n"),ppmfile.write(cols + "\n"),ppmfile.write(rows + "\n"),ppmfile.write(max + "\n")
            rows, cols, max= int(rows), int(cols), int(max)
            vals = []
            for i in range(rows):
                for j in range(cols):
                    r,g,b = random.randint(max+1, max+2),random.randint(0, max),random.randint(0, max)
                    vals.append(r), vals.append(g), vals.append(b)
                    r,g,b = str(r), str(g), str(b)
                    ppmfile.write(r + " " + g + " " + b + "\n")
        if corruption_type == 3: #high rgb value = low rgb value
            ppmfile.write("P3\n"),ppmfile.write(cols + "\n"),ppmfile.write(rows + "\n"),ppmfile.write(max + "\n")
            rows, cols, max= int(rows), int(cols), int(max)
            vals = []
            for i in range(rows):
                for j in range(cols):
                    r,g,b = 1,1,1
                    vals.append(r), vals.append(g), vals.append(b)
                    r,g,b = str(r), str(g), str(b)
                    ppmfile.write(r + " " + g + " " + b + "\n")
    im = ppm_image(ppmfilename, rows, cols, max, vals, True, corruption_type)
    return im

#create folders for the ppms to go into so you dont flood your main directory with files
def create_io_storage():
    os.makedirs("HW4_inputs", exist_ok=True)
    os.makedirs("HW4_outputs", exist_ok=True)

def run_test(i):
    i = str(i)
    decider = random.randint(1,10)
    rows = str(random.randint(2,6))
    cols = str(random.randint(2,6))
    max = "255"
    image = None
    if decider < 4:
        image = makeppm_corrupted(rows,cols,max,i)
        corrupted_test(image)
    else:
        image = makeppm(rows,cols,max,i)
        normal_test(image)
    print("----------")


def corrupted_test(image):
    ppm_name = image.filename
    normalized_ppm_name = "normalized_" + ppm_name
    result = subprocess.run(["./HW4", ppm_name, normalized_ppm_name], stderr = subprocess.PIPE, universal_newlines=True)
    returncode = result.returncode

    shutil.move(ppm_name, "HW4_inputs/" + ppm_name)
    try:
        shutil.move(normalized_ppm_name, "HW4_outputs/" + normalized_ppm_name)
        if image.corruption_type == 3:
            raise FileNotFoundError("yuh")
        print("ERROR 🚫: your program should have not produced an output file for input " + ppm_name + " with corruption code " + str(image.corruption_type))
        print("Your program's stderr -> '" + result.stderr + "'")
        return
    except FileNotFoundError as e:
        if returncode!= 255:
            print("ERROR 🚫: your program did not return -1 for input " + ppm_name + " with corruption code " + str(image.corruption_type))
            print("Your program's stderr -> '" + result.stderr + "'")
            return
        print("SUCCESS ✅: for input " + ppm_name + " with corruption code " + str(image.corruption_type))


def normal_test(image):
    ppm_name = image.filename
    normalized_ppm_name = "normalized_" + ppm_name
    returncode = subprocess.run(["./HW4", ppm_name, normalized_ppm_name]).returncode
    shutil.move(ppm_name, "HW4_inputs/" + ppm_name)
    try:
        shutil.move(normalized_ppm_name, "HW4_outputs/" + normalized_ppm_name)
    except FileNotFoundError as e:
        print("ERROR 🚫: expected output of " + normalized_ppm_name + " given input " + ppm_name + " not found.")
        return
    output_values = parse_output("HW4_outputs/" + normalized_ppm_name)[4:]
    if len(output_values) != len(image.normalized_values):
        print("ERROR 🚫: number of values in normalized image is not correct for input " + ppm_name)
        return
    for i in range(len(output_values)):
        produced = int(output_values[i])
        expected = image.normalized_values[i]
        if produced != expected:
            print("ERROR 🚫:  value at index " + str(i) + ", " + str(produced) + ", does not match expected value of " + str(expected) + " for input file " + ppm_name)
            return

    print("SUCCESS ✅: for input " + ppm_name)
def parse_output(filename):
    vals = []
    with open(filename, 'r') as output:
        items_read = 0
        for line in output:
            line = str(line.strip())
            line_array = line.split(' ')
            for item in line_array:
                vals.append(item)
    return vals



def main():
    makeresult = subprocess.run(["make", "HW4"])
    if makeresult.returncode != 0:
        print("ERROR: 'make HW4' failed.")
        return
    num_tests = int(input("How many tests do you want to run?: "))
    create_io_storage()
    for i in range(1,num_tests+1):
        run_test(i)

if __name__ == "__main__":
    main()