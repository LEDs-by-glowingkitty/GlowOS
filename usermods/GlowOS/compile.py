import subprocess
import shutil
import os
import sys
import pkg_resources
import json


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


def create_manifest_json(env_name, version_name, chipFamily):
    manifest = {
        "name": "GlowOS",
        "version": version_name.replace('_', '.')[1:],  # remove 'v' and replace '_' with '.'
        "home_assistant_domain": "glowos",
        "new_install_prompt_erase": False,
        "builds": [
            {
                "chipFamily": chipFamily,
                "parts": [
                    {"path": f"{env_name}_glowos_{version_name}.bin", "offset": 0}
                ]
            }
        ]
    }
    json_str = json.dumps(manifest, indent=4)
    script_dir = os.path.dirname(os.path.realpath(__file__))
    script_dir = script_dir.replace("GlowOS/usermods/GlowOS", "")
    json_file_path = os.path.join(script_dir, f"GlowOS-WebInstaller/manifests/{env_name}_glowos_{version_name}.json")
    with open(json_file_path, 'w') as json_file:
        json_file.write(json_str)


def update_devices_json(env_name, version_name, name, chipFamily):
    script_dir = os.path.dirname(os.path.realpath(__file__))
    script_dir = script_dir.replace("GlowOS/usermods/GlowOS", "")
    json_file_path = os.path.join(script_dir, "GlowOS-WebInstaller/devices.json")

    with open(json_file_path, 'r') as json_file:
        data = json.load(json_file)

    for device in data['devices']:
        if device['name'] == name:
            new_project = {
                "tag": f"{env_name}_glowos_{version_name}",
                "name": "GlowOS v" + version_name.replace('_', '.')[1:]  # prepend 'GlowOS v', remove 'v' and replace '_' with '.'
            }
            # Check if a project with the same name already exists
            if not any(project['name'] == new_project['name'] for project in device['projects']):
                device['projects'].append(new_project)
                device['projects'].sort(key=lambda x: x['name'], reverse=True)
            break
    else:
        new_device = {
            "tag": env_name,
            "chipfamily": chipFamily,
            "name": name,
            "projects": [
                {
                    "tag": f"{env_name}_glowos_{version_name}",
                    "name": "GlowOS v" + version_name.replace('_', '.')[1:]  # prepend 'GlowOS v', remove 'v' and replace '_' with '.'
                }
            ]
        }
        # Check if a device with the same name already exists
        if not any(device['name'] == new_device['name'] for device in data['devices']):
            data['devices'].append(new_device)

    data['devices'].sort(key=lambda x: x['projects'][0]['name'] if x['projects'] else '', reverse=True)

    with open(json_file_path, 'w') as json_file:
        json.dump(data, json_file, indent=4)

def compile_and_merge_all(environments, version_name):
    print(f"Compiling and merging binary files for version '{version_name}'")
    for env in environments:
        if compile_environment(env['env'], version_name):
            if merge_bin_files(env['env'], version_name):
                move_merged_firmware(env['env'], version_name)
                create_manifest_json(env['env'], version_name, env['chipFamily'])
                update_devices_json(env['env'], version_name, env['name'], env['chipFamily'])
            else:
                print(f"Merging binary files for environment '{env['env']}' failed.")


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

    environments = [
        {"name":"GlowCore v2.0", "env": "glowcore_v2_0", "chipFamily": "ESP32"},
        {"name":"GlowCore v3.0", "env": "glowcore_v3_0", "chipFamily": "ESP32-S3"},
        # {"name":"MagiqWand v1.0 RC1", "env": "magiqwand_v1_0_RC1", "chipFamily": "ESP32"},
        {"name":"MagiqWand v1.0 RC2", "env": "magiqwand_v1_0_RC2", "chipFamily": "ESP32"},
        # {"name":"GlowBand v1.0 Beta 1", "env": "glowband_v1_0_beta1", "chipFamily": "ESP32"},
        {"name":"GlowSign v1.0 RC2", "env": "glowsign_v1_0_RC2", "chipFamily": "ESP32"},
        {"name":"GlowTower v1.0", "env": "glowtower_v1_0", "chipFamily": "ESP32"},
        {"name":"GlowLight v1.0", "env": "glowlight_v1_0", "chipFamily": "ESP32"},
        {"name":"GlowLight/GlowTower v2.0", "env": "glowlight_glowtower_v2_0", "chipFamily": "ESP32-S3"},
        {"name":"GlowLight/GlowTower v3.0", "env": "glowlight_glowtower_v3_0", "chipFamily": "ESP32-S3"}

    ]
    compile_and_merge_all(environments, version_name)

    remove_build_folder(build_folder)

if __name__ == "__main__":
    main()