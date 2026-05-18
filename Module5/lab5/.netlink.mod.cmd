savedcmd_netlink.mod := printf '%s\n'   netlink.o | awk '!x[$$0]++ { print("./"$$0) }' > netlink.mod
