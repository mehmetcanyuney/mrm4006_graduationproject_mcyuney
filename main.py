import speech_recognition as sr

r = sr.Recognizer()

with sr.Microphone() as source:
    print("Say Something!")
    audio = r.listen(source)
    print("Audio received.")

print("You said : " + r.recognize_google(audio,language="tr"))