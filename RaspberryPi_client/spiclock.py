import spidev
import datetime

spi_bus = 0
spi_device = 0

spi = spidev.SpiDev()
spi.open(spi_bus, spi_device)
spi.max_speed_hz=500000
spi.mode= 0b00

dt_now = datetime.datetime.now()

to_send = [dt_now.hour, dt_now.minute, dt_now.second]
spi.xfer(to_send)
