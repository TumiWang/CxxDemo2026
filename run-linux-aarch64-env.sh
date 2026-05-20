cd $(dirname "$0")

. $(pwd)/tools/check_env.sh

$DockerTool run --rm -it -w /source -v $(pwd):/source -e LINUX_TARGET=aarch64 crosscomiling/$SysArch:clang20 bash