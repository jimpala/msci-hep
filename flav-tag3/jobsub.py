import os
import subprocess
import fnmatch

tim_directory = "/unix/atlas2/scanlon/group.perf-flavtag.mc15_13TeV."\
    "410000.PowhegPythiaEvtGen_s2608_s2183_r7725_r7676."\
    "BTAGNTUP_V47_full_Akt4EMTo/"

james_directory = "./"

root_files_in_directory = [ james_directory + f for f in os.listdir(james_directory)
                            if fnmatch.fnmatch(f, "*.root")]
print root_files_in_directory

while root_files_in_directory:
    # Setup array for files dispatched to given node.
    node_files = []

    try:
        # Fill node_files
        for x in range(100): node_files.append(root_files_in_directory.pop())

    except LookupError:
        pass

    finally:

        # Dispatch the array as a string environment varibale
        os.environ["NODEBATCH"] = str(node_files)

        # Run bash script as a subprocess that will use NODEBATCH