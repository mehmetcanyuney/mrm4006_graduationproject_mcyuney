from VoiceHandler import VoiceHandler
from InternetHandler import InternetHandler
from UartHandler import UartHandler
import helpFunctions
import re


class Recognition:
    def __init__(self):
        # necessary lists
        self.function_list = []
        self.stm_list = []
        self.basic_list = []

        # necessary files
        function_file = open("functions.txt", "r+", encoding='utf8')
        stm_file = open("stmsignals.txt", "r+", encoding='utf8')
        basic_file = open("basics.txt", "r+", encoding='utf8')

        # creating necessary lists
        # function file
        line = function_file.readline().rstrip("\n")
        while line != "":
            self.function_list.append(line)
            line = function_file.readline().rstrip("\n")

        # stm file
        line = stm_file.readline().rstrip("\n")

        while line != "":
            self.stm_list.append(line)
            line = stm_file.readline().rstrip("\n")

        # basic file
        line = basic_file.readline().rstrip("\n")
        while line != "":
            self.basic_list.append(line)
            line = basic_file.readline().rstrip("\n")

        # closing unnecessary files
        function_file.close()
        stm_file.close()
        basic_file.close()

        # creating voice handler for information
        self.voice_handler = VoiceHandler()

        # creating internet searcher
        self.internet_handler = InternetHandler()

        # creating uart handler
        self.uart_handler = UartHandler(baudrate=9600)

    def do_recognition_and_action(self, text):
        text = text.lower()

        function_code = [0 for x in range(len(self.function_list))]
        for i in range(0, len(self.function_list)):
            words = self.function_list[i].split()
            for j in range(1, len(words)):
                if re.search(words[j], text) is not None:
                    function_code[int(words[0])] += 1

        check_all_zero = True
        for x in range(0, len(function_code)):
            if function_code[x] != 0:
                check_all_zero = False
                break

        if check_all_zero:
            function_code = None
        else:
            function_code = function_code.index(max(function_code))

        if function_code is None:
            self.voice_handler.text_to_speech(text="Bu cümleden bir anlam çıkartamadım.")
            self.voice_handler.text_to_speech(text="Bu cümlede hangi fonksiyonu temsil ediyor acaba?")
            # func = input("Fonksiyon seç : ")
            func = helpFunctions.basic_numbers(self.voice_handler.speech_to_text())
            self.voice_handler.text_to_speech(text="Bu cümledeki anahtar kelime ne acaba?")
            # key = input("Anahtar Kelimeyi seç : ")
            key = str(self.voice_handler.speech_to_text())
            write_line = "\n" + str(func) + " " + key
            function_file = open("functions.txt", "a", encoding='utf8')
            function_file.writelines(write_line)
            function_file.close()

            function_code = func

        # user settings
        if function_code == 0:
            # user settings
            print("on going...")
        # stm32f4 functions
        elif function_code == 1:
            # stm32f4 codes
            stm_code = [0 for x in range(100)]
            for m in range(0, len(self.stm_list)):
                words = self.stm_list[m].split()
                for n in range(1, len(words)):
                    if re.search(words[n], text) is not None:
                        stm_code[int(words[0])] += 1

            stm_code = stm_code.index(max(stm_code))

            sent_data = []
            more_code = None
            on_off_code = None
            selection_code = None
            for p in range(0, 3):
                words = self.basic_list[p].split()
                for r in range(1, len(words)):
                    if re.search(words[r], text) is not None:
                        if int(words[0]) == 2:
                            more_code = 1
                            break
                        on_off_code = int(words[0])

            number = helpFunctions.search_number_string(text)
            if number is not None:
                number = int(number)
                if on_off_code is None:
                    sent_data.append(stm_code)
                    sent_data.append(int(number))
                    self.uart_handler.writeReg(sent_data)
                elif on_off_code:
                    if more_code == 1:
                        # Uart handler must take value
                        # current_value = int(input("mevcut değeri al : "))
                        current_value = self.uart_handler.readReg([stm_code, "b"])
                        if re.search("yüzde", text) is not None or re.search("%", text) is not None:
                            if current_value - number < 0:
                                temp_text = "Verilen değerde bir hata var. Şu anki değer " + str(current_value)
                                self.voice_handler.text_to_speech(temp_text)
                            else:
                                sent_data.append(stm_code)
                                sent_data.append(current_value - number)
                                # uart handler
                                self.uart_handler.writeReg(sent_data)
                        else:
                            sent_data.append(stm_code)
                            sent_data.append(number)
                            # uart handler
                            self.uart_handler.writeReg(sent_data)

                elif not on_off_code:
                    if more_code == 1:
                        # uart handler must take value
                        # current_value = int(input("mevcut değeri al : "))
                        current_value = self.uart_handler.readReg([stm_code, "b"])
                        # uart handler
                        self.uart_handler.writeReg(sent_data)

            else:
                for t in range(3, len(self.basic_list)):
                    words = self.basic_list[t].split()
                    for y in range(1, len(words)):
                        if re.search(words[y], text) is not None:
                            selection_code = int(words[0])
                if selection_code == 5:
                    sent_data.append(stm_code)
                    sent_data.append("b")
                    # uart handler
                    current_value = self.uart_handler.readReg(sent_data)
                    temp_text = "Aradığın değer" + str(current_value)
                    self.voice_handler.text_to_speech(temp_text)
                elif selection_code == 6:
                    sent_data.append(stm_code)
                    sent_data.append(4)
                    # uart handler
                    self.uart_handler.writeReg(sent_data)
                elif selection_code == 7:
                    sent_data.append(stm_code)
                    sent_data.append(3)
                    # uart handler
                    self.uart_handler.writeReg(sent_data)
                elif selection_code == 8:
                    sent_data.append(stm_code)
                    sent_data.append(0)
                    # uart handler
                    self.uart_handler.writeReg(sent_data)
                elif selection_code == 9:
                    sent_data.append(stm_code)
                    sent_data.append(5)
                    # uart handler
                    self.uart_handler.writeReg(sent_data)
                elif on_off_code == 1:
                    if selection_code == 4:
                        sent_data.append(stm_code)
                        sent_data.append(-10)
                        # uart handler
                        self.uart_handler.writeReg(sent_data)
                    elif selection_code == 3:
                        sent_data.append(stm_code)
                        sent_data.append(50)
                        # uart handler
                        self.uart_handler.writeReg(sent_data)
                    else:
                        sent_data.append(stm_code)
                        sent_data.append(100)
                        # uart handler
                        self.uart_handler.writeReg(sent_data)
                else:
                    if selection_code == 4:
                        sent_data.append(stm_code)
                        sent_data.append(-5)
                        # uart handler
                        self.uart_handler.writeReg(sent_data)
                    elif selection_code == 3:
                        sent_data.append(stm_code)
                        sent_data.append(50)
                        # uart handler
                        self.uart_handler.writeReg(sent_data)
                    else:
                        sent_data.append(stm_code)
                        sent_data.append(0)
                        # uart handler
                        self.uart_handler.writeReg(sent_data)
        # internet functions
        elif function_code == 2:
            # internet search
            self.voice_handler.text_to_speech("Neyi aramamı istersin?")
            search = self.voice_handler.speech_to_text()
            if re.search("yemek", search) is not None or re.search("tarif", search) is not None:
                self.voice_handler.text_to_speech("Hangi yemeğin tarifini görmek istersin?")
                recipe = self.voice_handler.speech_to_text()
                self.internet_handler.search_web_food(recipe)
            elif re.search("müzik", search) is not None:
                if re.search("ingilizce", search) is not None or re.search("İngilizce", search) is not None:
                    self.voice_handler.text_to_speech("Tabii, hangi müziği dinlemek istersin?")
                    self.voice_handler.language = "en-us"
                    music_name = self.voice_handler.speech_to_text()
                    self.voice_handler.language = "tr"
                else:
                    self.voice_handler.text_to_speech("Tabii, hangi müziği dinlemek istersin?")
                    music_name = self.voice_handler.speech_to_text()

                self.internet_handler.check_web_music(music_name)
            else:
                self.internet_handler.check_web_google(search)
        # weather forecast function
        elif function_code == 3:
            # country = input("Ülke : ")
            country = "tr"
            # city = input("Şehir : ")
            self.voice_handler.text_to_speech("Hangi şehirin hava durumunu öğrenmek istersin")
            city = self.voice_handler.speech_to_text()
            location = city + "," + country
            helpFunctions.check_weather(location)
        # date and hour questioning
        elif function_code == 4:
            if re.search("saat", text) is not None:
                self.voice_handler.text_to_speech(helpFunctions.check_time())
            else:
                self.voice_handler.text_to_speech(helpFunctions.check_date())
