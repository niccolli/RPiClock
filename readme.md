# Raspberry Piに載せるLED Clock

## 使い方

### 起動前

事前にspidevをインストールします。

```
$ sudo pip3 install spidev
```

### 動作テスト

実行時の時刻を取得し、LED基板に書き込みます。

```
$ sudo python3 spiclock.py
```

### 定期実行

crontabに書き込みます。SPIを使うため、rootのcrontabに設定します。

```
@hourly /usr/bin/python3 /home/ubuntu/spiclock.py
@reboot /usr/bin/python3 /home/ubuntu/spiclock.py
```

- @hourly: ATmega内部のクロックを使うため、定期的に時刻設定を行います。daily等でもいいかもしれません。
- @reboot: 起動時に時刻設定を行います。

## 作り方・組み立て方

1. [ガーバー](./PCB/Gerber)を使い、基板を製造する。
1. 部品表に添って部品をハンダ付けする。
1. Arduinoを使ってファームウェアを書き込む。
1. Raspberry Piのピンヘッダに差し込む。

### ファームウェアの書き込み

ArduinoのSPI書き込み機能を使い、[From Arduino to a Microcontroller on a Breadboard](https://www.arduino.cc/en/Tutorial/BuiltInExamples/ArduinoToBreadboard)の"Minimal Circuit (Eliminating the External Clock)"の項に従ってブートローダー・ファームウェアを書き込みます。

1. Arduino UnoのSPIとLED基板の短辺のスルーホールを接続する。
1. Arduino IDEから、Arduino UnoにArduino ISPのサンプルを書き込む。
1. Arduino IDEから、"ATmega328 on a breadboard (8 MHz internal clock)"を選択してブートローダーを書き込む。
1. [ファームウェア](./LED_Driver)をIDEで開き、"書込装置を使って書き込む"を選択。IDEの→ボタンは使わない。
