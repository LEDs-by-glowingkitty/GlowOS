import subprocess
import shutil
import os

build_folder = ".pio/build/"
if os.path.exists(build_folder):
    shutil.rmtree(build_folder)

def compile_environment(env_name):
    result = subprocess.run(["platformio", "run", "-e", env_name], stdout=subprocess.PIPE)
    if result.returncode == 0:
        print(f"Compilation of environment '{env_name}' completed successfully.")
        # The path to the generated binary file
        source_file = f'.pio/build/{env_name}/firmware.bin'

        # The path to the directory where you want to move the file
        script_dir = os.path.dirname(os.path.realpath(__file__))
        script_dir = script_dir.replace("GlowOS/usermods/GlowOS", "")
        dst_path = os.path.join(script_dir, f"GlowOS-WebInstaller/bin-v2/{env_name}/{env_name}.bin")

        # Move the file to the destination folder
        shutil.move(source_file, dst_path)
        print(f"Moved '{env_name}'")
    else:
        print(f"Compilation of environment '{env_name}' failed.")

environments = [
    "glowcore_v2_0",
    "magiqwand_v1_0_RC2",
    "magiqwand_v1_0_RC1",
    "glowcore_mini_v2_0",
    "glowcore_mini_v3_0",
    "glowband_v1_0_beta1",
    "glowsign_v1_0_RC2",
    "glowtower_v1_0",
    "glowlight_v1_0"
    ]
for env in environments:
    compile_environment(env)

shutil.rmtree(build_folder)