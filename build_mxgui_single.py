import re

includes_files = ["stb_textedit.h","mxgui_font_awesome.h", "mxgui_icons_font_awesome7.hpp" , "mxgui_notosans.hpp","stb_truetype.h", "mxgui_raylib.hpp", "mxgui_sfml.hpp"]
mxgui_single = ""
mxgui = open("mxgui.hpp")
count = 0

def get_include_filter_range(line: str):
    # Extracts (start, end) from a comment include_filter(n,n) on the line.
    match = re.search(r'include_filter\(\s*(\d+)\s*,\s*(\d+)\s*\)', line)
    if match:
        return int(match.group(1)), int(match.group(2))
    return None


def contain_text(line : str):
    global count
    for include_file in includes_files:
        if ("#include" in line) and (include_file in line) and ("include_filter" in line):
            return include_file_filter(include_file, line)
        elif ("#include" in line) and (include_file in line):
            return include_file_all(include_file)
    count += 1
    return line

def include_file_all(file : str):
    global count
    print(f"Replaced mxgui line {count + 1} to file {file}")
    text = f"//////////////////////////////////////////// start {file}////////////////////////////////////////////\n\n"
    with open(file) as new_file:
        for line_header in new_file:
            text += line_header
            count += 1
    text += f"\n\n///////////////////////////////////////////// end {file}////////////////////////////////////////////\n\n"
    count += 6
    return text

def include_file_filter(file : str, line : str):
    global count
    start, end = get_include_filter_range(line)
    print(f"Replaced mxgui line {count + 1} to file {file} line ({start} to {end})")
    text = f"//////////////////////////////////////////// start {file}////////////////////////////////////////////\n\n"
    with open(file) as new_file:
        for i, line_header in enumerate(new_file, start=1):
            if start <= i <= end:
                text += line_header
                count = count + 1 
    text += f"\n\n//////////////////////////////////////////// end {file}////////////////////////////////////////////\n\n"
    count += 6 
    return text

for line in mxgui:
    mxgui_single += contain_text(line)  


with open("mxgui_single.hpp", "w") as f:
    f.write(mxgui_single)