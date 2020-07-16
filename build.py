# I will rewrtie this file...

import subprocess
import os

def fix_magic_value():
  lng = ( os.stat(f"{patha}/stage2").st_size +
          os.stat("kernel.exe").st_size )
 
  a = lng / 512
  b = lng // 512
  if a != b:
    b = b + 1

  b = int(b)
  with open(f"{patha}/stage1", "rb") as f:
    d = f.read()
  
  i = d.index(b"\xb0\x00\x66\x90")
  di = b""
  di += d[:i+1]
  di += bytes([b])
  di += d[i+4:]
  di += b"\x00\x00"

  with open("stage1", "wb") as f:
    f.write(di)    

pathc = "source/kernel"
patha = "source/boot"
in_com = [
  f"nasm {patha}/stage1.asm",
  f"gcc -c {pathc}/HAL.c -O3 -nostdlib -std=c99 -m32 -masm=intel -Wall -Wextra",
  f"gcc -c {pathc}/terminal.c -O3 -nostdlib -std=c99 -m32 -masm=intel -Wall -Wextra",
  f"gcc -c {pathc}/kernel.c -O3 -nostdlib -m32 -masm=intel  -Wall -Wextra",
  f"gcc kernel.o terminal.o HAL.o -O3 -nostdlib -m32 -masm=intel -o kernel.exe -Wall -Wextra",
  f"nasm {patha}/stage2.asm",
  fix_magic_value,
]

out_com = [
  f"{patha}/stage1",
  False, f"{patha}/stage2",
  "kernel.exe"
]


def main():
  for i in in_com:
    if type(i) is str:
      subprocess.check_output(i, shell=True)

    else:
      i()

  b = b""
  algn = True
  for o in out_com:
    if type(o) is type(False):
      algn = False
      continue

    with open(o, "rb") as f:
      d = f.read()
      b += d

      if not algn:
        algn = True
        continue

      lng = len(d) % 512
      if lng == 0:
        continue

      lng = 512 - lng
      b += (b"\0" * lng)

  with open("floppy.bin", "wb") as f:
    f.write(b)

if __name__ == "__main__":
  main()