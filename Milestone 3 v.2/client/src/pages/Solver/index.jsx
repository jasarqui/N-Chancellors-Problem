import React, { Component } from 'react';
import axios from 'axios';
import update from 'immutability-helper';
import { InputNumber, Button, Form, Card } from 'antd';
import SolN from '../Solutions/';
import './Solver.css';


const FormItem = Form.Item;
const matrix = (n) => new Array(n).fill(0).map((o, i) => new Array(n).fill(0))
const isNValid = (num) =>  !(isNaN(num) || num<=0)


/* TODO

  - label/captions w/ style 
  - 'new' button to reset state
  - enhance layout/UI
*/
class Solver extends Component {
  constructor(props) {
    super(props);
    this.state = {
      N: null,
      matrix: [[]],
      showSolutions: false,
      loading: false,
    }
    this.handleNChange = this.handleNChange.bind(this);
    this.checkN = this.checkN.bind(this);
    this.handleClick = this.handleClick.bind(this);
    this.handleSolveBoard = this.handleSolveBoard.bind(this);

  }

  handleNChange = (value) => {
    this.setState({
      N: value,
      matrix: isNValid(value) ? matrix(value) : [[]],
    });
  }

  checkN = (rule, value, callback) => {
    if (!isNValid(value)) {
      callback('Input must be a number greater than 0.')
    } 
    callback();
    return;
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
  async handleSolveBoard(e) {
    const {matrix} = this.state;
    this.setState({loading: true});
    axios.post('http://localhost:3001/api/solver',{matrix})
    .then((response) => {
      const { hasSoln,solCount, solutions } = response.data;
      this.setState({
        hasSoln,
        solCount,
        solutions,
        current: 2, 
        showSolutions: true,
        loading: false,
      });
      console.log(this.state);
    })
    .catch((error) => {
      this.setState({loading: false});
      console.log(error);
    });
  }

  resetBoard = () => {
    this.setState ({
      N: null,
      matrix: [[]],
      showSolutions: false,
      loading: false,
    });
  }
  
  render() {
    const{ getFieldDecorator } = this.props.form;
    const { N, matrix, showSolutions, hasSoln, loading } = this.state; 
    return (
      <div className="solver-container" style={{ margin: N>=12 ? '0 auto' : '48px auto', width: N>=12 ? '100%': '80%'}}>
        <Card className="solver-content" loading={loading}>  
          <h3 class="ribbon"> SOLVER </h3>
          <Form hideRequiredMark className="form-container" onSubmit={this.handleSolveBoard}>
            { !showSolutions ? 
              <FormItem label="Enter board size (N)" colon={false} >
                {getFieldDecorator('N', {
                  initialValue: this.state.N,
                  rules:[
                    {required: true, message: 'Board size required.'},
                    {validator: this.checkN}
                  ], 
                  validateFirst: true,
                })(<InputNumber size="large" onChange={this.handleNChange}/>)}
              </FormItem>
              : <h2> {`Solving for ${N}x${N} Board`}</h2>
            }
            {
              !(isNaN(N) || N<=0) === true ? 
                <div className="site-content">
                  { !showSolutions ? <p> Input Chancellors </p> : ''}
                  <table className="site-table" align="center">
                    <tbody>
                      { 
                        [...Array(N).keys()].map((r) => 
                          <tr key={r}>
                            { 
                              [...Array(N).keys()].map((c) => 
                                <td 
                                  key={c}
                                  position={`${r}_${c}`}
                                  onClick={showSolutions? null : this.handleClick}
                                  style={{ 
                                    width: N>=12? '30px':'60px',
                                    height: N>=12? '30px':'60px',
                                    background: ((r%2===0 && c%2===1) || (r%2===1 && c%2===0))? '#96928d':'#d1d1d1'
                                  }}
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
                  {
                    showSolutions? '' : <Button type="primary" size="large" className="btn" onClick={this.handleSolveBoard}> Solve </Button>
                  }

                  {
                    <Button type="primary" size="large" className="btn" onClick={this.resetBoard}> Reset </Button> 
                  }
                </div> 
              : ''
            }
          </Form>
          {
            hasSoln && showSolutions? <SolN {...this.state}/> 
              : !hasSoln && showSolutions? <p> No solutions found.</p> : ''

          }


        </Card>

      </div>
    )
  }
}

export default Form.create()(Solver);
