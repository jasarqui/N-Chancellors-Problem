import React, { Component } from 'react';
import { Layout } from 'antd';
import Hero from './pages/Hero/';
import Solver from './pages/Solver/';

const { Content } = Layout;


/* @TODO
  (optional)
  - 3 column section (ex: How it works, About the project, link to repo)
  - section that includes group members
 */

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
