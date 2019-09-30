import React from 'react';
// import Wins from './wins'
// import Damage from './damage'
import StatsCompiler from './StatsCompilertest'
import { map } from 'lodash';

class StatsPage extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
        graphType: "wins",
        personOrGroup: "group",
        personalstats: "kills",
        focusOnPlayer: this.props.players[0][0]
    }
  this.preferenceChange = this.preferenceChange.bind(this)
  this.personOrGroupChange = this.personOrGroupChange.bind(this)
  this.focusOnPlayerChange = this.focusOnPlayerChange.bind(this)
  this.personalstatsChange = this.personalstatsChange.bind(this)
  }
  preferenceChange(event) {
    console.log(event.target.value)
    this.setState({ graphType: event.target.value });

  }
  personalstatsChange(event){
    this.setState({ personalstats: event.target.value });
  }
  personOrGroupChange(event) {
    this.setState({ personOrGroup: event.target.value });
    if (event.target.value === "group"){
        this.setState({ graphType: "wins" })
    } else if (event.target.value === "personal"){
        this.setState({ graphType: "kills"})
    }
  }
  focusOnPlayerChange(event) {
    this.setState({ focusOnPlayer: event.target.value });
  }
    render(){
        return (
            (this.state.personOrGroup === "group"?
        <div>
          <div className="form-group">
              <select name="personalOrGroup" className="form-control" value={this.state.personOrGroup} onChange={this.personOrGroupChange} required>
                <option value="group">group</option>
                <option value="personal">personal</option>
              </select>
            </div>
          <div className="form-group">
              <select name="groupstats" className="form-control" value={this.state.graphType} onChange={this.preferenceChange} required>
                <option value="wins">wins</option>
                <option value="damage">damage</option>
                <option value="losecontrol">losecontrol</option>
                <option value="gamesplayed">gamesplayed</option>
                <option value="ninjaRope">Ninja Ropes</option>

              </select>
            </div>
          <StatsCompiler personOrGroup={this.state.personOrGroup} graphType={this.state.graphType} listAllPlayers={this.props.players} focusOnPlayer={this.state.focusOnPlayer}/>
            </div>
            :
            <div>
                          <div className="form-group">
              <select name="personalOrGroup" className="form-control" value={this.state.personOrGroup} onChange={this.personOrGroupChange} required>
                <option value="group">group</option>
                <option value="personal">personal</option>
                
              </select>
            </div>
            <div className="form-group">
                <select name="playerStats" className="form-control" value={this.state.focusOnPlayer} onChange={this.focusOnPlayerChange} required>
                {map(this.props.players[0], (value, key) => {
                return <option value={value}>{value}</option>;
            })}
                    </select>
            </div>
            <div className="form-group">
              <select name="groupstats" className="form-control" value={this.state.personalstats} onChange={this.personalstatsChange} required>
                <option value="weapons">weapons</option>
                <option value="kills">killed</option>
                

              </select>
            </div>
            

          
     
          <StatsCompiler personOrGroup={this.state.personOrGroup} graphType={this.state.graphType} listAllPlayers={this.props.players} focusOnPlayer={this.state.focusOnPlayer} personalstats={this.state.personalstats}/>
            </div>
            )
        )
    }
}

export default StatsPage;
