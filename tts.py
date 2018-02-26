import pyttsx3


class tts:
    def __init__(self):
        self.engine = pyttsx3.init()

    def text_to_speech(self, text="Empty String"):
        if text == "Error Text":
            print("There is something wrong with google service!")
        else:
            self.engine.say(text)
            self.engine.runAndWait()
