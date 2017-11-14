# Projeto2---SOB
#link para criaçao da particao https://www.vivaolinux.com.br/dica/Criando-uma-particao-virtual-no-Linux

Comandos para criar particao

dd if=/dev/zero of=file.img bs=1k count=10000
   42  losetup /dev/loop0 file.img
   43  reboot
   44  dd if=/dev/zero of=file.img bs=1k count=10000
   45  losetup /dev/loop0 file.img
   46  mkfs.minix -c /dev/loop0 10000
   47  mkdir /mnt/point1
   48  mount -t minix /dev/loop0 /mnt/point1
