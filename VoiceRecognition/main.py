from recognition import Recognition
from VoiceHandler import VoiceHandler
import time
import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BCM)

# GPIO configuration
buttonPin = 17

GPIO.setup(buttonPin, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)

recognizer = Recognition()
voice_handler = VoiceHandler()

print("Sistem Hazır")

try:
    while True:
        if GPIO.input(buttonPin):
            voice_handler.text_to_speech("Nasıl yardımcı olabilirim?")
            recognizer.do_recognition_and_action(voice_handler.speech_to_text())
        time.sleep(.1)
finally:
    GPIO.cleanup()
