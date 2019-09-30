import React from 'react';
import Home from './HomePage';
import Profiles from './ProfilesPage';
import Stats from './StatsPage';
import Feedback from './AskQuestions';
import statsjson from '../Assets/all_games'

class MainPage extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
        page: <Home />
    }
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
  changePage(input){
    if (input === 'Home'){
        this.setState({page: <Home />});
    } else if (input === "Profiles") {
        this.setState({page: <Profiles />});
    } else if (input === "Stats") {
        this.setState({page: <Stats players={this.listAllPlayers()}/>});
    } else if (input === "Feedback"){
        this.setState({page: <Feedback />})
    }
  }
    render(){
        return (
        <div>
      <div>
      <nav className="navbar navbar-expand-lg  bg-light position-top" >

  <div className="collapse navbar-collapse" id="navbarSupportedContent" >
    <ul className="navbar-nav mr-auto" >
      <li className="nav-item active">
        <button className="nav-link" onClick={() => {this.changePage('Home');}} >Home</button>
      </li>
      <li className="nav-item active">
        <button className="nav-link" onClick={() => {this.changePage('Profiles');}}>Player Profiles</button>
      </li>
      <li className="nav-item active">
        <button className="nav-link" onClick={() => {this.changePage('Stats');}}>Stats </button>
      </li>
      <li className="nav-item active">
        <button className="nav-link" onClick={() => {this.changePage('Feedback');}}>Feedback</button>
      </li>
    </ul>
    <form className="form-inline my-2 my-lg-0">
      <input className="" type="search" placeholder="Search" aria-label="Search" />
      <button className="" type="submit">Search</button>
    </form>
  </div>
</nav>
</div>
            <div className="margin">
            {this.state.page}
            </div>
            </div>
        )
    }
}

export default MainPage;
