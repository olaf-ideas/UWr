awk '
/^VmSize:/ {vms+=$2}
/^VmRSS:/  {rss+=$2}
END {print "VmSize:",vms,"kB\nVmRSS:",rss,"kB"}
' /proc/[0-9]*/status
