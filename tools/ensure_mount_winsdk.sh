
# 挂载Windowws SDK
if ! mount | grep  -q "/sdk/winsdk type fuse.ciopfs"; then
    if [ -d /sdk/winsdk ]; then
        rm -rf /sdk/winsdk
    fi
    mkdir /sdk/winsdk
    ciopfs -o use_ino /sdk/win10.0.26100 /sdk/winsdk
fi