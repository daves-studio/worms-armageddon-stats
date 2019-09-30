import React from 'react';
import Pics from '../Assets/Images/worms2.png'


class PlayerProfiles extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
        FullProfile: false,
    }
    this.displayFullProfile = this.displayFullProfile.bind(this);
  }
  displayFullProfile(){
    if (this.state.FullProfile){
        this.setState({FullProfile: false});
    } else {
        this.setState({FullProfile: true});
    }
  } 
    render(){
        return (
   
            (this.state.FullProfile?
                <div className="jumbotron full-card">
                <h1 className="display-3">Player Profile: {this.props.player.name} </h1>
                <p className="small-font">Style: {this.props.player.style}</p>
                <p className="small-font">Favorite Weapon: {this.props.player.faveweapon}</p>
                <p className="small-font">Bio: {this.props.player.bio}</p>
                <p className="small-font">Quote: {this.props.player.quote}</p>
                <p className="small-font">Team: {this.props.player.team}</p>
                <button className="" onClick={() => {this.displayFullProfile();}}>Less Info</button>
              
              </div>
                : 
     
                <div className="miniPlayerCard">
                <div className="card profile-card" >
                    <img className="card-img-top" src={Pics}  alt="Card cap" />
                    <div className="card-body">
                        <h5 className="card-title">{this.props.player.name}</h5>
                        <p className="small-font">Style: {this.props.player.style}</p>
                        <button className="" onClick={() => {this.displayFullProfile();}}>More Info</button>
                    </div>
                </div>
               
            </div>
         
            ) 
       
        )
    }
}

export default PlayerProfiles;
