import requests
import datetime
import locale
import pyowm


def transform_to_eng(data):
    l = len(data)
    for x in range(0, l):
        y = x + 1
        if data[x] == 'ç':
            data = data[:x] + 'c' + data[y:]
        elif data[x] == 'ğ':
            data = data[:x] + 'g' + data[y:]
        elif data[x] == 'ı':
            data = data[:x] + 'i' + data[y:]
        elif data[x] == 'ö':
            data = data[:x] + 'o' + data[y:]
        elif data[x] == 'ş':
            data = data[:x] + 's' + data[y:]
        elif data[x] == 'ü':
            data = data[:x] + 'u' + data[y:]
    return data


def check_internet():
    url = 'http://www.google.com/'
    timeout = 5
    try:
        _ = requests.get(url, timeout=timeout)
        return True
    except requests.ConnectionError:
        print("İnternet bağlantısı yok. Lütfen gerekli ayarları gerçekleştirin!")
    return False


def basic_numbers(text):
    if text == "bir":
        return 1
    elif text == "sıfır":
        return 0
    else:
        return int(text)


def search_number_string(string):
    index_list = []
    del index_list[:]
    for i, x in enumerate(string):
        if x.isdigit():
            index_list.append(i)
    if len(index_list) == 0:
        return None
    start = index_list[0]
    end = index_list[-1] + 1
    ret = string[start:end]
    return ret


def check_time():
    locale.setlocale(locale.LC_ALL, '')
    now = datetime.datetime.now()
    time = str(now.hour)
    time = time + "." + str(now.minute)
    return time


def check_date():
    locale.setlocale(locale.LC_ALL, '')
    now = datetime.datetime.now()
    date = str(datetime.datetime.strftime(now, "%d")) + " "
    date = date + str(datetime.datetime.strftime(now, "%B")) + " "
    date = date + str(datetime.datetime.strftime(now, "%Y")) + " "
    date = date + str(datetime.datetime.strftime(now, "%A"))
    return date


def check_weather(location):
    owm = pyowm.OWM('8ff2ea8803e609a73629da5e70b37baa')

    observation = owm.weather_at_place(location)
    w = observation.get_weather()

    print(w.get_wind())
    print(w.get_humidity())
    print(w.get_temperature('celsius'))

    dict = w.get_temperature('celsius')
    return str(dict['temp'])


