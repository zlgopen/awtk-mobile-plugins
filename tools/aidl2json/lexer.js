const Token = require("./token")

class Lexer {
  constructor(str) {
    this.c = null;
    this.col = 0;
    this.line = 0;
    this.str = str;
    this.cursor = 0;
  }
  
  expectToken(type) {
    let t = this.getToken();
    if (!t || t.type !== type) {
      this.dump();
      throw ('expected token type: ' + type);
    }

    return t;
  }

  
  ungetToken(t) {
    this.token = t;
  }

  getToken() {
    if (this.token) {
      let t = this.token;
      this.token = null;
      return t;
    }

    this.skipChars(' \t\r\n;');
    let c = this.getChar();

    while (c == '/') {
      this.skipComment();
      this.skipChars(' \t\r\n;');
      c = this.getChar();
    }

    switch (c) {
      case ',': {
        return new Token(Token.COMMA, c);
      }
      case '{': {
        return new Token(Token.LBRACE, c);
      }
      case '}': {
        return new Token(Token.RBRACE, c);
      }
      case '[': {
        return new Token(Token.LBRACKET, c);
      }
      case ']': {
        return new Token(Token.RBRACKET, c);
      }
      case '(': {
        return new Token(Token.LPAREN, c);
      }
      case ')': {
        return new Token(Token.RPAREN, c);
      }
      case null: {
        return null;
      }
      default: {
        if (/[a-zA-Z_]/.test(c)) {
          return this.getID(c);
        } else {
          this.dump();
          throw ("parser error:" + c);
        }
      }
    }
  }


  dump() {
    console.log(`${this.line},${this.col}: ${this.str.substr(this.cursor, 100)}`)
  }

  skipComment() {
    let c = this.getChar();
    if (c == '*') {
      let lastC = this.getChar();
      do {
        c = this.getChar();
        if (c == '/' && lastC == '*') {
          break;
        } else {
          lastC = c;
        }
      } while (c);
    } else {
      while (c && c !== '\n' && c !== '\r') {
        c = this.getChar();
      }
    }
  }


  getID(c) {
    let id = c;
    c = this.getChar();
    while (/[a-z0-9A-Z_.]/.test(c)) {
      id += c;
      c = this.getChar();
    }
    this.ungetChar(c)

    return new Token(Token.ID, id);
  }

  skipChars(chars) {
    let c = this.getChar();
    while (chars.indexOf(c) >= 0) {
      c = this.getChar();
    }
    this.ungetChar(c);

    return;
  }

  getChar() {
    if (this.c) {
      const c = this.c;
      this.c = null;

      return c;
    }

    if (this.cursor >= this.str.length) {
      return null;
    } else {
      const c = this.str[this.cursor++];
      if (c == '\n') {
        this.line++;
        this.col = 0;
      } else {
        this.col++;
      }

      return c;
    }
  }

  ungetChar(c) {
    this.c = c;
  }
}

module.exports = Lexer;
