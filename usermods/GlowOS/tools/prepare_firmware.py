import os
import shutil
import subprocess

# Delete temp folder if it exists
temp_folder = "tmp_flashing_files"
if os.path.exists(temp_folder):
    shutil.rmtree(temp_folder)

# Execute terminal command with environment variable
environments = ["glowcore_v3_0"]
for env in environments:
    subprocess.run(["platformio", "run", "--environment", env])

    # Copy files to temp folder
    os.makedirs(f"{temp_folder}/{env}")
    shutil.copy(".pio/build/{}/firmware.bin".format(env), f"{temp_folder}/{env}")
    shutil.copy(".pio/build/{}/partitions.bin".format(env), f"{temp_folder}/{env}")

    # Execute terminal command with target and environment
    subprocess.run(["platformio", "run", "--target", "buildfs", "--environment", env])

    # Copy spiffs.bin file to temp folder
    shutil.copy(".pio/build/{}/spiffs.bin".format(env), f"{temp_folder}/{env}")

    # Execute merge_bin command
    cmd = f"python3 -m esptool --chip ESP32-S3 merge_bin -o ../GlowOS-WebInstaller/firmware/{env}__glowos_v0_15_1.bin --flash_mode dio --flash_size 8MB 0x0000 /Users/kitty/.platformio/packages/framework-arduinoespressif32@3.20004.0/tools/sdk/esp32s3/bin/bootloader_dio_80m.bin 0x8000 {temp_folder}/{env}/partitions.bin 0xe000 /Users/kitty/.platformio/packages/framework-arduinoespressif32@3.20004.0/tools/partitions/boot_app0.bin 0x10000 {temp_folder}/{env}/firmware.bin 0x410000 {temp_folder}/{env}/spiffs.bin"
    subprocess.run(cmd.split())

# Delete temp folder
shutil.rmtree(temp_folder)