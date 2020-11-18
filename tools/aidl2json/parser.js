const Token = require("./token");
const Lexer = require("./lexer");

class Parser {
  constructor(str, filename) {
    this.filename = filename;
    this.lexer = new Lexer(str);

    this.result = {};
    this.result.interfaces = [];
    this.result.packageName = '';
    this.result.filename = filename;
    this.result.importPackages = [];
  }

  parsePackage() {
    let t = this.lexer.expectToken(Token.ID);
    this.result.packageName = t.value;
  }

  parseImport() {
    let t = this.lexer.expectToken(Token.ID);
    this.result.importPackages.push(t.value);
  }

  parseParam() {
    let param = {};
    let t = this.lexer.expectToken(Token.ID);

    if (t.value === 'in' || t.value === 'out' || t.value === 'inout') {
      param.io_type = t.value;
      t = this.lexer.expectToken(Token.ID);
    }

    param.type = t.value;

    t = this.lexer.getToken();
    //int[] a
    if (t && t.type == Token.LBRACKET) {
      t = this.lexer.expectToken(Token.RBRACKET);
      param.isArray = true;
      t = this.lexer.expectToken(Token.ID);
    } else {
      param.isArray = false;
    }

    param.name = t.value;
    return param;
  }

  parseReturn() {
    let returnType = {};
    let t = this.lexer.expectToken(Token.ID);

    returnType.type = t.value;
    t = this.lexer.getToken();
    if (t && t.type == Token.LBRACKET) {
      t = this.lexer.expectToken(Token.RBRACKET);
      returnType.isArray = true;
    } else {
      this.lexer.ungetToken(t);
      returnType.isArray = false;
    }

    return returnType;
  }

  parseMethod() {
    let method = {}
    method.returnType = this.parseReturn();

    let t = this.lexer.expectToken(Token.ID);
    method.name = t.value;

    method.params = [];
    t = this.lexer.expectToken(Token.LPAREN);

    do {
      if (!t || t.type == Token.RPAREN) {
        break;
      }
      t = this.lexer.getToken();
      if (!t || t.type == Token.RPAREN) {
        break;
      }
      this.lexer.ungetToken(t);
      method.params.push(this.parseParam());
      t = this.lexer.getToken();
      if (!t || t.type !== Token.COMMA) {
        break;
      }
    } while (true);

    return method;
  }

  parseInterface() {
    let intf = {};
    let t = this.lexer.expectToken(Token.ID);

    intf.name = t.value;
    intf.methods = [];

    t = this.lexer.expectToken(Token.LBRACE);
    do {
      t = this.lexer.getToken();
      if (!t || t.type == Token.RBRACE) {
        break;
      } else {
        this.lexer.ungetToken(t);
      }

      intf.methods.push(this.parseMethod());
    } while (t);
    this.result.interfaces.push(intf);
  }

  parse() {
    let t = this.lexer.expectToken(Token.ID);
    do {
      if (!t) {
        break;
      }

      switch (t.value) {
        case 'package': {
          this.parsePackage();
          break;
        }
        case 'import': {
          this.parseImport();
          break;
        }
        case 'interface': {
          this.parseInterface();
          break;
        }
      }
      t = this.lexer.getToken();
    } while (t);

    return this.result;
  }
}

module.exports = Parser;

