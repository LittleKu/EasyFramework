set GYP_DEFINES="target_arch=ia32"
set GYP_MSVS_VERSION=2022
set DEPOT_TOOLS_WIN_TOOLCHAIN=0
set vs2022_install=C:\Program Files\Microsoft Visual Studio\2022\Enterprise\
gn gen out/Debug_GN_x86 --ide=vs2022 --sln=easy --args="treat_warnings_as_errors=false is_debug=true target_cpu=\"x86\" target_os=\"win\""