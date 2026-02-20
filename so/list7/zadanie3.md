# Zadanie 3

### Wyświetl zużycie pamięci procesu wykonującego kod X-serwera.
```
olaf@laptop:~/uwr/UWr/so$ cat /proc/$(pgrep Xorg)/status | egrep 'Vm|Rss'
VmPeak:  1548644 kB
VmSize:  1496932 kB
VmLck:         0 kB
VmPin:         0 kB
VmHWM:    110424 kB
VmRSS:     29724 kB
RssAnon:           17308 kB
RssFile:           12288 kB
RssShmem:            128 kB
VmData:   169628 kB
VmStk:       132 kB
VmExe:      1676 kB
VmLib:    155368 kB
VmPTE:       648 kB
VmSwap:    31832 kB
```

### Wyjaśnij znaczenie pól

```
VmPeak Peak virtual memory size.

VmSize Virtual memory size.

VmLck  Locked memory size (see mlock(2)).

VmPin  Pinned  memory  size (since Linux 3.2).  These are pages that can't be moved because something
        needs to directly access physical memory.

VmHWM  Peak resident set size ("high water mark").  This value  is  inaccurate;  see  /proc/pid/statm
        above.

VmRSS  Resident  set  size.   Note  that the value here is the sum of RssAnon, RssFile, and RssShmem.
        This value is inaccurate; see /proc/pid/statm above.

RssAnon
        Size of resident anonymous  memory.   (since  Linux  4.5).   This  value  is  inaccurate;  see
        /proc/pid/statm above.

RssFile
        Size  of  resident  file  mappings.   (since  Linux  4.5).   This  value  is  inaccurate;  see
        /proc/pid/statm above.

RssShmem
        Size of resident shared memory (includes System V shared memory, mappings from  tmpfs(5),  and
        shared anonymous mappings).  (since Linux 4.5).

VmData, VmStk, VmExe
        Size of data, stack, and text segments.  This value is inaccurate; see /proc/pid/statm above.

VmLib  Shared library code size.

VmPTE  Page table entries size (since Linux 2.6.10).

VmPMD  Size of second-level page tables (added in Linux 4.0; removed in Linux 4.15).

VmSwap Swapped-out  virtual  memory size by anonymous private pages; shmem swap usage is not included
        (since Linux 2.6.34).  This value is inaccurate; see /proc/pid/statm above
```

### Przypomnij jaka jest różnica między zbiorem roboczym i rezydentnym procesu

Zbiór roboczy to są strony w RAM'ie, które czekają i są gotowe do działąnia. Rezydentny to wszystko co jest w pamięci odwzorowane, każdy `mmap`, każda pamięć anonimowa.

### 