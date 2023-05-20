
const express = require('express');
const app = express();
const account = require('./account');
const users = [new account("Bobert@umass565", "bobpassword"),new account("Alice@umass565","alicepassword"), new account("Eve@umass565", "evepassword")];
const fp = require('fs');
const file_path = 'log.txt';
const secrets_path = "supersecrets/bank_info.json"
const diff = require('diff');
//users.forEach(x => {console.log(x.user)});


function showStringDifference(str1, str2) {
    const differences = diff.diffChars(str1, str2);
  
    differences.forEach((part) => {
        const value = part.value;
        if (part.added) {
          console.log(`${JSON.stringify(value)}`);
        } else if (part.removed) {
          console.log(`${JSON.stringify(value)}`);
        }
      });
  }
//password = "bbpassword";
function writeip_log(ip, account) {
    const timestamp = Date.now();
    const date = new Date(timestamp);
    const isoString = date.toISOString();
    
    fp.appendFile(file_path, ip+ " " + isoString+ " " + account + "\n", "utf-8", (err) => {
        if (err) {
          console.error('Error writing file:', err);
          return;
        }
      
        console.log('File successfully written.');
      });
}
app.get('/welcome', (req, res) => { 
    writeip_log(req.ip, "N/A");
    res.json({messsage: 'hi'});});

app.get('/565', (req, res) => { 
        writeip_log(req.ip, "N/A");
        res.json({messsage: 'cool class'});});
app.get('/secrets', (req, res) => {
    const authHeader = req.headers.authorization;
    if(!authHeader || !authHeader.startsWith('Basic')){
        res.status(401).send("Unauthorized");
        return;
    }
    const creds = authHeader.slice('Basic'.length);

    const [username, password] = Buffer.from(creds, 'base64').toString('ascii').split(':');
    const username_n = username.replaceAll('\r\n', '');
    const password_n = password.replaceAll('\r\n', '');
    console.log(users.some(e => e.pass === password_n));
    if(users.some(e => e.user === username_n && e.pass === password_n)) {
        writeip_log(req.ip, username_n)
        fp.readFile(secrets_path,'utf-8', (err, data) => {
            if (err) throw err;
            console.log(data);
            res.json(data);
          }); 
    } else {
	    res.status(401).send("unauthorized");
    }
});
	
app.listen(3000, ()=> {
  console.log('listening on port 3000');
});
