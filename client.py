import os
import sys
import shutil

if len(sys.argv) < 2:
    print("provide -c for compiling and -r for running and both for both")
    sys.exit(1)

if len(sys.argv) > 3:
    print("too many arguments")

os.chdir("client")

# Compile C++ code

if "-cr" in sys.argv and "-cs" in sys.argv:
    print("cannot compile both first and second client")
    exit()

if "-cr" in sys.argv:
    dest = ""  

if "-cs" in sys.argv:
    if not os.path.exists("../sec_client"):
        os.mkdir("../sec_client")
    dest = "../sec_client/"

if "-cs" in sys.argv or "-cr" in sys.argv:
    compile_command = "gcc client.c ../shared/constants.h ../shared/constants.c ../shared/time.h ../shared/time.c ../shared/log.h ../shared/log.c IO.h IO.c html.h html.c ../shared/html.h ../shared/html.c -o" + dest + "client"
    compile_status = os.system(compile_command)
    print("compiled client")

if "-s" in sys.argv and "-r" in sys.argv:
    print("cannot run first and second client")
    exit()

if "-s" in sys.argv:
    os.chdir("../sec_client")
    if os.path.exists("index.html"):
        os.remove("index.html")
        print("index.html removed")
    print("start running second client")
    run_command = "./client"
    run_status = os.system(run_command)

if "-r" in sys.argv:
    if os.path.exists("index.html"):
        os.remove("index.html")
        print("index.html removed")
    print("start running first client")
    run_command = "./client"
    run_status = os.system(run_command)