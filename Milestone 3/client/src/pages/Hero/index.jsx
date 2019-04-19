import React, { Component } from 'react';
import { Button } from 'antd';
import './Hero.css';


class Hero extends Component {
  render() {
    return (
      <div className="hero-container">
        <div className="hero-content">
          <h1> N-CHANCELLORS SOLVER </h1>
          <p> A di ko alam info brief info here </p>
          <Button id="down-btn" size="large" shape="round"> Get Started </Button>
        </div>
      </div>
    );
  }
}

export default Hero;