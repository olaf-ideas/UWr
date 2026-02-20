# Zadanie 1

### Czym są punkty montażowe?

VAll files accessible in a Unix system are arranged in one big tree, the file hierarchy, rooted at /. These files can be spread out over several devices. The mount command serves to attach the filesystem found on some device to the big file tree

```
findmnt
```

TARGET SOURCE FSTYPE OPTIONS

/proc <- pseudo systemy plików

relatime -> inode update relatime to modify or change time
noexec -> don't permit direct execution of any binaries on the mount filesystem
nodev -> don't interpret character or block special devices on the filesystem

