savedcmd_gt610.mod := printf '%s\n'   gt610.o | awk '!x[$$0]++ { print("./"$$0) }' > gt610.mod
