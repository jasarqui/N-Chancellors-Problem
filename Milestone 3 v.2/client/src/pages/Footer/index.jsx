mport React, { Component } from ‘react’;
import {Footer} from ‘./components/Footer’;

class Footedr extends Component {
    render() {
        return (
            <div className="fixed-bottom">  
            <Navbar color="dark" dark>
                <Container>
                    <NavbarBrand>Footer</NavbarBrand>
                </Container>
            </Navbar>
        </div>
        );
    }
}

export default Footer;