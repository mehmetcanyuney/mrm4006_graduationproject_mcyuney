from stt import stt
from tts import tts

# en-US for english and tr for Turkish #
stt_op = stt(language="en-US")


texti = stt_op.speech_to_text()
print("You said = " + texti)

tts_op = tts()

tts_op.text_to_speech(text=texti)
