$pyver = python --version
Get-ChildItem -Directory "C:\Users\builder\miniconda3\envs\build-env\pkgs\numpy*" | Rename-Item -NewName numpy-base
