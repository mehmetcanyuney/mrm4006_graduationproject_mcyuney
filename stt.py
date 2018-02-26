import speech_recognition as sr


class stt:
    def __init__(self, language="en-US"):
        self.recorder = sr.Recognizer()
        self.language = language
        self.audio = None
        self.errortext = "Error Text"

    def speech_to_text(self):
        with sr.Microphone() as source:
            print("Say Something!")
            self.audio = self.recorder.listen(source)
            print("Audio received.")

        try:
            ret_text = self.recorder.recognize_google(self.audio, language=self.language)
            return ret_text
        except sr.UnknownValueError:
            print("Google Speech Recognition could not understand audio")
            return self.errortext
        except sr.RequestError as e:
            print("Could not request results from Google Speech Recognition service; {0}".format(e))
            return self.errortext
