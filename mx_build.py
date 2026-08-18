import re
import os
import sys


def show_help():
    print(''' 
Usage: mx_build [options] file...
Options:
--help or -h                      Display this information. 
--name or -n    <filename>        Change the default name of the saved file.

mx_build.py aims to create a single header file containing all mxgui files. This 
script works in conjunction with mxgui.hpp; it reads the file line by line, and 
if it encounters a comment such as "// @mx_build some_function(parameters)", the 
script takes action.

Functions available in this script @mx_build:

replace_file(pathfile) - Replaces the line with the file specified in the parameter.
replace_file_filter("pathfile", start_line, end_line) - Replaces the line with the file specified in the parameter, allowing filtering by start and end lines.
replace_with_text("textline") - Replaces the current line with the "textline" parameter. NOTE: the parameter does not accept \\n.
ignore_line() - Omits the current line.
''')

total_lines_files = 0

def add_line(line: str):
    global total_lines_files
    total_lines_files += 1
    return line.replace("\n", "") + f"\n"  
    # return line.replace("\n", "") + f" // {total_lines_files} new break line>\n"  # for debug

def get_replace_file(line: str):
    """ Extracts (file) from a comment replace_file(f) on the line. """
    match = re.search(r'replace_file\(\s*"([0-9a-zA-Z._]+)"\s*\)', line)
    if match:
        return str(match.group(1))
    return None

def get_replace_file_filter_range(line: str):
    """ Extracts (file, start, end) from a comment replace_file_filter("f",n,n) on the line. """
    match = re.search(r'replace_file_filter\(\s*"([0-9a-zA-Z._]+)"\s*,\s*(\d+)\s*,\s*(\d+)\s*\)', line)
    if match:
        return str(match.group(1)), int(match.group(2)), int(match.group(3))
    return None

def get_replace_with_text(line: str):
    """ Extracts (text) from a comment replace_with_text(f) on the line. """
    match = re.search(r'replace_with_text\(\s*"([^\n]+)"\s*\)', line)
    if match:
        return str(match.group(1))
    return None

def add_line_text(line : str):
    if not ("@mx_build" in line):
        return add_line(line)
    elif (re.search(r'ignore_line\s*\(', line)):
        return ""
    elif (re.search(r'replace_file\s*\(', line)):
        return replace_file(line)
    elif (re.search(r'replace_file_filter\s*\(', line)):
        return replace_file_filter(line)
    else:
        return get_replace_with_text(line)

def start_text(file: str):
    text = add_line(f"//////////////////////////////////////////// (START FILE {file}) ////////////////////////////////////////////")
    text += add_line("")
    return text

def end_text(file: str):
    text = add_line(f"///////////////////////////////////////////// (END FILE {file}) ////////////////////////////////////////////")
    text += add_line("")
    return text

def replace_file(line : str):
    global total_lines_files
    file = get_replace_file(line)
    print(f"Replaced mxgui line {total_lines_files + 1} to file {file}")
    text = start_text(file)
    with open(file) as new_file:
        for line_file in new_file:
            text += add_line(line_file)
    text += end_text(file)
    return text

def replace_file_filter(line : str):
    global total_lines_files
    file, start, end = get_replace_file_filter_range(line)
    print(f"Replaced mxgui line {total_lines_files + 1} to file {file} line ({start} to {end})")
    text = start_text(file)
    with open(file) as new_file:
        for i, line_file in enumerate(new_file, start=1):
            if start <= i <= end:
                text += add_line(line_file)
    text += end_text(file)
    return text


def main():

    mxgui_single_text = ""
    total_lines_writed = 0
    total_lines_commented_writed = 0
    file_name = "mxgui.hpp"
    gen_file_name = "mxgui_single.hpp"

    if len(sys.argv) > 1:
        if sys.argv[1] == "-h" or sys.argv[1] == "--help": 
            show_help()
            sys.exit(0)
        elif (len(sys.argv) > 2) and (sys.argv[1] == "-n" or sys.argv[1] == "--name"): 
            gen_file_name = sys.argv[2]


    print(f"generating {gen_file_name}...")

    # Check if the file exists
    if not os.path.exists(file_name):
        print(f"Check if {file_name} is in the same directory as mx_build.py.")
        sys.exit(0)  # Early exit


    with open(file_name, "r") as mxgui_file:  
        for line in mxgui_file:
            if line.strip() != "" and not line.strip().startswith("//"):
                total_lines_writed += 1
            elif line.strip().startswith("//"):
                total_lines_commented_writed += 1
            mxgui_single_text += add_line_text(line)  


    with open(gen_file_name, "w") as mxgui_single_file:
        mxgui_single_file.write(mxgui_single_text)

    print(f"total code lines writed {total_lines_writed}")
    print(f"total comments lines writed {total_lines_commented_writed}")

if __name__ == "__main__":
    main()





    