import React from 'react';
import VegaLite from 'react-vega-lite';
class HeatmapComponent extends React.Component {
  constructor(props) {
    super(props);
    this.state = {

    }
  }
    render(){
        const spec = {
            "data": {"url": "data/cars.json"},
            "mark": "rect",
            "encoding": {
              "y": {"field": this.props.xaxis, "type": "nominal"},
              "x": {"field": this.props.yaxis, "type": "ordinal"},
              "color": {"aggregate": "mean", "field": this.props.zaxis, "type": "quantitative"}
            }
          }
          console.log(this.props.graphtype)
        return (
        <div>
          <div >
             <VegaLite className="graphs" spec={spec} data={this.props.graphtype} />
            </div>
            </div>
        )
    }
}

export default HeatmapComponent;
