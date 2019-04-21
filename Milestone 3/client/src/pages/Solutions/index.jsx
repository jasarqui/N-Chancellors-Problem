import React, { Component } from 'react';
import { Table} from 'antd';
import './Solutions.css';

/* @TODO
  - responsive pagination (width)
  - labels (Solution #1)
  - include other info needed
 */
class SolN extends Component {
  render() {
    const { N, solutions, solCount} = this.props;
    const data  = [...Array(solCount).keys()].map(x =>{ return { key: String(x), board: {key: String(x), matrix: solutions[x]}}})
    const renderBoard = (board) => (
      <table key={board.key} align="center" className="solns">
        <tbody>
          { 
            [...Array(N).keys()].map((r) => 
              <tr key={r}>
                { 
                  [...Array(N).keys()].map((c) => 
                    <td 
                      key={c}
                      position={`${r}_${c}`}
                      style={{ 
                        width: N>=12? '30px':'60px',
                        height: N>=12? '30px':'60px',
                        background: ((r%2===0 && c%2===1) || (r%2===1 && c%2===0))? '#f58c00':'#ffcf8f'
                      }}                    >
                      {
                        (board.matrix) && ((board.matrix)[r][c] === 1) ? "C" : ""
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
    const column  = [{title: '', dataIndex: 'board', key: 'board', render: board => renderBoard(board)}];
    return (
      <div className="solver-content">
        <Table 
          className="main-table"
          showHeader={false}
          dataSource={data} 
          columns={column} 
          pagination = {{
            defaultCurrent: 1,
            defaultPageSize: 1,
            showQuickJumper: true,
            showSizeChanger: true,
            pageSizeOptions: ['10','20','30'],
            hideOnSinglePageSize: true, 
            showLessItems: true,
            showTotal: function(total) { return `Total of ${total} solutions`},
            total: solCount, 
          }}
        />
      </div>
    )
  }
}
export default SolN;