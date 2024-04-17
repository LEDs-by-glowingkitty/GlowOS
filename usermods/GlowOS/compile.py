import subprocess
import shutil
import os
import sys
import pkg_resources

def check_platformio():
    try:
        dist = pkg_resources.get_distribution('platformio')
        print(f"'platformio' is installed: {dist}")
    except pkg_resources.DistributionNotFound:
        print("'platformio' is not installed, installing now...")
        result = subprocess.run([sys.executable, "-m", "pip", "install", "platformio"])
        if result.returncode != 0:
            print("Failed to install 'platformio'. Please install it manually.")
            sys.exit(1)

def remove_build_folder(build_folder):
    if os.path.exists(build_folder):
        shutil.rmtree(build_folder)

def compile_environment(env_name, version_name):
    log_filename = f'log_{env_name}_{version_name}.txt'
    with open(log_filename, 'w') as f:
        result = subprocess.run(["platformio", "run", "-e", env_name], stdout=f, stderr=subprocess.STDOUT)
    if result.returncode == 0:
        print(f"Compilation of environment '{env_name}' completed successfully.")
        # Delete the log file if the compilation is successful
        os.remove(log_filename)
        return True
    else:
        print(f"Compilation of environment '{env_name}' failed.")
        return False

def merge_bin_files(env_name, version_name):
    home_dir = os.path.expanduser("~")
    bootloader_bin_path = os.path.join(home_dir, ".platformio/packages/framework-arduinoespressif32@src-e9b1fbd6563a55e19ddae15e1fc09589/tools/sdk/bin/bootloader_dout_40m.bin")
    partitions_bin_path = os.path.join(home_dir, "Library/CloudStorage/Dropbox/Software/GlowOS/.pio/build", env_name, "partitions.bin")
    boot_bin_path = os.path.join(home_dir, ".platformio/packages/framework-arduinoespressif32@src-e9b1fbd6563a55e19ddae15e1fc09589/tools/partitions/boot_app0.bin")
    firmware_bin_path = os.path.join(".pio/build", env_name, "firmware.bin")

    merge_bin_command = [
        "venv/bin/esptool.py", "--chip", "esp32", "merge_bin",
        "-o", f"{env_name}-{version_name}.bin",
        "--flash_mode", "dio",
        "--flash_freq", "40m",
        "--flash_size", "4MB",
        "0x1000", bootloader_bin_path,
        "0x8000", partitions_bin_path,
        "0xe000", boot_bin_path,
        "0x10000", firmware_bin_path
    ]
    result = subprocess.run(merge_bin_command)
    return result.returncode == 0

def move_merged_firmware(env_name, version_name):
    source_file = f'{env_name}-{version_name}.bin'
    script_dir = os.path.dirname(os.path.realpath(__file__))
    script_dir = script_dir.replace("GlowOS/usermods/GlowOS", "")
    dst_path = os.path.join(script_dir, f"GlowOS-WebInstaller/manifests/{env_name}_glowos_{version_name}.bin")
    shutil.move(source_file, dst_path)
    print(f"Moved merged firmware '{env_name}'")

def compile_and_merge_all(environments, version_name):
    print(f"Compiling and merging binary files for version '{version_name}'")
    for env in environments:
        if compile_environment(env, version_name):
            if merge_bin_files(env, version_name):
                move_merged_firmware(env, version_name)
            else:
                print(f"Merging binary files for environment '{env}' failed.")

def main():
    if len(sys.argv) != 2:
        print("Usage: python compile.py <version_name>")
        sys.exit(1)

    check_platformio()

    version_name = sys.argv[1]

    # Ensure version_name starts with 'v' and replace '.' with '_'
    if not version_name.startswith('v'):
        version_name = 'v' + version_name
    version_name = version_name.replace('.', '_')

    build_folder = ".pio/build/"
    # remove_build_folder(build_folder)

    environments = [
        "glowcore_v2_0",
        "glowcore_v3_0",
        "magiqwand_v1_0_RC2",
        # "magiqwand_v1_0_RC1",
        # "glowband_v1_0_beta1",
        "glowsign_v1_0_RC2",
        "glowtower_v1_0",
        "glowlight_v1_0",
        "glowlight_glowtower_v2_0",
        "glowlight_glowtower_v3_0"
    ]
    compile_and_merge_all(environments, version_name)

    remove_build_folder(build_folder)

if __name__ == "__main__":
    main()