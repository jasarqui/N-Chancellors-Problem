import React, { Component } from 'react';
import update from 'immutability-helper';
import {Button} from 'antd';
import axios from 'axios';
import SolN from '../Solutions/';
import './Board.css';


const matrix = (n) => new Array(n).fill(0).map((o, i) => new Array(n).fill(0))


class Board extends Component {

  constructor(props) {
    super(props);
    this.state = {
      matrix: [[]],
      N: null,
      current: 1,
    }
    this.handleClick = this.handleClick.bind(this);
    this.handleSubmitBoard = this.handleSubmitBoard.bind(this);
  }
  handleClick(e) {
    const position = e.target.getAttribute('position');
    if (position) {
      const r = position.split("_")[0];
      const c= position.split("_")[1];
      let mat = this.state.matrix;    
      this.setState({matrix: update(mat, 
        {
          [r] : { 
            [c] : {$set: mat[r][c] === 0 ? 1 : 0}
          }
        })
      });
    }
  }
  async handleSubmitBoard(e) {
    const {matrix} = this.state;
    axios.post('http://localhost:3001/api/solver',{matrix})
    .then((response) => {
      const { hasSoln,solCount, solutions } = response.data;
      this.setState({
        hasSoln, solCount, solutions, current: 2,
      });
    })
    .catch((error) => {
      console.log(error);
    });
  }
  componentWillMount() {
    const { N } = this.props;
    this.setState({ N: N, matrix: matrix(N)});
  }

  render() {
    const { N } = this.props;
    const { matrix, current } = this.state;
    return (
      <div> 
        {
          current === 1 ? 
            <div className="site-table">
              <table id="board" align="center">
                <tbody>
                  { 
                    [...Array(N).keys()].map((r) => 
                      <tr key={r}>
                        { 
                          [...Array(N).keys()].map((c) => 
                            <td 
                              key={c}
                              position={`${r}_${c}`}
                              onClick={this.handleClick}
                              style={{ background: ((r%2===0 && c%2===1) || (r%2===1 && c%2===0))? '#f58c00':'#ffcf8f'}}
                            >
                              {
                                matrix && (matrix[r][c] === 1) ? "C" : ""
                              }
                            </td>
                          )
                        }
                      </tr>,
                    )
                  }
                </tbody>
              </table> 
              <Button type="primary" onClick={this.handleSubmitBoard}> Solve </Button>
            </div>
          : <SolN {...this.state}/>
        }
      </div>
    )
  }
}

export default Board;
