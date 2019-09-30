import React from 'react';
import VegaLite from 'react-vega-lite';
class GraphComponent extends React.Component {
  constructor(props) {
    super(props);
    this.state = {

    }
  }
    render(){
        const spec = {
            "description": "A simple bar chart with embedded data.",
            "mark": "bar",
            "title": this.props.title,
            "encoding": {
              "x": {"field": this.props.xaxis, "type": "ordinal"},
              "y": {"field": this.props.yaxis, "type": "quantitative"}
            }
          };
        return (
        <div>
      
          <h2>This is a basic component!</h2>
          <div >
             <VegaLite className="graphs" spec={spec} data={this.props.graphtype} />
            </div>
            </div>
        )
    }
}

export default GraphComponent;
