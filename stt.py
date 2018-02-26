import speech_recognition as sr


class stt:
    def __init__(self, language="en-US"):
        self.recorder = sr.Recognizer()
        self.language = language
        self.audio = None

    def speech_to_text(self):
        with sr.Microphone() as source:
            print("Say Something!")
            self.audio = self.recorder.listen(source)
            print("Audio received.")

        ret_text = self.recorder.recognize_google(self.audio, language=self.language)

        return ret_text
