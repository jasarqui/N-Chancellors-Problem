import React, { Component } from 'react';
import './Solutions.css';

/* @TODO
  UI
  pagination
 */
class SolN extends Component {
  
  render() {
    const { N, solutions, solCount} = this.props;
    return (
      <div>
        {
          [...Array(solCount).keys()].map((soln) => 
              <table key={soln} align="center" className="solns">
                <tbody>
                  { 
                    [...Array(N).keys()].map((r) => 
                      <tr key={r}>
                        { 
                          [...Array(N).keys()].map((c) => 
                            <td 
                              key={c}
                              position={`${r}_${c}`}
                              style={{ background: ((r%2===0 && c%2===1) || (r%2===1 && c%2===0))? '#f58c00':'#ffcf8f'}}
                            >
                              {
                                (solutions[soln]) && ((solutions[soln])[r][c] === 1) ? "C" : ""
                              }
                            </td>
                          )
                        }
                      </tr>,
                    )
                  }
                </tbody>
              </table> 
          )
        }
      </div>
    )
  }
}
export default SolN;