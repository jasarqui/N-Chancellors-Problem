import React, { Component } from 'react';
import { Button } from 'antd';
import './Hero.css';


class Hero extends Component {
  render() {
    return (
      <div className="hero-container">
        <div className="hero-content">
          <img src={ require('./rook.png') } />
          <img src={ require('./knight.png') } />
          <h1> N-CHANCELLORS GAME SOLVER </h1>
          <h3>How can N Chancellors be placed in an NxN board so that none of them attack each other?</h3>
          <p>CMSC142 Arquilita Evangelista Faustino (c) 2019 </p>
          
        </div>
      </div>
    );
  }
}

export default Hero;