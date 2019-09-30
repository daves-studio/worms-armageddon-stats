import statsjson from './Assets/all_games'
export function listCompletedGames(){
    let listOfCompletedGames = []
      Object.keys(statsjson).forEach(key => {
        if (statsjson[key]['gameCompleted']){
          listOfCompletedGames.push(statsjson[key]['name'])
        }
      });
      return listOfCompletedGames;
    }

export function weaponsUsed(player){
        let weaponsUsed = {}
        Object.keys(statsjson).forEach(key => {
          if (statsjson[key]['gameCompleted']){
            if (statsjson[key]['gameteams']){
              (statsjson[key]['gameteams']).forEach(function (item) {
                if (item['team']['name'] === player){
                if (item['turns'].length > 0){
                  (item['turns']).forEach(function (index) {
                    if (index['weapon'] !== null){
                
                    if (index['weapon']['name'] in weaponsUsed){
                      weaponsUsed[index['weapon']['name']] = weaponsUsed[index['weapon']['name']] +1
    
                  } else {
                      weaponsUsed[index['weapon']['name']] = 1
                  }
                    }
                  })
                }
              }
              });
          }
        }
      });
      return weaponsUsed
      }

export function utilitiesUsed(player){
        let utilityUsed = {}
        let totalTurns = 0
        Object.keys(statsjson).forEach(key => {
          if (statsjson[key]['gameCompleted']){
            if (statsjson[key]['gameteams']){
              (statsjson[key]['gameteams']).forEach(function (item) {
                if (item['team']['name'] === player){
                if (item['turns'].length > 0){
                  (item['turns']).forEach(function (index) {
                    totalTurns += 1;
                    if (index['turnutilityweapons'] !== null && index['turnutilityweapons'].length > 0){
                      (index['turnutilityweapons']).forEach(function(element) {
                        if (element['utilityweapon']['name'] in utilityUsed){
                       
                          utilityUsed[element['utilityweapon']['name']] = utilityUsed[element['utilityweapon']['name']] + element['numberOfFires']
    
                  } else {
                      utilityUsed[element['utilityweapon']['name']] = element['numberOfFires']
                  }
                    })
                    }
                  })
                }
              }
              });
          }
        }
      });
    Object.keys(utilityUsed).forEach(key => {
      utilityUsed[key] = utilityUsed[key] / totalTurns
    })
      return utilityUsed
      }
export function killsanddamage(player){
    let killsanddamage= {}
    Object.keys(statsjson).forEach(key => {
      if (statsjson[key]['gameCompleted']){
        if (statsjson[key]['gameteams']){
          (statsjson[key]['gameteams']).forEach(function (item) {
            if (item['team']['name'] === player){
            if (item['turns'].length > 0){
              (item['turns']).forEach(function (index) {
                if (index['turndamages'] !== null && index['turndamages'].length >0  ){
                  (index['turndamages']).forEach(function (element){
                    if (element['team']['name'] in killsanddamage){
                     
                      killsanddamage[element['team']['name']]['kills'] = killsanddamage[element['team']['name']]['kills'] + element['kills']
                      killsanddamage[element['team']['name']]['damage'] = killsanddamage[element['team']['name']]['damage'] + element['damage']
    
                  } else {
                    killsanddamage[element['team']['name']] = {'kills': element['kills'], 'damage': element['damage']}
                  }
                  })
                }
              })
            }
          }
          });
      }
    }
  });

    return killsanddamage
  }
  export function listAllPlayers(){
    let playerList = []
    let playergames = {}
    Object.keys(statsjson).forEach(key => {
      if (statsjson[key]['gameCompleted']){
      Object.keys(statsjson[key]['gameteams']).forEach(index => {
        if (playerList.includes(statsjson[key]['gameteams'][index]['team']['name'] )){
          playergames[statsjson[key]['gameteams'][index]['team']['name']] = playergames[statsjson[key]['gameteams'][index]['team']['name']] +1
        } else {
        playerList.push(statsjson[key]['gameteams'][index]['team']['name'])
        playergames[statsjson[key]['gameteams'][index]['team']['name']] = 1
        }
      });
    }
    });
    return [playerList, playergames]
  }
  export function createGroupGraphDataBase(){
    let loseControl = {}
    let totalDamage ={}
    let winners = {}
    let gamesplayed = listAllPlayers()[1];
    let barData = {"values": [
    ]
  }
  let playerList = listAllPlayers()[0];
    Object.keys(statsjson).forEach(key => {
        if (statsjson[key]['gameCompleted']){
            var teams = statsjson[key]['gameteams']
            Object.keys(teams).forEach(index => {
                let damage = 0
                let controllost = 0
                if(teams[index]['winner']){
                    if (teams[index]['team']['name'] in winners){
                        winners[teams[index]['team']['name']] = winners[teams[index]['team']['name']] +1

                    } else {
                        winners[teams[index]['team']['name']] = 1
                    }
                }
                if ((teams[index]['turns']).length > 0 ){
                
                  let turns = (teams[index]['turns'])
                  turns.forEach(function(element){
                   
                    element['turndamages'].forEach(function(x){
                  
                      damage = damage + x['damage']
                    })

                  })
              }
              
                if (teams[index]['team']['name'] in totalDamage){
                    totalDamage[teams[index]['team']['name']] = totalDamage[teams[index]['team']['name']] + damage

                } else {
                    totalDamage[teams[index]['team']['name']] = damage
                }
                damage = 0
                if ((teams[index]['turns']).length > 0 ){
                
                  let turns = (teams[index]['turns'])
                  turns.forEach(function(element){
                   
                    if (element['lossOfControl']){
                  
                      if (teams[index]['team']['name'] in loseControl){
                          loseControl[teams[index]['team']['name']] = loseControl[teams[index]['team']['name']] +1
  
                      } else {
                          loseControl[teams[index]['team']['name']] = 1
                      }
                    }

                  })
              }
              });
            }
          });
          Object.keys(winners).forEach(key => {
            let utilities = utilitiesUsed(key);
            barData['values'].push({"name": key ,"wins": winners[key]})
        })
        Object.keys(playerList).forEach(key => {
            console.log(playerList[key])
            let utilities = utilitiesUsed(playerList[key]);
            barData['values'].push({"name": playerList[key], "damage": totalDamage[playerList[key]], "losecontrol": loseControl[playerList[key]], "gamesplayed": gamesplayed[playerList[key]], "ninjaRope": utilities['Ninja Rope']})
        })
        
        return barData
  }

  export function createPersonalGraphDataBase(personalstats, focusOnPlayer) {
    let personalData = {}
    let weapons = {}
    let allplayers = listAllPlayers()[0];
    let barData = {"values": [
    ]
  }
    allplayers.forEach((element) => {
      let personalVendetta = killsanddamage(element);
      let personalWeapons = weaponsUsed(element);
      personalData[element]= {'values': []}
      Object.keys(personalVendetta).forEach(key => {
        personalData[element]['values'].push({"name": key ,"kills": personalVendetta[key]['kills'], "damage": personalVendetta[key]['damage'], "weapons": personalWeapons})
      })
      weapons[element] = {'values': []}
      Object.keys(personalWeapons).forEach(key => {     
        weapons[element]['values'].push({"weapon": key ,"usage": personalWeapons[key]})
      })

    });
    if (personalstats === 'weapons' ){
      
      barData = weapons[focusOnPlayer]

    } else{
   barData = personalData[focusOnPlayer]

    }
    return barData
  }

  export function heatmap(){
    let barData= {'values': []}

    let allplayers = listAllPlayers()[0];

  allplayers.forEach((element) => {
    let personalVendetta = killsanddamage(element);
    Object.keys(personalVendetta).forEach(key => {
      barData['values'].push({"attacker": element, "defender": key, "damage": personalVendetta[key]['damage']})
    })
  });
  return barData
      }
  