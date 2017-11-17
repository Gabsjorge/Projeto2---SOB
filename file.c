/*
 *  linux/fs/minix/file.c
 *
 *  Copyright (C) 1991, 1992 Linus Torvalds
 *
 *  minix regular file handling primitives
 */

#include "minix.h"
#include <linux/moduleparam.h>
#include <linux/crypto.h>
#include <linux/uio.h>
#include <linux/string.h>
static char *key = "asd";
module_param(key,charp,0000);
MODULE_PARM_DESC(key,"Chave");
static unsigned char key2[17];
#define  AES_BLOCK_SIZE 16
static char string[400];

int decifrarDado(char **dados){
  struct crypto_cipher *tfm;
    int i,count,div,modd;

	if(strlen(key)<16)
	{
	for(i=strlen(key);i<16;i++)
	{
	key2[i]=0;
	}

	}
	key2[16]='\0';
    div=strlen(*dados)/AES_BLOCK_SIZE;
    modd=strlen(*dados)%AES_BLOCK_SIZE;
    if(modd>0)
        div++;
    count=div;
    tfm=crypto_alloc_cipher("aes", 0, 16);
    crypto_cipher_setkey(tfm,key2,16);
    for(i=0;i<count;i++)
    {
        crypto_cipher_decrypt_one(tfm,string,*dados);
        *dados=*dados+AES_BLOCK_SIZE;
    }
    *dados=string;
    printk(KERN_INFO "Crypto: Valor da string decifrada: %s\n", *dados);

    return strlen(string);
}


//funcoes criadas para modularizar parte de crypto deve retornar o tamanho de caracteres que devem ser lidos, ou seja o tamanho da string que sera enviada de volta para o usuario.
int cifrarDado(char **dados){

     struct crypto_cipher *tfm;
     int i,j,count,div,modd;
     char array[16];
     strcpy(key2,key);
     memset(string,0,400);

     div=strlen(*dados)/AES_BLOCK_SIZE;
     modd=strlen(*dados)%AES_BLOCK_SIZE;

     if(modd>0)
     div++;
     count=div;


     if(strlen(key)<16)
	{
	for(i=strlen(key);i<16;i++)
	{
	key2[i]=0;
	}

	}
	key2[16]='\0';

     printk(KERN_INFO "Crypto: TESTE VALORES VARIAVEIS: %s,%s\n", *dados, key2);

     tfm=crypto_alloc_cipher("aes", 0, 16);
     crypto_cipher_setkey(tfm,key2,16);

     for(i=0;i<count;i++)
     {

	 crypto_cipher_encrypt_one(tfm,string,*dados);
         *dados=*dados+AES_BLOCK_SIZE;

     }
    *dados=string;

    printk(KERN_INFO "Crypto: Valor da string encriptada: %s\n", *dados);
     crypto_free_cipher(tfm);

    return strlen(string);
}


static ssize_t custom_file_write_iter(struct kiocb *iocb, struct iov_iter *from)
{

	cifrarDado((char **)&(from->iov->iov_base));
	printk(KERN_ALERT "Cifrado iov_base %s",from->iov->iov_base);
	generic_file_write_iter(iocb, from);

}

static ssize_t custom_file_read_iter(struct kiocb *iocb, struct iov_iter *iter)
{
	decifrarDado((char **)&(iter->iov->iov_base));
	decifrarDado((char **)&(iter->iov->iov_base));
	generic_file_read_iter(iocb, iter);
}



/*
 * We have mostly NULLs here: the current defaults are OK for
 * the minix filesystem.
 */
const struct file_operations minix_file_operations = {
	.llseek		= generic_file_llseek,
	.read_iter	= custom_file_read_iter,
	.write_iter	= custom_file_write_iter,
	.mmap		= generic_file_mmap,
	.fsync		= generic_file_fsync,
	.splice_read	= generic_file_splice_read,
};

static int minix_setattr(struct dentry *dentry, struct iattr *attr)
{
	struct inode *inode = d_inode(dentry);
	int error;

	error = setattr_prepare(dentry, attr);
	if (error)
		return error;

	if ((attr->ia_valid & ATTR_SIZE) &&
	    attr->ia_size != i_size_read(inode)) {
		error = inode_newsize_ok(inode, attr->ia_size);
		if (error)
			return error;

		truncate_setsize(inode, attr->ia_size);
		minix_truncate(inode);
	}

	setattr_copy(inode, attr);
	mark_inode_dirty(inode);
	return 0;
}





const struct inode_operations minix_file_inode_operations = {
	.setattr	= minix_setattr,
	.getattr	= minix_getattr,
};

