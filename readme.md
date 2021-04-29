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
