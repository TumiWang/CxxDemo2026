cd $(dirname "$0")

. $(pwd)/tools/check_env.sh

$DockerTool run --rm -it -w /source -v $(pwd):/source -e LINUX_TARGET=x86_64 crosscomiling/$SysArch:clang20 bash