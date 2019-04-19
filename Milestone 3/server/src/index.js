const express = require('express');
const bodyParser = require('body-parser');
const cors = require('cors');
const fs = require('fs');
const util = require('util');
const exec = util.promisify(require('child_process').exec);
const child_process = require('child_process');
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
    let board = [];
    matrix.map(row => board.push(row.join(' ')));
    board = board.join('\n');
    const content = `1\n${N}\n${board}`;
    await fs.writeFile('src/utils/board.in', content, (err) => {
      if (err) {
        console.log(err);
      }
    });
    const {stdout} = await exec('cd src/utils && make run');
    let ans = await fs.readFileSync('src/utils/solutions.out', 'utf8', function(err, data) {
      if (err) throw err;
      return(data);
    }); 
    ans = ans.split('\n');
    if (ans.length === 4) {
      res.status(200).json({
        hasSoln: false
      });
    }
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
        mat[row][col] = 1;
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