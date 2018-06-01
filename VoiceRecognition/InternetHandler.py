import bs4 as bs
import webbrowser
import urllib.request
import helpFunctions
import re


class InternetHandler:
    def __init__(self):
        # nothing to init
        self.url = None

    def search_web_food(self, searching_data):
        searching_data = searching_data.lower()
        searching_data = helpFunctions.transform_to_eng(searching_data)
        for x in range(0, len(searching_data)):
            if searching_data[x] == " ":
                searching_data = searching_data[:x] + "+" + searching_data[(x + 1):]

        self.url = "http://www.nefisyemektarifleri.com/?ep=true&sa=&s=" + searching_data
        source = urllib.request.urlopen(self.url).read()
        soup = bs.BeautifulSoup(source, "lxml")

        for i in soup.find_all('div', class_='padder fullpage'):
            self.url = i.find_all('a')[2].get('href')

        source = urllib.request.urlopen(self.url).read()
        soup = bs.BeautifulSoup(source, "lxml")

        begin = 0
        listI = []
        for i in soup.find_all('div', class_="entry entry_content"):
            if re.search("Malzemeler", i.text) is not None:
                begin = re.search("Malzemeler", i.text).span()[0]
        for i in soup.find_all('div', class_="entry entry_content"):
            listI.append(i.text)
            # print to screen
            print(i.text[begin:])

        if not listI:
            for i in soup.find_all('div', class_="entry_content tagoninread"):
                if re.search("Malzemeler", i.text) is not None:
                    begin = re.search("Malzemeler", i.text).span()[0]
            for i in soup.find_all('div', class_="entry_content tagoninread"):
                # print to screen
                print(i.text[begin:])

    def check_web_google(self, searching_data):
        new = 1

        searching_data = searching_data.lower()
        searching_data = helpFunctions.transform_to_eng(searching_data)
        for x in range(0, len(searching_data)):
            if searching_data[x] == " ":
                searching_data = searching_data[:x] + "+" + searching_data[(x + 1):]

        self.url = "https://www.google.com.tr/?#q=" + searching_data
        webbrowser.open(self.url, new=new)

    def check_web_music(self, searching_data):
        searching_data = searching_data.lower()
        searching_data = helpFunctions.transform_to_eng(searching_data)
        for x in range(0, len(searching_data)):
            if searching_data[x] == " ":
                searching_data = searching_data[:x] + "+" + searching_data[(x + 1):]

        self.url = "https://www.youtube.com/results?search_query=" + searching_data
        source = urllib.request.urlopen(self.url).read()
        soup = bs.BeautifulSoup(source, "lxml")

        self.url = soup.find_all('a', class_="yt-uix-tile-link")[0].get("href")
        self.url = "https://www.youtube.com" + self.url
        webbrowser.open(self.url, new=1)
