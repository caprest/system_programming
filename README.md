# test

## 2
cpの実装：
ディスク溢れの際のエラー処理：小さなディスク領域を作る方法
```
dd if=/dev/zero of=fsfile bs=1024 count=1024
mke2fs -F fsfile
sudo mount -o loop fsfile /mnt
```
df fdiskなどで確認できる


## 3
