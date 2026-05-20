cd $(dirname "$0")

. $(pwd)/tools/check_env.sh

$DockerTool run --privileged --rm -it -w /source -v $(pwd):/source -e WINDOWS_TARGET=arm64 crosscomiling/$SysArch:clang20 bash