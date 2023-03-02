""" Main test module """
import platform

import numpy as np

if __name__ == "__main__":
    print(f"Running on a {platform.system()} {platform.release()} system!")

    arr = np.array([[1, 2, 3],[4, 5, 6]])

    print(arr ** 2)
