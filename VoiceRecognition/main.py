from recognition import Recognition
from VoiceHandler import VoiceHandler
import time
import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BCM)

# GPIO configuration
buttonPin = 17
redLed = 22
blueLed = 18

GPIO.setup(buttonPin, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(blueLed, GPIO.OUT)
GPIO.setup(redLed, GPIO.OUT)
GPIO.output(blueLed, GPIO.LOW)
GPIO.output(redLed, GPIO.LOW)

recognizer = Recognition()
voice_handler = VoiceHandler()

print("Sistem Hazir")

try:
    while True:
        if GPIO.input(buttonPin):
            GPIO.output(blueLed, GPIO.LOW)
            voice_handler.text_to_speech("Nasıl yardımcı olabilirim?")            
            speech_text = voice_handler.speech_to_text()
            recognizer.do_recognition_and_action(speech_text)
            GPIO.output(blueLed, GPIO.HIGH)
            print("Done")
        time.sleep(.1)
finally:
    GPIO.cleanup()
