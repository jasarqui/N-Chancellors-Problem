import React, { Component } from 'react';
import { Layout } from 'antd';
import Hero from './pages/Hero/';
import Solver from './pages/Solver/';

const { Content } = Layout;
class App extends Component {
  render() {
    return (
      <Layout>
        <Content>
          <Hero/>
          <Solver/>
        </Content>
      </Layout>
    )    
  }
}

export default App;
