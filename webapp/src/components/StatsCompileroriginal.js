import React from 'react';
import statsjson from '../Assets/all_games'
import VegaLite from 'react-vega-lite';
import GraphComponent from './GraphComponent';

class Wins extends React.Component {
  constructor(props) {
    super(props);
    this.state = {

    }
    this.listCompletedGames = this.listCompletedGames.bind(this);
    this.listAllPlayers = this.listAllPlayers.bind(this);
    this.weaponused = this.weaponused.bind(this);
    this.killsanddamage = this.killsanddamage.bind(this);
  }
  listCompletedGames(){
  let listOfCompletedGames = []
    Object.keys(statsjson).forEach(key => {
      if (statsjson[key]['gameCompleted']){
        listOfCompletedGames.push(statsjson[key]['name'])
      }
    });
    return listOfCompletedGames;
  }
  listAllPlayers(){
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

  weaponused(player){
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
  }

  killsanddamage(player){
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
  makeagraph(){

  }
    render(){  
           
      let gamesList = this.listCompletedGames();
      let allplayers = this.listAllPlayers()[0];
      let gamesplayed = this.listAllPlayers()[1];
      // let weaponsused = this.weaponused('Alice');
      let personalkills = this.killsanddamage('Alice');
        let loseControl = {}
        let totalDamage ={}
        let winners = {}
        let barData = {"values": [
          ]
        }
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
        };
        Object.keys(winners).forEach(key => {
            barData['values'].push({"name": key ,"wins": winners[key], "damage": totalDamage[key], "losecontrol": loseControl[key], "gamesplayed": gamesplayed[key] })
       
        })
        this.personalData = {}
        allplayers.forEach((element) => {
          let personalVendetta = this.killsanddamage(element);
          
          let name = []
          this.personalData[element]= {'values': []}
          Object.keys(personalVendetta).forEach(key => {
            this.personalData[element]['values'].push({"name": key ,"kills": personalVendetta[key]['kills'], "damage": personalVendetta[key]['damage']})
          })
          
        })

        });
        console.log(this.personalData['Alice'])
        return (
        <div className="graphs">
             <GraphComponent xaxis="name" yaxis = "gamesplayed" graphtype={barData} />
             {/* 
             <GraphComponent xaxis="name" yaxis = "damage" graphtype={barData} />
             <GraphComponent xaxis="name" yaxis = "losecontrol" graphtype={barData} /> */}
             
             {Object.keys(this.personalData).map((element) => {
               return <GraphComponent title={element} xaxis='name' yaxis = 'kills' graphtype={this.personalData[element] } />
             })}
                 {Object.keys(this.personalData).map((element) => {
               return <GraphComponent className="graphs" title={element} xaxis='name' yaxis = 'damage' graphtype={this.personalData[element] } />
             })}
            </div>
        )
    }
}

export default Wins;
