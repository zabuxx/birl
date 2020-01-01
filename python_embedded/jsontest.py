import requests

import logging

logging.basicConfig(format='[PYTHON] %(levelname)s:%(message)s', level=logging.DEBUG)

class JSonTest:
    def __init__(self):
        self.status_code = 0

    def getip(self):
        resp = requests.get("http://ip.jsontest.com/")

        self.status_code = resp.status_code
        if resp.status_code == 200:
            return resp.text
        else:
            return ""

    def restecho(self, str1, str2, str3, str4):
        url = "http://echo.jsontest.com/" + str1 + '/' + str2 + '/' + str3 + '/' + str4
        resp = requests.get(url)

        self.status_code = resp.status_code
        if resp.status_code == 200:
            return resp.json()
        else:
            return {}

    def getitems(self):
        resp = requests.get('https://jsonplaceholder.typicode.com/posts')
        json_obj = resp.json()

        self.status_code = resp.status_code
        if resp.status_code == 200:
            return json_obj
        else:
            return []

jst = JSonTest()