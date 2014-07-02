i2c_drv
====================
NiosII＋PIOペリフェラル用のI2C通信ライブラリです。


使い方
==========
NiosIIとPIOおよびタイマペリフェラルを用意します。  
usleep()を使うためHALが必須になります。縮小ライブラリを使う場合はソース中のusleep関数を適当な時間待ち関数に差し替えてください。  


サンプルコード(LM75のリード)
-----------------------------
<span>
  i2c_init();
    :
    :

  i2c_start();
  i2c_sendbyte(0x90);
  temp = i2c_recvbyte(I2C_ACK) << 8;
  temp |= i2c_recvbyte(I2C_NACK);
  i2c_stop();
</span>


ライセンス
=========

[Apache License, Version 2.0](http://www.apache.org/licenses/LICENSE-2.0)
