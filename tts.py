import pyttsx3


class tts:
    def __init__(self):
        self.engine = pyttsx3.init()

    def text_to_speech(self, text="Empty String"):
        self.engine.say(text)
        self.engine.runAndWait()
