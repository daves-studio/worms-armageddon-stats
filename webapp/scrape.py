import requests
import json
import sys

all_gamedata = []
for game in requests.get("http://172.29.65.195/game/").json():
    gamedata = requests.get(
        "http://172.29.65.195/game/{id}/".format(id=game["id"])
    ).json()[0]
    all_gamedata.append(gamedata)

json.dump(all_gamedata, sys.stdout)