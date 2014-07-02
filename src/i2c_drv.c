// ------------------------------------------------------------------- //
//  I2C�}�X�^�h���C�o�i�\�t�g�E�F�A����)                               //
// ------------------------------------------------------------------- //
//
//  ver 0.1
//		2012/10/24	s.osafune@gmail.com
//
//
// ******************************************************************* //
//     Copyright (C) 2012, J-7SYSTEM Works.  All rights Reserved.      //
//                                                                     //
// * This module is a free sourcecode and there is NO WARRANTY.        //
// * No restriction on use. You can use, modify and redistribute it    //
//   for personal, non-profit or commercial products UNDER YOUR        //
//   RESPONSIBILITY.                                                   //
// * Redistributions of source code must retain the above copyright    //
//   notice.                                                           //
//                                                                     //
// ******************************************************************* //

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <system.h>
#include <io.h>
#include "i2c_drv.h"


// SCL���C���̒l���Z�b�g 
static void i2c_set_scl(int scl)
{
	const alt_u32 dev_i2cpio  = I2C_PIO_BASE;
	int	reg;

	reg = IORD(dev_i2cpio, 1);

	if ( scl == 0 ) {
		reg |= I2C_PIN_SCL;			// SCL���C����0���o�� 
	} else {
		reg &= ~I2C_PIN_SCL;		// SCL���C������� 
	}

	IOWR(dev_i2cpio, 1, reg);
}

// SCL�̒l���擾 
static int i2c_get_scl(void)
{
	const alt_u32 dev_i2cpio  = I2C_PIO_BASE;

	if ( IORD(dev_i2cpio, 0) & I2C_PIN_SCL ) {
		return 1;
	} else {
		return 0;
	}
}

// SDA���C���̒l���Z�b�g 
static void i2c_set_sda(int sda)
{
	const alt_u32 dev_i2cpio  = I2C_PIO_BASE;
	int	reg;

	reg = IORD(dev_i2cpio, 1);

	if ( sda == 0 ) {
		reg |= I2C_PIN_SDA;			// SDA���C����0���o�� 
	} else {
		reg &= ~I2C_PIN_SDA;		// SDA���C������� 
	}

	IOWR(dev_i2cpio, 1, reg);
}

// SDA�̒l���擾 
static int i2c_get_sda(void)
{
	const alt_u32 dev_i2cpio  = I2C_PIO_BASE;

	if ( IORD(dev_i2cpio, 0) & I2C_PIN_SDA ) {
		return 1;
	} else {
		return 0;
	}
}

// SCL��1�ɂȂ�̂�҂� 
static int i2c_wait_scl_ready(int timeout)
{
	int	i;

	for(i=0 ; i<timeout*10 ; i++) {
		if ( i2c_get_scl() ) return 0;
		usleep(100);
	}

//	printf("[!] scl timeout\n");

	return 1;
}

// 1bit�̑���M 
static int i2c_bittrans(int bit)
{
	int res;

	usleep(I2C_CLK4DIV_COUNT);
	i2c_set_sda(bit);
	usleep(I2C_CLK4DIV_COUNT);

	i2c_set_scl(1);
	if ( i2c_wait_scl_ready(I2C_TIMEOUT_COUNT) ) return I2C_ERROR_TIMEOUT;	// SCL��1�ɂȂ�̂�҂� 
	usleep(I2C_CLK4DIV_COUNT*2);

	res = i2c_get_sda();
	i2c_set_scl(0);

	return res;
}


/* I2C_GPIO�̏����� */

int i2c_init(void)
{
	const alt_u32 dev_i2cpio  = I2C_PIO_BASE;
	int i,res=I2C_OK;

	IOWR(dev_i2cpio, 1, 0x00 );		// PIO���� 
	IOWR(dev_i2cpio, 0, 0x00 );		// �o�͒l�͏��0 

//	printf("I2C PIO Device Addr : 0x%08X\n",dev_i2cpio);

	// �o�X���A�C�h���ɂȂ�̂�҂� 
	res = I2C_ERROR_NOIDLE;

	for(i=0 ; i<I2C_TIMEOUT_COUNT ; i++) {
		if ( (IORD(dev_i2cpio, 0) & (I2C_PIN_SCL | I2C_PIN_SDA))==(I2C_PIN_SCL | I2C_PIN_SDA) ) {
			res = I2C_OK;
			break;
		}
		usleep(1000);
	}

	return res;
}


/* �X�^�[�g�R���f�B�V�����̍쐬 */

int i2c_start(void)
{
	i2c_set_sda(1);
	usleep(I2C_CLK4DIV_COUNT);

	i2c_set_scl(1);
	if ( i2c_wait_scl_ready(I2C_TIMEOUT_COUNT) ) return I2C_ERROR_TIMEOUT;	// SCL��1�ɂȂ�̂�҂� 
	usleep(I2C_CLK4DIV_COUNT*2);

	if ( !i2c_get_sda() ) return I2C_ERROR_START;		// SDA�������[�X����ĂȂ���΃G���[ 
	i2c_set_sda(0);
	usleep(I2C_CLK4DIV_COUNT*2);

	i2c_set_scl(0);

	return I2C_OK;
}


/* �X�g�b�v�R���f�B�V�����̍쐬 */

int i2c_stop(void)
{
	usleep(I2C_CLK4DIV_COUNT);
	i2c_set_sda(0);
	usleep(I2C_CLK4DIV_COUNT);

	i2c_set_scl(1);
	if ( i2c_wait_scl_ready(I2C_TIMEOUT_COUNT) ) return I2C_ERROR_TIMEOUT;	// SCL��1�ɂȂ�̂�҂� 
	usleep(I2C_CLK4DIV_COUNT*2);

	i2c_set_sda(1);
	usleep(I2C_CLK4DIV_COUNT*2);
	if ( !i2c_get_sda() ) return I2C_ERROR_STOP;		// SDA�������[�X����ĂȂ���΃G���[ 

	return I2C_OK;
}


/* �P�o�C�g�������� */

int i2c_sendbyte(int data)
{
	int i;

	// �f�[�^���M(MSB�t�@�[�X�g) 

	for(i=0 ; i<8 ; i++) {
		i2c_bittrans( data & (1<<7) );
		data <<= 1;
	}

	// �f�o�C�X�A�N�m���b�W�`�F�b�N 

	if ( !i2c_bittrans(1) ) { 
		return I2C_ACK;
	} else  {
		return I2C_NACK;
	}
}


/* �P�o�C�g�ǂݍ��� */

int i2c_recvbyte(int ack)
{
	int i,data=0;

	// �f�[�^��M(MSB�t�@�[�X�g) 

	for(i=0 ; i<8 ; i++) {
		data = (data << 1) | i2c_bittrans(1);
	}

	// �z�X�g�A�N�m���b�W���M 

	if ( ack == I2C_ACK ) {
		i2c_bittrans(0);
	} else {
		i2c_bittrans(1);
	}

	return data;
}



