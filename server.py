import os
import sys
from pathlib import Path

if len(sys.argv) < 2:
    print("provide -c for compiling and -r for running and both for both")
    sys.exit(1)

if len(sys.argv) > 4:
    print("too many arguments")

os.chdir("server")

def delete_files_in_directory(directory_path):
    try:
        files = os.listdir(directory_path)
        for file in files:
            file_path = os.path.join(directory_path, file)
            if os.path.isfile(file_path):
                os.remove(file_path)
        print("deleted all log files")
    except OSError:
        print("Error occurred while deleting files.")

if "-l" in sys.argv:
    delete_files_in_directory("./../log/")

if "-c" in sys.argv:
    compile_command = "gcc server.c ../shared/constants.h ../shared/constants.c ../shared/time.h ../shared/time.c ../shared/log.h ../shared/log.c IO.h IO.c html.h html.c ../shared/html.h ../shared/html.c -o server"
    compile_status = os.system(compile_command)
    print("compiled server")

if "-r" in sys.argv:
    print("start running server")
    run_command = "./server"
    run_status = os.system(run_command)