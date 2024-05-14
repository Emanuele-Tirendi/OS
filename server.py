import os
import sys

if len(sys.argv) < 2:
    print("provide -c for compiling and -r for running and both for both")
    sys.exit(1)

if len(sys.argv) > 3:
    print("too many arguments")

os.chdir("server")

# Compile C++ code
if "-c" in sys.argv:
    compile_command = "gcc server.c ../shared/constants.h ../shared/constants.c ../shared/time.h ../shared/time.c ../shared/log.h ../shared/log.c IO.h IO.c -o server"
    compile_status = os.system(compile_command)
    print("compiled server")

if "-r" in sys.argv:
    print("start running server")
    run_command = "./server"
    run_status = os.system(run_command)