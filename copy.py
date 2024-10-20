import glob, os, shutil, fileinput
import datetime

Import("env", "projenv")
print("Post build scripts > copy_elf.py")


def after_upload(source, target, env):
    print("Copying elf file")
    # ".pio/build/esp32dev/firmware.elf"
    shutil.copyfile(".pio/build/genericCH592F/firmware.elf", 'test/firmware.elf')
    shutil.copyfile(".pio/build/genericCH592F/firmware.bin", 'test/firmware.bin')
    e = datetime.datetime.now()
    print (e.strftime("%H:%M:%S %d-%m-%Y"))
    
    
env.AddPostAction("upload", after_upload)
env.AddPostAction("buildprog", after_upload)