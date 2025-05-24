import subprocess
import random
import os
import shutil
import math

# C++-style round that always rounds .5 **away from zero**
def cpp_round(x):
    return int(x + 0.5) if x >= 0 else int(x - 0.5)

class ppm_image:
    def __init__(self, filename: str, rows: int, cols: int, maxval: int, values: list, corrupted=False, corruption_type=-1):
        self.filename = filename
        self.rows = rows
        self.cols = cols
        self.maxval = maxval
        self.values = values
        self.corrupted = corrupted
        self.corruption_type = corruption_type
        if not corrupted and corruption_type != 3:
            self.low, self.high = self.find_extremes()
            self.normalized_values = self.normalize_values()
        else:
            self.normalized_values = []

    def find_extremes(self):
        return min(self.values), max(self.values)

    def normalize_values(self):
        if self.high == self.low:
            return None  # No contrast
        return [
            max(0, min(255, cpp_round((val - self.low) * 255.0 / (self.high - self.low))))
            for val in self.values
        ]

   

def create_directories():
    os.makedirs("HW5_inputs", exist_ok=True)
    os.makedirs("HW5_outputs", exist_ok=True)

def makeppm(rows, cols, maxval, filenum="") -> ppm_image:
    filename = f"Test{filenum}.ppm"
    vals = []
    with open(filename, 'w') as f:
        f.write("P3\n")
        f.write(f"{cols} {rows}\n{maxval}\n")
        for _ in range(rows * cols):
            rgb = [random.randint(0, maxval) for _ in range(3)]
            vals.extend(rgb)
            f.write(" ".join(map(str, rgb)) + "\n")
    return ppm_image(filename, rows, cols, maxval, vals)

def makeppm_corrupt(rows, cols, maxval, filenum="") -> ppm_image:
    filename = f"corruptTest{filenum}.ppm"
    corruption_type = random.randint(0, 3)
    vals = []
    with open(filename, 'w') as f:
        if corruption_type == 0:
            f.write("P1\n")
        else:
            f.write("P3\n")
        if corruption_type == 1:
            f.write(f"{cols + 3} {rows + 2}\n{maxval}\n")
        else:
            f.write(f"{cols} {rows}\n{maxval}\n")
        for _ in range(rows * cols):
            if corruption_type == 2 or corruption_type == 3:
                rgb = [maxval + 300, -20, 500]  # Obviously invalid values
            else:
                rgb = [1, 1, 1]
            vals.extend(rgb)
            f.write(" ".join(map(str, rgb)) + "\n")
    return ppm_image(filename, rows, cols, maxval, vals, True, corruption_type)

def parse_output_file(filepath):
    values = []
    with open(filepath) as f:
        lines = f.readlines()

    idx = 0
    while lines[idx].strip().startswith("P") or lines[idx].strip().startswith("#"):
        idx += 1

    # Safely extract width and height
    dimensions = list(map(int, lines[idx].strip().split()))
    if len(dimensions) < 2:
        raise ValueError("Header line missing width and height")
    width, height = dimensions[0], dimensions[1]
    idx += 1

    while lines[idx].strip().startswith("#"):
        idx += 1

    # Safely read maxVal
    maxval_line = lines[idx].strip().split()
    if not maxval_line:
        raise ValueError("Missing maxval line")
    maxval = int(maxval_line[0])
    idx += 1

    # Remaining pixel values
    pixel_data = " ".join(lines[idx:]).split()
    values = list(map(int, pixel_data))
    return values, width, height



def run_valid(image, testnum):
    infile = image.filename
    outfile = f"normalized_{infile}"
    result = subprocess.run(["./HW5", infile, outfile])
    shutil.move(infile, f"HW5_inputs/{infile}")
    if result.returncode != 0:
        print(f"🚫 ERROR: Program returned {result.returncode} on valid input {infile}")
        return
    try:
        shutil.move(outfile, f"HW5_outputs/{outfile}")
        out_vals = parse_output_file(f"HW5_outputs/{outfile}")
        out_vals, w, h = parse_output_file(f"HW5_outputs/{outfile}")
        if len(out_vals) != len(image.normalized_values):
            print(f"🚫 ERROR: Output pixel count mismatch in test {testnum}")
            return
        for i in range(len(image.normalized_values)):
            if out_vals[i] != image.normalized_values[i]:
                print(f"🚫 ERROR: Pixel {i}: expected {image.normalized_values[i]}, got {out_vals[i]}")
                return

        print(f"✅ SUCCESS: Test {testnum} passed.")
    except FileNotFoundError:
        print(f"🚫 ERROR: Output file {outfile} missing.")

def run_corrupt(image, testnum):
    infile = image.filename
    outfile = f"normalized_{infile}"
    result = subprocess.run(["./HW5", infile, outfile], stderr=subprocess.PIPE)
    shutil.move(infile, f"HW5_inputs/{infile}")
    try:
        shutil.move(outfile, f"HW5_outputs/{outfile}")
        print(f"🚫 ERROR: Output file was created for corrupt test {testnum} (type {image.corruption_type})")
    except FileNotFoundError:
        if result.returncode != 255 and result.returncode != -1:
            print(f"🚫 ERROR: Wrong return code {result.returncode} for corrupt test {testnum}")
        else:
            print(f"✅ SUCCESS: Corrupt test {testnum} correctly rejected (type {image.corruption_type})")

def run_test(testnum):
    rows, cols, maxval = random.randint(2, 6), random.randint(2, 6), 255
    is_corrupt = random.randint(1, 4) == 1
    if is_corrupt:
        img = makeppm_corrupt(rows, cols, maxval, str(testnum))
        run_corrupt(img, testnum)
    else:
        img = makeppm(rows, cols, maxval, str(testnum))
        run_valid(img, testnum)

def main():
    subprocess.run(["make", "HW5"])
    create_directories()
    num = int(input("How many HW5 tests? "))
    for i in range(1, num + 1):
        run_test(i)

if __name__ == "__main__":
    main()
