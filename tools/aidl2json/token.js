
class Token {
  static NONE = 0;
  static ID = 1;
  static STR = 2;
  static COMMENT = 3;
  static LBRACKET = 4;
  static RBRACKET = 5;
  static LBRACE = 6;
  static RBRACE = 7;
  static LPAREN = 8;
  static RPAREN = 9;
  static COMMA = 10;

  constructor(type, value) {
    this.type = type;
    this.value = value;
  }
}


module.exports = Token;
