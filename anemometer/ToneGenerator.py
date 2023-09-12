import RPi.GPIO as GPIO
import time

class ToneGenerator:
    out = 18

    def generate_tone(self, freq, duration):
        GPIO.setmode(GPIO.BCM)
        GPIO.setup(self.out, GPIO.OUT)
    
        interval = 1.0 / freq
        half_cycle = interval / 2
     
        print('Freq %6d , interval %f, half_cycøe %f' % (freq, interval, half_cycle))
        start = time.time()
        print(start)
        while True:
            GPIO.output(self.out, GPIO.HIGH)
            time.sleep(half_cycle)
            GPIO.output(self.out, GPIO.LOW)
            time.sleep(half_cycle)
            tst = time.time()
            if tst > start + duration:
                break;

        GPIO.cleanup()


if __name__ == "__main__":
    tg = ToneGenerator()
    tg.generate_tone(1000.0, 5)
