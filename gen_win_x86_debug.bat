set GYP_DEFINES="buildtype=Official branding=Chrome target_arch=ia32"
set GYP_MSVS_VERSION=2022
set DEPOT_TOOLS_WIN_TOOLCHAIN=0
set vs2022_install=C:\Program Files\Microsoft Visual Studio\2022\Enterprise\
set gn_path=third_party\ninja\ninja.exe
set gn_full_path=%~dp0%gn_path%
gn gen out/Debug_GN_x86 --ninja-executable=%gn_full_path% --ide=vs2022 --sln=easy --args="is_component_build=false treat_warnings_as_errors=false is_debug=true target_cpu=\"x86\" target_os=\"win\""