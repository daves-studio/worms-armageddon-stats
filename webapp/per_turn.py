import json
import csv
import csv

d = json.load(open("all_games.json"))
out = csv.DictWriter(
    open("nr.csv", "w"), ["team", "ropes", "self_damage", "other_damage"]
)
out.writeheader()

for game in d:
    for team in game["gameteams"]:
        name = team["team"]["name"]
        for turn in team["turns"]:
            nr = 0
            self_damage = 0
            other_damage = 0
            for utility in turn["turnutilityweapons"]:
                if utility["utilityweapon"]["name"] == "Ninja Rope":
                    nr += utility["numberOfFires"]
            for damage in turn["turndamages"]:
                if damage["team"]["name"] == name:
                    self_damage += damage["damage"]
                else:
                    other_damage += damage["damage"]

            out.writerow(
                dict(
                    team=name,
                    ropes=nr,
                    self_damage=self_damage,
                    other_damage=other_damage,
                )
            )