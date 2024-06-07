import os
import sys
import shutil

os.chdir("client")

# Compile C++ code

if ("-c1" in sys.argv and "-c2" in sys.argv or
"-c1" in sys.argv and "-r2" in sys.argv or
"-r1" in sys.argv and "-c2" in sys.argv or
"-r1" in sys.argv and "-r2" in sys.argv):
    print("cannot process both first and second client")
    exit()

if "-c1" in sys.argv:
    dest = ""  

if "-c2" in sys.argv:
    if not os.path.exists("../sec_client"):
        os.mkdir("../sec_client")
    dest = "../sec_client/"

if "-c1" in sys.argv or "-c2" in sys.argv:
    compile_command = "gcc client.c ../shared/constants.h ../shared/constants.c ../shared/time.h ../shared/time.c ../shared/log.h ../shared/log.c IO.h IO.c html.h html.c ../shared/html.h ../shared/html.c -o" + dest + "client"
    compile_status = os.system(compile_command)
    print("compiled client")

ipaddress = ""

if "-r2" in sys.argv:
    os.chdir("../sec_client")
    if os.path.exists("index.html"):
        os.remove("index.html")
        print("index.html removed")
    print("start running second client")
    run_command = "./client " + ipaddress
    run_status = os.system(run_command)

if "-r1" in sys.argv:
    if os.path.exists("index.html"):
        os.remove("index.html")
        print("index.html removed")
    print("start running first client")
    run_command = "./client " + ipaddress
    run_status = os.system(run_command)