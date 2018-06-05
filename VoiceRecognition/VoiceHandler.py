import speech_recognition as sr
import pyttsx3
from gtts import gTTS
import os
import pygame
import RPi.GPIO as GPIO


GPIO.setmode(GPIO.BCM)
redLed = 22
GPIO.setup(redLed, GPIO.OUT)
GPIO.output(redLed, GPIO.LOW)

class VoiceHandler:
    def __init__(self, language="tr"):
        self.recorder = sr.Recognizer()
        self.recorder.energy_threshold = 1000
        self.language = language
        self.audio = None

        # self.engine = pyttsx3.init()
        # self.engine.setProperty('rate', 150)

        pygame.init()
        pygame.mixer.init()

    def speech_to_text(self):
        with sr.Microphone(sample_rate=48000) as source:
            print("Say Something!")
            GPIO.output(redLed, GPIO.HIGH)
            self.audio = self.recorder.listen(source)
            print("Audio received.")
            GPIO.output(redLed, GPIO.LOW)

        ret_text = self.recorder.recognize_google(self.audio, language=self.language)

        return ret_text

    def text_to_speech(self, text="Sistemde bir hata oluştu."):
        # self.engine.say(text)
        # self.engine.runAndWait()
        self.tts = gTTS(text=text, lang=self.language)
        self.tts.save("temp.wav")
        os.system("mpg321 temp.wav")
