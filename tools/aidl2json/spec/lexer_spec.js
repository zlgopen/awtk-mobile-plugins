let Token = require('../token');
let Lexer = require('../lexer');

describe("Lexer Tester", function () {
  it("brace1", function () {
    let lexer = new Lexer("{}");
    let token = lexer.getToken();
    expect(token.type).toBe(Token.LBRACE);

    token = lexer.getToken();
    expect(token.type).toBe(Token.RBRACE);
  });

  it("brace2", function () {
    let lexer = new Lexer(" { \n} ");
    let token = lexer.getToken();
    expect(token.type).toBe(Token.LBRACE);

    token = lexer.getToken();
    expect(token.type).toBe(Token.RBRACE);
  });
  
  it("bracket1", function () {
    let lexer = new Lexer("[]");
    let token = lexer.getToken();
    expect(token.type).toBe(Token.LBRACKET);

    token = lexer.getToken();
    expect(token.type).toBe(Token.RBRACKET);
  });

  it("bracket2", function () {
    let lexer = new Lexer(" [ \n] ");
    let token = lexer.getToken();
    expect(token.type).toBe(Token.LBRACKET);

    token = lexer.getToken();
    expect(token.type).toBe(Token.RBRACKET);
  });
  it("paren1", function () {
    let lexer = new Lexer("()");
    let token = lexer.getToken();
    expect(token.type).toBe(Token.LPAREN);

    token = lexer.getToken();
    expect(token.type).toBe(Token.RPAREN);
  });

  it("paren2", function () {
    let lexer = new Lexer(" ( \n) ");
    let token = lexer.getToken();
    expect(token.type).toBe(Token.LPAREN);

    token = lexer.getToken();
    expect(token.type).toBe(Token.RPAREN);

    token = lexer.getToken();
    expect(token).toBe(null);
  });

  it("id", function () {
    let lexer = new Lexer(" interface abc {");
    let token = lexer.getToken();
    expect(token.type).toBe(Token.ID);
    expect(token.value).toBe('interface')

    token = lexer.getToken();
    expect(token.type).toBe(Token.ID);
    expect(token.value).toBe('abc')

    token = lexer.getToken();
    expect(token.type).toBe(Token.LBRACE);

    token = lexer.getToken();
    expect(token).toBe(null);
  });

  it("comment", function () {
    let lexer = new Lexer("/*hello*/ interface /*workd*/\n//good\nabc {");
    let token = lexer.getToken();
    expect(token.type).toBe(Token.ID);
    expect(token.value).toBe('interface')

    token = lexer.getToken();
    expect(token.type).toBe(Token.ID);
    expect(token.value).toBe('abc')

    token = lexer.getToken();
    expect(token.type).toBe(Token.LBRACE);

    token = lexer.getToken();
    expect(token).toBe(null);
  });

});
