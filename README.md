# Kernel compilation 


TODO


# Modules

## Compilation

```
make -C ~/kernel M=$PWD modules
```

OR

```
make
```

## Instalation

```
sudo insmod ./hello_world.ko
dmesg  # show the debug/info logs
```

## Listing

```
sudo lsmod
dmesg  # show the debug/info logs
```

## Removal

```
sudo rmmod hello_world
dmesg  # show the debug/info logs
```
