import speech_recognition as sr
import pyttsx3


class VoiceHandler:
    def __init__(self, language="tr"):
        self.recorder = sr.Recognizer()
        self.recorder.energy_threshold = 1000
        self.language = language
        self.audio = None

        self.engine = pyttsx3.init()
        self.engine.setProperty('rate', 150)

    def speech_to_text(self):
        with sr.Microphone(sample_rate=48000) as source:
            print("Say Something!")
            self.audio = self.recorder.listen(source)
            print("Audio received.")

        ret_text = self.recorder.recognize_google(self.audio, language=self.language)

        return ret_text

    def text_to_speech(self, text="Sistemde bir hata oluştu."):
        self.engine.say(text)
        self.engine.runAndWait()
