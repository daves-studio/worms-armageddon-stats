'use strict'
import React from 'react';

import Heatmap from './Heatmap'
import GraphComponent from './GraphComponent';
import * as statsAPI from '../statsAPI';


class Wins extends React.Component {
  constructor(props) {
    super(props);
    this.state = {

    }

  }
  
    render(){  

      statsAPI.heatmap();

        let barData = {"values": [
          ]
        }
        let barData2 = {"values": [
        ]
      }
        if (this.props.personOrGroup === 'group'){
          barData = statsAPI.createGroupGraphDataBase();
     
            barData2 = statsAPI.heatmap();
           
        } else if (this.props.personOrGroup ==='personal') {
          if (this.props.graphType !== 'heatmap'){
          let personalstats = this.props.personalstats
          let focusOnPlayer = this.props.focusOnPlayer
          barData = statsAPI.createPersonalGraphDataBase(personalstats, focusOnPlayer);
          } 
      
      } 
        return (
          (this.props.personalstats === 'weapons' ?
        <div className="graphs">
             <GraphComponent xaxis="weapon" yaxis = "usage" graphtype={barData} />
             
            </div>
            :
            <div className="graphs">
              <GraphComponent xaxis="name" yaxis = {this.props.graphType} graphtype={barData} />
              <Heatmap xaxis="attacker" yaxis = "defender" zaxis = "damage" graphtype={barData2} />
            
            
           </div>
          )
        )
    }
}

export default Wins;
