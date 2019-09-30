import React from 'react';

class AskQuestions extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
        inputText: '',
    }
    this.inputChange = this.inputChange.bind(this);
    this.validateForm = this.validateForm.bind(this);
  }
  inputChange(event){
    this.setState({inputText: event.target.value});
  }
  validateForm(){
    console.log(this.state.inputText);
    this.setState({inputText: ''});
  }
    render(){
        return (
        <div>
          <form>
          <div className="input-group">
  <div className="input-group-prepend">
    <span className="input-group-text">anything we can add?</span>
  </div>
  <textarea className="form-control" aria-label="With textarea" value={this.state.inputText} onChange={this.inputChange}></textarea>
  <button type="button" onClick={() => {this.validateForm();}} className="">Submit</button>

</div>
</form>
            </div>
        )
    }
}

export default AskQuestions;
