#!/usr/bin/env python3
import os
import json

def latest_modification_time(directory: str):
    latest_time = 0

    for root, _, files in os.walk(directory):

        for name in files:
            filepath = os.path.join(root, name)
            latest_time = max(latest_time, os.path.getmtime(filepath))

    return latest_time


def cmd(comment: str, s: str):
    print(comment, s)
    os.system(s)

def s(l: list[str]):
    return " ".join(l)

CFLAGS = s([
    "-Wall",
    "-Wextra",
    "-Iext/glfw-3.4/include",
    "-Iext/glad/include",
    "-Iinclude",

    "-g",
    "-c",
])

CXXFLAGS = CFLAGS

LFLAGS = s([
    "-L/usr/local/lib",
    "-lglfw3",
    "-lGL",
    "-lm",
    #"-lX11",
    #"-lpthread",
    #"lXrandr",
    #"-lXi",
    #"-lDL",
])

CC="clang"
CXX="clang++"

def generate_compile_commands():
    compile_commands = []

    for example in os.listdir("examples"):

        dir = os.path.join("examples", example)

        for fname in os.listdir(dir):
            src_file = os.path.join(dir, fname)
            dest_file = os.path.join("tmp", fname)

            if fname.endswith(".c"):
                compile_commands.append({
                    "file": src_file,
                    "directory": root,
                    "command": f"{CC} {CFLAGS} {src_file} -o {dest_file}.o",
                })
            
            if fname.endswith(".cpp"):
                compile_commands.append({
                    "file": src_file,
                    "directory": root,
                    "command": f"{CC} {CFLAGS} {src_file} -o {dest_file}.o",
                })
    
    with open("compile_commands.json", "w") as f:
        f.write(json.dumps(compile_commands))

def compile(fname: str, src: str, dest: str):

    src_file = os.path.join(src, fname)
    dest_file = os.path.join(dest, fname)

    if fname.endswith(".c"):
        cmd(f"[compile {fname}]", f"{CC} {CFLAGS} {src_file} -o {dest_file}.o")
    
    if fname.endswith(".cpp"):
        cmd(f"[compile {fname}]", f"{CXX} {CXXFLAGS} {src_file} -o {dest_file}.o")

root = os.getcwd()

def clean():
    for f in os.listdir("tmp"):
        os.remove(os.path.join("tmp", f))

def link(fname: str):
    os.chdir("tmp")
    cmd(f"[link {fname}]", f"{CXX} {' '.join(os.listdir())} {os.path.join('..', 'include', 'glad.o')} {LFLAGS} -o {os.path.join(root, 'bin', fname)}")
    os.chdir(root)

def main():
    clean()

    for example in os.listdir("examples"):

        dir = os.path.join("examples", example)

        # skip if the files weren't modified
        if os.path.exists(os.path.join("bin", example)) and latest_modification_time(dir) <= os.path.getmtime(os.path.join("bin", example)) and not (latest_modification_time("include") >= os.path.getmtime(os.path.join("bin", example))):
            print(f"[skip {example}]\n")
            continue

        for f in os.listdir(dir):
            compile(f, dir, "tmp")
        
        link(example)
        print()
        clean()
    
    generate_compile_commands()

if __name__ == "__main__":
    main()
