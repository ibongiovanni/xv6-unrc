### USAGE

`make`:  compiles

`make qemu`:  compiles & runs

`make clean`:  cleans directory

### Para crear los parches

`diff -rupN xv6-unrc-orig/ xv6-unrc/ > projN.patch`

### Para probar los parches

`patch -p1 --dry-run < patch_file.patch`