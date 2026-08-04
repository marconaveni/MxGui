files_includes = ["mxgui_font_awesome.h", "mxgui_icons_font_awesome7.hpp" , "mxgui_notosans.hpp","stb_truetype.h"]
mxgui_single = ""
mxgui = open("mxgui.hpp")


def contain_text(string : str):
    for substring in files_includes:
        if "#include" in string and substring in string:
            return include_file(substring)
    return string

def include_file(file):
    new_file = open(file)
    text = f"\n\n// =============={file}================\n\n"
    text += new_file.read()
    text += f"\n\n// =============end {file}=============\n\n"
    return text

for line in mxgui:
    mxgui_single += contain_text(line) 


with open("mxgui_single.hpp", "w") as f:
    f.write(mxgui_single)