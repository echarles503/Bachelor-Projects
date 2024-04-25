import os

filename = "500MB_file.txt"
file_size = 1024 * 1024 * 500 # 500 MB

with open(filename, 'wb') as f:
    f.write(os.urandom(file_size))