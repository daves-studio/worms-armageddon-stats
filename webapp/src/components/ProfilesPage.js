import React from 'react';
import Profile from './PlayerProfiles'
import PlayerProfiles from '../Assets/PlayerProfiles'
import { map } from 'lodash';
class ProfilesPage extends React.Component {
  constructor(props) {
    super(props);
    this.state = {

    }
  }

    render(){
        return (

            <div >
            {map(PlayerProfiles, (value, key) => {
                return <Profile  player={value} />;
            })}
            </div>
        )
    }
}

export default ProfilesPage;
