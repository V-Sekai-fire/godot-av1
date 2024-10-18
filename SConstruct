#!/usr/bin/env python
import os
from glob import glob
from SCons.Script import DefaultEnvironment, Builder, Variables, BoolVariable, Glob, Default, SConscript, ARGUMENTS, Help

# Initialize SCons environment
env = DefaultEnvironment()

# Define the base directory for dav1d relative to this script
base_dir = os.path.abspath('addons/godot-av1/deps/dav1d')

# Define the directory where Meson build should be configured for dav1d
meson_build_dir = os.path.join(base_dir, 'build')

# Ensure the build directory exists and is not a subdirectory of the source
if not os.path.exists(meson_build_dir):
    os.makedirs(meson_build_dir)

# Command to setup Meson build directory for dav1d
meson_setup_cmd = f'meson setup --default-library=static {meson_build_dir} {base_dir}'

# Command to compile using Meson for dav1d
meson_compile_cmd = f'meson compile -C {meson_build_dir}'

def meson_build(target, source, env):
    # Setup Meson build directory for dav1d
    print("Setting up Meson build directory for dav1d...")
    os.system(meson_setup_cmd)

    # Compile using Meson for dav1d
    print("Compiling dav1d with Meson...")
    os.system(meson_compile_cmd)

# Custom SCons builder for Meson
meson_builder = Builder(action=meson_build)
env.Append(BUILDERS={'MesonBuild': meson_builder})

# Define target that uses the custom Meson builder for dav1d
env.MesonBuild(target='build_dav1d', source=[])

# Additional SCons configuration and build logic for godot-av1
SAVED_ARGUMENTS = ARGUMENTS.copy()
ARGUMENTS.pop('intermediate_delete', True)

env = SConscript("addons/godot-av1/deps/godot-cpp/SConstruct")

ARGUMENTS = SAVED_ARGUMENTS

# Custom options and profile flags.
customs = ["custom.py"]
profile = ARGUMENTS.get("profile", "")
if profile:
    if os.path.isfile(profile):
        customs.append(profile)
    elif os.path.isfile(profile + ".py"):
        customs.append(profile + ".py")
opts = Variables(customs, ARGUMENTS)

opts.Add(
    BoolVariable("intermediate_delete", "Enables automatically deleting unassociated intermediate binary files.", True)
)

opts.Update(env)
Help(opts.GenerateHelpText(env))

def GlobRecursive(pattern, node='.'):
    from SCons.Node.FS import Dir  # Import here to avoid NameError
    results = []
    for f in Glob(str(node) + '/*', source=True):
        if type(f) is Dir:
            results += GlobRecursive(pattern, f)
    results += Glob(str(node) + '/' + pattern, source=True)
    return results

sources = GlobRecursive("*.cpp", "addons/godot-av1/src")

if env["intermediate_delete"]:
    def remove_extension(file : str):
        if file.find(".") == -1: return file
        return file[:file.rindex(".")]

    found_one = False
    for obj_file in [file[:-len(".os")] for file in glob("addons/godot-av1/src/*.os", recursive=True)]:
        found = False
        for source_file in sources:
            if remove_extension(str(source_file)) == obj_file:
                found = True
                break
        if not found:
            if not found_one:
                found_one = True
                print("Unassociated intermediate files found...")
            print("Removing "+obj_file+".os")
            os.remove(obj_file+".os")

env.Append(CPPPATH=["addons/godot-av1/src/", "addons/godot-av1/deps/dav1d/build/include/dav1d", "addons/godot-av1/deps/dav1d/include"])
env.Append(LIBPATH=['addons/godot-av1/deps/dav1d/build/src'])

env.Append(LIBS=['dav1d'])

if env["platform"] == "macos":
    library = env.SharedLibrary(
        "addons/godot-av1/bin/godot-av1/libgodot-av1.{}.{}.framework/libgodot-av1.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
else:
    suffix = ".{}.{}.{}".format(env["platform"], env["target"], env["arch"])
    library = env.SharedLibrary(
        "addons/godot-av1/bin/godot-av1/libgodot-av1{}{}".format(suffix, env["SHLIBSUFFIX"]),
        source=sources,
    )

Default(env.Alias('default', 'build_dav1d'))
Default(library)