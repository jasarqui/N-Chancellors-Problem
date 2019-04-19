import React, { Component } from 'react';
import { InputNumber, Button, Form } from 'antd';
import Board from '../Board/';
import './Solver.css';


const FormItem = Form.Item;

class Solver extends Component {
  constructor(props) {
    super(props);
    this.state = {
      N: null,
      current: 0,
    }
    this.handleNChange = this.handleNChange.bind(this);
    this.checkN = this.checkN.bind(this);
    this.handleSubmitN = this.handleSubmitN.bind(this);
  }

  handleNChange = (value) => {
    this.setState({
      N: value,
      current: 0,
    })
  }
  checkN = (rule, value, callback) => {
    if (isNaN(value) || value<=0) {
      callback('Input must be a number greater than 0.')
    } 
    callback();
    return;
  }

  handleSubmitN = (e) => {
    e.preventDefault();
    this.setState({
      current: 1,
    })
  }
  render() {
    const{ getFieldDecorator } = this.props.form;
    const { current } = this.state; 
    return (
      <div className="solver-container">
        <div className="solver-content">
          <h3> SOLVER </h3>
          {
            current === 0 ?   
              <Form  className="form-container" onSubmit={this.handleSubmitN}>
                <FormItem label="Board Size" colon={false}>
                  {getFieldDecorator('N', {
                    rules:[
                      {required: true, message: 'Board size required.'},
                      {validator: this.checkN}
                    ], 
                    validateFirst: true,
                  })(<InputNumber size="large" onChange={this.handleNChange}/>)}
                </FormItem>
                 <Button
                    type="primary"
                    htmlType="submit"
                    className="submit-button"
                  >
                    Generate NxN Board
                  </Button>
              </Form>
            : current === 1 ? 
                <Board N={this.state.N}/>
            : <p> aa </p>
          }
        </div>
      </div>
    )
  }
}

export default Form.create()(Solver);
