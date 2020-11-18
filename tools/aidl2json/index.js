const fs = require('fs');
const path = require('path');
const Parser = require('./parser')

if(process.argv.length < 3) {
  console.log(`${process.argv[0]} ${process.argv[1]} aidl_ifilename`);
  process.exit(0);
}

const ifilename = process.argv[2];
const str = fs.readFileSync(ifilename).toString();
const parser = new Parser(str, ifilename);
const result = JSON.stringify(parser.parse(), null, '\t');
const ofilename = path.basename(ifilename) + '.json';

fs.writeFileSync(ofilename, result);

console.log(`output to ${ofilename}`);


