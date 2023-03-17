#bin/sh

# Remove last 2 characters from string
filename=${1%??}

pcompile $1 &&
ldpic32 $filename &&
pterm