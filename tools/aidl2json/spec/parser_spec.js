let Parser = require('../parser');
let Token = require('../token');
let Lexer = require('../lexer');

describe("Parser Tester", function () {
  it("parse package", function () {
    let parser = new Parser("package com.demo.aidl;");
    parser.parse();
    expect(parser.result.packageName).toBe('com.demo.aidl');
  });

  it("parse package imports", function () {
    let parser = new Parser("package com.demo.aidl;\nimport android.graphics.Bitmap;\nimport android.content.Context\n");
    parser.parse();
    expect(parser.result.packageName).toBe('com.demo.aidl');

    expect(parser.result.importPackages.length).toBe(2);
    expect(parser.result.importPackages[0]).toBe('android.graphics.Bitmap');
    expect(parser.result.importPackages[1]).toBe('android.content.Context');
  });

  it("parse interface", function () {
    let parser = new Parser("interface abc {}\n");
    parser.parse();

    expect(parser.result.interfaces.length).toBe(1);
    expect(parser.result.interfaces[0].name).toBe("abc")
  });

  it("parse interface1", function () {
    let parser = new Parser(`interface abc {
    int foo();
    \n\}\n`);
    parser.parse();

    expect(parser.result.interfaces.length).toBe(1);
    expect(parser.result.interfaces[0].name).toBe("abc")
    expect(parser.result.interfaces[0].methods.length).toBe(1)
    expect(parser.result.interfaces[0].methods[0].name).toBe('foo')
    expect(parser.result.interfaces[0].methods[0].returnType.type).toBe('int')
  });
  
  it("parse interface2", function () {
    let parser = new Parser(`interface abc {
    int foo(in int a, out int b);
    int hello(in int[] a, out int[] b);
    String[] world(in int[] a, out int[] b);
    \n\}\n`);
    parser.parse();

    expect(parser.result.interfaces.length).toBe(1);
    expect(parser.result.interfaces[0].name).toBe("abc")
    expect(parser.result.interfaces[0].methods.length).toBe(3)
    expect(parser.result.interfaces[0].methods[0].name).toBe('foo')
    expect(parser.result.interfaces[0].methods[0].params.length).toBe(2)
    expect(parser.result.interfaces[0].methods[0].params[0].type).toBe('int')
    expect(parser.result.interfaces[0].methods[0].returnType.type).toBe('int')
    
    expect(parser.result.interfaces[0].methods[1].name).toBe('hello')
    expect(parser.result.interfaces[0].methods[1].params.length).toBe(2)
    expect(parser.result.interfaces[0].methods[1].params[0].name).toBe('a')
    expect(parser.result.interfaces[0].methods[1].params[0].type).toBe('int')
    expect(parser.result.interfaces[0].methods[1].params[0].isArray).toBe(true)
    expect(parser.result.interfaces[0].methods[1].returnType.type).toBe('int')
    
    expect(parser.result.interfaces[0].methods[1].name).toBe('hello')
    expect(parser.result.interfaces[0].methods[1].params.length).toBe(2)
    expect(parser.result.interfaces[0].methods[1].params[1].name).toBe('b')
    expect(parser.result.interfaces[0].methods[1].params[1].type).toBe('int')
    expect(parser.result.interfaces[0].methods[1].params[1].isArray).toBe(true)
    expect(parser.result.interfaces[0].methods[1].returnType.type).toBe('int')
    
    expect(parser.result.interfaces[0].methods[2].name).toBe('world')
    expect(parser.result.interfaces[0].methods[2].params.length).toBe(2)
    expect(parser.result.interfaces[0].methods[2].params[0].name).toBe('a')
    expect(parser.result.interfaces[0].methods[2].params[0].type).toBe('int')
    expect(parser.result.interfaces[0].methods[2].params[0].isArray).toBe(true)
    expect(parser.result.interfaces[0].methods[2].returnType.isArray).toBe(true)
    expect(parser.result.interfaces[0].methods[2].returnType.type).toBe('String')
    
    expect(parser.result.interfaces[0].methods[2].params.length).toBe(2)
    expect(parser.result.interfaces[0].methods[2].params[1].name).toBe('b')
    expect(parser.result.interfaces[0].methods[2].params[1].type).toBe('int')
    expect(parser.result.interfaces[0].methods[2].params[1].isArray).toBe(true)
  });
});
