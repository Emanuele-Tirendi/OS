import os
import sys

if len(sys.argv) < 2:
    print("provide -c for compiling and -r for running and both for both")
    sys.exit(1)

if len(sys.argv) > 3:
    print("too many arguments")

os.chdir("client")

# Compile C++ code
if "-c" in sys.argv:
    compile_command = "gcc client.c ../shared/constants.h ../shared/constants.c ../shared/time.h ../shared/time.c ../shared/log.h ../shared/log.c IO.h IO.c -o client"
    compile_status = os.system(compile_command)
    print("compiled client")

if "-r" in sys.argv:
    print("start running client")
    run_command = "./client"
    run_status = os.system(run_command)