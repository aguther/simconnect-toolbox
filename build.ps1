
# clear commands
Clear-Host

# remove build folder
Remove-Item -Force -Recurse build

# create build files
cmake -B build

# build it
cmake --build build --config Release
