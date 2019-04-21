const express = require('express');
const bodyParser = require('body-parser');
const cors = require('cors');
const fs = require('fs');
const util = require('util');
const exec = util.promisify(require('child_process').exec);
const app = express();

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));
app.use(cors());

/*@TODO
  modularize
  error handling 
*/
app.post('/api/solver',async (req, res) => {
  const { matrix } = req.body;
  const N = Object.keys(matrix).length;
  try {
    // create board.in 
    let board = [];
    matrix.map(row => board.push(row.join(' ')));
    board = board.join('\n');
    const content = `1\n${N}\n${board}`;
    await fs.writeFile('src/utils/board.in', content, (err) => {
      if (err) {
        console.log(err);
      }
    });
    // run solver.exe to generate solutions
    const {stdout} = await exec('cd src/utils && make run');
    // read solutions.out and store content
    let ans = await fs.readFileSync('src/utils/solutions.out', 'utf8', function(err, data) {
      if (err) throw err;
      return(data);
    }); 
    ans = ans.split('\n');
    console.log(ans);
    if (ans.length === 4) {
      res.status(200).json({
        hasSoln: false,
      });
    }
    // create array of matrices based on generated answers
    ans = ans.slice(1,ans.length-3);
    solCount = ans.length;
    let solutions = [];
    for ( let i = 0; i < solCount; i+=1) {
      let c = ans[i].split(' ');
      c.pop();
      let mat = new Array(N).fill(0).map((o, i) => new Array(N).fill(0))
      for(let j=0; j <c.length; j+=1) {
        row = c[j][1];
        col = c[j][3];
        mat[row][col] = 1; //assign 1 to positions included in the file 
      }
      solutions.push(mat);
    }
    res.status(200).json({
      hasSoln: 'true',
      solCount,
      solutions,
    });
  } catch (err) {
    console.log(err);
    res.status(500).json(err);
  }
});

const port = process.env.PORT || 3001;
app.listen(port, () => {
  console.log(`Server is listening at port: ${port}`);
});