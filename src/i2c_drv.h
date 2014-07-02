// ------------------------------------------------------------------- //
//  I2Cマスタドライバ（ソフトウェア実装)                               //
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

#ifndef __i2c_drv_h__
#define __i2c_drv_h__


#define I2C_PIN_SCL				(1<<0)
#define I2C_PIN_SDA				(1<<1)

#define I2C_CLK4DIV_COUNT		(5)			// クロック幅の1/4の時間(us) 

#define I2C_OK					(0)
#define I2C_ACK					I2C_OK
#define I2C_NACK				(1)
#define I2C_ERROR_NOIDLE		(-1)
#define I2C_ERROR_START			(-2)
#define I2C_ERROR_STOP			(-3)
#define I2C_ERROR_TIMEOUT		(-4)

#define I2C_TIMEOUT_COUNT		(100)		// 100msでタイムアウト 


/* I2C_GPIOの初期化 */

int i2c_init(void);


/* スタートコンディションの作成 */

int i2c_start(void);


/* ストップコンディションの作成 */

int i2c_stop(void);


/* １バイト書き込み */

int i2c_sendbyte(int data);


/* １バイト読み込み */

int i2c_recvbyte(int ack);


#endif
